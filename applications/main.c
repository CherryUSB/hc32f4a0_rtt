/*
 * Copyright (C) 2022-2024, Xiaohua Semiconductor Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-04-28     CDT          first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <dfs_FS.h>


#define DBG_TAG               "MAIN"
#define DBG_LVL               DBG_INFO
#include <rtdbg.h>

/* defined the LED_GREEN pin: PB2 */
#define LED_GREEN_PIN GET_PIN(B, 2)


int main(void)
{
    int cnt = 0;
    /* set LED_GREEN_PIN pin mode to output */
    rt_pin_mode(LED_GREEN_PIN, PIN_MODE_OUTPUT);
  
    rt_err_t ret = RT_EOK;

    /* 设置日期 */
    ret = set_date(2024, 6, 6);
    if (ret != RT_EOK)
    {
        rt_kprintf("set RTC date failed\n");
        return ret;
    }
    /* 设置时间 */
    ret = set_time(12, 00, 00);
    if (ret != RT_EOK)
    {
        rt_kprintf("set RTC time failed\n");
        return ret;
    }

    /* 挂载sd卡 */
    while(cnt++ < 10)
    {
        rt_thread_mdelay(200);
        mkdir("/sdcard",0x777);
        if(dfs_mount("sd","/sdcard","elm",0,0)==0)
        {
            LOG_I("File System initialized");
            break;
        }
        else
        {
            LOG_E("File System init failed");
                      /* 格式化文件系统 */
            if (RT_EOK != dfs_mkfs("elm", "sd"))
            {
                LOG_E("Failed to Format fs!");
            }
        }
    }
    cnt = 0;


//    extern void cdc_acm_init(uint8_t busid, uint32_t reg_base);
//    cdc_acm_init(0, CM_USBFS_BASE);
//    
    extern void msc_storage_init(uint8_t busid, uint32_t reg_base);
    msc_storage_init(0, CM_USBFS_BASE);

    while (1)
    {
        rt_pin_write(LED_GREEN_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED_GREEN_PIN, PIN_LOW);
        rt_thread_mdelay(500);
//        extern void cdc_acm_data_send_with_dtr_test(uint8_t busid);
//        cdc_acm_data_send_with_dtr_test(0); 
    }
}

