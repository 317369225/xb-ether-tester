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
#include <tchar.h>     
#include <stdio.h>
#include "gui.h"
#include "debug.h"

int scrn_width;
int scrn_height;

int cxChar, cyChar;
int cxChar_2, cyChar_2;
t_gui_size_value gt_gui_size_value = 
{
    .scrn_width  = 1440,
    .scrn_height = 900,
    .cxChar      = 10,
    .cyChar      = 20,
};
void get_sys_gui_info()
{
    scrn_width = GetSystemMetrics(SM_CXSCREEN);
    scrn_height = GetSystemMetrics(SM_CYSCREEN);

    dbg_print("screen size: %d %d", scrn_width, scrn_height);
}

HFONT  char_font, char_font_2, char_font_3;
HFONT  create_font(int height, const char *font_name)
{
    HFONT  h_font;
    LOGFONT lf ;
    
    memset(&lf, 0, sizeof(lf));
    lf.lfCharSet = 1;

    lf.lfHeight = height;
    lf.lfPitchAndFamily = FIXED_PITCH;
    lf.lfWeight  = FW_BLACK;
dbg_print("font height: %d", height);
    if (NULL != font_name)
        strcpy(lf.lfFaceName, font_name);
    else
        strcpy(lf.lfFaceName, "Fixedsys");

    
    h_font = CreateFontIndirect(&lf);

    return h_font;
}

void get_font_size(HFONT  the_font, int *width, int *height)
{
    HDC hdc;
    TEXTMETRIC textmetric;

    hdc = CreateIC (TEXT ("DISPLAY"), NULL, NULL, NULL) ;
    SelectObject(hdc, the_font) ;
    GetTextMetrics(hdc, &textmetric) ;
    SelectObject (hdc, GetStockObject(SYSTEM_FONT)) ;
    DeleteDC (hdc) ;
    
    *width = textmetric.tmAveCharWidth ;
    *height = textmetric.tmHeight + textmetric.tmExternalLeading ;

}

void init_gui_info()
{
    get_sys_gui_info();
    char_font = create_font(gt_gui_size_value.cyChar*WIDTH_COEFFICIENT, "Courier New");
    char_font_2 = create_font(gt_gui_size_value.cyChar*4/3*WIDTH_COEFFICIENT, "Courier New");

    get_font_size(char_font, &cxChar, &cyChar);
    get_font_size(char_font_2, &cxChar_2, &cyChar_2);
    dbg_print("Char size: %d %d", cxChar, cyChar);
    dbg_print("Char size 2: %d %d", cxChar_2, cyChar_2);
}

