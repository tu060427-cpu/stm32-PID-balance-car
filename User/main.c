#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "motor.h"
#include "encoder.h"
#include "Key.h"
#include "timer.h"
#include "LED.h"
#include <stdio.h>
#include "USART.h"

uint8_t key_num = 0;//1加速，2减速
int16_t speed = 0;//CCR
extern int16_t actual_speed;//CNT(40ms)
extern int16_t target_speed;
extern int32_t pid_out;
extern uint8_t print_flag;

int main(void)
{
	OLED_Init();
	motor_Init();
	Key_Init();
	encoder_Init();
	timer_Init();
	timer4_Init();
	LED_Init();
	USART1_Init();
	
	OLED_ShowString(1,1,"CCR:");
	OLED_ShowString(2,1,"CNT:");
	OLED_ShowString(3,1,"speed:");
	OLED_ShowString(4,1,"PWM:");
	
	while (1)
	{
		OLED_ShowSignedNum(2,5,actual_speed,4);
		if(key_num == 1)//加速
		{
			target_speed += 10;
			if(target_speed > 60)target_speed = 60;
			
			key_num = 0;
		}
		if(key_num == 2)//减速
		{
			target_speed -= 10;
			if(target_speed < - 60)target_speed = - 60;
			
			key_num = 0;
		}
		OLED_ShowSignedNum(1,5,pid_out,4);
		OLED_ShowSignedNum(4,5,pid_out / 10,4);
		OLED_ShowSignedNum(3,8,target_speed,4);
		if(print_flag == 1)
		{
			print_flag = 0;
			print_wave(target_speed,actual_speed,pid_out);
		}

		Delay_ms(20);
	}
}
