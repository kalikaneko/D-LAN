TEMPLATE = subdirs
CONFIG += ordered
MAKEFILE = Makefile-GUI
QMAKE_CXXFLAGS += $$(CXXFLAGS)
QMAKE_CFLAGS += $$(CFLAGS)
QMAKE_LFLAGS += $$(LDFLAGS)
SUBDIRS = Common \
   Common/LogManager \
   Common/RemoteCoreController \
   Client
