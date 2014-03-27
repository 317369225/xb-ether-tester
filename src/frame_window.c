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


TCHAR   szFrameWinClassName[] = TEXT ("fsb_frame") ;
HWND    hwnd_frame;

HWND hwndTip;
int  doc_modified;
HWND    hwnd_splt_we;
HWND    hwnd_splt_ns;

int we_pos;
int ns_pos;


static HCURSOR 	hcSizeEW;
int display_left=1;
int display_bottom=1;
int display_toolbar=1;
int display_statusbar=1;

int query_clr_stats=1;
int auto_clr_stats=0;
int auto_clr_captured_pkts=0;

char cfg_file_path[MAX_FILE_PATH_LEN];
char file_to_open[MAX_FILE_PATH_LEN];

BOOL CALLBACK AboutDlgProc (HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam);

void ui_switch(int is_sending)
{
    SendMessage(hwnd_toolbar, TB_CHECKBUTTON, IDT_TOOLBAR_START, is_sending);
    EnableWindow(hwnd_tv, !is_sending);
    EnableWindow(hwnd_right, !is_sending);
    EnableWindow(hwnd_net_card_comb, !is_sending);
    SendMessage(hwnd_toolbar, TB_ENABLEBUTTON, IDT_TOOLBAR_VIEW_CAPTURE, !is_sending);
    SendMessage(hwnd_toolbar, TB_ENABLEBUTTON, IDT_TOOLBAR_CAPTURE, !is_sending);
    SendMessage(hwnd_toolbar, TB_ENABLEBUTTON, IDT_TOOLBAR_CAP_WHILE_SND, !is_sending);
}

int doc_save_proc()
{
    int ret;
    if (doc_modified)
    {
        ret=AskConfirmation_3state(hwnd_frame, TEXT("�Ƿ񱣴浱ǰ����?"), szAppName);
        if (IDCANCEL == ret) return 1;
        if (IDYES == ret)
        {
            SendMessage(hwnd_frame, WM_COMMAND, IDM_FILE_SAVE, 0);
            return doc_modified;
        }
    }

    return 0;
}

int stats_captured_pkts_proc()
{
    char file_name[MAX_FILE_PATH_LEN];
    int ret;

PKT_SAVE_PROC:
    if (!cap_save_cnt) goto STATS_PROC;
    if (auto_clr_captured_pkts) goto CLR_CAP_PKTS;

    ret=AskConfirmation_3state(hwnd_frame, TEXT("����������е�ץ����\r\n�Ƿ񱣴�?"), szAppName);
    if (IDCANCEL == ret) return 1;
    if (IDYES == ret)
    {
        file_name[0]=0;
        if (0==get_save_file_name(file_name, hwnd_frame, "pcap file(*.pcap)\0*.pcap\0\0", "pcap"))
            CopyFile(PKT_CAP_FILE_WHILE_SND,  file_name,  FALSE);
        else
            return 1;
    }

CLR_CAP_PKTS:
        cap_save_cnt= 0;
        cap_save_bytes_cnt = 0;

STATS_PROC:
    if (!gt_pkt_stat.send_total && !gt_pkt_stat.rcv_total)
        return 0;

    if (query_clr_stats)
    {
        ret= AskConfirmation_3state(hwnd_frame, TEXT("������е�ͳ��?"), szAppName);
        if (IDCANCEL == ret) return 1;
        if (IDNO == ret) return 0;

    }
    else if (!auto_clr_stats)
    {
        return 0;
    }

    
CLR_STAT:
    clear_stats();

    return 0;
}

void open_file()
{
    if (doc_save_proc()) return 0;

    load_stream(file_to_open);
    re_populate_items();
    strcpy(cfg_file_path, file_to_open);
    set_frame_title(strrchr(cfg_file_path, '\\')+1);
    update_statusbar();
}

