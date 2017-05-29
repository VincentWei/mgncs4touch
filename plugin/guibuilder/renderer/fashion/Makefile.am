######################################################################
# Copyright (C) 2002 ~ 2010, Beijing FMSoft Technology Co., Ltd.     #
# Room 902, Floor 9, Taixing, No.11, Huayuan East Road, Haidian      #
# District, Beijing, P. R. CHINA 100191.                             #
# All rights reserved.                                               #
#                                                                    #
# This software is the confidential and proprietary information of   #
# Beijing FMSoft Technology Co., Ltd. ("Confidential Information").  #
# You shall not disclose such Confidential Information and shall     #
# use it only in accordance you entered into with FMSoft.            #
#                                                                    #
#          http://www.minigui.com                                    #
#                                                                    #
######################################################################

ETCDIR = $(prefix)/etc/guibuilder/renderer/fashion

DataFiles = switchbutton.xml \
			newtrackbar.xml  \
			btnnavbar.xml    \
			imgnavbar.xml    \
			animation.xml    \
			picker.xml       \
			combopicker.xml  \
			timepicker.xml   \
			datepicker.xml   \
			itembar.xml

install-data-local:
	if [ ! -d $(ETCDIR) ]; then $(INSTALL) -d $(ETCDIR); fi

	$(INSTALL_DATA) $(DataFiles) $(ETCDIR)

EXTRA_DIST = $(DataFiles)

