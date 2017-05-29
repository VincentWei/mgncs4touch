/* 
** $Id: helloworld.c 767 2009-12-08 06:42:19Z houhuihua $
**
** Listing 1.1
**
** helloworld.c: Sample program for MiniGUI Programming Guide
**      The first MiniGUI application.
**
** Copyright (C) 2004 ~ 2009 Feynman Software.
**
** License: GPL
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>
#include "balloon_tip_maker.h"

#define NDEBUG	1
#include <../include/mpaddebug.h>

static int direction = TRIANGLE_UP;
static int HelloWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{

    HDC hdc;

    switch (message) {
        case MSG_CREATE:
            /* 
             * you can only use this method to make a balloon tip 
             * */
            SetBalloonTipStyle (hWnd, 200, direction);
            break;

        case MSG_PAINT:
            hdc = BeginPaint (hWnd);
            EndPaint (hWnd, hdc);
            return 0;

        case MSG_CLOSE:
            KillTimer (hWnd, 100);
            DestroyMainWindow (hWnd);
            PostQuitMessage (hWnd);
            return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

int MiniGUIMain (int argc, const char* argv[])
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "helloworld" , 0 , 0);
#endif

    if (argc > 1) {
        if (strcmp (argv [1], "u") == 0) {
            direction = TRIANGLE_UP;
        } else if (strcmp (argv [1], "d") == 0) {
            direction = TRIANGLE_DOWN;
        } else if (strcmp (argv [1], "l") == 0) {
            direction = TRIANGLE_LEFT;
        } else if (strcmp (argv [1], "r") == 0) {
            direction = TRIANGLE_RIGHT;
        } else {
            LOGE("man, not support this direction!\n");
            return 1;
        }
    } else {
            direction = TRIANGLE_DOWN;
    }



    CreateInfo.dwStyle = WS_VISIBLE;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "test";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = HelloWinProc;
    CreateInfo.lx = 20;
    CreateInfo.ty = 20;
    CreateInfo.rx = 340;
    CreateInfo.by = 260;
    CreateInfo.iBkColor = COLOR_green;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;
    
    hMainWnd = CreateMainWindow (&CreateInfo);
    
    if (hMainWnd == HWND_INVALID)
        return -1;

    ShowWindow(hMainWnd, SW_SHOWNORMAL);

    while (GetMessage(&Msg, hMainWnd)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);

    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

