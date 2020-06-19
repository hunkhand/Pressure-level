/**@file        ModbusRtu.h
* @details      ModbusRtu.c��ͷ�ļ�,������Modbus RTU ͨ�ŵ�API����
* @author       ���
* @date         2020-04-29
* @version      V1.0.0
* @copyright    2020-2030,��������Ϊ�Ƽ���չ���޹�˾
**********************************************************************************
* @par �޸���־:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2020/04/29  <td>1.0.0    <td>���    <td>������ʼ�汾
* </table>
*
**********************************************************************************
*/

#ifndef __MODBUSRTU_H
#define __MODBUSRTU_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "usart_app.h"
#include "ModBus_Conf.h"


#define SEND_SIZE                       32              ///< ���ͻ����С

#ifndef RECEIVE_SIZE
#define RECEIVE_SIZE                    32              ///< ���ջ���Ĵ�С
#endif

#define AUTOUPLOAD_CYCLE                1000            ///< �Զ��ϴ�����,��λ ms

#define BROADCASTADDR                   0x00                                  ///< �㲥��ַ

#define HOLDING_REG_REGION1_BGEIN       0x0030                                ///< ���ּĴ�����ʼ��ַ1
#define HOLDING_REG_REGION1_END         0x0041                                ///< ���ּĴ���������ַ1

#define INPUT_REG_REGION1_BEGIN         0x0000                                ///< ����Ĵ�����ʼ��ַ1
#define INPUT_REG_REGION1_END           0x0006                                ///< ����Ĵ���������ַ1
#define INPUT_REG_REGION2_BEGIN         0x0080                                ///< ����Ĵ�����ʼ��ַ2
#define INPUT_REG_REGION2_END           0x0080                                ///< ����Ĵ���������ַ2

#define SINGLE_COIL_REGION1_BEGIN       0x0050                                ///< ������Ȧ�Ĵ�����ʼ��ַ1
#define SINGLE_COIL_REGION1_END         0x0053                                ///< ������Ȧ�Ĵ���������ַ1

#define SPECIAL_COIL_REGION1_BEGIN      0x0000                                ///< ����Ĵ�����ʼ��ַ1
#define SPECIAL_COIL_REGION1_END        0x0005                                ///< ����Ĵ���������ַ1

#define MUL_REG_REGION1_BEGIN           0x0030                                ///< ����Ĵ�����ʼ��ַ1
#define MUL_REG_REGION1_END             0x0041                                ///< ����Ĵ���������ַ1

#define _25_FNUC_REG_REGION1_BEGIN      0x0000                                ///< �Զ���25������Ĵ�����ʼ��ַ
#define _25_FNUC_REG_REGION1_END        0x0009                                ///< �Զ���25������Ĵ���������ַ

#define DEF_MUL_REG_REGION1_BEGIN       0x0080                                ///< �Զ���26/27������Ĵ�����ʼ��ַ1
#define DEF_MUL_REG_REGION1_END         0x00A2                                ///< �Զ���26/27������Ĵ���������ַ1

#define MUL_VERSION_INF_BEGIN           0x00E0                                ///< �汾��Ϣ��ʼ��ַ
#define MUL_VERSION_INF_END             0x00E5                                ///< �汾��Ϣ������ַ

#define MB_NONE_EXCEPTION               0x00                                  ///< Modbus���쳣
#define MB_FUNC_EXCEPTION               0x01                                  ///< Modbus��������Ч
#define MB_ADDR_EXCEPTION               0x02                                  ///< Modbus���ʵ�ַʧ��
#define MB_VALU_EXCEPTION               0x03                                  ///< Modbus������ֵ��Ч
#define MB_DEVC_EXCEPTION               0x04                                  ///< Modbus�����豸ʧ��
#define MB_ACKN_EXCEPTION               0x05                                  ///< Modbus�����豸��Ӧ��
#define MB_BUSY_EXCEPTION               0x06                                  ///< Modbus�����豸æ
#define MB_PARI_EXCEPTION               0x08                                  ///< Modbus�豸У�����
#define MB_REQ_FAILURE                  0x80                                  ///< Modbus�������

#define CALIB_CAPMIN_FLAG               0x01                                  ///< �궨��������־
#define CALIB_CAPMAX_FLAG               0x10                                  ///< �궨�������ȱ�־
#define CALIB_CAPEOC_FLAG               0x11                                  ///< �����ݽ�����־

#define CALIB_CAPADMIN_FLAG             0x01                                  ///< �궨����AD����־
#define CALIB_CAPADLOW_FLAG             0x02                                  ///< �궨����AD�¿̶ȱ�־
#define CALIB_CAPADHIH_FLAG             0x10                                  ///< �궨����AD�Ͽ̶ȱ�־
#define CALIB_CAPADMAX_FLAG             0x20                                  ///< �궨����AD���ȱ�־
#define CALIB_CAPADEOC_FLAG             0x33                                  ///< �궨����AD������־

