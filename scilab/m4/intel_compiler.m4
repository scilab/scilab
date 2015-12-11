dnl SCI_INTEL_COMPILER
dnl ------------------------------------------------------
dnl Set compilation options for intel C/Fortran compilers
dnl

AC_DEFUN([SCI_INTEL_COMPILER],
[
		case "$host" in
		  i*86-*-linux-gnu | *-pc-linux-gnu )
			# -pc64 : double precision
			EXTRA_OPTIONS=" -pc64 -ffnalias -falias"
			;;
		esac

		if test "$enable_debug_CXX" = yes; then
			COMPILER_CXXFLAGS="-g $COMPILER_CXXFLAGS $EXTRA_OPTIONS"
		else
			COMPILER_CXXFLAGS="-O -DNDEBUG $COMPILER_CXXFLAGS "
		fi

		if test "$enable_debug_C" = yes; then
			COMPILER_CFLAGS="-g $COMPILER_CFLAGS $EXTRA_OPTIONS"
		else
			COMPILER_CFLAGS="-O -DNDEBUG $COMPILER_CFLAGS "
		fi

		if test "$enable_debug_fortran" = yes; then
			COMPILER_FFLAGS="-g $COMPILER_FFLAGS $EXTRA_OPTIONS"
		else
			COMPILER_FFLAGS="-O -DNDEBUG $COMPILER_FFLAGS "
		fi

		# TODO : voir si obligatoire vu que c'est en partie le boulot de AC_PROG_C

])dnl SCI_INTEL_COMPILER
