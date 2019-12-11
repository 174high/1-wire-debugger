#include "stm32f0xx_hal.h"
#include <stdbool.h>

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart1;

#if  KEIL

int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
    osDelay(1);
    return 0;
}

#else

int __io_putchar(int ch)
{
        uint8_t c[1];
        c[0] = ch & 0x00FF;
        HAL_UART_Transmit(&huart1, &*c, 1, 10);
        return ch;
}

int _write(int file,char *ptr, int len)
{
        int DataIdx;
        for(DataIdx= 0; DataIdx< len; DataIdx++)
        {
                __io_putchar(*ptr++);
        }
        return len;
}

#endif

bool Host=false ;

void Change_Status(void)
{

    if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13))
    {
	 HAL_Delay(300);

         if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13))
         {              
	     printf("press \r\n");
             HAL_Delay(1000);
	     printf("time out \r\n");
             if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13))
             {
	         printf("change \r\n");
                 if(true==Host)
	         { 
		     Host=false  ;
                     HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
	         }
                 else
	         {
                     Host=true ;
	         }

		 while(GPIO_PIN_RESET==HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13))
			 ;
              }

	 }
    }

    if(Host==true)
    {
//	printf("toggling \r\n") ;
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        HAL_Delay(100);
    }



}


void test(void)
{

    Change_Status();

    




}









