#include "LedTrans.h"


CLEDTrans::CLEDTrans()
{
	int ret = 0;
	
	setLooping(true);
	ret = pthread_create(&m_threadBreast, NULL, &CLEDTrans::ThreadBreast, this);
	if (ret != 0)
	{
		printf("pthread_create err\n");
	}	

        pthread_mutex_init(&m_mutexBreast, NULL);
        pthread_cond_init(&m_condBreast, NULL);  		
}

CLEDTrans::~CLEDTrans()
{
	setLooping(false);
	pthread_join(m_threadBreast, NULL);
	pthread_mutex_destroy(&m_mutexBreast);	
	pthread_cond_destroy(&m_condBreast);	

	printf("~CLEDTrans done\n");	
}

void *CLEDTrans::ThreadBreast(void* arg)
{
	CLEDTrans* This = static_cast<CLEDTrans*>(arg);

	uchar recvBuf[64] = {0};
	uchar recvLen     = 0;
	int expDelay = 0;
	struct timespec ts;

	printf("Enter  ThreadBreast\n");
	while(This->Looping())
	{
		//线程处理
		CI2CPacket* hSend = This->breastMsg.popFront();
		if(NULL == hSend)
		{
			printf("Get Head Msg failed\n");
			continue;
		}		

		//Do something

		expDelay = hSend->Get_I2CParam1();//ms

		pthread_mutex_lock(&(This->m_mutexBreast));	
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += expDelay / 1000;//sec
		ts.tv_nsec += ( expDelay % 1000) * 1000000;//nsec
		if(ts.tv_nsec > NSECS_PER_SEC)
		{
			ts.tv_sec += 1;
			ts.tv_nsec %= NSECS_PER_SEC;
		}
		
		if(pthread_cond_timedwait(&(This->m_condBreast), &(This->m_mutexBreast), &ts))
		{
			/* 延时 */
			printf("Breast expDelay:%d\n", expDelay);
		}
		
		delete hSend;
		hSend = NULL;
		pthread_mutex_unlock(&(This->m_mutexBreast));
	}
	printf("Exit  ThreadBreast\n");
	return NULL;
}


int CLEDTrans::Breast_Reset(void)
{
	pthread_mutex_lock(&m_mutexBreast);
	pthread_cond_signal(&m_condBreast);
	pthread_mutex_unlock(&m_mutexBreast);

	breastMsg.clean();
	return RET_SUCCESS;
}

int CLEDTrans::Breast_PushIn(uchar addr, uchar* data, uchar inputlen, int expTime)
{
	CI2CPacket* hSend = new CI2CPacket(addr, data, inputlen, expTime);
	if(breastMsg.pushback(hSend))
	{
		printf("pushback breastMsg failed.\n");
		delete hSend;
		hSend = NULL;	
		return -1;
	}
	return 0;
}


