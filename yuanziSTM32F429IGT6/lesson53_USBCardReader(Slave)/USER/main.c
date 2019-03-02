#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "string.h"
#include "sdram.h"
#include "malloc.h"
#include "w25qxx.h"
#include "string.h"
#include "sdio_sdcard.h"
#include "ftl.h"
#include "nand.h"
#include "usbd_msc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usb_conf.h"
#include "usbd_msc_bot.h"
#include "pcf8574.h"
/************************************************
 ALIENTEK ������STM32F429������ʵ��53
 USB������(Slave)ʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com  
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 �������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

USB_OTG_CORE_HANDLE USB_OTG_dev;
extern vu8 USB_STATUS_REG;		//USB״̬
extern vu8 bDeviceState;		//USB���� ���

int main(void)
{
//	u8 offline_cnt=0;
//	u8 tct=0;
//	u8 USB_STA;
//	u8 Divece_STA;
	
    Stm32_Clock_Init(384,25,2,8);   //����ʱ��,192Mhz   
    delay_init(192);                //��ʼ����ʱ����
    uart_init(115200);              //��ʼ��USART
//    LED_Init();                     //��ʼ��LED 
//    KEY_Init();                     //��ʼ������
    SDRAM_Init();                   //SDRAM��ʼ��
//    LCD_Init();                     //LCD��ʼ��
//	W25QXX_Init();				    //��ʼ��W25Q256
    PCF8574_Init();				    //��ʼ��PCF8574 
 	my_mem_init(SRAMIN);		    //��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);		    //��ʼ���ⲿ�ڴ��
	my_mem_init(SRAMCCM);		    //��ʼ��CCM�ڴ�� 

	if(SD_Init())
	{
		printf("���SD������\r\n");
//		LCD_ShowString(30,130,200,16,16,"SD Card Error!");	//���SD������
	}
	else //SD ������
	{   			
		printf("SD Card Successful!\r\n");
//		LCD_ShowString(30,130,200,16,16,"SD Card Size:     MB"); 
// 		LCD_ShowNum(134,130,SDCardInfo.CardCapacity>>20,5,16);	//��ʾSD������
 	}
//	if(W25QXX_ReadID()!=W25Q256)LCD_ShowString(30,130,200,16,16,"W25Q128 Error!");	//���W25Q128����
//	else //SPI FLASH ����
//	{   														 
//		LCD_ShowString(30,150,200,16,16,"SPI FLASH Size:25MB");	 
//	} 
//	if(FTL_Init())LCD_ShowString(30,170,200,16,16,"NAND Error!");	//���W25Q128����
//	else //NAND FLASH ����
//	{   														 
//		LCD_ShowString(30,170,200,16,16,"NAND Flash Size:    MB"); 
// 		LCD_ShowNum(158,170,nand_dev.valid_blocknum*nand_dev.block_pagenum*nand_dev.page_mainsize>>20,4,16);	//��ʾSD������
//	}  
// 	LCD_ShowString(30,190,200,16,16,"USB Connecting...");	//��ʾ���ڽ������� 	
	MSC_BOT_Data=mymalloc(SRAMIN,MSC_MEDIA_PACKET);			//�����ڴ�
	USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_MSC_cb,&USR_cb);
	delay_ms(1800);			    
	while(1)
	{
#if 0
		
//		printf("Hello USB!\r\n");
        delay_ms(1);				  
		if(USB_STA!=USB_STATUS_REG)//״̬�ı��� 
		{	 						   
//			LCD_Fill(30,210,240,210+16,WHITE);//�����ʾ			  	   
			if(USB_STATUS_REG&0x01)//����д		  
			{
				LED1=0;
				printf("USB Writeing...");
//				LCD_ShowString(30,210,200,16,16,"USB Writing...");//��ʾUSB����д������	 
			}
			if(USB_STATUS_REG&0x02)//���ڶ�
			{
				LED1=0;
				printf("USB Reading...");
//				LCD_ShowString(30,210,200,16,16,"USB Reading...");//��ʾUSB���ڶ�������  		 
			}	 										  
			if(USB_STATUS_REG&0x04)
			{
				printf("USB Write Err111");
//				LCD_ShowString(30,230,200,16,16,"USB Write Err ");//��ʾд�����
			}
		
			if(USB_STATUS_REG&0x08)
			{
				printf("USB Read Err222");
//				LCD_ShowString(30,250,200,16,16,"USB Read  Err ");//��ʾ��������
			}
			  
			USB_STA=USB_STATUS_REG;//��¼����״̬
		}
		if(Divece_STA!=bDeviceState) 
		{
			if(bDeviceState==1)
			{
				printf("USB Connected...");
//				LCD_ShowString(30,190,200,16,16,"USB Connected    ");//��ʾUSB�����Ѿ�����
			}
			else 
			{
				printf("USB DisConnected...");
//				LCD_ShowString(30,190,200,16,16,"USB DisConnected ");//��ʾUSB���γ���
			}
			Divece_STA=bDeviceState;
		}
		tct++;
//		if(tct==200)
//		{
//			tct=0;
//			LED1=1;
//			LED0=!LED0;//��ʾϵͳ������
//			if(USB_STATUS_REG&0x10)
//			{
//				offline_cnt=0;//USB������,�����offline������
//				bDeviceState=1;
//			}else//û�еõ���ѯ 
//			{
//				offline_cnt++;  
//				if(offline_cnt>10)bDeviceState=0;//2s��û�յ����߱��,����USB���γ���
//			}
//			USB_STATUS_REG=0;
//		}

#endif		
	}   //end if while(1)
}