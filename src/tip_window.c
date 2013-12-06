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
#include "global_info.h"
#include "res.h"

TCHAR szTipWinClassName[] = TEXT ("TipWin");
HWND    hwnd_tip;
TCHAR tip_info[128]={"this is a tip. ����һ����ʾ"};

#define    TIP_WIN_WIDTH    (700)
#define    TIP_WIN_HEIGHT   (cyChar*2)

LRESULT CALLBACK Tip_WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps ;
	RECT		rect ;

    switch (message)
    {
        case WM_CREATE:
            AnimateWindow(hwnd, 200, AW_BLEND);
            SetWindowPos(hwnd, HWND_TOP
                , (scrn_width-TIP_WIN_WIDTH)/2,(scrn_height-TIP_WIN_HEIGHT)/2
                ,TIP_WIN_WIDTH,TIP_WIN_HEIGHT
                , 0);

            return 0 ;


        case WM_PAINT :
        {
            hdc = BeginPaint (hwnd, &ps) ;
            SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT)) ;
            GetClientRect (hwnd, &rect) ;

                SetBkColor(hdc, RGB(0x0,0x0,0x0)) ;
                SetTextColor(hdc, RGB(0xff,0xff,0xff)) ;


            DrawText (hdc, tip_info, -1, &rect,
			DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;

            DeleteObject(SelectObject (hdc, GetStockObject(SYSTEM_FONT))) ;
  			EndPaint (hwnd, &ps) ;

			return 0 ;
        }

        case WM_MOUSEMOVE:
            break;

}
    
    return DefWindowProc (hwnd, message, wParam, lParam) ;
}



int register_tip_win()
{
    WNDCLASS    sub_wndclass;
    sub_wndclass.style      = CS_HREDRAW | CS_VREDRAW;
    sub_wndclass.lpfnWndProc= Tip_WndProc;
    sub_wndclass.cbClsExtra = 0;
    sub_wndclass.cbWndExtra = 0;
    sub_wndclass.hInstance  = g_hInstance;
    sub_wndclass.hCursor    = LoadCursor(NULL, IDC_ARROW);
    sub_wndclass.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
    sub_wndclass.lpszMenuName   = NULL;
    sub_wndclass.lpszClassName  = szTipWinClassName;


    if (!RegisterClass (&sub_wndclass))
     {
        MessageBox (NULL, TEXT ("Program requires Windows NT!"),
          szAppName, MB_ICONERROR) ;
        return FAIL;
     }

    return SUCCESS;

}








