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
#include "mdatepicker.h"

#define NDEBUG	1
#include "mtouchdebug.h"

static int nMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static int lMonth[] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

#define IsLeapYear(y)      ((((y)%4==0) && ((y)%100!=0)) || ((y)%400==0))
#define getMonthDays(y,m)  (IsLeapYear(y) ? lMonth[m] : nMonth[m]) 


#define DP_YEAR_ID		1001
#define DP_MONTH_ID		1002
#define DP_DAY_ID		1003


static void initDateCtrlTempl(NCS_WND_TEMPLATE* ctrl, int id, PRECT pRc)
{
	ctrl->class_name = NCSCTRL_PICKER;
	ctrl->id = id;
	ctrl->x = pRc->left;
	ctrl->y = pRc->top;
	ctrl->w = RECTWP(pRc);
	ctrl->h = RECTHP(pRc);
	ctrl->ex_style = WS_EX_NONE;
	ctrl->caption = "";
	ctrl->props = NULL;
	ctrl->rdr_info = NULL;
	ctrl->handlers = NULL;
	ctrl->ctrls = NULL;
	ctrl->count = 0;
	ctrl->user_data = 0;
	ctrl->bk_color = 0;
	ctrl->font_name = NULL;
	ctrl->style = WS_VISIBLE | NCSS_NOTIFY | NCSS_PCK_NUMBER;
	if (id != DP_YEAR_ID)
		ctrl->style |= NCSS_PCK_LOOP;
}


static void initPickerElement(mPicker* pself, int visial, int effect, int count, int min, int max)
{
	if (!pself)
		return;

	_c(pself)->setProperty(pself, NCSP_PCK_LINES, visial);
	_c(pself)->setProperty(pself, NCSP_PCK_COUNT, count);
	_c(pself)->setRange(pself, min, max);
}


static void showDate(mDatePicker *self)
{
	mPicker* pYear  = NULL;
	mPicker* pMonth = NULL;
	mPicker* pDay   = NULL;

	if (NULL != (pYear = (mPicker*)_M(self, getChild, DP_YEAR_ID))){
		_M(pYear, setProperty, NCSP_PCK_SELIDX, self->year - 1970);
    }
	
    if (NULL != (pMonth = (mPicker*)_M(self, getChild, DP_MONTH_ID))){
		_M(pMonth, setProperty, NCSP_PCK_SELIDX, self->month - 1);
    }
	
    if (NULL != (pDay = (mPicker*)_M(self, getChild, DP_DAY_ID))) {
		_M(pDay, setProperty, NCSP_PCK_SELIDX, self->day - 1);
	}
}


static BOOL resetYearMonthDay(mDatePicker* self, mPicker* pDay, BOOL bYMChange, int oldDay)
{
	int days = 0;

	if (!self || !pDay)
		return FALSE;

	days = getMonthDays(self->year, self->month);

	_M(pDay, enableItems, 28, -1);
	_M(pDay, disableItems, days, -1);

	if (bYMChange) { // change from Year or Month Picker
		if (self->day > days){
			_M(self, setProperty, NCSP_DPCK_DAY, days);
		} else { 
			InvalidateRect(pDay->hwnd, NULL, TRUE);
		}
	} else { //change from Day Picker
		if (self->day > days && days < 31) {   // 1 <<-->> 30/29/28 
			_M(self, setProperty, NCSP_DPCK_DAY, oldDay == 1 ? days : 1);
		} 
	}

	return TRUE;
}


