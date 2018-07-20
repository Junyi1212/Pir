#ifndef __LED_TRANS_H__
#define __LED_TRANS_H__

#include <pthread.h>

class CLEDTrans
{
public:
	CLEDTrans();
	~CLEDTrans();	
	int Breast_Reset(void);
	int Breast_PushIn(uchar addr, uchar* data, uchar inputlen, int expTime);

private:
	static void *ThreadBreast(void *arg);	
	bool Looping(){return m_threadLoop;}
	void setLooping(bool bLoop){m_threadLoop = bLoop;}
	
private:
	pthread_t  m_threadBreast;
	pthread_mutex_t m_mutexBreast;
	pthread_cond_t m_condBreast;	

	bool m_threadLoop;
	CXTMsgQueue breastMsg;
};

#endif

