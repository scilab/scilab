#!/bin/sh
# Warning : some old versions of sh dont allow inline function definitions
# like do_scilex()... . In this case use a system V sh (sh5)
# Set the SCI environment variable if not already set
# Copyright Inria/Enpc 
# XXXXX : Attention quand on est en mingwin on veut des 
# env avec c: pas /cygdrive 
# de plus en cygwin scilab peut se lancer sans script 
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
if test "$HOME" = ""; then
  HOME="SCILAB_DIRECTORY"
fi
export HOME
if test "$SCI" = ""; then
  SCI="SCILAB_DIRECTORY"
fi
export SCI
if test "$WSCI" = ""; then
  WSCI="SCILAB_DIRECTORY"
fi
export WSCI 

PWD=`pwd`
export PWD

if test "$MANCHAPTERS" = ""; then
  MANCHAPTERS=$SCI/man/Chapters
fi
export  MANCHAPTERS
export  PRINTERS
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
    $SCI/bin/scilex $* 
}

do_scilex_now()
{
    PATH=$PATH:$SCI:$SCI/util
    export PATH
    XAPPLRESDIR=$SCI/X11_defaults
    export XAPPLRESDIR
    XLESSHELPFILE=$SCI/X11_defaults/xless.help
    export XLESSHELPFILE
    NETHELPDIR=$SCI/X11_defaults
    export NETHELPDIR
    $SCI/bin/scilex $* 
}

do_geci_scilex()
{
    do_scilex
}

do_geci_scilex_now()
{
    do_scilex_now
}


do_help()
{
echo "Usage  :"
echo     "	scilab [-ns -nw -nwni -display display]"
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
do_chapters()
{
# more efficient than do_chapters_alt but not as clean
      newest=`ls -t -1 $MANCHAPTERS .scilab ~/.scilab $SCI/scilab.star 2>/dev/null |sed -n -e '1p'`
      if [ "`basename $newest`" != "`basename $MANCHAPTERS`" ]; then
        echo "mputl(%helps(:,1),'$MANCHAPTERS');quit" |scilab -nwni >/dev/null
      fi
}

do_chapters_alt()
{
        echo "mputl(%helps(:,1),'$MANCHAPTERS');quit" |scilab -nwni >/dev/null
}

do_mank()
{
	do_chapters
	tst=""
	chapters=`cat $MANCHAPTERS | awk '{print " " $1 }'` 
	for i in $chapters
	do
		eval "mpath=\"$i\""
		f=`grep -i $1 $mpath/whatis  2> /dev/null`
		if  test -n "$f"
		then 
			grep -i $1 $mpath/whatis 2> /dev/null |sed -e "s/^.*://"
			tst="found"
		fi 
	done  
	if [ "$tst" != "found" ]
	then
		echo $1: nothing appropriate
	fi
}

do_mank_x()
{
    do_mank $1 > /tmp/Sci_mankx 
    f=`grep nothing appropriate /tmp/Sci_mankx 2> /dev/null`
    if test -n "$f"
    then 
	exit 19
    else
	( $SCI/bin/xless /tmp/Sci_mankx ;rm -f  /tmp/Sci_mankx  ) &
    fi 
}


do_man()
{
	do_chapters
	tst=""
	chapters=`cat $MANCHAPTERS | awk '{print " " $1 }'` 
	for i in $chapters
	do
		eval "mpath=\"$i\""
		f=`ls  $mpath/$1.cat 2> /dev/null`
		if  test -n "$f"
		then 
			cat $mpath/$1.cat
			tst="found"
			break
		fi 
	done  
	if [ "$tst" != "found" ]
	then
		echo No manual entry for $1
	fi

}


do_man_x()
{
	tst=""
	XAPPLRESDIR=$SCI/X11_defaults
	export XAPPLRESDIR
	chapters=`cat $MANCHAPTERS | awk '{print " " $1 }'` 
	for i in $chapters
	do
		eval "mpath=\"$i\""
		f=`ls  $mpath/$1.cat 2> /dev/null`
		if  test -n "$f"
		then 
			$SCI/bin/xless $mpath/$1.cat &
			tst="found"
			break
		fi 
	done  
	if [ "$tst" != "found" ]
	then
		#echo No manual entry for $1
		exit 19
	fi

}


do_compile()
{
	umask 002
	rm -f report
	name=`basename $1 .sci`
	echo generating $name.bin
	echo "predef();getf('$name.sci');save('$name.bin');quit"\
	      | $SCI/bin/scilex -ns -nwni | sed 1,8d 1>report 2>&1
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
	      | $SCI/bin/scilex -ns -nwni |sed 1,/--\>/d 1>report 2>&1
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
	      | $SCI/bin/scilex -ns -nwni | cat  1>report 2>&1
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
	lpr $2.eps
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

# calling Scilab with no argument or special cases of calling Scilab
rest="no"
case $# in
    0)
	do_geci_scilex &
        ;;
    2)
        case $1 in
            -help)
		do_man $2|more
                ;;
            -xhelp)
		do_man_x $2
                ;;
            -comp)
		do_compile $2
                ;;
	    -k)
		do_mank $2
		;;
	    -xk)
		do_mank_x $2 
		;;
            -link)
                shift
		$SCI/bin/scilink $SCI $*
                ;;
            -function)
		$SCI/bin/minfopr $SCI $2
		;;
            *)
		rest="yes"
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
            -link)
                shift
		$SCI/bin/scilink $SCI $*
                ;;
            *)
		rest="yes"
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
		rest="yes"
                ;;
        esac
        ;;
esac

# really calling Scilab with arguments
if test "$rest" = "yes"; then
  nos=
  now=
  display=
  start_file=
  prevarg=
  for sciarg 
  do
    # If the previous argument needs an argument, assign it.
    if test -n "$prevarg"; then
      eval "$prevarg=\$sciarg"
      prevarg=
      continue
    fi
    case $sciarg in
      -ns)
          nos="yes"
          ;;
      -nw)
          now="yes"
          ;;
      -nwni)
          nowi="yes"
          ;;
      -display|-d)
          prevarg="display"
          ;;
      -f)
          prevarg="start_file"
          ;;
      -e)
          prevarg="start_exp"
          ;;

      -args)
           prevarg="arguments"
          ;;

      *)
          do_help
          ;;
    esac
  done

  if test -n "$display"; then
    display="-display $display"
  fi

  if test -n "$start_file"; then
    start_file="-f $start_file"
  fi

  if test -n "$start_exp"; then
    start_file="-e $start_exp"
  fi

  if test -n "$nos"; then
     if test -n "$now"; then
       do_scilex_now -ns -nw $start_file $arguments
     else
        if test -n "$nowi"; then 
	    do_scilex_now -ns -nwni $display $start_file  $arguments 
        else
	    do_scilex -ns $display $start_file  $arguments &
        fi
     fi
  else
     if test -n "$now"; then
       do_geci_scilex_now -nw $start_file $arguments
     else
        if test -n "$nowi"; then 
	    do_scilex_now -nwni $display $start_file  $arguments 
        else
	    do_scilex $display $start_file  $arguments 
        fi
       do_geci_scilex $display $start_file  $arguments&
     fi
  fi    

fi
