#ifndef _MGUI_NCSCTRL_EXLIST_H
#define _MGUI_NCSCTRL_EXLIST_H
 

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define NCSCTRL_EXLIST          NCSCLASSNAME("exlist")

typedef struct _mExList           mExList;
typedef struct _mExListClass      mExListClass;
typedef struct _mExListRenderer   mExListRenderer;

//#define NCSS_EXLST_LOOP              (1<<NCSS_LIST_SHIFT)
//#define NCSS_EXLST_MULTIPLE          (2<<NCSS_LIST_SHIFT)
#define NCSS_EXLST_SHIFT             (NCSS_ASTLST_SHIFT+0) 

#define mExListHeader(clsName)  \
	mListHeader(clsName)        \
    unsigned long effector;     \
    int duration;               \
    int key;                    \
    int mouseY; 

struct _mExList
{
	mExListHeader(mExList)
};

#define mExListClassHeader(clsName, superCls)               \
	mListClassHeader(clsName, superCls)                     \
    void (*runAnimation)(clsName*, int startV, int endV);
    
struct _mExListClass
{
	mExListClassHeader(mExList, mList)
};

#define mExListRendererHeader(clsName, parentClass) \
	mListRendererHeader(clsName, parentClass)

struct  _mExListRenderer {
	mExListRendererHeader(mExList, mList)
};

enum mExListProp
{
    NCSP_EXLIST_EFFECTOR = NCSP_LIST_MAX,
    NCSP_EXLIST_MAX,
};

enum mExListNotify
{
    NCSN_EXLIST_NODE_CHANGED = NCSN_LIST_MAX,
    NCSN_EXLIST_MAX,
};


MPAD_EXPORT extern mExListClass g_stmExListCls;

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MGUI_NCSCTRL_EXLIST_H */

