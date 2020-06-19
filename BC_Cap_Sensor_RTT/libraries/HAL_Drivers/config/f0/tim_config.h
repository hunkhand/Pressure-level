/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-12-24     zylx         first version
 */

#ifndef __TIM_CONFIG_H__
#define __TIM_CONFIG_H__

#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TIM_DEV_INFO_CONFIG
#define TIM_DEV_INFO_CONFIG                     \
    {                                           \
        .maxfreq = 1000000,                     \
        .minfreq = 2000,                        \
        .maxcnt  = 0xFFFF,                      \
        .cntmode = HWTIMER_CNTMODE_UP,          \
    }
#endif /* TIM_DEV_INFO_CONFIG */
    
#ifdef BSP_USING_TIM2
#ifndef TIM2_CONFIG
#define TIM2_CONFIG                             \
    {                                           \
       .tim_handle.Instance     = TIM2,         \
       .tim_irqn                = TIM2_IRQn,    \
       .name                    = "timer2",     \
    }
#endif /* TIM2_CONFIG */
#endif /* BSP_USING_TIM2 */
    
#ifdef BSP_USING_TIM3
#ifndef TIM3_CONFIG
#define TIM3_CONFIG                             \
    {                                           \
       .tim_handle.Instance     = TIM3,         \
       .tim_irqn                = TIM3_IRQn,    \
       .name                    = "timer3",     \
    }
#endif /* TIM3_CONFIG */
#endif /* BSP_USING_TIM3 */

#ifdef BSP_USING_TIM14
#ifndef TIM14_CONFIG
#define TIM14_CONFIG                             \
    {                                           \
       .tim_handle.Instance     = TIM14,         \
       .tim_irqn                = TIM14_IRQn,    \
       .name                    = "timer14",     \
    }
#endif /* TIM14_CONFIG */
#endif /* BSP_USING_TIM14 */

#ifdef BSP_USING_TIM16
#ifndef TIM16_CONFIG
#define TIM16_CONFIG                             \
    {                                           \
       .tim_handle.Instance     = TIM16,         \
       .tim_irqn                = TIM16_IRQn,    \
       .name                    = "timer16",     \
    }
#endif /* TIM16_CONFIG */
#endif /* BSP_USING_TIM16 */

#ifdef BSP_USING_TIM17
#ifndef TIM17_CONFIG
#define TIM17_CONFIG                             \
    {                                           \
       .tim_handle.Instance     = TIM17,         \
       .tim_irqn                = TIM17_IRQn,    \
       .name                    = "timer17",     \
    }
#endif /* TIM17_CONFIG */
#endif /* BSP_USING_TIM17 */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_CONFIG_H__ */
