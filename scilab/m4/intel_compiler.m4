dnl SCI_INTEL_COMPILER
dnl ------------------------------------------------------
dnl Set compilation options for intel C/Fortran compilers
dnl

AC_DEFUN([SCI_INTEL_COMPILER],
[
		
		CFLAGS=" $CFLAGS -Dlinux -DNARROWPROTO -mp"
		# -cm remove comments displays, -w90  -w95 remove warnings abort f90 f95 non standard

		FFLAGS=" $FFLAGS -cm  -w90 -w95 -mp"


		case "$host" in
		  i*86-*-linux-gnu | *-pc-linux-gnu )
			# -pc64 : double precision 
			EXTRA_OPTIONS=" -pc64 -ffnalias -falias"
			;;
		esac

		if test "$enable_debug_C" = yes; then
			CFLAGS="-g $CFLAGS $EXTRA_OPTIONS"
		else
			CFLAGS="-O -DNDEBUG $CFLAGS "
		fi

		if test "$enable_debug_fortran" = yes; then
			FFLAGS="-g $FFLAGS $EXTRA_OPTIONS"
		else
			FFLAGS="-O $FFLAGS "
		fi		
	
		# TODO : voir si obligatoire vu que c'est en partie le boulot de AC_PROG_C

])dnl SCI_INTEL_COMPILER