#define CALIB_CAPDAMIN_FLAG             0x01                                  ///< �궨����DA����־
#define CALIB_CAPDALOW_FLAG             0x02                                  ///< �궨����DA�¿̶ȱ�־
#define CALIB_CAPDAHIH_FLAG             0x10                                  ///< �궨����DA�Ͽ̶ȱ�־
#define CALIB_CAPDAMAX_FLAG             0x20                                  ///< �궨����DA���ȱ�־
#define CALIB_CAPDAEOC_FLAG             0x33                                  ///< �궨����DA������־

#define CALIB_TEMPDAMIN_FLAG            0x01                                  ///< �궨�¶�DA����־
#define CALIB_TEMPDAMAX_FLAG            0x10                                  ///< �궨�¶�DA���ȱ�־
#define CALIB_TEMPDAEOC_FLAG            0x11   

///< �����豸
#define FREEZE_ENABLE                   0x0F
///< �ⶳ�豸
#define FREEZE_DISABLE                  0x00

#define ADCLIB_ENABLE                   0x0F
#define ADCLIB_DISABLE                  0x00

#define NO_CHECK_ADDRESS                0
#define CHECK_ADDRESS                   1
    
    
/** ModBus����/���մ����ṹ�� */
typedef struct {
    uint8_t *Send_Buf;                  ///< ModBus���ͻ���ָ��
    uint16_t Send_Len;                  ///< ModBus���ͻ��泤��
    uint8_t *Receive_Buf;               ///< ModBus���ջ���ָ��
}ModBus_TX_RX_TypeDef;

/** ModBus�����Ļ��������ṹ */
typedef struct _ModBusBaseParam_TypeDef {
    uint8_t Device_Addr;                ///< �豸��ַ
    uint8_t BaudRate;                   ///< ���ڲ����ʴ���
    uint8_t Parity;                     ///< ������żУ�����
    uint8_t AutoUpload;                 ///< �Զ��ϴ�����
    uint8_t Output_Mode;                ///< ���ģʽ
    uint8_t Factory_Reset;              ///< �ָ���������
    uint8_t Freeze;                     ///< �豸����
    uint8_t InRomWrEn;                  ///< �ڲ��洢��дʹ��
    ModBus_TX_RX_TypeDef ModBus_TX_RX;  ///< ModBus����/���մ����ṹ��
#ifdef USING_RT_THREAD_OS
    rt_sem_t TX_Lock;                   ///< ������
#endif
    /** ModBus�ص����� */
    int     (*ModBus_CallBack) (struct _ModBusBaseParam_TypeDef *arg);
}ModBusBaseParam_TypeDef;


#ifdef __IN_FLASH_APP_H

/**@brief       ModBus��ʼ��
* @param[in]    ModBusBaseParam : ModBus�����Ļ��������ṹ��
* @return       ����ִ�н��
* - None
*/
void ModBus_Init(ModBusBaseParam_TypeDef *ModBusBaseParam);
#else

/**@brief       ModBus��ʼ��
* @param[in]    ModBusBaseParam : ModBus�����Ļ��������ṹ��
* @param[in]    Param : ModBus��ʼ������������
* @param[in]    Param_Size : Param�����ݳ���
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
* @note         Param����������(���ֽ�˳������): 0,�豸��ַ; 
* 1,�����ʴ���; 2��żУ�����; 3�Զ��ϴ�����; 4,���ģʽ;
* 5,�豸�����־;
*/
uint8_t ModBus_Init(ModBusBaseParam_TypeDef *ModBusBaseParam,
                uint8_t *Param, uint16_t Param_Size);
#endif

/**@brief       ���������Ϣ֡����CRC16У��
* @param[in]    Msg : ��Ϣ�׵�ַ;
* @param[in]    MsgLen : ��Ϣ֡����;
* @return       ����ִ�н��
* - CRC16У����
* @note         ��Ϣ���Ȳ���Ϊ0
*/
uint16_t MDB_Get_CRC16(uint8_t *Msg, uint16_t MsgLen);

/**@brief       ���� Modbus RTU��Ϣ֡
* @param[in]    ModBusBaseParam : ModBus�����Ļ��������ṹ��;
* @param[in]    Msg : ��Ϣ�׵�ַ; 
* @param[in]    MsgLen : ��Ϣ֡����;
* @param[in]    Check_Addr : �Ƿ����豸��ַ��Ч
* @return       ����ִ�н��
* - None
* @note         ��Ϣ���Ȳ���Ϊ0������ʱ�Ƚⶳ�豸
*/
void MDB_Snd_Msg_RTU(ModBusBaseParam_TypeDef *ModBusBaseParam, uint8_t *pMsg, uint16_t len, uint8_t Check_Addr);

