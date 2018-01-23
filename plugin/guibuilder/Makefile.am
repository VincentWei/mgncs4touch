ETCDIR = $(prefix)/etc/guibuilder

DataFiles = uieditor.layout

MV = mv

install-data-local:
	if [ ! -d $(ETCDIR) ]; then \
		echo 'No guibuilder Found, You should install guibuilder first!'; \
	fi

	$(INSTALL_DATA) $(DataFiles) $(ETCDIR)

SUBDIRS = uieditor icon renderer

EXTRA_DIST = $(DataFiles)

