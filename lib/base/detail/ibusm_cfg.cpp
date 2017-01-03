#include "base/config_guard.h"
#include "base/utils.h"
#include "base/log_wrap.h"
#include "base/sysv_shm.h"
#include "base/sys_types.h"
#include "base/detail/circle_buffer.h"
#include "ibusm_cfg.h"
#include "ibusm.h"

namespace detail
{
static int GetChannelsName(config_t* _root, std::map<std::string, int>& _channelsName)
{
	config_setting_t* subCfg = config_lookup(_root, "svrID");
	XXZ_ASSERT(config_setting_is_group(subCfg));
	int count = config_setting_length(subCfg);
	for (int i = 0; i < count; ++i)
	{
		config_setting_t* elem = config_setting_get_elem(subCfg, i);
		_channelsName.insert(std::make_pair(std::string(config_setting_name(elem))
							, int(config_setting_get_int(elem))));
	}
	return 0;
}

static int GetChannels(config_t* _root, std::vector<ChannelCfg>& _channles, long _maxSize)
{
    config_setting_t * subCfg = config_lookup(_root, "channels");
    int count = config_setting_length(subCfg);
    _channles.reserve(count);
    int allocSize = 0;
    for (int i = 0; i < count; ++i)
    {
        config_setting_t* channel = config_setting_get_elem(subCfg, i);
        const int ChannelCfgItemNum = 3;
        if (config_setting_is_list(channel)
            && ChannelCfgItemNum ==  config_setting_length(channel))
        {
            int srcID = (int)config_setting_get_int_elem(channel, 0);
            int dstID = (int)config_setting_get_int_elem(channel, 1);
            long channelSize = AscStrToSize(config_setting_get_string_elem(channel, 2));
            if (channelSize > 0)
            {
                if (allocSize + channelSize <= _maxSize)
                {
                    allocSize += channelSize;
                    _channles.push_back(ChannelCfg(
                        srcID, dstID, channelSize));
                }
                else
                {
                    LOG_INFO("The %d channels over the total size!", i);
                }
            }
            else
            {
                LOG_INFO("The %d channels size %d invalid!", i, channelSize);
            }
        }
    }
    return allocSize;
}

IBusmCfg sCfgInfo;
IBusmCfg* ReadIBUSConfig(const char* _cfgName)
{
    config_t configData;
    config_t* conf = &configData;
    CConfigGuard guard(conf);
    if (CONFIG_FALSE == config_read_file(conf, _cfgName))
    {
        LOG_WARN("%s:%d faile to parse. %s",
                 _cfgName, config_error_line(conf), config_error_text(conf));
        return 0;
    }
 
	config_lookup_int(conf, "key", &sCfgInfo.key);
	//config_lookup_int(conf, "maxChannels", &sCfgInfo.capacity);
	const char* totalSize = "100M"; // 100M
	config_lookup_string(conf, "size", &totalSize);
	sCfgInfo.maxLength = AscStrToSize(totalSize);
    GetChannels(conf, sCfgInfo.channels, sCfgInfo.maxLength);
    GetChannelsName(conf, sCfgInfo.channelsName);
	sCfgInfo.capacity = sCfgInfo.channels.size();
    return &sCfgInfo;
}

int GetProcID(IBusmCfg* _cfg, const char* _channelName)
{
    XXZ_ASSERT( 0 != _cfg);
    typedef std::map<std::string, int>::const_iterator iterator;
    iterator itr = _cfg->channelsName.find(_channelName);
    if (_cfg->channelsName.end() != itr)
    {
        return itr->second;
    }
    return INVALID_CHANNEL_ID;
}

#ifndef ALIGN_SIZE
// 所有的指针需要align字节对齐。align必须是2的幂。一般4字节或者8字节对齐即可，
#	define ALIGN_SIZE(Size, align) ((align - 1 + Size) & (~(align - 1)))
#	define ALIGN_POINT(pointer, align) ALIGN_SIZE((reinterpret_cast<std::size_t>(pointer)), align)
#endif

IBUSHead* CreateIBUS(const char* _cfgName)
{
	IBusmCfg* cfg = ReadIBUSConfig(_cfgName);
	if (0 != cfg)
	{
		size_t headSize = sizeof(IBUSHead) + (cfg->capacity - 1) * sizeof(IBUSChannel);
		size_t totalSize = headSize + cfg->maxLength;
		CShmWrapper shm;
		if (0 != shm.CreateShm(cfg->key, totalSize, CreateShmVerify(cfg->key), true))
		{
			return 0;
		}

		IBUSHead* head = (IBUSHead*)shm.GetShmData();
		if (shm.IsResume())
		{
			//TODO: 判断配置信息是否兼容（channel对应的大小是否兼容，调整channel顺序）
			// 增加新增加的channel
		}
		
		// 设置共享内存信息
		head->capacity = cfg->capacity;
		head->count = 0;
		head->maxLength = totalSize;
		head->offset = ALIGN_SIZE(headSize, 16);
		typedef std::vector<ChannelCfg>::const_iterator const_iterator;
		const_iterator last = cfg->channels.end();
		for (const_iterator first = cfg->channels.begin(); first != last; ++first)
		{
			IBUSChannel& channel = head->channeles[head->count];
			channel.srcID = first->srcID;
			channel.dstID = first->dstID;
			channel.start = head->offset;
			channel.maxLength = ALIGN_SIZE(first->maxLength, 16);
			head->offset += channel.maxLength;
			if (head->offset <= head->maxLength)
			{
				++head->count;
				tagCircleBuffer* cb = reinterpret_cast<tagCircleBuffer*>(
					reinterpret_cast<char*>(head) + channel.start);
				cb->maxLength_ = channel.maxLength - sizeof(tagCircleBuffer);
				CircleBuffer::Clear(cb);
			}
			else
			{
				break;
			}
		}
		return 	head;
	}
	return 0;
}

struct ChannelFinder
{
	ChannelFinder(int _srcID, int _dstID)
		: srcID(_srcID), dstID(_dstID)
	{
	}
	inline bool operator()(const ChannelCfg& _cfg)
	{
		return srcID == _cfg.srcID && dstID == _cfg.dstID;
	}
	int srcID;
	int dstID;
};

bool CheckChannel(int _srcID, int _dstID)
{
	typedef std::vector<ChannelCfg>::const_iterator iterator;
	iterator ret = std::find_if(sCfgInfo.channels.begin(), sCfgInfo.channels.end(),
		ChannelFinder(_srcID, _dstID));
	return ret != sCfgInfo.channels.end();
}
}


