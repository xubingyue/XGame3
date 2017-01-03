#ifndef IBUSM_H_INCLUDED
#define IBUSM_H_INCLUDED
//#include <sys/types.h>
namespace detail
{
const int INVALID_CHANNEL_ID = -1;
struct IBUSChannel
{
    int srcID; // 写入的服务编号。人工分配
    int dstID; // 读出的服务编号。人工分配
    int start; // 对应于IBUSHead的实际偏移量
    int maxLength; // 最大长度
	//pid_t srcPID;  // 写入进程的进程号(getpid获取)
	//pid_t dstPID;  // 读取进程的进程号(getpid获取)
};

struct IBUSHead
{
    int capacity;  // 可以有多少个channels
    int count; 	   // 已经有多少个channels
    long maxLength; // 共享内存的大小。不小于所有Channels的总和
    long offset;    // 已经使用的共享内存
	IBUSChannel channeles[1];
};

struct IBUSChannelCfg
{
    IBUSChannelCfg(int _srcID, int _dstID, long _maxLength)
    : srcID(_srcID), dstID(_dstID), maxLength(_maxLength)
    {
    }
    int srcID; // 写入的服务编号
    int dstID; // 读出的服务编号
    long maxLength; // 最大长度
};

/**
 * Attach 到IBUS所在的共享内存
 * 
 * @param _key   共享内存对应的key
 * 
 * @return 0 失败
 *         其他成功， 对应IBUS共享内存头
 */
IBUSHead* AttachIBus(key_t _key);

/**
 * detach 当前IBUS对应的共享内存
 * 
 * @param _head  attach返回的共享内存的头
 * 
 * @return 0 成功
 *         其他失败
 */
int DetachBus(IBUSHead* _head);

char* CreateShmVerify(key_t _key);

/**
 * 创建IBUS对应的共享内存
 * 
 * @param _cfgName IBUS的配置文件名称
 * 
 * @return 0 失败
 *         其他成功， 对应IBUS共享内存头
 */
IBUSHead* CreateIBUS(const char* _cfgName);

IBUSChannel* GetChannel(IBUSHead* _head, int _srcID, int _dstID);
int ClearChannel(IBUSHead* _head, int _srcID, int _dstID);
}
#endif // IBUSM_H_INCLUDED
