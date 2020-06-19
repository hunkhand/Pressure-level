/****************************************************************************************************************************************************************
** ��Ȩ:          2020-2030,��������Ϊ�Ƽ���չ���޹�˾
** �ļ���:        In_Flash.c
** ����:          ���
** �汾:          V1.0.0
** ����:          2020-04-29
** ����:          ��STM32F072���ڲ�Flash�ж�ȡ��д������
** ����:          ���ڲ�Flash��ȡ��д������
*****************************************************************************************************************************************************************
** �޸���:        No
** �汾:          No
** �޸�����:      No
** ����:          No
****************************************************************************************************************************************************************/

#include "In_Flash.h"

//��ʱ����
static uint32_t Flash_Buf[FLASH_PAGE_SIZE / 4];    

/**
* ����       : InFlash_Write_OneByte()
* ��������   : 2020-04-29
* ����       : ���
* ����       : ���ڲ�Flashָ��λ��д��һ�ֽ�����
* �������   : RWAddr��д���ַ����WrData��д�����ݣ�
* �������   : ��
* ���ؽ��   : OP_SUCCESS���ɹ�����OP_FAILED��ʧ�ܣ�
* ע���˵�� : ��ַ����Խ��
* �޸�����   :
*/
uint8_t InFlash_Write_OneByte(uint16_t RWAddr, uint8_t WrData)
{
    uint32_t WrAddr;
    uint16_t i;
    uint8_t *buf = (uint8_t *)Flash_Buf;
    HAL_StatusTypeDef Halstatus;

    if(FLASH_END < RWAddr)
    {
        return OP_FAILED;
    }
    WrAddr = (RWAddr / FLASH_PAGE_SIZE) * FLASH_PAGE_SIZE;
    InFlash_Read_MultiBytes(WrAddr, buf, FLASH_PAGE_SIZE);
    buf[RWAddr % FLASH_PAGE_SIZE] = WrData;
    
    Halstatus = (HAL_StatusTypeDef)InFlash_Erase_Page(RWAddr, 1);
    if(HAL_OK != Halstatus)
        return OP_FAILED;
    //__disable_irq();    
    HAL_FLASH_Unlock();
    for(i = 0; i < FLASH_PAGE_SIZE; i += 4)
    {
        Halstatus = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, \
                                        (WrAddr + i) + FLASH_BASE_ADDRESS, (uint32_t)Flash_Buf[i / 4]);
        if(HAL_OK != Halstatus)
            break;
    }
    HAL_FLASH_Lock();
    //__enable_irq();
    if(HAL_OK != Halstatus)
    {
        return OP_FAILED;
    }

    return OP_SUCCESS;
}

/**
* ����       : InFlash_Read_OneByte()
* ��������   : 2020-04-29
* ����       : ���
* ����       : ���ڲ�Flashָ��λ�ö�ȡһ�ֽ�����
* �������   : RWAddr����ȡ��ַ��
* �������   : ��
* ���ؽ��   : 1���ֽ�����
* ע���˵�� : ��ַ����Խ��
* �޸�����   :
*/
uint8_t InFlash_Read_OneByte(uint16_t RWAddr)
{
  uint8_t RdData;
  uint32_t RealAddr;
  
  RdData = 0;
  RealAddr = FLASH_BASE_ADDRESS + RWAddr;
  assert_param(IS_FLASH_PROGRAM_ADDRESS(RdAddr));
  RdData = *(__IO uint8_t *)RealAddr;
  return RdData;
}

