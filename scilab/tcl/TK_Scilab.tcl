############################################
# TKSCILAB Gui Interface facility for Scilab
# Bertrand Guiheneuf - 1998 
############################################
#


if {[info exists SciPath]} { set env(SCIPATH) $SciPath };
source "$env(SCIPATH)/tcl/misc.tcl"
source "$env(SCIPATH)/tcl/callbacks.tcl"
source "$env(SCIPATH)/tcl/tkcontrols.tcl"
source "$env(SCIPATH)/tcl/set.tcl"
source "$env(SCIPATH)/tcl/get.tcl"
source "$env(SCIPATH)/tcl/figure.tcl"
source "$env(SCIPATH)/tcl/uicontrol.tcl"
source "$env(SCIPATH)/tcl/uimenu.tcl"





######################################################################################
######################################################################################
######################################################################################

proc bgerror { m } {
puts " Error: $m";
}
# the root object is the first tk object
set root .;


# figure are special objects
# their parent *must* be the root object
# and we must keep a list of all figures,
# delete them from the list when they are destroyed
# and remember their order of creation.
# This will allow to manage the gcf feature
# which allows to know in which figure to draw.
set FigList {};
# FreeFigHandle is also a global list 
# containing all the free figure handles.
set FreeFigHandle {};


set Win(0) $root;
# this is the first object handle
# the handle between 1 and 999 are reverved for figures handles
set WinIdx 1000;


set gcbo 0; # object which callback is currently executing 
set gcf 0; #current figure index
set gco 0; #current object handle 
#. configure -bg #d0d0d0 ;
wm withdraw .;


# default bindings 
bind figure <Destroy> {CloseFigure %W};

#set h [CreateFigure 0];


#puts $h
#set h1 [CreateUIControl 0  button]
 
