#ifndef _SYSTEM_H_
#define _SYSTEM_H_

//#define DEBUG_NEW_NO_NEW_REDEFINITION
//#include "debug_new.h"
//#define NEW new

#ifdef _DEBUG_NEW_
#define DEBUG_NEW_NO_NEW_REDEFINITION
#include "debug_new.h"
#else
#define NEW new
#endif



#pragma warning (disable: 4250)
#pragma warning (disable: 4251)
#pragma warning (disable: 4819) // ���ļ����������ڵ�ǰ����ҳ(936)�б�ʾ���ַ����뽫���ļ�����Ϊ Unicode ��ʽ�Է�ֹ���ݶ�ʧ 

#ifdef WIN32
#ifdef ASPOSE_CELLS_LIBRARY
#define ASPOSE_CELLS_API __declspec(dllexport)
#else
#define ASPOSE_CELLS_API __declspec(dllimport)
#endif
#endif

#include <inttypes.h>
#include <stdio.h>
#include <String.h>

#include <typeinfo>

#endif	// _SYSTEM_H_