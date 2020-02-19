bin_PROGRAMS = ut-fap2vis-lib

AM_CPPFLAGS = -D_PLAT_LINUX_ -g -O0

ut_fap2vis_lib_SOURCES = \
		       ut_main.cpp \
		       ut_wdg.cpp \
		       ut_cnt.cpp \
		       ut_cre.cpp

ut_fap2vis_lib_LDFLAGS=-L../src/.libs/ -lfap2vis -lgrayb  -lcppunit -lrt
ut_fap2vis_lib_CXXFLAGS=-I@srcdir@/../src

check: test_fap2vis_lib

test_grayb_lib:
	 ./ut-fap2vis-lib


