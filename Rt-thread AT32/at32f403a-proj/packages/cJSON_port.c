#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <rtthread.h>

#include "cJSON.h"
#include "malloc.h"

int cJSON_hook_init(void)
{
    cJSON_Hooks cJSON_hook;

    cJSON_hook.malloc_fn = (void *(*)(size_t sz))my_malloc;  //修改malloc函数->my_malloc   不使用Rt-thread相关接口
    cJSON_hook.free_fn = my_free;                            //修改free函数->my_free   不使用Rt-thread相关接口

    cJSON_InitHooks(&cJSON_hook);

    return RT_EOK;
}
INIT_COMPONENT_EXPORT(cJSON_hook_init);
