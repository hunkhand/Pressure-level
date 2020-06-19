/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     zylx         first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "Picocap_app.h"
#include "dac_bsp.h"

int main(void)
{
    rt_device_t                 pcap_device = RT_NULL;
    rt_device_t                 dac_device  = RT_NULL;
    struct rt_pcap_device_obj   *pcap_device_obj;
    rt_uint32_t                 FilterResult;           //�˲���Ľ��
    
    pcap_device = rt_device_find(PCAP_DEVICE_NAME);
    if(pcap_device != RT_NULL)
    {
        pcap_device_obj = (struct rt_pcap_device_obj *)pcap_device->user_data;
        rt_device_open(pcap_device, RT_DEVICE_OFLAG_RDWR);
    }
    else
    {
        goto __dev_error;
    }
    
    dac_device = rt_device_find(DAC_DEVICE_NAME);
    if(dac_device != RT_NULL)
    {
        rt_device_open(dac_device, RT_DEVICE_OFLAG_WRONLY);
    }
    else
    {
        goto __dev_error;
    }        

    while (1)
    {
        if(rt_device_read(pcap_device, RESULT_REG1_ADDR, 
                    &pcap_device_obj->PCap_DataConvert_Out.PCap_ResultValue, 1) != 0)
        {
            //�����˲����ж��Ƿ�ɹ�
            if(Sensor_DataFilter(&pcap_device_obj->DataFilter, 
                                pcap_device_obj->PCap_DataConvert_Out.PCap_ResultValue, 
                                &FilterResult) == OP_SUCCESS)
            {
                //��ֵת��
                Sensor_PCap_DataConvert(&pcap_device_obj->PCap_DataConvert, 
                                        FilterResult, 
                                        &pcap_device_obj->PCap_DataConvert_Out);
                //DAת��,ʹ��ͨ��2
                if(pcap_device_obj->PCap_DataConvert.CapDA_ClibEn == DAOUTCLIB_ENABLE)
                {                   
                    rt_device_write(dac_device, 
                                    DA_CHANNEL_2, 
                                    &pcap_device_obj->PCap_DataConvert_Out.PCapDA_ResultValue, 
                                    1);
                }
                else
                {
                    rt_device_write(dac_device, 
                                    DA_CHANNEL_2, 
                                    &pcap_device_obj->PCap_DataConvert_Out.PCapDA_OutValue, 
                                    1);
                }
                
            }
        }
        rt_thread_mdelay(PCAP_COLLECT_CYCLE);
    }
__dev_error:
    while(1)
    {
        rt_thread_mdelay(1000);
    }
}

#define WDT_DEVICE_NAME     "wdt"    /* ���Ź��豸���� */
#define WDT_TIMEOUT         1000    /* ��ʱʱ��,��λ���� */

static rt_device_t wdg_dev;         /* ���Ź��豸��� */

static void idle_hook(void)
{
    /* �ڿ����̵߳Ļص�������ι�� */
    rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_KEEPALIVE, NULL);
}

static int wdt_init(void)
{
    rt_err_t ret = RT_EOK;
    rt_uint32_t timeout = WDT_TIMEOUT * (LSI_VALUE / 1000) / 32;    /* ���ʱ�� */

    /* �����豸���Ʋ��ҿ��Ź��豸����ȡ�豸��� */
    wdg_dev = rt_device_find(WDT_DEVICE_NAME);
    if (wdg_dev != RT_NULL)
    {
        /* ��ʼ���豸 */
        ret = rt_device_init(wdg_dev);
        if (ret != RT_EOK)
        {
            return RT_ERROR;
        }
        /* ���ÿ��Ź����ʱ�� */
        ret = rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_SET_TIMEOUT, &timeout);
        if (ret != RT_EOK)
        {
            return RT_ERROR;
        }
        /* ���ÿ����̻߳ص����� */
        rt_thread_idle_sethook(idle_hook);
    }                

    return ret;
}
INIT_APP_EXPORT(wdt_init);
