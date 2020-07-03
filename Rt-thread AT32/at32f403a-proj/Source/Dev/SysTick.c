#include "config.h"

void SysTickInit(void)
{
  /*Config Systick*/
//  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	    SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);
    NVIC_SetPriority(SysTick_IRQn, 0);
}

/***************************************************************************************
***************************************end of file**************************************
***************************************************************************************/
