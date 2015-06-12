TEMPLATE = subdirs
CONFIG += ordered
MAKEFILE = Makefile-GUI
QMAKE_CXXFLAGS += $$(CXXFLAGS)
QMAKE_CFLAGS += $$(CFLAGS)
QMAKE_LFLAGS += $$(LDFLAGS)
SUBDIRS = Common \
   Common/LogManager \
   Common/RemoteCoreController \
   GUI
TRANSLATIONS = translations/d_lan_gui.fr.ts \
   translations/d_lan_gui.ko.ts
CODECFORTR = UTF-8
