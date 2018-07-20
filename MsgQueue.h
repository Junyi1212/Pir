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


///��Ϣ���У��������ڴ�������ע��
class CXTMsgQueue
{
public:
    CXTMsgQueue();
    ~CXTMsgQueue();

    /// @��ȡ���Ͷ���ָ��
	/// @para[in] newMsg ����Ϣ
	/// @para[out] 0 �ɹ� -1 ʧ��
	int pushback(const CI2CPacket *newMsg);

	/// @��ȡ��������Ϣ
	/// @para[in] ��
	/// @para[out] ���ض���ָ��
	CI2CPacket *front(void);

	/// @��ȡ����β��Ϣ
	/// @para[in] ��
	/// @para[out] ���ض�βָ�룬���ǲ�ɾ��
	CI2CPacket *getBack(void);

	/// @ȡ��������Ϣ
	/// @para[in] ��
	/// @para[out] ���ض���ָ��
	CI2CPacket *popFront(void);

	/// @��ȡ���д�С			
	/// @para[in] ��
	/// @para[out] ���г���			
	int getSize(void);

	/// @�������
	/// @para[in] ��
	/// @para[out] 0 �ɹ� -1 ʧ��	
	int clean(void);
private:
	std::queue<CI2CPacket*> m_msgQueue;
	pthread_mutex_t m_queueMutex;
	pthread_cond_t m_queueCond;
};

#endif

