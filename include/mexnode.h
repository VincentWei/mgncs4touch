///////////////////////////////////////////////////////////////////////////////
//
//                          IMPORTANT NOTICE
//
// The following open source license statement does not apply to any
// entity in the Exception List published by FMSoft.
//
// For more information, please visit:
//
// https://www.fmsoft.cn/exception-list
//
//////////////////////////////////////////////////////////////////////////////
/*
 * \file mexnode.h
 * \author miniStudio team of FMSoft
 * \date 2010/10/09
 *
 \verbatim

    This file is part of mGNCS4Touch, one of MiniGUI components.

    Copyright (C) 2008-2018 FMSoft (http://www.fmsoft.cn).

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Or,

    As this program is a library, any link to this program must follow
    GNU General Public License version 3 (GPLv3). If you cannot accept
    GPLv3, you need to be licensed from FMSoft.

    If you have got a commercial license of this program, please use it
    under the terms and conditions of the commercial license.

    For more information about the commercial license, please refer to
    <http://www.minigui.com/blog/minigui-licensing-policy/>.

 \endverbatim
*/

#ifndef _MGUI_NCS4TOUCH_ENOD_H_
#define _MGUI_NCS4TOUCH_ENOD_H_
 
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

typedef struct _mExNode         mExNode;
typedef struct _mExNodeClass    mExNodeClass;

#define mExNodeHeader(clsName)  \
	mObjectHeader(clsName)      \
    mObject *body;              \
    mObject *parent;            \
    list_t  children;           \
    list_t  sibling;            \
    Uint16  flags;              \
    Uint16  count;              \
    int     style;              \
    int     height;             \
    char    *text;              \
	char    *subTitle;          \
	char    *detail;            \
    DWORD   addData;


struct _mExNode
{
	mExNodeHeader(mExNode)
};

#define mExNodeClassHeader(clsName, superCls)           \
	mObjectClassHeader(clsName, superCls)               \
	BOOL (*setProperty)(clsName*, int id, DWORD value); \
	DWORD (*getProperty)(clsName*, int id);             \
    BOOL (*computeSize)(clsName*, HDC hdc, SIZE *size); \
    BOOL (*canAsRoot)(clsName*);                        \
    int (*addNode)(clsName*, mNode *node);              \
    int (*removeNode)(clsName*, mNode *node);           \
    int (*insertNode)(clsName*, mNode *node, mNode *prev, mNode *next, int index);\
    int (*moveNode)(clsName*, mNode *curNode, int count, mNode *prevNode);  \
    mNode* (*findNode)(mNode *self, DWORD info, int type, BOOL recursion, int startIndex);\
    mNode* (*findNodeEx)(mNode *self, DWORD info, int type, \
            BOOL recursion, int startIndex, int (*cb)(mNode *node, DWORD info)); \
    void (*removeAll)(clsName*);                        \
    void (*sendEvent)(clsName*, int eventId);           \
    mNode* (*getNode)(clsName*, int index);             \
    int (*indexOf)(clsName*);                           \
    mNode* (*getPrev)(clsName*);                        \
    mNode* (*getNext)(clsName*);                        \
    BOOL (*isEnabled)(clsName*);                        \
    BOOL (*isSelected)(clsName*);                       \
    BOOL (*hasChildren)(clsName*);                      \
    BOOL (*setText)(clsName*, const char* text);        \
    BOOL (*setImage)(clsName*, const PBITMAP image);    \
    const char* (*getText)(clsName*);                   \
    PBITMAP (*getImage)(clsName*);                      \
    void (*paint)(clsName*, HDC hdc, const RECT *rect, mObject *owner);  \
    int (*msgHandler)(clsName*, int msg, WPARAM wParam, LPARAM lParam, mObject *owner);\
    void (*setTitle) (clsName *, const char *);     \
    void (*setSubTitle) (clsName *, const char *);  \
    void (*setDetail) (clsName *, const char *);
    

struct _mExNodeClass
{
	mExNodeClassHeader(mExNode, mObject)
};

#define NCSS_ENOD_NORMAL        (0x00)
#define NCSS_ENOD_BOOLEAN       (0x01)
#define NCSS_ENOD_DETAIL        (0x02)
#define NCSS_ENOD_CHECKABLE     (0x04)	
#define NCSS_ENOD_DELETABLE     (0x08)	
#define NCSS_ENOD_NODETAIL      (0x10)
#define NCSS_ENOD_NOSUBTITLE    (0x20)
#define NCSS_ENOD_NOICON        (0x40)
#define NCSS_ENOD_SHIFT         (7)

enum mExNodeProp
{ 
	NCSP_ENOD_TITLE = 0,
	NCSP_ENOD_SUBTITLE,
	NCSP_ENOD_DETAIL,
	NCSP_ENOD_ICON,
	NCSP_ENOD_MAX
};

enum eExNodeNotify
{
	NCSN_ENOD_CHANGED = 0, 
	NCSN_ENOD_MAX
};

MTOUCH_EXPORT extern mExNodeClass g_stmExNodeCls;

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* _MGUI_NCS4TOUCH_ENOD_H_ */

