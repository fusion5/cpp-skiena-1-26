## Process this file with automake to produce Makefile.in

## Created by Anjuta



AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(SKIENA_1_26_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = skiena_1_26

skiena_1_26_SOURCES = \
	main.cc \
	data.h \
	plotter.cxx \
	plotter.h \
	algorithms.h \
	algorithms.cxx \
	data.cxx

skiena_1_26_CXXFLAGS = --std=c++11 

skiena_1_26_LDFLAGS = 

skiena_1_26_LDADD = $(SKIENA_1_26_LIBS)


if NATIVE_WIN32
skiena_1_26_LDFLAGS += -mwindows
endif



