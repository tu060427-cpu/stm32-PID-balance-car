#include "stm32f10x.h"                  // Device header

int32_t PID_calc(int16_t target_speed,int16_t actual_speed)
{
	static int32_t pid_out = 0;
	static float kp =3,ki = 0.8,kd = 0;//比例 积分 微分
	static int16_t error0 = 0,error1 = 0,error2 = 0;//本次误差，上次误差,上上次误差
//	static int32_t error_sum = 0;//误差积分
	int32_t p_out,i_out,d_out;
	
	error0 = target_speed - actual_speed;
	//计算p
//	p_out = (int32_t)(kp * error0);
	p_out = (int32_t)(kp * (error0 - error1));
	
	//抗饱和
//	if(pid_out < 1000 && pid_out > -1000)
//	{
//		error_sum += error0;
//	}
	
	//积分限幅
//	if(error_sum > 5000)error_sum = 5000;
//	if(error_sum < -5000)error_sum = -5000;
	
	//计算i,d
//	i_out = (int32_t)(ki * error_sum);
//	d_out = (int32_t)(kd *(error0 - error1));
	i_out = (int32_t)(ki * error0);
	d_out = (int32_t)(kd * (error0 - 2 * error1 + error2));
	
	int32_t delta_u = p_out + i_out + d_out;
	if((pid_out >= 1000 && delta_u > 0) || (pid_out <= -1000 && delta_u < 0))
	{
	}
	else
	{
		pid_out += delta_u;//PID运算
	}

	
	//输出CRR限幅
	if(pid_out > 1000)pid_out = 1000;
	else if(pid_out < -1000)pid_out = -1000;
	
	//更新误差
	error2 = error1;
	error1 = error0;
	return pid_out;
}
