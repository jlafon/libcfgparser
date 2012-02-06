AC_INIT([libcfgparser],[0.1],[github-hpc@lanl.gov],[libcfgparser],[http://github.com/hpc/libcfgparser])

# Keep artifacts in directories as much as possible.
AC_CONFIG_AUX_DIR(auxdir)
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([-Wall no-define])
AC_CONFIG_HEADERS([config.h])

# Check for compilers.
AC_PROG_CC
AM_PROG_CC_C_O

# Pull in libtool. We need to support ancient autotools, so don't use LT_INIT.
#LT_INIT
AC_PROG_LIBTOOL

# Checks for typedefs, structs, and compiler options.
AC_TYPE_SIZE_T
AC_C_INLINE

# Checks for library functs.
AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_FUNC_MALLOC

# Define the library version.
AC_SUBST([LIBCFGPARSER_SO_VERSION], [0:0:0])
AC_SUBST([LIBCFGPARSER_API_VERSION], [0.1])

# Check for MPI

AC_OUTPUT([Makefile      \
          libcfgparser.pc:libcfgparser.pc.in])
