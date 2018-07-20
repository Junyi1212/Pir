
static CLEDTrans *hLed = NULL; 

int setCurrentLed(bool flag)
{
    uchar send_buf[16] = {0};
    uchar send_len = 16;
    int expTime = 5000;
	
    if (flag)
    {
    	//�������ͷ������ն��У�����ִ��
    	printf("Clear Breast led fifo\n");
    	hLed->Breast_Reset();
    }
    printf("Breast expTime = %d\n", expTime);
    hLed->Breast_PushIn(0x11, send_buf, send_len, expTime);		

    return 0;
}


