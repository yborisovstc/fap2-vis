includedir = @includedir@/libfap2vis

libdir = @libdir@/fap2/plugins

AM_CPPFLAGS = -D_PLAT_LINUX_ -g -O0

lib_LTLIBRARIES = libfap2vis.la

libfap2vis_la_SOURCES = \
		       mwidget.h \
		       mcontainer.h \
		       visenv.h \
		       visenv.cpp \
		       scene.h \
		       scene.cpp \
		       scelem.h \
		       scelem.cpp \
		       sce_rect.h \
		       sce_rect.cpp \
		       visprov.h \
		       plugin.cpp \
		       mdlvis.h \
		       mdlvis.cpp \
		       widget.h \
		       widget.cpp \
		       container.h \
		       container.cpp \
		       vlayout.h \
		       vlayout.cpp \
		       mvrcontroller.h \
		       vrcontroller.h \
		       vrcontroller.cpp \
		       label.h \
		       label.cpp \
		       button.h \
		       button.cpp \
		       magentvr.h \
		       agentvr.h \
		       agentvr.cpp \
		       visrepr.h \
		       visrepr.cpp \
		       visprov.cpp

include_HEADERS = \
		  mwidget.h \
		  magentvr.h \
		  mvrcontroller.h \
		  visenv.h


libfap2vis_la_LDFLAGS = -rdynamic -no-undefined -lrt