static void initDatePicker(mDatePicker* self)
{
	mPicker* pYear  = NULL;
	mPicker* pMonth = NULL;
	mPicker* pDay   = NULL;
	DWORD dwStyle = GetWindowStyle(self->hwnd);

	pYear = (mPicker*)_M(self, getChild, DP_YEAR_ID);
	if ((dwStyle & NCSS_DPCK_YYMMDD)
			|| (dwStyle & NCSS_DPCK_MMDDYY)) {
        _M(pYear, setProperty, NCSP_PCK_SHOWBERBITS, 2);
	}
	initPickerElement(pYear, 5, 2038 - 1970 + 1, 2038 - 1970 + 1, 1970, 2038);
	_M(pYear, setProperty, NCSP_PCK_NUMBERBITS, 4);

	ncsAddEventListener((mObject*)pYear, 
			(mObject*)self, _c(self)->listenerCallBack, NCSN_PCK_SELCHANGED);

	pMonth = (mPicker*)_M(self, getChild, DP_MONTH_ID);
	initPickerElement(pMonth, 5, 12, 12, 1, 12);
	ncsAddEventListener((mObject*)pMonth, 
			(mObject*)self, _c(self)->listenerCallBack, NCSN_PCK_SELCHANGED);

	pDay = (mPicker*)_M(self, getChild, DP_DAY_ID);
	initPickerElement(pDay, 5, 30, 31, 1, 31);
	resetYearMonthDay(self, pDay, FALSE, 0);
	ncsAddEventListener((mObject*)pDay, 
			(mObject*)self, _c(self)->listenerCallBack, NCSN_PCK_SELCHANGED);
}


static void set_date_from_localtime(mDatePicker * self)
{
	time_t tim;
	struct tm *ptm;

	time(&tim);
	ptm = localtime(&tim);

	if (!ptm)
		return;

	self->year  = ptm->tm_year + 1900;
	self->month = ptm->tm_mon + 1;
	self->day   = ptm->tm_mday;
}


static void mDatePicker_construct(mDatePicker* self, DWORD addData)
{
	Class(mComboPicker).construct((mComboPicker*)self, addData);

	self->idYear  = DP_YEAR_ID;
	self->idMonth = DP_MONTH_ID;
	self->idDay   = DP_DAY_ID;

	set_date_from_localtime(self);
}


static BOOL CalcRect(mDatePicker *self, PRECT all, PRECT rc1, PRECT rc2, PRECT rc3)
{
	int width;
	DWORD dwStyle = GetWindowStyle(self->hwnd);

	if (NULL == all)
		return FALSE;

	memcpy(rc1, all, sizeof(RECT));
	memcpy(rc2, all, sizeof(RECT));
	memcpy(rc3, all, sizeof(RECT));

	if (dwStyle & NCSS_DPCK_MMDDYYYY) {
		width = RECTWP(all) / 4;
		rc1->left  = all->left; 
		rc1->right = rc1->left + width; 
		rc2->left  = rc1->right; 
		rc2->right = rc2->left + width; 
		rc3->left  = rc2->right; 
		rc3->right = all->right; 
	} else if ((dwStyle & NCSS_DPCK_YYMMDD)
			|| (dwStyle & NCSS_DPCK_MMDDYY)) {
		width = RECTWP(all) / 3;
		rc1->left  = all->left; 
		rc1->right = all->left + width; 
		rc2->left  = rc1->right; 
		rc2->right = rc2->left + width; 
		rc3->left  = rc2->right; 
		rc3->right = all->right; 
	} else {
		width = RECTWP(all) / 4;
		rc1->left  = all->left; 
		rc1->right = all->left + (width<<1); 
		rc2->left  = rc1->right; 
		rc2->right = rc2->left + width; 
		rc3->left  = rc2->right; 
		rc3->right = all->right; 
	}

	return TRUE;
}