/**
* ����       : InFlash_Write_MultiBytes()
* ��������   : 2020-04-29
* ����       : ���
* ����       : ���ڲ�Flashָ��λ��д����ֽ�
* �������   : RWAddr(д��ʼ��ַ)��pWrbuf(�̶������ڴ�),Wrlen(д���ݳ���)
* �������   : ��
* ���ؽ��   : OP_SUCCESS���ɹ�����OP_FAILED��ʧ�ܣ�
* ע���˵�� : ��ַ����Խ��
* �޸�����   :
*/
uint8_t InFlash_Write_MultiBytes(uint16_t RWAddr, uint8_t const *pWrbuf, uint16_t Wrlen)
{
    uint16_t WrAddr;
    uint16_t i;
    uint8_t j;
    uint16_t remain;
    uint16_t off;
    uint8_t *wbuf = (uint8_t *)pWrbuf;
    uint8_t *flash_buf = (uint8_t *)Flash_Buf;
    HAL_StatusTypeDef Halstatus;

    if(FLASH_END < RWAddr)
    {
        return OP_FAILED;
    }
    WrAddr = (RWAddr / FLASH_PAGE_SIZE) * FLASH_PAGE_SIZE;
    off = RWAddr % FLASH_PAGE_SIZE;
    remain = FLASH_PAGE_SIZE - off;
    if(Wrlen <= remain)
        remain = Wrlen;
    for(j = 0; j < (Wrlen / FLASH_PAGE_SIZE) + 1; j++)
    {        
        InFlash_Read_MultiBytes(WrAddr, flash_buf, FLASH_PAGE_SIZE);
        for(i = 0; i < remain; i++)
        {
            flash_buf[i + off] = wbuf[i];
        }
        
        Halstatus = (HAL_StatusTypeDef)InFlash_Erase_Page(WrAddr, 1);
        if(HAL_OK != Halstatus)
            break;
        //__disable_irq();        
        HAL_FLASH_Unlock();                        
        for(i = 0; i < FLASH_PAGE_SIZE; i += 4)
        {
            Halstatus = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, 
                                        (WrAddr + i) + FLASH_BASE_ADDRESS, (uint32_t)Flash_Buf[i / 4]);
            if(HAL_OK != Halstatus)
                break;
        }
        HAL_FLASH_Lock();
        //__enable_irq();
        WrAddr += FLASH_PAGE_SIZE;
        off = 0;
        wbuf += remain;
        Wrlen -= remain;
        if(Wrlen > FLASH_PAGE_SIZE)
            remain = FLASH_PAGE_SIZE;
        else
            remain = Wrlen;
    }
    if(HAL_OK != Halstatus)
    {
        return OP_FAILED;
    }

    return OP_SUCCESS;
}

/**
* ����       : InFlash_Read_MultiBytes()
* ��������   : 2020-04-29
* ����       : ���
* ����       : ���ڲ�Flashָ��λ�ö�����ֽ�
* �������   : RWAddr(����ʼ��ַ)��Rdlen(�����ݳ���)
* �������   : pRdbuf(���ݶ��ڴ�)
* ���ؽ��   : ��
* ע���˵�� : ��ַ����Խ��
* �޸�����   :
*/
void InFlash_Read_MultiBytes(uint16_t RWAddr, uint8_t *pRdbuf, uint16_t Rdlen)
{
  uint16_t FlashCnt;
  uint16_t RWAddress;
  
  RWAddress = RWAddr;
  for(FlashCnt = 0; FlashCnt < Rdlen; FlashCnt++)
  {
    *(pRdbuf + FlashCnt) = InFlash_Read_OneByte(RWAddress);
    
    RWAddress++;
  }
}

/**
* ����       : InFlash_Erase_Page()
* ��������   : 2020-04-29
* ����       : ���
* ����       : ���ڲ�Flashָ��λ�ò���ҳ
* �������   : RWAddr(������ʼ��ַ)��PageNb(����ҳ��Ŀ)
* �������   : ��
* ���ؽ��   : OP_SUCCESS���ɹ�����OP_FAILED��ʧ�ܣ�
* ע���˵�� : ��ַ����Խ��
* �޸�����   :
*/
uint8_t InFlash_Erase_Page(uint16_t RWAddr, uint8_t PageNb)
{
    uint8_t sta = OP_SUCCESS;
    uint32_t EepAddress;
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t PageError;

    EepAddress = (RWAddr / FLASH_PAGE_SIZE) * FLASH_PAGE_SIZE;
    EepAddress += FLASH_BASE_ADDRESS;
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = EepAddress;
    EraseInitStruct.NbPages = PageNb;
    HAL_FLASH_Unlock();    
    sta = HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
    HAL_FLASH_Lock();    

    return sta;
}
