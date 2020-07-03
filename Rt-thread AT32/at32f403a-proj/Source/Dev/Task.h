#ifndef  __TASK_H__
#define  __TASK_H__

#include "config.h"

/**********************************开发板LED灯引脚宏定义，使用PIN设备获取引脚编号*****************************************/
/* defined the LED2 pin: PD13 */
#define LED2_PIN    GET_PIN(D, 13)
/* defined the LED3 pin: PD14 */
#define LED3_PIN    GET_PIN(D, 14)
/* defined the LED4 pin: PD15 */
#define LED4_PIN    GET_PIN(D, 15)
/***********************************************结束*************************************************************/


typedef struct
{
   const char *name;
	 void (*entry)(void *parameter);
	 void       *parameter;
	 rt_uint32_t stack_size;
	 rt_uint8_t  priority;
	 rt_uint32_t tick;
}TaskStruct;

void TaskInit(void);

#endif

