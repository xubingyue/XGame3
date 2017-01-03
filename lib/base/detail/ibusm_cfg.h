#ifndef IBUS_DETAIL_IBUSM_CFG_H__
#define IBUS_DETAIL_IBUSM_CFG_H__
#include <map>
#include <string>
#include <vector>
namespace detail
{
struct ChannelCfg
{
    ChannelCfg(int _srcID, int _dstID, long _maxLength)
    : srcID(_srcID), dstID(_dstID), maxLength(_maxLength)
    {
    }
	int srcID; // 写入的服务编号。人工分配
    int dstID; // 读出的服务编号。人工分配
    long maxLength; // 最大长度
};

struct IBusmCfg
{
    cfg_int_t key;      // Ibus的共享内存所在位置
	cfg_int_t capacity;  // 可以有多少个channels
	int maxLength; // 共享内存的大小。不小于所有Channels的总和
	std::vector<ChannelCfg> channels; // 各channel的服务进程以及大小
	std::map<std::string, int> channelsName; // 各channel的名字信息
};

IBusmCfg* ReadIBUSConfig(const char* _cfgName);
int GetProcID(IBusmCfg* _cfg, const char* _channelName);
bool CheckChannel(int _srcID, int _dstID);
}
#endif  /* IBUS_DETAIL_IBUSM_CFG_H__ */
