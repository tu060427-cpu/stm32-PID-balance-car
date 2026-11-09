#include "stm32f10x.h"                  // Device header
#include "encoder.h" 
#include "PID.h"
#include "motor.h"

extern uint8_t key_num;
int16_t target_speed = 0;
int16_t actual_speed = 0;
int32_t pid_out = 0;
extern uint8_t sum;
uint8_t print_flag = 0;
void timer_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	TIM_InternalClockConfig(TIM1);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM1,ENABLE);
}

void TIM1_UP_IRQHandler(void)
{
	static uint8_t count = 0;
	static uint8_t Key_state = 1;//1松开,0按下
	if(TIM_GetITStatus(TIM1,TIM_IT_Update) == SET)
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14) == 1)
		{	
			if(Key_state == 1)
			{
				count++;
				if(count >=19)
				{
					Key_state = 0;
					count = 0;
					key_num = 1;
				}
			}	
		}
		else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8) == 1)
		{	
			if(Key_state == 1)
			{
				count++;
				if(count >=19)
				{
					Key_state = 0;
					count = 0;
					key_num = 2;
				}
			}	
		}
		else
		{
			if(Key_state == 0)
			{
				Key_state = 1;
				count = 0;
			}
		}
		
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
			
	}
	
}



void timer4_Init(void)
{
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	TIM_InternalClockConfig(TIM4);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM4,ENABLE);

}




void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) == SET)
	{
		static uint8_t cnt = 0;
		
		actual_speed = encoder_get();
		pid_out = PID_calc(target_speed,actual_speed);
		motor_setspeed(pid_out);
		
		cnt++;
		if(cnt >= 5)
		{
			cnt = 0;
			print_flag = 1;
		}
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	}
}
