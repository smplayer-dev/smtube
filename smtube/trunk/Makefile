
PREFIX=/usr/local
#PREFIX=/tmp/smplayer

CONF_PREFIX=$(PREFIX)

DATA_PATH=$(PREFIX)/share/smtube
DOC_PATH=$(PREFIX)/share/doc/smtube
TRANSLATION_PATH=$(PREFIX)/share/smtube/translations

KDE_PREFIX=$(PREFIX)
KDE_ICONS=$(KDE_PREFIX)/share/icons/hicolor/
KDE_APPLNK=$(KDE_PREFIX)/share/applications/

QMAKE=qmake
LRELEASE=lrelease

DEFS=DATA_PATH=\\\"$(DATA_PATH)\\\" \
     TRANSLATION_PATH=\\\"$(TRANSLATION_PATH)\\\" \
     DOC_PATH=\\\"$(DOC_PATH)\\\"

src/smtube:
#	./get_svn_revision.sh
	+cd src && $(QMAKE) $(QMAKE_OPTS) && $(DEFS) make
	cd src && $(LRELEASE) smtube.pro

clean:
	if [ -f src/Makefile ]; then cd src && make distclean; fi
	-rm src/translations/smtube_*.qm

install: src/smtube
	-install -d $(DESTDIR)$(PREFIX)/bin/
	install -m 755 src/smtube $(DESTDIR)$(PREFIX)/bin/
	-install -d $(DESTDIR)$(DATA_PATH)
	-install -d $(DESTDIR)$(TRANSLATION_PATH)
	install -m 644 src/translations/*.qm $(DESTDIR)$(TRANSLATION_PATH)
	-install -d $(DESTDIR)$(DOC_PATH)
	install -m 644 Changelog *.txt $(DESTDIR)$(DOC_PATH)

	-install -d $(DESTDIR)$(KDE_ICONS)/256x256/apps/
	-install -d $(DESTDIR)$(KDE_ICONS)/128x128/apps/
	-install -d $(DESTDIR)$(KDE_ICONS)/64x64/apps/
	-install -d $(DESTDIR)$(KDE_ICONS)/48x48/apps/
	-install -d $(DESTDIR)$(KDE_ICONS)/32x32/apps/
	-install -d $(DESTDIR)$(KDE_ICONS)/16x16/apps/
	install -m 644 src/icons/logo.png $(DESTDIR)$(KDE_ICONS)/256x256/apps/smtube.png
	install -m 644 icons/smtube_128.png $(DESTDIR)$(KDE_ICONS)/128x128/apps/smtube.png
	install -m 644 icons/smtube_64.png $(DESTDIR)$(KDE_ICONS)/64x64/apps/smtube.png
	install -m 644 icons/smtube_48.png $(DESTDIR)$(KDE_ICONS)/48x48/apps/smtube.png
	install -m 644 icons/smtube_32.png $(DESTDIR)$(KDE_ICONS)/32x32/apps/smtube.png
	install -m 644 icons/smtube_16.png $(DESTDIR)$(KDE_ICONS)/16x16/apps/smtube.png
	-install -d $(DESTDIR)$(KDE_APPLNK)
	install -m 644 smtube.desktop $(DESTDIR)$(KDE_APPLNK)
#	-install -d $(DESTDIR)$(PREFIX)/share/man/man1/
#	install -m 644 man/smtube.1 $(DESTDIR)$(PREFIX)/share/man/man1/
#	gzip -9 -f $(DESTDIR)$(PREFIX)/share/man/man1/smtube.1

uninstall:
	-rm -f $(PREFIX)/bin/smtube
	-rm -f $(TRANSLATION_PATH)/*.qm
	-rm -f $(DOC_PATH)/Changelog
	-rm -f $(DOC_PATH)/*.txt
	-rm -f $(DESTDIR)$(KDE_ICONS)/256x256/apps/smtube.png
	-rm -f $(DESTDIR)$(KDE_ICONS)/128x128/apps/smtube.png
	-rm -f $(DESTDIR)$(KDE_ICONS)/64x64/apps/smtube.png
	-rm -f $(DESTDIR)$(KDE_ICONS)/48x48/apps/smtube.png
	-rm -f $(DESTDIR)$(KDE_ICONS)/32x32/apps/smtube.png
	-rm -f $(DESTDIR)$(KDE_ICONS)/16x16/apps/smtube.png
	-rm -f $(KDE_APPLNK)/smtube.desktop
#	-rm -f $(PREFIX)/share/man/man1/smtube.1.gz
	-rmdir $(TRANSLATION_PATH)/