#define    HISTORY_FILE_NAME    "history"
void populate_recent_files(HMENU	 hMenu)
{
    int i;
    int file_num;
    char    menu_name[64];
    

    for (i=0; i<MAX_RECENT_FILE_NUM; i++)
        DeleteMenu(hMenu, 0, MF_BYPOSITION);


    if (!file_exists(HISTORY_FILE_NAME))
    {
        fclose(fopen(HISTORY_FILE_NAME, "w"));
    }

    file_num=GetPrivateProfileInt("recent files", "FileNum", 0, HISTORY_FILE_NAME);

    for (i=0; i<file_num; i++)
    {
        sprintf(menu_name, "&%d haha", i);
        AppendMenu(hMenu, MF_STRING, ID_FILE_RECENT_FILE_BEGIN+i,  menu_name) ;
    }

    
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc ;
    PAINTSTRUCT ps ;
    static int   				cxClient, cyClient;
HMENU	 hMenu;
    POINT point;
    RECT		rect ;
    int  item_id;
    int upper_win_y=toolbar_height, 
        right_win_x=we_pos, 
        right_win_width=cxClient-we_pos, 
        upper_win_height,
        bottom_win_height;
    
    int ret;

    switch (message)
    {
        case WM_CREATE:
            hwnd_frame = hwnd;
            CreateToolbar();

            GetClientRect (hwnd, &rect) ;
            cxClient = rect.right;
            cyClient = rect.bottom;

            //point.x=cxClient / 4;
            //point.y=cyClient / 4;
            point.x=0;
            point.y=0;

            //ClientToScreen(hwnd, &point);
           			// Move the buttons to the new center
      hwndTip =CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
                        WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP |TTS_BALLOON,
                      CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
                         hwnd, 	//Handle of the Parent window.
				//(i.e. Window for which you want Tooltip)
                         NULL,
                         g_hInstance, 
                         NULL);


            hwnd_left = CreateWindow (szSubWinClassName, TEXT ("sub win"),
                WS_CHILD,
                point.x, point.y,
                we_pos-SPLT_WIDTH, ns_pos-SPLT_WIDTH,
                hwnd, NULL, g_hInstance, NULL) ;

            ShowWindow (hwnd_left, 1) ;
            UpdateWindow (hwnd_left) ;

            hwnd_splt_we= CreateWindow (szSpltWeClassName, TEXT ("sub win"),
                WS_CHILD,
                we_pos-SPLT_WIDTH,
                            point.y,
                  			SPLT_WIDTH, ns_pos-SPLT_WIDTH,
                hwnd, NULL, g_hInstance, NULL) ;
            ShowWindow (hwnd_splt_we, 1) ;
            UpdateWindow (hwnd_splt_we) ;


            hwnd_right = CreateWindow (szRightWinClassName, TEXT ("sub win"),
                WS_CHILD,
                we_pos,
                            point.y,
                  			cxClient-we_pos, ns_pos-SPLT_WIDTH,
                hwnd, NULL, g_hInstance, NULL) ;

            ShowWindow (hwnd_right, 1) ;
            UpdateWindow (hwnd_right) ;

            hwnd_splt_ns= CreateWindow (szSpltNsClassName, TEXT ("sub win"),
                WS_CHILD,
                point.x, ns_pos-SPLT_WIDTH,
                cxClient, SPLT_WIDTH,
                hwnd, NULL, g_hInstance, NULL) ;
            
            ShowWindow (hwnd_splt_ns, 1) ;
            UpdateWindow (hwnd_splt_ns) ;

            hwnd_bottom = CreateWindow (szBottomWinClassName, TEXT ("sub win"),
                WS_CHILD,
                point.x,    ns_pos,
                  			cxClient, cyClient-ns_pos,
                hwnd, NULL, g_hInstance, NULL) ;

            ShowWindow (hwnd_bottom, 1) ;
            UpdateWindow (hwnd_bottom) ;
            //init_stats_ui();

CreateStatusBar();

            hwnd_tip = CreateWindow (szTipWinClassName, TEXT ("tip win"),
                WS_POPUP,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                hwnd, NULL, g_hInstance, NULL) ;

            ShowWindow (hwnd_tip, 0) ;

            DragAcceptFiles(hwnd, TRUE);

            return 0 ;


        case WM_SPLITTER_X:
        {
            we_pos=wParam;
            resize_window(hwnd);
            return 0 ;
        }

        case WM_SPLITTER_Y:
        {
            ns_pos=wParam;
            resize_window(hwnd);
            return 0 ;
        }

        case WM_SIZE:
#if 1
      		cxClient = LOWORD (lParam) ;
      		cyClient = HIWORD (lParam) ;

            if (we_pos==0 || ns_pos==0)
            {
                we_pos = 200;
                ns_pos = cyClient-300;

            }
            //GetClientRect (hwnd, &rect) ;
            //cxClient = rect.right;
            //cyClient = rect.bottom;

            //point.x=cxClient / 4;
            //point.y=cyClient / 4;
            //point.x=0;
            //point.y=toolbar_height;

        upper_win_y=display_toolbar?toolbar_height:0;

        right_win_x=display_left?we_pos:0;
        right_win_width=display_left?cxClient-we_pos:cxClient;
        if (display_bottom)
        {
            upper_win_height=ns_pos-SPLT_WIDTH;
            upper_win_height -= upper_win_y;
        }
        else
        {
            upper_win_height=cyClient;
            upper_win_height -= upper_win_y;
            upper_win_height -= (display_statusbar?statusbar_height:0);

        }

        bottom_win_height = cyClient-ns_pos;
        bottom_win_height -= (display_statusbar?statusbar_height:0);

            //ClientToScreen(hwnd, &point);
           			// Move the buttons to the new center

      	    MoveWindow	(	hwnd_toolbar, 	0, 0,
                cxClient, toolbar_height, TRUE) ;

      	    MoveWindow	(	hwnd_left, 	0, upper_win_y,
                we_pos-SPLT_WIDTH, upper_win_height, TRUE) ;

      	    MoveWindow	(	hwnd_splt_we, 	we_pos-SPLT_WIDTH,
                            upper_win_y,
                  			SPLT_WIDTH, upper_win_height, TRUE) ;

      	    MoveWindow	(	hwnd_right, 	right_win_x,
                            upper_win_y,
                  			right_win_width, upper_win_height, TRUE) ;

      	    MoveWindow	(	hwnd_splt_ns, 	0, ns_pos-SPLT_WIDTH,
                cxClient, SPLT_WIDTH, TRUE) ;

      	    MoveWindow	(	hwnd_bottom, 0,    ns_pos,
                  			cxClient, bottom_win_height, TRUE) ;

      	    MoveWindow	(	hwnd_statusbar, 	0, cyClient-statusbar_height,
                cxClient, statusbar_height, TRUE) ;

#endif
          	return 0 ;

        case WM_INITMENU:

            if (lParam == 0)
            {
                hMenu = GetMenu(hwnd);
                hMenu = GetSubMenu(hMenu, 0);
                hMenu = GetSubMenu(hMenu, 7);
                populate_recent_files(hMenu);
            }


            return 0;

        case WM_DROPFILES :
            DragQueryFile((HDROP)wParam,
                          0,
                          file_to_open,
                          sizeof(file_to_open));
            DragFinish((HDROP)wParam);
            open_file();
            return 0;

        case 	WM_COMMAND:
            hMenu = GetMenu (hwnd) ;
            item_id = LOWORD(wParam);
            switch (item_id)
            {
                case    IDM_APP_ABOUT:
                DialogBox (g_hInstance, TEXT("ABOUTBOX"), hwnd, AboutDlgProc) ;
               	return 0 ;

                case    IDM_GET_NEW_VERSION:
                ShellExecute(NULL, "open"
                    , "http://sourceforge.net/projects/xb-ether-tester/files/latest/download?source=directory"
                    , NULL, NULL, SW_SHOWNORMAL);

               	return 0 ;

                case    IDM_GET_SOURCE:
                ShellExecute(NULL, "open"
                    , "http://sourceforge.net/projects/xb-ether-tester/files/v2.x/"
                    , NULL, NULL, SW_SHOWNORMAL);

               	return 0 ;

                case    IDM_OPEN_OFFICIAL_SITE:
                ShellExecute(NULL, "open"
                    , "http://sunmingbao.freevar.com/"
                    , NULL, NULL, SW_SHOWNORMAL);

               	return 0 ;

                case    IDM_APP_EXIT:
       				SendMessage (hwnd, WM_CLOSE, 0, 0) ;
       				return 0 ;

                case    IDM_APP_BOTTOM_WIN:
                    if (display_bottom)
                    {
                        display_bottom = 0;
           				ShowWindow(hwnd_bottom, 0);
                        ShowWindow(hwnd_splt_ns, 0);
                        UpdateWindow (hwnd_frame) ;
                        CheckMenuItem (hMenu, item_id, MF_UNCHECKED) ;

                    }
                    else
                    {
                        display_bottom= 1;
           				ShowWindow(hwnd_bottom, 1);
                        ShowWindow(hwnd_splt_ns, 1);
                        UpdateWindow (hwnd_frame) ;
                        CheckMenuItem (hMenu, item_id, MF_CHECKED) ;
                    }
                    resize_window(hwnd_frame);
       				return 0 ;

                case    IDM_APP_LEFT_WIN:
                    if (display_left)
                    {
                        display_left = 0;
           				ShowWindow(hwnd_left, 0);
                        ShowWindow(hwnd_splt_we, 0);
                        UpdateWindow (hwnd_frame) ;
                        CheckMenuItem (hMenu, item_id, MF_UNCHECKED) ;



                    }
                    else
                    {
                        display_left= 1;
           				ShowWindow(hwnd_left, 1);
                        ShowWindow(hwnd_splt_we, 1);
                        UpdateWindow (hwnd_frame) ;
                        CheckMenuItem (hMenu, item_id, MF_CHECKED) ;
                    }
                    resize_window(hwnd_frame);
       				return 0 ;

                case    IDM_APP_TOOLBAR_WIN:
                    if (display_toolbar)
                    {
                        display_toolbar = 0;
           				ShowWindow(hwnd_toolbar, 0);
                        UpdateWindow (hwnd_frame) ;
                        CheckMenuItem (hMenu, item_id, MF_UNCHECKED) ;



                    }
                    else
                    {
                        display_toolbar= 1;
           				ShowWindow(hwnd_toolbar, 1);
                        UpdateWindow (hwnd_frame) ;
                        CheckMenuItem (hMenu, item_id, MF_CHECKED) ;
                    }
                    resize_window(hwnd_frame);
       				return 0 ;

                case    IDM_APP_STATUSBAR_WIN:
                    if (display_statusbar)
                    {
                        display_statusbar = 0;
           				ShowWindow(hwnd_statusbar, 0);
                        UpdateWindow (hwnd_frame) ;
                        CheckMenuItem (hMenu, item_id, MF_UNCHECKED) ;



                    }
                    else
                    {
                        display_statusbar= 1;
           				ShowWindow(hwnd_statusbar, 1);
                        UpdateWindow (hwnd_frame) ;
                        CheckMenuItem (hMenu, item_id, MF_CHECKED) ;
                    }
                    resize_window(hwnd_frame);
       				return 0 ;

                case    IDM_APP_QUERY_CLR_STATS:
                    if (query_clr_stats)
                    {
                        query_clr_stats = 0;
                        CheckMenuItem (hMenu, item_id, MF_UNCHECKED) ;
                    }
                    else
                    {
                        query_clr_stats = 1;
                        CheckMenuItem (hMenu, item_id, MF_CHECKED) ;
                    }

       		        return 0 ;

                case    IDM_APP_CLR_STATS:
                    auto_clr_stats = 1;
                    CheckMenuItem (hMenu, item_id, MF_CHECKED) ;
                    CheckMenuItem (hMenu, IDM_APP_NOT_CLR_STATS, MF_UNCHECKED) ;
       		        return 0 ;

                case    IDM_APP_NOT_CLR_STATS:
                    auto_clr_stats = 0;
                    CheckMenuItem (hMenu, item_id, MF_CHECKED) ;
                    CheckMenuItem (hMenu, IDM_APP_CLR_STATS, MF_UNCHECKED) ;
       		        return 0 ;

                case    IDM_APP_QUERY_SAVE_CAPTURED_PKS:
                    auto_clr_captured_pkts = 0;
                    CheckMenuItem (hMenu, item_id, MF_CHECKED) ;
                    CheckMenuItem (hMenu, IDM_APP_CLR_CAPTURED_PKS, MF_UNCHECKED) ;
       		        return 0 ;

                case    IDM_APP_CLR_CAPTURED_PKS:
                    auto_clr_captured_pkts = 1;
                    CheckMenuItem (hMenu, item_id, MF_CHECKED) ;
                    CheckMenuItem (hMenu, IDM_APP_QUERY_SAVE_CAPTURED_PKS, MF_UNCHECKED) ;
       		        return 0 ;

                case    IDT_TOOLBAR_START:
                    if (0==need_stop) return;

                    if (nr_cur_stream<=0)
                    {
                        WinPrintf(hwnd, TEXT("���ȴ�����"));
                        //SendMessage(hwnd_toolbar, TB_CHECKBUTTON, IDT_TOOLBAR_START, 0);
                        return 0 ;
                    }

                    if (GetSelCnt(hwnd_lv)<=0)
                    {
                        WinPrintf(hwnd, TEXT("��ѡ��Ҫ���͵���"));
                        //SendMessage(hwnd_toolbar, TB_CHECKBUTTON, IDT_TOOLBAR_START, 0);
                        return 0 ;
                    }

                    if (stats_captured_pkts_proc())
                    {
                        return 0;
                    }

PREPARE_LAUNCH:
                    need_stop=snd_stopped=rcv_stopped=0;
                    snd_started=rcv_started=0;

                    ui_switch(1);
                    
                    launch_thread(wpcap_snd_test, NULL);
                    launch_thread(wpcap_rcv_test, NULL);
                    while (!rcv_started) Sleep(1);
                    SetTimer(hwnd_frame, TIMER_STATUS_BAR, TIMER_STATUS_GAP, NULL);
                    gettimeofday(&last_stat_tv, NULL);

               	return 0 ;

                case    IDT_TOOLBAR_STOP:
                if ((1==need_stop) || (snd_stopped && rcv_stopped))  return 0;
                need_stop=1;
                //SendMessage(hwnd_toolbar, TB_ENABLEBUTTON, IDT_TOOLBAR_START, 1);
                ui_switch(0);
                //SendMessage(hwnd_toolbar, TB_ENABLEBUTTON, IDT_TOOLBAR_STOP, 1);
                //SendMessage(hwnd_toolbar, TB_CHECKBUTTON, IDT_TOOLBAR_STOP, 1);

                while (!snd_stopped || !rcv_stopped) Sleep(1);
                SendMessage(hwnd, WM_TIMER, TIMER_STATUS_BAR, NULL);
               	return 0 ;

                case    IDT_TOOLBAR_CAPTURE:
                    DialogBox(g_hInstance, TEXT("PKT_CAP_DLG"), hwnd, PktCapDlgProc);
       				return 0 ;

                case    IDT_TOOLBAR_CAP_WHILE_SND:
                need_capture=!need_capture;
                SendMessage(hwnd_toolbar, TB_CHECKBUTTON, IDT_TOOLBAR_CAP_WHILE_SND, need_capture);
                update_statusbar();
               	return 0 ;
                
                case    IDT_TOOLBAR_VIEW_CAPTURE:
                    if (!cap_save_cnt|| !file_exists(PKT_CAP_FILE_WHILE_SND))
                    {
                        WinPrintf(hwnd, TEXT("��������"));
                        return 0;
                    }
                    strcpy(pcap_file_to_view, PKT_CAP_FILE_WHILE_SND);
                    DialogBox(g_hInstance, TEXT("PKT_VIEW_DLG"), hwnd, PktViewDlgProc);
       				return 0 ;


                case    IDM_FILE_SAVE:
                {
                    if (cfg_file_path[0]==0)
                    {
                        SendMessage(hwnd, WM_COMMAND, IDM_FILE_SAVE_AS, 0);
                        return 0 ;
                    }
                    save_stream(cfg_file_path);
                   	return 0 ;
                }

                case    IDM_FILE_SAVE_AS:
                {
                        ret=get_save_file_name(file_to_open, hwnd, CFG_FILE_FILTER, CFG_FILE_SUFFIX);
                        if (ret) return 0 ;
                        strcpy(cfg_file_path, file_to_open);
                        set_frame_title(strrchr(cfg_file_path, '\\')+1);
                        save_stream(cfg_file_path);
                       	return 0 ;
                }

                case    IDM_FILE_NEW:
                {
                    if (doc_save_proc()) return 0;
                    del_all_streams();
                    update_statusbar();
                    cfg_file_path[0]=0;
                    set_frame_title(TEXT("�ޱ���"));
                    doc_modified=0;
                    return 0 ;
                }
                
                case    IDM_FILE_OPEN:
                {
                    if (get_open_file_name(file_to_open, hwnd, CFG_FILE_FILTER))
                        return 0;
                    open_file();
                   	return 0 ;
                }

                case    IDM_VIEW_PCAP_FILE:
                {
                    if (0==get_open_file_name(file_to_open, hwnd, "etherreal dump(*.pcap)\0*.pcap\0\0"))
                    {
                        strcpy(pcap_file_to_view, file_to_open);
                        DialogBox(g_hInstance, TEXT("PKT_VIEW_DLG"), hwnd, PktViewDlgProc);
                    }
                   	return 0 ;
                }


            }

            return 0;

        case 	WM_INITMENUPOPUP:
        {

            if (lParam == 0)
            {
                EnableMenuItem ((HMENU) wParam, IDM_APP_CLR_STATS, !query_clr_stats ? MF_ENABLED : MF_GRAYED);
                EnableMenuItem ((HMENU) wParam, IDM_APP_NOT_CLR_STATS, !query_clr_stats ? MF_ENABLED : MF_GRAYED);

                return 0;

            }

            break;
        }

        case	WM_TIMER:
        {
            struct timeval cur_tv;
            unsigned long  time_gap;
            gettimeofday(&cur_tv, NULL);
            gt_pkt_stat_tmp=gt_pkt_stat;

            if (snd_stopped && rcv_stopped)
            {
                KillTimer (hwnd, TIMER_STATUS_BAR) ;
            }
            

            time_gap=time_a_between_b(&last_stat_tv, &cur_tv);
            time_elapsed.tv_usec+=time_gap;
            if (time_elapsed.tv_usec>=1000000)
            {
                time_elapsed.tv_usec-=1000000;
                time_elapsed.tv_sec+=1;

            }
            last_stat_tv = cur_tv;
            update_stats(&gt_pkt_stat_tmp, time_gap);
            return 0;

        }

     	case	WM_CLOSE:
            if (doc_save_proc()) return 0;
            DestroyWindow (hwnd) ;
            return 0 ;

        case    WM_DESTROY :
            PostQuitMessage (0) ;
            return 0 ;
    }
    return DefWindowProc (hwnd, message, wParam, lParam) ;
}

