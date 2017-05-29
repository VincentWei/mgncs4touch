/*********************************************************************
 * Copyright (C) 2002 ~ 2010, Feynman Software Technology Co., Ltd.
 * Room 508B-C, Floor 5, Citic Guoan Shumagang, No.32, Haidian South
 * Road, Haidian District, Beijing, P. R. CHINA 100080.
 * All rights reserved.
 *
 * This software is the confidential and proprietary information of
 * Feynman Software Technology Co. Ltd. ("Confidential Information").
 * You shall not disclose such Confidential Information and shall use
 * it only in accordance you entered into with Feynman Software.
 *          http://www.minigui.com
 *********************************************************************/

#ifndef _MGUI_NCS4PAD_ENOD_H_
#define _MGUI_NCS4PAD_ENOD_H_
 
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

MPAD_EXPORT extern mExNodeClass g_stmExNodeCls;

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* _MGUI_NCS4PAD_ENOD_H_ */

