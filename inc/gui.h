/* 
 * �����Ϊ��ѡ���Դ�����
 * ������İ�Ȩ(����Դ�뼰�����Ʒ����汾)��һ�й������С�
 * ����������ʹ�á������������
 * ��Ҳ�������κ���ʽ���κ�Ŀ��ʹ�ñ����(����Դ�뼰�����Ʒ����汾)���������κΰ�Ȩ���ơ�
 * =====================
 * ����: ������
 * ����: sunmingbao@126.com
 */

#ifndef __GUI_H_
#define __GUI_H_

extern int scrn_width;
extern int scrn_height;

extern int cxChar, cyChar;
extern int cxChar_2, cyChar_2;
void init_gui_info();

typedef struct
{
    int scrn_width;
    int scrn_height;
    int cxChar;
    int cyChar;

} t_gui_size_value;

#define    WIDTH_COEFFICIENT   scrn_width/gt_gui_size_value.scrn_width
#define    HEIGHT_COEFFICIENT  scrn_height/gt_gui_size_value.scrn_height

extern t_gui_size_value gt_gui_size_value;
extern HFONT  char_font, char_font_2, char_font_3;
#endif

