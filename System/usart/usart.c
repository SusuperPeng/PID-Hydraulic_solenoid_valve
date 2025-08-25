#include "usart.h"
#include "stdio.h"      //������Ҫ��ͷ�ļ�
#include "stdarg.h"		//������Ҫ��ͷ�ļ� 
#include "string.h"     //������Ҫ��ͷ�ļ�

#if USART1_RX_ENABLE	
char USART1_RX_FLAG = 0;//������ɱ�־λ
unsigned int USART1_RX_LENG = 0;//�����ֽ���
char USART1_RXBUFF[USART1_RX_SIZE] = {0};
#endif

/****************************************************************************
�������ƣ�void Usart1_Init(u32 bound)
����������u32 bound  ������
��������ֵ����
���������ڳ�ʼ��  PA9:TX   PA10 RX
****************************************************************************/
void Usart1_Init(u32 bound)
{
    // 1. ����ṹ��
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
#if USART1_RX_ENABLE
    NVIC_InitTypeDef NVIC_InitStruct;
#endif

    // 2. ����ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    // GPIOAʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);   // USART1ʱ��

    // 3. ���� PA9 (TX) -> �����������
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 4. ���� PA10 (RX) -> ��������
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 5. USART ����
    USART_InitStruct.USART_BaudRate            = bound;                     // ������
    USART_InitStruct.USART_WordLength          = USART_WordLength_8b;      // 8λ����
    USART_InitStruct.USART_StopBits            = USART_StopBits_1;         // 1λֹͣλ
    USART_InitStruct.USART_Parity              = USART_Parity_No;          // ����żУ��
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ��Ӳ������
#if USART1_RX_ENABLE
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;           // ���� + ����
#else
    USART_InitStruct.USART_Mode = USART_Mode_Tx;                           // ������
#endif
    USART_Init(USART1, &USART_InitStruct);         // ��ʼ������
    USART_Cmd(USART1, ENABLE);                     // ʹ�ܴ���

#if USART1_RX_ENABLE
    // 6. �����ж����ȼ�
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // �ж����ȼ�������

    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    // 7. �������ڽ����ж�
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // ʹ�ܽ����ж�
#endif
}
/****************************************************************************
�������ƣ�void usart1_printf(char *fmt,...)
�����������ɱ䴫��
��������ֵ����
��������ʽ������ַ�������
****************************************************************************/
__align(8) char USART1_TXBUFF[USART1_TX_SIZE];		// ���ͻ�������8�ֽڶ�������DMAЧ�ʣ��罫��ʹ�ã�

void u1_printf(char *fmt,...)
{
	//��������������
	//length����������Ҫ�����ַ����ĳ��ȡ�
	//i������ѭ������ÿһ���ַ���
	unsigned int length = 0,i = 0;
	
	va_list ap;  //���������б�
	va_start(ap,fmt);//ʹ�ò���fmt��ʼ��ap�б�
	vsprintf(USART1_TXBUFF,fmt,ap);
	va_end(ap);//������Ҫ��
	
	length = strlen((const char *)USART1_TXBUFF);
	while((USART1->SR & 0x40)== 0);//������ɱ�־λ 0 δ���
	for(i = 0;i < length;i++)
	{
		USART1->DR = USART1_TXBUFF[i];
		while((USART1->SR & 0x40)== 0);
	}
	
}
/***************************************************
�������ƣ�USART1_IRQHandler
����������1�����жϴ���
***************************************************/
void USART1_IRQHandler(void)
{
#if USART1_RX_ENABLE
    if ( (USART1_RX_FLAG == 0) && (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) )
    {
        char rx = USART_ReceiveData(USART1);  // ��ȡ��������

        // ���ջ������洢
        if (USART1_RX_LENG < USART1_RX_SIZE)
        {
            USART1_RXBUFF[USART1_RX_LENG++] = rx;

            // ��ѡ���������жϽ����������� '\n'����Ϊ������ɱ�־
            if (rx == 0x0A) // �ɸ���Э�黻�� '\r' �� 0x0A/0x0D           
                USART1_RX_FLAG = 1;  // ���ý�����ɱ�־           
        }
        else       
            USART1_RX_LENG = 0;  // ����������������½���
        
		
		//���ÿ����ж϶�ȡ��Ϊ������ɱ�־���������ֶ��ĵ�������
		//���ܲ���USART_ClearITPendingBit(USART3, USART_IT_IDLE); // ? ��Ч��������� IDLE �ж�
//		if(USART_GetITStatus(USART3,USART_IT_IDLE) != RESET)
//		{
//		a=USART3->SR;  		//USART_IT_IDLE �����жϱ�־�������,�����һֱ���������ж�
//		a=USART3->DR;		//��־λ����
//		USART1_RX_FLAG = 1;
//		}
		
		
        // ����жϱ�־
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
#endif
}

/*  	
	if(USART1_RX_FLAG == 1)
	{		
		u1_printf("���յ��������ǣ�%s".USART1_RXBUFF);
		
		if(USART3_Rcv_Buff[2] == 'A')//ǰ
		{
			//���ܺ���	
		}
		

		USART1_RX_FLAG=0;
		memset(USART3_Rcv_Buff,0,256);
		USART1_RX_LENG=0;
	}

*/



