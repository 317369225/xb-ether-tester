/* 
 * �����Ϊ��ѡ���Դ�����
 * ������İ�Ȩ(����Դ�뼰�����Ʒ����汾)��һ�й������С�
 * ����������ʹ�á������������
 * ��Ҳ�������κ���ʽ���κ�Ŀ��ʹ�ñ����(����Դ�뼰�����Ʒ����汾)���������κΰ�Ȩ���ơ�
 * =====================
 * ����: ������
 * ����: sunmingbao@126.com
 */

#ifndef __DEBUG_H_
#define __DEBUG_H_

#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)


void __dbg_print_w(WCHAR * szFormat, ...);
void __dbg_print(TCHAR * szFormat, ...);

#define    dbg_print(fmt, args...) \
    do \
    { \
        __dbg_print("DBG:%s(%d)-%s:\n"fmt"\n", __FILE__,__LINE__,__FUNCTION__,##args); \
    } while (0)

#define    dbg_print_w(fmt, args...) \
    do \
    { \
        __dbg_print_w(L"DBG:%s(%d):\n"fmt"\n", __WFILE__,__LINE__,,##args); \
    } while (0)

#endif

