includedir = @includedir@/libfap2vis

libdir = @libdir@/fap2/plugins

AM_CPPFLAGS = -D_PLAT_LINUX_ -g -O0

lib_LTLIBRARIES = libfap2vis.la

libfap2vis_la_SOURCES = \
		       mwidget.h \
		       widget.h \
		       widget.cpp \
		       gwcont.h \
		       gwcont.cpp \
		       visprov.h \
		       plugin.cpp \
		       visprov.cpp

include_HEADERS = \
		  mwidget.h \
		  widget.h


libfap2vis_la_LDFLAGS = -rdynamic -no-undefined -lrt
