/* 
 * �����Ϊ��ѡ���Դ�����
 * ������İ�Ȩ(����Դ�뼰�����Ʒ����汾)��һ�й������С�
 * ����������ʹ�á������������
 * ��Ҳ�������κ���ʽ���κ�Ŀ��ʹ�ñ����(����Դ�뼰�����Ʒ����汾)���������κΰ�Ȩ���ơ�
 * =====================
 * ����: ������
 * ����: sunmingbao@126.com
 */

#include <windows.h>
#include "common.h"
#include <tchar.h>     
#include <stdio.h>

#ifdef _DEBUG
void __dbg_print(TCHAR * szFormat, ...)
{
    TCHAR   szBuffer[1024] ;
    va_list pArgList ;
    int len;
    static FILE *fp = NULL;

    if (NULL==fp) fp=fopen("dbg_info.txt", "wb");

    va_start(pArgList, szFormat) ;
    len=_vsntprintf(szBuffer, sizeof (szBuffer) / sizeof (TCHAR), 
			szFormat, pArgList) ;

    va_end (pArgList) ;
    
    szBuffer[len] = '\r';
    szBuffer[len+1] = '\n';
    len+=2;
    
    fwrite(szBuffer, 1, len, fp);
    fflush(fp);
}

void __dbg_print_w(WCHAR * szFormat, ...)
{
    WCHAR   szBuffer[1024] ;
    va_list pArgList ;
    int len;
    static FILE *fp = NULL;

    if (NULL==fp) fp=fopen("dbg_info_w.txt", "wb");

    va_start(pArgList, szFormat) ;
    len=_snwprintf(szBuffer, sizeof (szBuffer) / sizeof (WCHAR), 
			szFormat, pArgList) ;

    va_end(pArgList) ;
    
    szBuffer[len] = L'\r';
    szBuffer[len+1] = L'\n';
    len+=2;
    
    fwrite(szBuffer, 1, len*2, fp);
    fflush(fp);
}


#else
void __dbg_print(TCHAR * szFormat, ...)
{
    ;
}
#endif

