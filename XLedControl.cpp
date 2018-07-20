
static CLEDTrans *hLed = NULL; 

int setCurrentLed(bool flag)
{
    uchar send_buf[16] = {0};
    uchar send_len = 16;
    int expTime = 5000;
	
    if (flag)
    {
    	//放入队列头，并清空队列，立即执行
    	printf("Clear Breast led fifo\n");
    	hLed->Breast_Reset();
    }
    printf("Breast expTime = %d\n", expTime);
    hLed->Breast_PushIn(0x11, send_buf, send_len, expTime);		

    return 0;
}


