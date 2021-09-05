/**
  ******************************************************************************
  * @file    stm8s_cmdline.h
  * @author  Salnikov Aleksey
	* @mail		 acc1408@mail.ru
	* @github  acc1408
  * @version V1.0.0
  * @date    29-08-2021
  * @brief   This file contains all functions prototype and macros for the cmdline.
   ******************************************************************************
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_CMDLINE_H
#define __STM8S_CMDLINE_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "inc/stm8s_stdioinit.h"
#include <string.h>

// -3 - ������� ����� ����������
#define error_parametr "too many parametr\r\n"
// -2 - ������� ������� ������
#define error_long_string "too long string\r\n"
// -1 - ��� ������� �� ������� �������
#define error_command "command not found\r\n"
// ������ ��������� ������
#define cmdrxnum_max 20
// ������������ ���-�� ����������
#define cmdargc_max 3

// ��������� ������� �����������
typedef int16_t (*func_t)(uint8_t argc,char *argv[]);

// ��������� ������� ���������� �������� - ������� 
typedef struct
{
	char name[20];	// �������� �������
	func_t func;		// ��������� �� �������
} tablefunc_t;
// ��������� ������� ������������ ����� �������� �������- ����� ������
typedef struct
{
	int16_t retfunc;	// ����� �������� 
	char name[20];		// ����� ��� ������  
} tableRet_t;


typedef enum
{
	receive=0, 	// ����� ������
	exec=0b01,			// ������ ����-����
	error_ovf=0b10, // ������������ ��������� ������
	//error_param=0b100, //������������ ���-�� ����������
	//exec_error_param=0b101, //������ ����-��� � ���� ������������ ����������
	//exec_error_ovf_param=0x111, // ������ ����-���, ������������ ���������� � ������
	exec_error_ovf=0b011 // ������� ����-��� � ���� ������������ ������
	
}cmdstate_t;

// ��� ��������� ��������� ������
typedef struct
{
	char rx[cmdrxnum_max]; // �����
	uint8_t rxnum; // ����� �������� �����
	char stopbit; // ������ ����-��� ������������
	char stopbit2; // ������ ����-��� ��������������
	tablefunc_t *tabfunc; // ��������� �� ������ �������
	uint8_t numfunc;			// ���-�� �������
	cmdstate_t state; // ��������� ��������� ������
	uint8_t argc; // ���-�� ����������
	char *argv[cmdargc_max]; // ������ ���������� �� ��������� 
	tableRet_t *TabRet; //���������-������� �� ������ �������� ��������� �������
	uint8_t NumTabRet; // ������ ������� �������-����� 
}cmdline_t;

// ������������ ��������� ������
void cmdinit(	char stopbit, // ������ ������ ����-���
							char stopbit2, // ������ ������ ����-���
							tablefunc_t tabfunc[], //��������� �� ������ �������
							uint8_t numfunc, // ���-�� �������
							tableRet_t *tabret, //���������-������� �� ������ �������� ��������� �������
							uint8_t NumTabRet// ������ ������� �������-�����
							) ;
// ������� ���� ��� ��������� ������� ������
void cmdmainloop(void);

#endif