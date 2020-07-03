#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <rtthread.h>

#include "cJSON.h"
#include "malloc.h"

int cJSON_hook_init(void)
{
    cJSON_Hooks cJSON_hook;

    cJSON_hook.malloc_fn = (void *(*)(size_t sz))my_malloc;  //�޸�malloc����->my_malloc   ��ʹ��Rt-thread��ؽӿ�
    cJSON_hook.free_fn = my_free;                            //�޸�free����->my_free   ��ʹ��Rt-thread��ؽӿ�

    cJSON_InitHooks(&cJSON_hook);

    return RT_EOK;
}
INIT_COMPONENT_EXPORT(cJSON_hook_init);