static BOOL mDatePickerOnCreateInit(mDatePicker* self)
{
	RECT rc;
	RECT rc1, rc2, rc3;
	static NCS_WND_TEMPLATE ctrl[3];
	int picker_id1 = 0, picker_id2 = 0, picker_id3 = 0;
	int i = 0;
	DWORD dwStyle = GetWindowStyle(self->hwnd);

	_M(self, getClientArea, &rc);
	CalcRect(self, &rc, &rc1, &rc2, &rc3);

	if (dwStyle & NCSS_DPCK_YYMMDD) {
		picker_id1 = DP_YEAR_ID;
		picker_id2 = DP_MONTH_ID;
		picker_id3 = DP_DAY_ID;
	} else if ((dwStyle & NCSS_DPCK_MMDDYYYY)
			|| (dwStyle & NCSS_DPCK_MMDDYY)) {
		picker_id1 = DP_MONTH_ID;
		picker_id2 = DP_DAY_ID;
		picker_id3 = DP_YEAR_ID;
	} else {
		picker_id1 = DP_YEAR_ID;
		picker_id2 = DP_MONTH_ID;
		picker_id3 = DP_DAY_ID;
	}

	initDateCtrlTempl(&ctrl[0], picker_id1, &rc1);
	initDateCtrlTempl(&ctrl[1], picker_id2, &rc2);
	initDateCtrlTempl(&ctrl[2], picker_id3, &rc3);

	ctrl[0].style |= NCSS_PCK_LRC;
	ctrl[2].style |= NCSS_PCK_RRC;

	for (i = 0; i < sizeof(ctrl) / sizeof(NCS_WND_TEMPLATE); i++) {
		Class(mComboPicker).addPicker((mComboPicker*)self, &ctrl[i], i + 1);
	}

	initDatePicker(self);
	showDate(self);

	return TRUE;
}


static BOOL mDatePicker_onCreate(mDatePicker* self, DWORD addData)
{
	Class(mComboPicker).onCreate((mComboPicker *)self, addData);
	
	mDatePickerOnCreateInit(self);

	return TRUE; 
}


static void mDatePicker_setDate(mDatePicker* self, int year, int month, int day)
{
	if (!self)
		return;

	if (year >= 1970 && year <= 2038)
		self->year = year;

	if (month >= 1 && month <= 12)
		self->month = month;

	if (day >= 1 && day <= getMonthDays(year, month))
		self->day = day;

	showDate(self);
}


static BOOL mDatePicker_getDate(mDatePicker* self, int* year, int* month, int* day)
{
	if (!self)
		return FALSE;

	if (year) {
		*year = self->year;
	}
	if (month) {
		*month = self->month;
	}
	if (day) {
		*day = self->day;
	}

	return TRUE;
}


static BOOL mDatePicker_setProperty(mDatePicker* self, int id, DWORD value)
{
	if (id >= NCSP_DPCK_MAX)
		return FALSE;

	switch (id) {
		case NCSP_DPCK_YEAR:
			_c(self)->setDate(self, value, self->month, self->day);
			InvalidateRect(self->hwnd, NULL, TRUE);
			return TRUE;

		case NCSP_DPCK_MONTH:
			_c(self)->setDate(self, self->year, value, self->day);
			InvalidateRect(self->hwnd, NULL, TRUE);
			return TRUE;

		case NCSP_DPCK_DAY:
			_c(self)->setDate(self, self->year, self->month, value);
			InvalidateRect(self->hwnd, NULL, TRUE);
			return TRUE;

		default:
			break;
	}

	return Class(mComboPicker).setProperty((mComboPicker*)self, id, value);
}


static DWORD mDatePicker_getProperty(mDatePicker* self, int id)
{
	if (id >= NCSP_DPCK_MAX)
		return 0;

	switch (id) {
		case NCSP_DPCK_YEAR:
			return (DWORD)self->year;
		case NCSP_DPCK_MONTH:
			return (DWORD)self->month;
		case NCSP_DPCK_DAY:
			return (DWORD)self->day;
		default:
			break;
	}

	return Class(mComboPicker).getProperty((mComboPicker*)self, id);
}


