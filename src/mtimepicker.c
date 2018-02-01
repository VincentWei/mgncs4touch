/*
 *   This file is part of mGNCS4Touch, a component for MiniGUI.
 * 
 *   Copyright (C) 2008~2018, Beijing FMSoft Technologies Co., Ltd.
 * 
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 * 
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 * 
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 *   Or,
 * 
 *   As this program is a library, any link to this program must follow
 *   GNU General Public License version 3 (GPLv3). If you cannot accept
 *   GPLv3, you need to be licensed from FMSoft.
 * 
 *   If you have got a commercial license of this program, please use it
 *   under the terms and conditions of the commercial license.
 * 
 *   For more information about the commercial license, please refer to
 *   <http://www.minigui.com/en/about/licensing-policy/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgncs/mgncs.h>
#include <mgeff/mgeff.h>

#include "mtouchcomm.h"
#include "manimation.h"
#include "mpicker.h"
#include "mcombopicker.h"
#include "mtimepicker.h"

// #define NDEBUG	1
#include "mtouchdebug.h"


#define TP_AMPM_ID		1001
#define TP_HOUR_ID		1002
#define TP_MINUTE_ID	1003


static void initTimeCtrl(NCS_WND_TEMPLATE* ctrl, 
		int id, PRECT pRc, DWORD style)
{
	ctrl->class_name = NCSCTRL_PICKER;
	ctrl->id = id;
	ctrl->x = pRc->left;
	ctrl->y = pRc->top;
	ctrl->w = RECTWP(pRc);
	ctrl->h = RECTHP(pRc);
	ctrl->style = WS_VISIBLE | NCSS_NOTIFY | style;
	ctrl->ex_style = WS_EX_USEPARENTRDR;
	ctrl->caption = "";
	ctrl->props = NULL;
	ctrl->rdr_info = NULL;
	ctrl->handlers = NULL;
	ctrl->ctrls = NULL;
	ctrl->count = 0;
	ctrl->user_data = 0;
	ctrl->bk_color = 0;
	ctrl->font_name = NULL;
}


static void initPickerElement(mPicker* pself, int visial, int selIdx, int min, int max)
{
	if (pself)
    {
        _M(pself, setProperty, NCSP_PCK_LINES, visial);
        _M(pself, setProperty, NCSP_PCK_SELIDX, selIdx);
        _M(pself, setRange, min, max);
    }
}


static void addListener(mTimePicker* self)
{
	mPicker *pAmpm   = NULL;
	mPicker *pHour   = NULL;
	mPicker *pMinute = NULL;

	if (GetWindowStyle(self->hwnd) & NCSS_TPCK_12H) {
		pAmpm = (mPicker*)_M(self, getChild, TP_AMPM_ID);
		ncsAddEventListener((mObject *)pAmpm, 
				(mObject*)self, _c(self)->listenerCallBack, NCSN_PCK_SELCHANGED);
	}

	pHour = (mPicker*)_M(self, getChild, TP_HOUR_ID);
	ncsAddEventListener((mObject *)pHour, 
			(mObject*)self, _c(self)->listenerCallBack, NCSN_PCK_SELCHANGED);

	pMinute = (mPicker*)_M(self, getChild, TP_MINUTE_ID);
	ncsAddEventListener((mObject *)pMinute, 
			(mObject*)self, _c(self)->listenerCallBack, NCSN_PCK_SELCHANGED);
}

static int ParseString(const char* strSrc, char* str1,
		char* str2, int str1_len, int str2_len)
{
	char* p1 = NULL;
	char* p2 = NULL;

	if (NULL == strSrc)
		return -1;

	p1 = (char*)strSrc;
	p2 = strchr(strSrc, '\n');

	if (p2 == NULL) {
		// there is no '\n', 
		// so the second section is NULL
		snprintf(str1, str1_len, "%s", strSrc);
		memset(str2, 0, str2_len);
		return 0;
	}
	
	p2++;

	str1_len = MIN(str1_len, (p2 - p1));
	str2_len = MIN(str2_len, (strlen(p2) + 1));

	snprintf(str1, str1_len, "%s", p1);
	snprintf(str2, str2_len, "%s", p2);

	return 0;
}

static void resetAmPmPicker (mTimePicker *self)
{
	char strAm[128];
	char strPm[128];
	mPicker* pAmpm   = NULL;
    pAmpm = (mPicker*)_M(self, getChild, TP_AMPM_ID);

    ParseString (self->ampmStrs, strAm, strPm, 128, 128);
    
    _M(pAmpm, freeze, TRUE);
    _M(pAmpm, removeAll);
    _M(pAmpm, addItem, -1, strAm);
    _M(pAmpm, addItem, -1, strPm);
    _M(pAmpm, freeze, FALSE);
}

static void initTimePicker(mTimePicker* self)
{
	mPicker* pHour   = NULL;
	mPicker* pMinute = NULL;
	DWORD dwStyle = GetWindowStyle(self->hwnd);

	if (dwStyle & NCSS_TPCK_12H) {
        resetAmPmPicker (self);
		pHour = (mPicker*)_M(self, getChild, TP_HOUR_ID);
		initPickerElement(pHour, 5, 8, 1, 12);
	} else {
		pHour = (mPicker*)_M(self, getChild, TP_HOUR_ID);
		initPickerElement(pHour, 5, 8, 0, 23);
	}

	pMinute = (mPicker*)_M(self, getChild, TP_MINUTE_ID);
	initPickerElement(pMinute, 5, 0, 0, 59);
	
    _M(pHour,   setProperty, NCSP_PCK_NUMBERBITS, 2);
	_M(pMinute, setProperty, NCSP_PCK_NUMBERBITS, 2);
}


static void showTimes (mTimePicker *self)
{
	mPicker* pAmpm   = NULL;
	mPicker* pHour   = NULL;
	mPicker* pMinute = NULL;
	DWORD dwStyle = GetWindowStyle(self->hwnd);

	pHour = (mPicker*)_M(self, getChild, TP_HOUR_ID);
	pMinute = (mPicker*)_M(self, getChild, TP_MINUTE_ID);

    if (pHour == NULL || pMinute == NULL)
        return;

	if (dwStyle & NCSS_TPCK_12H) {
		pAmpm = (mPicker*)_M(self, getChild, TP_AMPM_ID);
        if (pAmpm == NULL)
            return;

		if (self->hour >= 12)
			_M(pAmpm, setProperty, NCSP_PCK_SELIDX, 1);
		else
			_M(pAmpm, setProperty, NCSP_PCK_SELIDX, 0);

		_M(pHour, setProperty, NCSP_PCK_SELIDX, (self->hour % 12) - 1);
	} else {
		_M(pHour, setProperty, NCSP_PCK_SELIDX, self->hour);
	}

	_M(pMinute, setProperty, NCSP_PCK_SELIDX, self->minute);
}

static void set_time_from_localtime(mTimePicker * self)
{
	time_t tim;
	struct tm *ptm;

	time(&tim);
	ptm = localtime(&tim);

	if (!ptm) return;

	self->hour  = ptm->tm_hour;
	self->minute = ptm->tm_min;
}

static void mTimePicker_construct(mTimePicker* self, DWORD addData)
{
	Class(mComboPicker).construct((mComboPicker*)self, addData);

	self->idAmpm   = 0;
	self->idHour   = 0;
	self->idMinute = 0;
    self->ampmStrs = strdup("AM\nPM");
    
    set_time_from_localtime (self);
}

static void mTimePicker_destroy (mTimePicker *self)
{
    if (self->ampmStrs)
        free(self->ampmStrs);
    self->ampmStrs = NULL;

	Class(mComboPicker).destroy((mComboPicker*)self);
}

static BOOL CalcRect (mTimePicker *self, PRECT all, PRECT rcA, PRECT rcH, PRECT rcM)
{
	int width;

	if (NULL == all)
		return FALSE;

	memcpy(rcH, all, sizeof(RECT));
	memcpy(rcM, all, sizeof(RECT));

	if (GetWindowStyle(self->hwnd) & NCSS_TPCK_12H) {
		width = RECTWP(all) / 3;
		memcpy(rcA, all, sizeof(RECT));
		rcA->left  = all->left;
		rcA->right = rcA->left + width; 
		rcH->left  = all->left + width; 
		rcH->right = rcH->left + width;
		rcM->left  = all->left + (width<<1);
		rcM->right = rcM->left + width;
	} else {
		width = RECTWP(all) / 2;
		rcH->left  = all->left; 
		rcH->right = rcH->left + width;
		rcM->left  = all->left + width;
		rcM->right = rcM->left + width;
	}
	return TRUE;
}


static BOOL mTimePickerOnCreateInit(mTimePicker* self)
{
	RECT rc;
	static NCS_WND_TEMPLATE ctrl[3];
	RECT rcA, rcH, rcM;
	int i = 0, count = 0;
	DWORD dwStyle = GetWindowStyle(self->hwnd);

	_M(self, getClientArea, &rc);

	CalcRect(self, &rc, &rcA, &rcH, &rcM);

	if (dwStyle & NCSS_TPCK_12H) {
		self->idAmpm = TP_AMPM_ID;
		count = 3;
		initTimeCtrl(&ctrl[0], TP_AMPM_ID, &rcA, NCSS_PCK_STRINGS);
		initTimeCtrl(&ctrl[1], TP_HOUR_ID, &rcH,
				NCSS_PCK_NUMBER | NCSS_PCK_LOOP);
		initTimeCtrl(&ctrl[2], TP_MINUTE_ID, &rcM,
				NCSS_PCK_NUMBER | NCSS_PCK_LOOP);
	} else {
		self->idAmpm = -1;
		count = 2;
		initTimeCtrl(&ctrl[0], TP_HOUR_ID, &rcH,
				NCSS_PCK_NUMBER | NCSS_PCK_LOOP);
		initTimeCtrl(&ctrl[1], TP_MINUTE_ID, &rcM,
				NCSS_PCK_NUMBER | NCSS_PCK_LOOP);
	}
	self->idHour   = TP_HOUR_ID;
	self->idMinute = TP_MINUTE_ID;

	ctrl[0].style |= NCSS_PCK_LRC;
	ctrl[count-1].style |= NCSS_PCK_RRC;

	for (i = 0; i < count; i++) {
		Class(mComboPicker).addPicker((mComboPicker*)self, &ctrl[i], i + 1);
	}

	addListener(self);
	initTimePicker(self);
	showTimes(self);

	return TRUE;
}


static BOOL mTimePicker_onCreate(mTimePicker* self, DWORD addData)
{
	Class(mComboPicker).onCreate((mComboPicker *)self, addData);
	
	mTimePickerOnCreateInit(self);
	
	return TRUE;
}


static void mTimePicker_setTime(mTimePicker* self, int hour, int minute)
{
	if (!self)
		return;

	if (hour >= 0 && hour <= 23)
		self->hour = hour;

	if (minute >= 0 && minute <= 59)
		self->minute = minute;
    
	showTimes(self);
	return;
}


static BOOL mTimePicker_getTime(mTimePicker* self, int* hour, int* minute)
{
    BOOL ret = FALSE;

	if (!self)
		return FALSE;

	if (hour) {
		*hour = self->hour;
		ret = TRUE;
	}
	if (minute) {
		*minute = self->minute;
		ret = TRUE;
	}

	return ret;
}


static BOOL mTimePicker_setProperty(mTimePicker* self, int id, DWORD value)
{
	if (id >= NCSP_TPCK_MAX)
		return FALSE;

	switch (id) {
        case NCSP_TPCK_HOUR:
			if ((int)value >= 0 && (int)value <= 23)
			{
				self->hour = (int)value;
				showTimes(self);
				InvalidateRect(self->hwnd, NULL, TRUE);
				return TRUE;
			}
			return FALSE;

		case NCSP_TPCK_MINUTE:
			if ((int)value >= 0 && (int)value <= 59)
			{
				self->minute = (int)value;
				showTimes(self);
				InvalidateRect(self->hwnd, NULL, TRUE);
				return TRUE;
			}
			return FALSE;

        case NCSP_TPCK_AMPMSTRINGS:
            if ((char *)value == NULL)
                return FALSE;
            if (self->ampmStrs)
                free (self->ampmStrs);
            self->ampmStrs = strdup ((char*)value);
            if (GetWindowStyle(self->hwnd) & NCSS_TPCK_12H)
                resetAmPmPicker (self);
            return TRUE;

		default:
			break;
	}

	return Class(mComboPicker).setProperty((mComboPicker*)self, id, value);
}


static DWORD mTimePicker_getProperty(mTimePicker* self, int id)
{
	if (id >= NCSP_TPCK_MAX)
		return 0;

	switch (id) {
		case NCSP_TPCK_HOUR:
			return (DWORD)self->hour;

		case NCSP_TPCK_MINUTE:
			return (DWORD)self->minute;

        case NCSP_TPCK_AMPMSTRINGS:
            return (DWORD)self->ampmStrs;

		default:
			break;
	}

	return Class(mComboPicker).getProperty((mComboPicker*)self, id);
}


static BOOL mTimePicker_listenerCallBack(mObject* self, mObject* sender,
		int eventId, DWORD param)
{
	int old;
	mPicker* pAmpm   = NULL;
	mPicker* pHour   = NULL;
	mPicker* pMinute = NULL;
	mTimePicker* _ths = (mTimePicker*)self;

	pAmpm   = (mPicker*)_M((mTimePicker *)self, getChild, TP_AMPM_ID);
	pHour   = (mPicker*)_M((mTimePicker *)self, getChild, TP_HOUR_ID);
	pMinute = (mPicker*)_M((mTimePicker *)self, getChild, TP_MINUTE_ID);

	if (pMinute == (mPicker*)sender) {
		old = _ths->minute;
		_ths->minute = _M(pMinute, getProperty, NCSP_PCK_SELIDX);
		if (old == 0 && _ths->minute == 59) {
			_M(_ths, setProperty, NCSP_TPCK_HOUR, 
					_ths->hour == 0 ? 23 :  _ths->hour - 1);
		} else if (old == 59 && _ths->minute == 0) {
			_M(_ths, setProperty, NCSP_TPCK_HOUR,
					_ths->hour == 23 ? 0 :  _ths->hour + 1);
		}
	} else if (pHour == (mPicker*)sender) {
		if (GetWindowStyle (_ths->hwnd) & NCSS_TPCK_12H) 
		{
			int isPm = _M(pAmpm, getProperty, NCSP_PCK_SELIDX);
			old = _ths->hour;
			if (isPm){
				_ths->hour = (_M(pHour, getProperty, NCSP_PCK_SELIDX) + 12 + 1) % 24;
				if ((old == 0 || old == 12 ) && _ths->hour == 23){
					_M(pAmpm, setProperty, NCSP_PCK_SELIDX, 0);
					_ths->hour = 11; 
				}
			} else {
				_ths->hour = _M(pHour, getProperty, NCSP_PCK_SELIDX) + 1;
				if (old == 11 && _ths->hour == 12){
					_M(pAmpm, setProperty, NCSP_PCK_SELIDX, 1);
					_ths->hour = 12; 
				}
			}
		} else {
			_ths->hour = _M(pHour, getProperty, NCSP_PCK_SELIDX);
		}
	}
	ncsNotifyParent (_ths, NCSN_TPCK_TIMECHANGED);

	return TRUE;
}


static int mTimePicker_onSizeChanged(mTimePicker *self, RECT *rtClient)
{
	RECT rcall, rcA, rcH, rcM;
	mPicker* pAmpm   = NULL;
	mPicker* pHour   = NULL;
	mPicker* pMinute = NULL;

	_M(self, getClientArea, &rcall);
	CalcRect(self, &rcall, &rcA, &rcH, &rcM);

	if (GetWindowStyle(self->hwnd) & NCSS_TPCK_12H) {
		if (NULL != (pAmpm = (mPicker*)_M(self, getChild, TP_AMPM_ID))){
			MoveWindow(pAmpm->hwnd, rcA.left, rcA.top, RECTW(rcA), RECTH(rcA), TRUE);
		}
		if (NULL != (pHour = (mPicker*)_M(self, getChild, TP_HOUR_ID))){
			MoveWindow(pHour->hwnd, rcH.left, rcH.top, RECTW(rcH), RECTH(rcH), TRUE);
		}
		if (NULL != (pMinute = (mPicker*)_M(self, getChild, TP_MINUTE_ID))){
			MoveWindow(pMinute->hwnd, rcM.left, rcM.top, RECTW(rcM), RECTH(rcM), TRUE);
		}
	} else {
		if (NULL != (pHour = (mPicker*)_M(self, getChild, TP_HOUR_ID))){
			MoveWindow(pHour->hwnd, rcH.left, rcH.top, RECTW(rcH), RECTH(rcH), TRUE);
		}
		if (NULL != (pMinute = (mPicker*)_M(self, getChild, TP_MINUTE_ID))){
			MoveWindow(pMinute->hwnd, rcM.left, rcM.top, RECTW(rcM), RECTH(rcM), TRUE);
		}
	}

	return 0;
}


#ifdef _MGNCS4TOUCH_GUIBUILDER_SUPPORT
static BOOL mTimePicker_refresh(mTimePicker* self)
{
	LOGELINE();
	_c(self)->removeAllPickers(self);
	mTimePickerOnCreateInit(self);
	InvalidateRect(self->hwnd, NULL, TRUE);

	return TRUE;
}
#endif


BEGIN_CMPT_CLASS(mTimePicker, mComboPicker)
	CLASS_METHOD_MAP(mTimePicker, construct)
	CLASS_METHOD_MAP(mTimePicker, destroy)
	CLASS_METHOD_MAP(mTimePicker, onCreate)
	CLASS_METHOD_MAP(mTimePicker, onSizeChanged)
	CLASS_METHOD_MAP(mTimePicker, setTime)
	CLASS_METHOD_MAP(mTimePicker, getTime)
	CLASS_METHOD_MAP(mTimePicker, setProperty)
	CLASS_METHOD_MAP(mTimePicker, getProperty)
	CLASS_METHOD_MAP(mTimePicker, listenerCallBack)
#ifdef _MGNCS4TOUCH_GUIBUILDER_SUPPORT
	CLASS_METHOD_MAP(mTimePicker, refresh)
#endif
END_CMPT_CLASS

