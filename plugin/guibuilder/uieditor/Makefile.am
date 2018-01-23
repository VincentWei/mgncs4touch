ETCDIR = $(prefix)/etc/guibuilder/uieditor

DataFiles = mgncs4touch.cfg ctrllist

install-data-local:
	if [ ! -d $(ETCDIR) ]; then $(INSTALL) -d $(ETCDIR); fi

	$(INSTALL_DATA) $(DataFiles) $(ETCDIR)

SUBDIRS = defines

EXTRA_DIST = $(DataFiles)

