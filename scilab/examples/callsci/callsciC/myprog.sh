#!/bin/sh
if test "$PRINTERS" = ""; then
  PRINTERS="lp:LaserHP:SalleBal:Secretariat:ColorPS:Color12"
fi
#############################################################################
#                                                                           #
#                       DO NOT MODIFY BELOW THIS LINE                       #
#                                                                           #
#############################################################################
if test "$SCI" = ""; then
  SCI="../../../"
fi
export SCI
if test "$DISPLAY" = ""; then
  DISPLAY="unix:0.0"
fi
export DISPLAY
if test "$MANCHAPTERS" = ""; then
  MANCHAPTERS=$SCI/man/Chapters
fi
export  MANCHAPTERS
export PRINTERS
VERSION="Scilab-2.4"
export VERSION

PVM_ROOT=$SCI/pvm3
PVM_DPATH=$PVM_ROOT/lib/pvmd
export PVM_ROOT PVM_DPATH

TMPDIR=/tmp
export TMPDIR

do_callsci()
{
    PATH=$PATH:$SCI:$SCI/util
    export PATH
    XAPPLRESDIR=$SCI/X11_defaults
    export XAPPLRESDIR
    XLESSHELPFILE=$SCI/X11_defaults/xless.help
    export XLESSHELPFILE
    NETHELPDIR=$SCI/X11_defaults
    export NETHELPDIR
    tty -s && stty kill '^U' intr '^C' erase '^H' quit '^\' eof '^D' susp '^Z'
    ./myprog $* 
}

do_callsci

