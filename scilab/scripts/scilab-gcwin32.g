#!/bin/sh
# Warning : some old versions of sh dont allow inline function definitions
# like do_scilex()... . In this case use a system V sh (sh5)
# Set the SCI environment variable if not already set

# Copyright INRIA

if test "$HOME" = ""; then
  HOME="SCILAB_DIRECTORY"
fi
export HOME
if test "$SCI" = ""; then
  SCI="SCILAB_DIRECTORY"
fi
if test "$PRINTERS" = ""; then
  PRINTERS="lp:LaserHP:SalleBal:Secretariat:ColorPS:Color12"
fi
# we use less for paging bug less doesn't recognize 
#    win32 pathnames so we use cat xxxx | less in man_nw
#
PAGER=less
#############################################################################
#                                                                           #
#                       DO NOT MODIFY BELOW THIS LINE                       #
#                                                                           #
#############################################################################
PWD=`pwd`
export PWD
export SCI
if test "$MANCHAPTERS" = ""; then
  MANCHAPTERS=$SCI/man/Chapters
fi
export  MANCHAPTERS
export PRINTERS
VERSION="SCILAB_VERSION"
export VERSION

do_scilex()
{
    PATH=$PATH:$SCI:$SCI/util
    export PATH
    XAPPLRESDIR=$SCI/X11_defaults
    export XAPPLRESDIR
    XLESSHELPFILE=$SCI/X11_defaults/xless.help
    export XLESSHELPFILE
    NETHELPDIR=$SCI/X11_defaults
    export NETHELPDIR
#    tty -s && stty kill '^U' intr '^C' erase '^H' quit '^\' eof '^D' susp '^Z'
    $SCI/bin/scilex.exe $* 
}

do_help()
{
echo "Usage  :"
echo     "	scilab [-ns -nw -display display]"
echo     "	scilab -xhelp <key>"
echo     "	scilab -help <key>"
echo     "	scilab -mini"
echo     "	scilab -k <key>"
echo     "	scilab -xk <key>"
echo     "	scilab -link <objects>"
echo     "	scilab -function <function-name>"
echo     "	scilab -print_p file printer"
echo     "	scilab -print_l file printer"
echo     "	scilab -save_p file format"
echo     "	scilab -save_l file format"
}

do_mank()
{
	f1=`grep -i $1 $SCI/man/*/whatis 2> /dev/null`
	if  test -n "$f1" 
	then
		grep -i $1 $SCI/man/*/whatis 2> /dev/null |sed -e "s/^.*://"
	else
		echo $1: nothing appropriate
	fi
}

do_man()
{
	MAN=$SCI/man
	f=`ls $MAN/*/$1.cat 2> /dev/null`
	if  test -n "$f"
	then 
		$SCI/bin/xless  $f
	else
		echo No manual entry for $1
	fi 
}

do_man_nw()
{
	MAN=$SCI/man
	f=`ls $MAN/*/$1.cat 2> /dev/null`
	if  test -n "$f"
	then 
		cat  $f | $PAGER
	else
		echo No manual entry for $1
	fi 
}
do_compile()
{
	umask 002
	rm -f report
	name=`basename $1 .sci`
	echo generating $name.bin
	echo "predef();getf('$name.sci');save('$name.bin');quit"\
	      | $SCI/bin/scilex -ns -nw | sed 1,8d 1>report 2>&1
	if (grep error report 1> /dev/null  2>&1);
	then cat report;echo " " 
	   echo see `pwd`/report for more informations
	   grep libok report>/dev/null; 
	else rm -f report;
	fi
	umask 022
	exit 0
}

do_lib()
{
	umask 002
	rm -f report
	echo "$1=lib('$2/');save('$2/lib',$1);quit"\
	      | $SCI/bin/scilex -ns -nw |sed 1,/--\>/d 1>report 2>&1
	if (grep error report 1> /dev/null  2>&1);
	then cat report;echo " " 
		echo see `pwd`/report for more informations
		grep libok report>/dev/null; 
	else rm -f report;
	fi
	umask 022
	exit 0
}



do_compile_test ()
{
	umask 002
	rm -f report
	name=`basename $1 .sci`
	echo generating $name.bin
	echo "predef();getf('$name.sci','c');save('$name.bin');quit"\
	      | $SCI/bin/scilex -ns -nw | cat  1>report 2>&1
	if (grep error report 1> /dev/null  2>&1);
	then cat report;echo " " 
	   echo see `pwd`/report for more informations
	   grep libok report>/dev/null; 
	fi
	umask 022
	exit 0
}


do_print() 
{
	$SCI/bin/BEpsf $1 $2 
	$SCI/bin/lpr $2.eps
	rm -f $2 $2.eps

}

do_save() 
{
	case $3 in 
          Postscript)
		$SCI/bin/BEpsf $1 $2 
          	 ;;
          Postscript-Latex)
		$SCI/bin/Blatexpr $1 1.0 1.0 $2 
	   	;;
	  Xfig)
		case $1 in
		-portrait)
			mv $2 $2.fig
		;;
		-landscape)
			sed -e "2s/Portrait/Landscape/" $2 >$2.fig
			rm -f $2
		;;
		esac
           	;;
          Gif)
		case $1 in
		-portrait)
			mv $2 $2.gif
		;;
		-landscape)
			mv $2 $2.gif
		;;
		esac
           	;;
	esac
}


case $# in
    0)
	do_scilex 
        ;;
    1)
        case $1 in
             -ns)
		do_scilex $* 
		;;
	     -nw)	
		do_scilex $*
		;;
	     -mini)
		$SCI/bin/test.exe 
		;;
	     -help)
		do_man scilab 
		;;
	     -help_nw)
		do_man_nw scilab 
		;;
             -link|-function|-k)
		do_help
                 ;;
             *)
		do_help
                ;;
        esac
        ;;
    2)
        case $1 in
             -ns)
                case $2 in 
                   -nw)
                      do_scilex $*
                      ;;
                   *)
                      do_scilex $* 
                      ;;
                esac
		;;
	     -nw)
		do_scilex $* 
		;;
             -display)
		do_scilex $*
		;;
            -help)
		do_man $2 
		;;
            -help_nw)
		do_man_nw $2 
                ;;
            -comp)
		do_compile $2
                ;;
	    -k)
		do_mank $2
		;;
            -link)
                shift
		$SCI/bin/scilink $SCI $*
                ;;
            -function)
		$SCI/bin/minfopr $SCI $2
		;;
            *)
		do_help
                ;;
        esac
        ;;
    3)
        case $1 in
            -lib)
		do_lib $2 $3
                ;;
            -print_l)
                do_print -landscape $2 $3
                ;;
            -print_p)
                do_print -portrait $2 $3
                ;;
            -save_l)
                do_save -landscape $2 $3
                ;;
            -save_p)
                do_save -portrait $2 $3
                ;;
            *)
		do_help
                ;;
        esac
        ;;
    *)
        case $1 in
            -link)
                shift
		$SCI/bin/scilink $SCI $*
                ;;
            *)
		do_help
                ;;
        esac
        ;;
esac
