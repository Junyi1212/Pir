#ifndef __MSG_QUEUE_H__
#define __MSG_QUEUE_H__

#include <stdio.h>
#include <pthread.h>
#include <queue>


#define MAX_QUEUE_NUM 512
using namespace std;


class CI2CPacket
{
public:
	CI2CPacket(uchar addr, uchar* buf, uint len, int param1);
	~CI2CPacket();
	uchar Get_I2CAddr(void);
	uchar* Get_I2CData(void);
	uint Get_I2CLen(void);	
	int Get_I2CParam1(void);
private:
	uchar slaveAddr;
	uchar* dataBuf;
	uint dataLen;
	int m_param1;
};


///消息队列，不负责内存的申请和注销
class CXTMsgQueue
{
public:
    CXTMsgQueue();
    ~CXTMsgQueue();

    /// @获取发送队列指针
	/// @para[in] newMsg 新消息
	/// @para[out] 0 成功 -1 失败
	int pushback(const CI2CPacket *newMsg);

	/// @获取到队首消息
	/// @para[in] 无
	/// @para[out] 返回队首指针
	CI2CPacket *front(void);

	/// @获取到队尾消息
	/// @para[in] 无
	/// @para[out] 返回队尾指针，但是不删除
	CI2CPacket *getBack(void);

	/// @取出队首消息
	/// @para[in] 无
	/// @para[out] 返回队首指针
	CI2CPacket *popFront(void);

	/// @获取队列大小			
	/// @para[in] 无
	/// @para[out] 队列长度			
	int getSize(void);

	/// @清除队列
	/// @para[in] 无
	/// @para[out] 0 成功 -1 失败	
	int clean(void);
private:
	std::queue<CI2CPacket*> m_msgQueue;
	pthread_mutex_t m_queueMutex;
	pthread_cond_t m_queueCond;
};

#endif

