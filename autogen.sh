#!/bin/sh
#
# Run this script to generate aclocal.m4, config.h.in, 
# Makefile.in's, and ./configure...
# 
# To specify extra flags to aclocal (include dirs for example),
# set ACLOCAL_FLAGS
#

DIE=0

# minimum required versions of autoconf/automake/libtool:
ACMAJOR=2
ACMINOR=59

AMMAJOR=1
AMMINOR=9
AMPATCH=0

LTMAJOR=1
LTMINOR=5
LTPATCH=8

(autoconf --version 2>&1 | \
 perl -n0e "(/(\d+)\.(\d+)/ && \$1>=$ACMAJOR && \$2>=$ACMINOR) || exit 1") || {
    echo
    echo "Error: You must have 'autoconf' version $ACMAJOR.$ACMINOR or greater"
    echo "installed to run $0. Get the latest version from"
    echo "ftp://ftp.gnu.org/pub/gnu/autoconf/"
    echo
    NO_AUTOCONF=yes
    DIE=1
}

amtest="
    if (/(\d+)\.(\d+)((-p|\.)(\d+))*/) { 
    exit 1 if (\$1 < $AMMAJOR || \$2 < $AMMINOR); 
    exit 0 if (\$2 > $AMMINOR); 
    exit 1 if (\$5 < $AMPATCH); 
}"

(automake --version 2>&1 | perl -n0e "$amtest" ) || {
    echo
    echo "Error: You must have 'automake' version $AMMAJOR.$AMMINOR.$AMPATCH or greater"
    echo "installed to run $0. Get the latest version from"
    echo "ftp://ftp.gnu.org/pub/gnu/automake/"
    echo
    NO_AUTOCONF=yes
    DIE=1
}

lttest="
    if (/(\d+)\.(\d+)((-p|\.)(\d+))*/) { 
    exit 1 if (\$1 < $LTMAJOR);
    exit 1 if (\$1 == $LTMAJOR && \$2 < $LTMINOR); 
    exit 1 if (\$1 == $LTMAJOR && \$2 == $LTMINOR && \$5 < $LTPATCH);
}"

(libtool --version 2>&1 | perl -n0e "$lttest" ) || {
    echo
    echo "Error: You must have 'libtool' version $LTMAJOR.$LTMINOR.$LTPATCH or greater"
    echo "installed to run $0. Get the latest version from"
    echo "ftp://ftp.gnu.org/pub/gnu/libtool/"
    echo
    DIE=1
}


test -n "$NO_AUTOMAKE" || (aclocal --version) < /dev/null > /dev/null 2>&1 || {
    echo
    echo "Error: \`aclocal' appears to be missing. The installed version of"
    echo "\`automake' may be too old. Get the most recent version from"
    echo "ftp://ftp.gnu.org/pub/gnu/automake/"
    NO_ACLOCAL=yes
    DIE=1
}

if test $DIE -eq 1; then
    exit 1
fi

# make sure that auxdir exists
mkdir auxdir 2>/dev/null

set -x

rm -f ./config.h.in
rm -f ./aclocal.m4
rm -rf ./autom4te*.cache
aclocal -I m4 -I auxdir $ACLOCAL_FLAGS || exit 1
glibtoolize --automake --copy --force || exit 1
autoheader || exit 1
automake --add-missing --copy --force-missing || exit 1
autoconf --force --warnings=no-obsolete || exit 1

set +x

if [ -e config.status ]; then
   echo "removing stale config.status."
   rm -f config.status 
fi
if [ -e config.log    ]; then
   echo "removing old config.log."
   rm -f config.log
fi

echo "now run ./configure to configure libcfgparser for your environment."
