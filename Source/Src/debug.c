#include "stm32f0xx_hal.h"
#include <stdbool.h>

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart1;
extern void HAL_Delay_Us(__IO uint32_t Delay);
	
	
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
bool Start=false; 

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
		     Start=true; 
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

bool m_init(void) 
{
    uint32_t  Count=0;

    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET);
    HAL_Delay_Us(120) ;
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET);
    HAL_Delay_Us(40) ;

    while(1)
    {
        if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4))
        {
            printf(" init successfully \r\n") ;
            return true ;
         }

         Count++;
         HAL_Delay_Us(1) ;
         if(Count>120)
         break;
     }


     return false ; 
}

bool s_init_v2(void)
{
    int i=0; 
    bool next=false; 

    if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10))
    {

        while(GPIO_PIN_RESET==HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10))
        {
	    i++ ; 
	    if(i>1000)
            {
                printf("i=%d \r\n",i);
                return false ;
            }
	}

        if(i>10&&i<100)
        {
                //printf("i=%d \r\n",i);
                next=true ;
        }

        if(next==true)
	{	 
            HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);
            HAL_Delay_Us(50) ;
//            printf("init v2 i=%d \r\n",i);
            return true ; 
        }
    }

    return false ; 

}

bool s_init(void)
{

    if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10))
    {

	    HAL_Delay_Us(30) ;

              if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10))
              {

                  HAL_Delay_Us(60) ;
                  if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10))
                  {
//		       printf("1\r\n");
                       HAL_Delay_Us(10) ;

                       if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10))
                       {
                            HAL_Delay_Us(40) ;

                            HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);

                            HAL_Delay_Us(100) ;

                            HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);

                            printf("responce to master \r\n");

                            return true ;

                        }
                   }
              }

     }

     return false; 

}


void send_a_byte(char data)
{
     unsigned char bit=0; 	
     	
     for(unsigned char i=0;i<8;i++)
     {
//	 printf("data=%d \r\n",data);
         bit=data&(0x01<<i) ;

         if(bit>0)
         {
             printf("send bit 1\r\n");
             HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET);
             HAL_Delay_Us(15) ;
             HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET);
             HAL_Delay_Us(45) ;
         }
	 else
         {
             printf("send bit 0\r\n");
	     HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET);
             HAL_Delay_Us(15) ;
             HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET);
             HAL_Delay_Us(45) ;
	 
	 }

         HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET);
         HAL_Delay_Us(3) ;

     }
}

char receive_v2(void)
{
    char data=0;
    uint32_t  Init_count=0;
    unsigned char i=0,j=0;
    unsigned char a[8]={0}; 

    if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10))
    {
         while(1)
         {		

         Init_count++;
         //HAL_Delay_Us(1) ;

         j=0;

         while(GPIO_PIN_RESET==HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10))
         {
            j++ ;
            if(j>1000)
            {
                printf("j=%d \r\n",j);
                return false ;
            }
         }
				 
//	    if(j>0)
//         {
//		   a[i]=j;
//		   printf("0 j=%d \r\n",j);
//                   data&=~(0x01<<i) ;
//                   i++;
//         }	   
         

         if(j>69&&j<120)
         {
		   a[i]=j;
//		   printf("0 j=%d \r\n",j);
                   data&=~(0x01<<i) ;
                   i++;
         }
         
	 if(j>0&&j<70)
         {
               a[i]=j; 
	       //    printf("1 j=%d \r\n",j);
                   data|=(0x01<<i);
                   i++;  
	 }

        // printf("data =%d \r\n",data);

         if(i==8)
         {
//             printf("0=%d,1=%d,2=%d,3=%d,4=%d,5=%d,6=%d,7=%d \r\n",a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7]);
//	     printf("data =0x%x \r\n",data);
             return data;
         }

         if(Init_count>50000)
         {
	      printf("e:0=%d,1=%d,2=%d,3=%d,4=%d,5=%d,6=%d,7=%d \r\n",a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7]);
              return '\n' ;
         }

	 }

    }

    return '\0' ;

}

char receive(bool On)
{
    char data=0; 
    uint32_t  Init_count=0;
    unsigned char i=0; 

    while(On==true)
    {
         Init_count++;
         HAL_Delay_Us(1) ;

	 if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10))
         {
                   HAL_Delay_Us(20) ;
                   if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10))
                   {
                       printf("read 0 \r\n");
                       data&=~(0x01<<i) ;
		       i++; 
                   }
                   else
                   {
                       printf("read 1 \r\n");
                       data|=(0x01<<i);
		       i++;
                   }
                   HAL_Delay_Us(40) ;

		   //printf("data =%d \r\n",data);
         }

        // printf("data =%d \r\n",data);

         if(i==8)
         {   
             printf("data =0x%x \r\n",data); 
             return data; 
	 }

         if(Init_count>500)
         {
              printf("time out after init data=0x%x \r\n",data);
              return ':' ; 
         }

    }

}


/*******************************************************************************
Description :                  Delay in uS.
Inputs :
Delay : Time in uS
outputs :
NONE
********************************************************************************/
void HAL_Delay_Us(__IO uint32_t Delay)
{
  for(int i=0;i<Delay*1;i++);
}

void test(void)
{
//    while(1)
//    {
//        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);
//	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);	
//	HAL_Delay_Us(1000000);
//    
//    }
  

    Change_Status();

    bool      Init_M=false ; 
    bool      Init_S=false ; 
    uint32_t  Count=0;  
    uint32_t  Init_count=0; 

    if(Host==true)
    { 
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET);
	HAL_Delay(2000) ;
    }

    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);

    if(Host==true)    
    {
        Init_M=m_init();

	HAL_Delay_Us(120) ;
	
	if(Init_M==true)
        {
              char *data_send="hello:" ;
              
              while((*data_send)!=NULL)
	      {
	          send_a_byte(*data_send);
	          data_send++; 
              }
        }

    }
    else
    {
         char data='\0';
	 char data2='\0'; 
         char data_one[20]={0}; 
	 char i=0;
         int  j=0; 


         while(1)
	 {
             data=receive_v2();
             
	    // if(data!='\0')
	    // printf("%c",data);
	     if(data!='\0')
             {		     
		 data_one[i]=data;
		 i++ ; 
		 j=0; 
             }
            
             if(data=='\n')
                 break; 

	     j++; 
	     if(j>5000)
		 break; 
         }

	 if(i>0) 
	 printf("i=%d %s ",i,data_one); 

    }


}