int register_frame()
{
    WNDCLASS    wndclass;
    wndclass.style      = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc= WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance  = g_hInstance;
    wndclass.hIcon      = LoadIcon (g_hInstance, TEXT("my_frame_icon"));
    //wndclass.hCursor    = LoadCursor (g_hInstance, TEXT("my_cursor"));
    wndclass.hCursor      = LoadCursor (NULL, IDC_ARROW);
    wndclass.hbrBackground  = (HBRUSH)GetStockObject(GRAY_BRUSH);
    wndclass.lpszMenuName   = TEXT("MY_MENU");
    wndclass.lpszClassName  = szFrameWinClassName;

    if (!RegisterClass (&wndclass))
     {
        MessageBox (NULL, TEXT ("Program requires Windows NT!"),
          szAppName, MB_ICONERROR) ;
        return FAIL;
     }

    return SUCCESS;

}
int reg_sys_win_classes()
{
    INITCOMMONCONTROLSEX icex;           // Structure for control initialization.
    icex.dwSize = sizeof(icex);
    icex.dwICC = ICC_STANDARD_CLASSES|ICC_BAR_CLASSES|ICC_TAB_CLASSES
        |ICC_TREEVIEW_CLASSES|ICC_LISTVIEW_CLASSES;


    InitCommonControlsEx(&icex);
    InitCommonControls();

}

