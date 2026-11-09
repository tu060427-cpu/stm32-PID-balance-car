#ifndef __USART_H
#define __USART_H
#include <stdio.h>


void USART1_Init(void);
int fputc(int ch,FILE *f);
void print_wave(int16_t target,int16_t actual,int32_t out);

#endif