static BOOL mDatePicker_listenerCallBack(mObject* self, mObject* sender,
		int eventId, DWORD param)
{
	mPicker* pYear  = NULL;
	mPicker* pMonth = NULL;
	mPicker* pDay   = NULL;
	mDatePicker *_ths = (mDatePicker*)self;
	BOOL bYearMonthChange = FALSE;
	int old_day = _ths->day;

	pYear  = (mPicker*)_M((mDatePicker *)self, getChild, DP_YEAR_ID);
	pMonth = (mPicker*)_M((mDatePicker *)self, getChild, DP_MONTH_ID);
	pDay   = (mPicker*)_M((mDatePicker *)self, getChild, DP_DAY_ID);

	if (pDay == (mPicker*)sender) {
		_ths->day = _M(pDay, getProperty, NCSP_PCK_SELIDX) + 1;	
	} else if (pMonth == (mPicker*)sender) {
		bYearMonthChange = TRUE;
		_ths->month = _M(pMonth, getProperty, NCSP_PCK_SELIDX) + 1;	
	} else if (pYear == (mPicker*)sender) {
		_ths->year = _M(pYear, getProperty, NCSP_PCK_SELIDX) + 1970;
		if (2 == _ths->month)
			bYearMonthChange = TRUE;
	} else {
		return FALSE;
	}

	resetYearMonthDay(_ths, pDay, bYearMonthChange, old_day);
	ncsNotifyParent(_ths, NCSN_DPCK_DATECHANGED);

	return TRUE;
}


static int mDatePicker_onSizeChanged(mDatePicker *self, RECT *rtClient)
{
	RECT rcall, rc1, rc2, rc3;
	mPicker* p1 = NULL;
	mPicker* p2 = NULL;
	mPicker* p3 = NULL;
	int id1, id2, id3;
	DWORD dwStyle = GetWindowStyle(self->hwnd);

	_M(self, getClientArea, &rcall);
	CalcRect(self, &rcall, &rc1, &rc2, &rc3);

	if (dwStyle & NCSS_DPCK_YYMMDD) {
		id1 = DP_YEAR_ID;
		id2 = DP_MONTH_ID;
		id3 = DP_DAY_ID;
	} else if ((dwStyle & NCSS_DPCK_MMDDYYYY)
			|| (dwStyle & NCSS_DPCK_MMDDYY)) {
		id1 = DP_MONTH_ID;
		id2 = DP_DAY_ID;
		id3 = DP_YEAR_ID;
	} else {
		id1 = DP_YEAR_ID;
		id2 = DP_MONTH_ID;
		id3 = DP_DAY_ID;
	}

	if (NULL != (p1 = (mPicker*)_M(self, getChild, id1))){
		MoveWindow(p1->hwnd, rc1.left, rc1.top, RECTW(rc1), RECTH(rc1), TRUE);
	}
	if (NULL != (p2 = (mPicker*)_M(self, getChild, id2))){
		MoveWindow(p2->hwnd, rc2.left, rc2.top, RECTW(rc2), RECTH(rc2), TRUE);
	}
	if (NULL != (p3 = (mPicker*)_M(self, getChild, id3))){
		MoveWindow(p3->hwnd, rc3.left, rc3.top, RECTW(rc3), RECTH(rc3), TRUE);
	}

	return 0;
}


#ifdef _MGNCS4TOUCH_GUIBUILDER_SUPPORT
static BOOL mDatePicker_refresh(mDatePicker* self)
{
	_c(self)->removeAllPickers(self);
	mDatePickerOnCreateInit(self);
	InvalidateRect(self->hwnd, NULL, TRUE);

	return TRUE;
}
#endif


BEGIN_CMPT_CLASS(mDatePicker, mComboPicker)
	CLASS_METHOD_MAP(mDatePicker, construct)
	CLASS_METHOD_MAP(mDatePicker, onCreate)
	CLASS_METHOD_MAP(mDatePicker, onSizeChanged)
	CLASS_METHOD_MAP(mDatePicker, setDate)
	CLASS_METHOD_MAP(mDatePicker, getDate)
	CLASS_METHOD_MAP(mDatePicker, setProperty)
	CLASS_METHOD_MAP(mDatePicker, getProperty)
	CLASS_METHOD_MAP(mDatePicker, listenerCallBack)
#ifdef _MGNCS4TOUCH_GUIBUILDER_SUPPORT
	CLASS_METHOD_MAP(mDatePicker, refresh)
#endif
END_CMPT_CLASS