void set_frame_title(TCHAR *file_name)
{
    TCHAR info[128];
    sprintf(info, TEXT("%s - %s"), szAppName, file_name);
    SetWindowText(hwnd_frame, info);

}

int create_windows(int iCmdShow)
{
    hwnd_frame = CreateWindow (szFrameWinClassName, szAppName,
            WS_OVERLAPPEDWINDOW|WS_MAXIMIZE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,
            CW_USEDEFAULT, CW_USEDEFAULT,
            CW_USEDEFAULT, CW_USEDEFAULT,
            NULL, NULL, g_hInstance, NULL) ;

    ShowWindow (hwnd_frame, SW_MAXIMIZE) ;
    UpdateWindow (hwnd_frame) ;
    set_frame_title(TEXT("�ޱ���"));


    return 0;

}

void resize_window(HWND hwnd)
{
    RECT rect;
    GetClientRect(hwnd,&rect);
    SendMessage(hwnd, WM_SIZE, SIZE_RESTORED, MAKELPARAM(rect.right, rect.bottom));
}

BOOL CALLBACK AboutDlgProc (HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
    TCHAR info[1024];

    sprintf(info
        , "%s    V%c.%c.%c\r\n"
        "(����ʱ��: %s) \r\n\r\n"
        "���ߣ������� \r\n\r\n"
        "======================\r\n\r\n"
        "�����Ϊ��ѡ���Դ�����\r\n"
        "������İ�Ȩ(����Դ�뼰�����Ʒ����汾)��һ�й������С�\r\n"
        "����������ʹ�á������������\r\n"
        "��Ҳ�������κ���ʽ���κ�Ŀ��ʹ�ñ����(����Դ�뼰�����Ʒ����汾)���������κΰ�Ȩ���ơ�\r\n\r\n"
        "======================\r\n\r\n"
        
        "�������һ�����PC������ʵ�ֵĲ����ǹ��ߡ� \r\n"
        "�����ͨ��winpcap���б����շ��� \r\n\r\n"
        "======================\r\n\r\n"
        "���ڱ�����������κ�������飬��ӭ��ϵ���ߡ�\r\n"
        "����:sunmingbao@126.com\r\n"
        "QQ:7743896"
        , szAppName
        , version[0] , version[1] , version[2]
        , BUILD_TIME);
     	switch (message)
     	{
     	case 	WM_INITDIALOG :
                center_child_win(hwnd_frame, hDlg);
                SetDlgItemText(hDlg, ID_ABOUT_INFO, info);
                SetFocus(GetDlgItem(hDlg, IDOK));
          		return FALSE ;

        case 	WM_CLOSE:
   				SendMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
   				return TRUE ;

     	case 	WM_COMMAND :
          		switch (LOWORD (wParam))
          		{

                case 	ID_AB_SOURCE:
               		SendMessage(hwnd_frame, WM_COMMAND, IDM_GET_SOURCE, 0);
               		return TRUE ;
               		
               	case 	ID_AB_SITE:
               		SendMessage(hwnd_frame, WM_COMMAND, IDM_OPEN_OFFICIAL_SITE, 0);
               		return TRUE ;
               		
               	case 	ID_AB_UPDATE:
               		SendMessage(hwnd_frame, WM_COMMAND, IDM_GET_NEW_VERSION, 0);
               		return TRUE ;
        
          		case 	IDOK :
          		case 	IDCANCEL :
               				EndDialog (hDlg, 0) ;
               				return TRUE ;
          }
          		break ;
     }
  	return FALSE ;
}

void add_tip(HWND htip_control, HWND hwnd, TCHAR *tips)
{
    TOOLINFO        ti;

    ti.cbSize = sizeof(TOOLINFO);
    //Flags TTF_IDISHWND show that the uID is the handle to control
    ti.uFlags =  TTF_IDISHWND | TTF_SUBCLASS;  

    //Handle of the Control for which you want to Tooltip to be displayed
    ti.uId    =(UINT)hwnd;

    //Handle of the parent window (The window in which the Control resides)
    ti.hwnd   = hwnd;
    ti.hinst  = g_hInstance;
    ti.lpszText  = tips;         //Text you want as a Tooltip
    	//Below is the Rectangle for ToolTip. But It will be ignored 
    	//If you use TTF_IDISHWND in Flag
    //ti.rect.left = ti.rect.top = ti.rect.bottom = ti.rect.right = 0; 

    //Will Activate the tooltip window
    SendMessage(htip_control,TTM_ACTIVATE,TRUE,0);  

    //will Add the Tooltip Window
    SendMessage(htip_control,TTM_ADDTOOL,0,(LPARAM)&ti);
}


