TEMPLATE = subdirs
CONFIG += ordered
MAKEFILE = Makefile-Core
QMAKE_CXXFLAGS += $$(CXXFLAGS)
QMAKE_CFLAGS += $$(CFLAGS)
QMAKE_LFLAGS += $$(LDFLAGS)
SUBDIRS = Common \
   Common/LogManager \
   Core/FileManager \
   Core/PeerManager \
   Core/UploadManager \
   Core/DownloadManager \
   Core/NetworkListener \
   Core/ChatSystem \
   Core/RemoteControlManager \
   Core

TRANSLATIONS = translations/d_lan_core.fr.ts \
   translations/d_lan_core.ko.ts
CODECFORTR = UTF-8
