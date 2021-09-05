/**
  ******************************************************************************
  * @file    stm8s_cmdline.c
  * @author  Salnikov Aleksey
	* @mail		 acc1408@mail.ru
	* @github  acc1408
  * @version V1.0.0
  * @date    29-08-2021
  * @brief   This file contains all functions prototype and macros for the cmdline.
   ******************************************************************************
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_CMDLINE_C
#define __STM8S_CMDLINE_C

/* Includes ------------------------------------------------------------------*/
#include "inc/stm8s_cmdline.h"
// -3 - ������� ����� ����������
// -2 - ������� ������� ������
// -1 - ��� ������� �� ������� �������

cmdline_t cmd; // ��������� ��������� ������
// ������������ ��������� ������
void cmdinit(	char stopbit, // ������ ������ ����-���
							char stopbit2, // ������ ������ ����-���
							tablefunc_t tabfunc[], //��������� �� ������ �������
							uint8_t numfunc, // ���-�� �������
							tableRet_t *tabret, //���������-������� �� ������ �������� ��������� �������
							uint8_t NumTabRet// ������ ������� �������-�����
							) 
{
	cmd.stopbit=stopbit; //������ ����-���
	cmd.stopbit2=stopbit2; //������  ����-���
	cmd.rxnum=0;
	cmd.tabfunc=tabfunc;
	cmd.numfunc=numfunc;
	cmd.argc=0;
	cmd.TabRet=tabret;
	cmd.NumTabRet=NumTabRet;
	
}


void cmdinputchar(char c)
{
	switch(cmd.state)
	{
		// ����� ������
		case receive:
				// ��������� ����� � ������
				if (cmd.rxnum<cmdrxnum_max) 
					cmd.rx[cmd.rxnum++]=c; // ��������� ����� 
				else 	
					{
					cmd.state=error_ovf; // ��������� ������ ������������ ������
					}
				//break;
		// ���� ��������� ������������ ������� ����-����
		case error_ovf:
				if (c==cmd.stopbit) // ���� ������� ����-���, 
						cmd.state|=exec; // ������ ����-��� �� ���������
				break;
		//default:
			//		nop();
			//	break;
	}
}
// ������ �� �����, ������ �������� ��������� ����� ������
// � ������ ������� ��������� �� ������ ����
// ���� � ���������� ������� ������� ������ ����������, ��� 
// ���������, �� ��������� ����� 0, ���� ������, �� ��������� 1
uint8_t cmdparcer(void)
{
	// �������� ��� ����� �������� �� ����� ������
	uint8_t i;
	for( i=0;i<cmd.rxnum;i++)
	if (cmd.rx[i]==' ') // ���� ������ ������
			cmd.rx[i]='\0'; // �� �������� ��� �� ����� ������
	cmd.rx[cmd.rxnum-1]='\0';
	//��������� ��������� ����-��� ���� �� ������ ������
	if (cmd.rx[0]==cmd.stopbit2) 
		cmd.rx[0]='\0'; // �������� ������ ����-��� �� ������
	// ���������� ������ �����
	// ��� ������� ������� � ������ ���������� ��� ��� ������ �������
	if (cmd.rx[0]) 
		{
			cmd.argv[cmd.argc++]=cmd.rx; // �������� ��� ������� �� ���� ����� ������
		}
	// ��� ��������� �������� ���������� ������ �����.
	for( i=1;i<cmd.rxnum-1;i++)
		if ((cmd.rx[i-1]=='\0')&&(cmd.rx[i]!='\0'))
			if (cmd.argc<cmdargc_max)
				cmd.argv[cmd.argc++]=cmd.rx+i;
			else
				return -1; //������ ���-�� ���������� ������, ��� ���������
	
	return 0; // ���-�� ���������� ���������
	
			
}

// ����� ��������� �������� ��������� ������
void cmdreset(void)
{
	cmd.rxnum=0;
	cmd.state=receive;
	cmd.argc=0;
}

// ����� ������� ��� ����������
void cmdexec(void)
{
	uint8_t i;
	int16_t ret_func; // ����� �������� �������
	char *funcfind; // ��������� �� ������
	func_t funcexec;// ��������� �� �������
	for(i=0;i<cmd.numfunc;i++)
	{
		funcfind=cmd.tabfunc[i].name; // ��������� �� ������ ������
		funcexec=cmd.tabfunc[i].func; // ��������� �� ��������������� ������
		if (strcmp(cmd.argv[0],funcfind)==0) // ���� ����� ��������������� �������
		{
			// ��������� ������� 
			ret_func=funcexec(cmd.argc,cmd.argv);
			// ��������� ����� ���������� �������-�����
			for(i=0;i<cmd.NumTabRet;i++)
			{
				if (ret_func==cmd.TabRet[i].retfunc) 
				{
					printf("%s",cmd.TabRet[i].name);
					cmdreset(); // ����� �������� ��� ����� ��������
					return;		// ����� �� �������
				}
			}
		}
	}	
		/*
		// ���� �� ����� �������, �� ���������� ����� ������ �� �������� �������
		for(i=0;i<cmd.NumTabRet;i++)
			{
				// -1 - ��� ������� �� ������� �������
				if (-1==cmd.TabRet[i].retfunc) 
				{
					printf("%s",cmd.TabRet[i].name); // ���� ������� ����������, �� �������� ������
					break; // ������� �� �����
				}
			}
			*/
			printf(error_command);
		cmdreset(); // ����� �������� ��� ����� ��������
		return;		// ����� �� �������
		
}

// ������� ���� ��� ��������� ������� ������
void cmdmainloop(void)
{
	uint8_t i;
	switch(cmd.state) // ��������� ��������� ������
		{
			case exec: // 
					if (!cmdparcer()) 	
							cmdexec();
						else
						{
							/*
							for(i=0;i<cmd.NumTabRet;i++)
							{
								// -3 - ������� ����� ����������
								if (-3==cmd.TabRet[i].retfunc) 
								printf("%s",cmd.TabRet[i].name);
							}
							*/
							printf(error_parametr);
							cmdreset();
						}
				break;
			case 	exec_error_ovf:
					/*
					for(i=0;i<cmd.NumTabRet;i++)
					{
						// -2 - ������� ������� ������
						if (-2==cmd.TabRet[i].retfunc) 
							printf("%s",cmd.TabRet[i].name);
					}
					*/
					printf(error_long_string);
					cmdreset();
				break;
		}
}


#endif