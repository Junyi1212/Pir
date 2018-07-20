#include "MsgQueue.h"

//-----------------------------------------
CI2CPacket::CI2CPacket(uchar addr, uchar* buf, uint len, int param1)
{
	slaveAddr = addr;
	dataLen = len;
	m_param1 = param1;

	if(dataLen > 0)
	{
		dataBuf = (uchar *)malloc(dataLen);
		if(NULL == dataBuf)
		{
			printf("malloc fail\n");
		}
		memcpy(dataBuf, buf, dataLen);
	}
	else
	{
		dataBuf = NULL;
	}
}

CI2CPacket::~CI2CPacket()
{
	if(NULL != dataBuf)
	{
		free(dataBuf);
		dataBuf = NULL;
	}
}

uchar CI2CPacket::Get_I2CAddr(void)
{
	return slaveAddr;
}

uchar* CI2CPacket::Get_I2CData(void)
{
	return dataBuf;
}

uint CI2CPacket::Get_I2CLen(void)
{
	return dataLen;
}

int CI2CPacket::Get_I2CParam1(void)
{
	return m_param1;
}
//-----------------------------------------
CXTMsgQueue::CXTMsgQueue()
:m_queueMutex(PTHREAD_MUTEX_INITIALIZER),
	m_queueCond(PTHREAD_COND_INITIALIZER)
{
}

CXTMsgQueue::~CXTMsgQueue()
{
}

int CXTMsgQueue::pushback(const CI2CPacket *newMsg)
{
    int iRet = -1;

    pthread_mutex_lock(&m_queueMutex);
    if ( m_msgQueue.size() < MAX_QUEUE_NUM)
    {
        m_msgQueue.push((CI2CPacket *)newMsg);
        iRet = 0;
    }
    else
    {
        printf("pushback Queue Full\n");
    }
    pthread_cond_signal(&m_queueCond);
    pthread_mutex_unlock(&m_queueMutex);
    return iRet;
}

CI2CPacket *CXTMsgQueue::front(void)
{
    CI2CPacket *pFront = NULL;
    pthread_mutex_lock(&m_queueMutex);
    if ( !m_msgQueue.empty() )
    {
        pFront = m_msgQueue.front();
    }
    else
    {
     
    }
    pthread_mutex_unlock(&m_queueMutex);
    return pFront;
}

CI2CPacket *CXTMsgQueue::popFront(void)
{
    CI2CPacket *pFront = NULL;
    pthread_mutex_lock(&m_queueMutex);
    if (!m_msgQueue.empty())
    {
        pFront = m_msgQueue.front();
        m_msgQueue.pop();
    }
    else
    {
    	pthread_cond_wait(&m_queueCond, &m_queueMutex);
        if (!m_msgQueue.empty())
        {
            pFront = m_msgQueue.front();
            m_msgQueue.pop();
        }		
    }
    pthread_mutex_unlock(&m_queueMutex);
    return pFront;
}

CI2CPacket *CXTMsgQueue::getBack(void)
{
    CI2CPacket *pBack = NULL;
    pthread_mutex_lock(&m_queueMutex);
    if (!m_msgQueue.empty())
    {
        pBack = m_msgQueue.back();
    }
    pthread_mutex_unlock(&m_queueMutex);
    return pBack;
}

int CXTMsgQueue::getSize(void)
{
    pthread_mutex_lock(&m_queueMutex);
    int size = m_msgQueue.size();
    pthread_mutex_unlock(&m_queueMutex);
    return size;
}

int CXTMsgQueue::clean(void)
{
    CI2CPacket* hNode;
    pthread_mutex_lock(&m_queueMutex);
    while ( !m_msgQueue.empty() )
    {
        hNode = m_msgQueue.front();
		if(NULL != hNode)
		{
			delete hNode;
			hNode = NULL;		
		}
		m_msgQueue.pop();
    }
    pthread_mutex_unlock(&m_queueMutex);
    return 0;
}