/**@brief       ��������
* @param[in]    ModBusBaseParam : ModBus�����Ļ��������ṹ��;
* @param[in]    Msg : ��Ϣ�׵�ַ;
* @param[in]    MsgLen : ��Ϣ֡����;
* @return       ����ִ�н��
* - None
*/
void Send_Data(ModBusBaseParam_TypeDef *ModBusBaseParam, uint8_t *pMsg, uint16_t len);

/**@brief       Modbus RTU��Ϣ֡��������
* @param[in]    ModBusBaseParam : ModBus�����Ļ��������ṹ��;
* @param[in]    arg : �û��Զ���Ĳ���,�����豸����
* @return       ����ִ�н��
* - None
*/
void ModbusHandle(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg);

/**@brief       Modbus 03��������Ϣ֡����
* @param[in]    ModBusBaseParam : ModBus�����Ļ��������ṹ��;
* @param[in]    arg : �û��Զ���Ĳ���,�����豸����
* @return       ����ִ�н��
* - None
*/
void ModbusFunc03(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg);

/**@brief       Modbus 04��������Ϣ֡����
* @param[in]    ModBusBaseParam : ModBus�����Ļ��������ṹ��;
* @param[in]    arg : �û��Զ���Ĳ���,�����豸����
* @return       ����ִ�н��
* - None
*/
void ModbusFunc04(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg);

/**@brief       Modbus 05��������Ϣ֡����
* @param[in]    ModBusBaseParam : ModBus�����Ļ��������ṹ��;
* @param[in]    arg : �û��Զ���Ĳ���,�����豸����
* @return       ����ִ�н��
* - None
*/
void ModbusFunc05(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg);

/**@brief       Modbus 10��������Ϣ֡����
* @param[in]    ModBusBaseParam : ModBus�����Ļ��������ṹ��;
* @param[in]    arg : �û��Զ���Ĳ���,�����豸����
* @return       ����ִ�н��
* - None
*/
void ModbusFunc10(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg);

/**@brief       Modbus 25��������Ϣ֡����
* @param[in]    ModBusBaseParam : ModBus�����Ļ��������ṹ��;
* @param[in]    arg : �û��Զ���Ĳ���,�����豸����
* @return       ����ִ�н��
* - None
*/
void ModbusFunc25(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg);

/**@brief       Modbus 26��������Ϣ֡����
* @param[in]    ModBusBaseParam : ModBus�����Ļ��������ṹ��;
* @param[in]    arg : �û��Զ���Ĳ���,�����豸����
* @return       ����ִ�н��
* - None
*/
void ModbusFunc26(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg);

/**@brief       Modbus 27��������Ϣ֡����
* @param[in]    ModBusBaseParam : ModBus�����Ļ��������ṹ��;
* @param[in]    arg : �û��Զ���Ĳ���,�����豸����
* @return       ����ִ�н��
* - None
*/
void ModbusFunc27(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg);

/**@brief       Modbus 2A��������Ϣ֡����
* @param[in]    ModBusBaseParam : ModBus�����Ļ��������ṹ��;
* @return       ����ִ�н��
* - None
*/
void ModbusFunc2A(ModBusBaseParam_TypeDef *ModBusBaseParam);

/**@brief       Modbus 2B��������Ϣ֡����
* @param[in]    ModBusBaseParam : ModBus�����Ļ��������ṹ��;
* @return       ����ִ�н��
* - None
*/
void ModbusFunc2B(ModBusBaseParam_TypeDef *ModBusBaseParam);

/**@brief       Modbus 41��������Ϣ֡����
* @param[in]    ModBusBaseParam : ModBus�����Ļ��������ṹ��;
* @return       ����ִ�н��
* - None
*/
void ModbusFunc41(ModBusBaseParam_TypeDef *ModBusBaseParam);

/**@brief       Modbus ��Ϣ֡�Զ��ϴ�����
* @param[in]    ModBusBaseParam : ModBus�����Ļ��������ṹ��;
* @param[in]    arg : �û��Զ���Ĳ���,�����豸����
* @return       ����ִ�н��
* - None
*/
void ModbusAutoUpload(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg);

/**@brief       �������¼�����,�Զ��ϴ����ڲ�Ϊ0ʱ,�����Եص���ModbusAutoUpload
* @param[in]    ModBusBaseParam : ModBus�����Ļ��������ṹ��;
* @param[in]    arg : �û��Զ���Ĳ���,�����豸����
* @return       ����ִ�н��
* - None
*/
void SensorEvent(ModBusBaseParam_TypeDef *ModBusBaseParam, void *arg);


#ifdef __cplusplus
}
#endif
#endif