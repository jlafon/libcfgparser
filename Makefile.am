include $(top_srcdir)/common.mk

AUTOMAKE_OPTIONS = subdir-objects foreign
ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

lib_LTLIBRARIES = libcfgparser.la

libcfgparser_la_LDFLAGS = -version-info $(LIBCFGPARSER_SO_VERSION)
libcfgparser_la_SOURCES = src/cfgparser.c    

libcfgparser_includedir = $(includedir)/
nobase_libcfgparser_include_HEADERS = libcfgparser.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libcfgparser.pc

dist_noinst_SCRIPTS = autogen.sh

MAINTAINERCLEANFILES =                                  \
        aclocal.m4 config.guess config.xml              \
        config.h.in config.sub configure install-sh     \
        ltconfig ltmain.sh missing mkinstalldirs        \
        stamp-h.in

distclean-local:
	-(cd $(top_srcdir) && rm -rf autom4te*.cache autoscan.*)
	-(cd $(top_srcdir) && rm -rf $(PACKAGE)-* libcfgparser.pc)

mrproper: distclean-local clean
	-(cd $(top_srcdir) && rm -rf autom4te.cache config.h config.log)
	-(cd $(top_srcdir) && rm -rf config.status libtool stamp-h1)
	-(cd $(top_srcdir) && rm -rf auxdir)
	-(cd $(top_srcdir) && rm -f configure configure.ac~)
	-(cd $(top_srcdir) && rm -f aclocal.m4 config.h.in config.h.in~)
	-(cd $(top_srcdir)/m4 && rm -f libtool.m4 ltoptions.m4)
	-(cd $(top_srcdir)/m4 && rm -f ltsugar.m4 ltversion.m4)
	-(cd $(top_srcdir)/m4 && rm -f lt~obsolete.m4)
	-(cd $(top_srcdir)/doc && rm -f Doxyfile)
	-(cd $(top_srcdir)/tests && rm -f circle*.txt)
	-find $(top_srcdir)/ -name "Makefile" -exec rm {} \;
	-find $(top_srcdir)/ -name "Makefile.in" -exec rm {} \;
	-find $(top_srcdir)/ -name ".dirstamp" -exec rm {} \;
	-find $(top_srcdir) -depth -name ".deps" -exec rm -rf {} \;
