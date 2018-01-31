AUTOMAKE_OPTIONS = foreign

TOP_DIR = ..

SUBDIRS = res

AM_CPPFLAGS = -I$(TOP_DIR) -I$(TOP_DIR)/include -I.

COMMON_LADD = ../src/libmgncs4touch.la

noinst_PROGRAMS = switchbutton picker newtrackbar commdraw datepicker \
    timepicker exlist btnnavbar imgnavbar name2value itembar balloontip \
    hscrollview iconflow tableview settingtableview

switchbutton_SOURCES = switchbutton.c
switchbutton_LDADD = $(COMMON_LADD) @APP_LIBS@

picker_SOURCES = picker.c
picker_LDADD = $(COMMON_LADD) @APP_LIBS@

newtrackbar_SOURCES = newtrackbar.c
newtrackbar_LDADD = $(COMMON_LADD) @APP_LIBS@

commdraw_SOURCES = comm_draw.c
commdraw_LDADD = $(COMMON_LADD) @APP_LIBS@

datepicker_SOURCES = datepicker.c
datepicker_LDADD = $(COMMON_LADD) @APP_LIBS@

timepicker_SOURCES = timepicker.c
timepicker_LDADD = $(COMMON_LADD) @APP_LIBS@

btnnavbar_SOURCES = btnnavbar.c
btnnavbar_LDADD = $(COMMON_LADD) @APP_LIBS@

imgnavbar_SOURCES = imgnavbar.c
imgnavbar_LDADD = $(COMMON_LADD) @APP_LIBS@

exlist_SOURCES = exlist.c
exlist_LDADD = $(COMMON_LADD) @APP_LIBS@

name2value_SOURCES = name2value.c
name2value_LDADD = $(COMMON_LADD) @APP_LIBS@

itembar_SOURCES = itembar.c
itembar_LDADD = $(COMMON_LADD) @APP_LIBS@

balloontip_SOURCES = balloontip.c
balloontip_LDADD = ${COMMON_LADD} @APP_LIBS@

hscrollview_SOURCES = hscrollview.c
hscrollview_LDADD = ${COMMON_LADD} @APP_LIBS@

iconflow_SOURCES = iconflow.c
iconflow_LDADD = ${COMMON_LADD} @APP_LIBS@

tableview_SOURCES = tableview.c msimpletableviewpiece.h
tableview_LDADD = ${COMMON_LADD} @APP_LIBS@

settingtableview_SOURCES = settingtableview.c settingtableview.h settingmainwinpiece.h \
	settingsubwinpiece.h settingmainwinpiece.c settingsubwinpiece.c
settingtableview_LDADD = ${COMMON_LADD} @APP_LIBS@

EXTRA_DIST =

