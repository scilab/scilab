#  Scipad - programmer's editor and debugger for Scilab
#
#  Copyright (C) 2002 -      INRIA, Matthieu Philippe
#  Copyright (C) 2003-2006 - Weizmann Institute of Science, Enrico Segre
#  Copyright (C) 2004-2008 - Francois Vogel
#
#  Localization files ( in tcl/msg_files/) are copyright of the 
#  individual authors, listed in the header of each file
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# See the file scipad/license.txt
#

toplevel $pad
setscipadicon $pad

set winopened 1
set textareaid $winopened
settextareacur $pad.new$winopened
set listoftextarea [list $textareacur]

set listoffile("$pad.new$winopened",fullname) "[mc "Untitled"]$winopened.sce"
set listoffile("$pad.new$winopened",displayedname) "[mc "Untitled"]$winopened.sce"
set listoffile("$pad.new$winopened",new)  1; # is not an opened file from disk
set listoffile("$pad.new$winopened",thetime) 0; # set the time of the last modify
set listoffile("$pad.new$winopened",readonly) 0; # file can be written
# set the scheme for colorization and allowed operations
set listoffile("$pad.new$winopened",language) "scilab"; 
setlistoffile_colorize "$pad.new$winopened" ""; 
set listoffile("$pad.new$winopened",undostackdepth) 0; # used to enable/disable the undo menu entry
set listoffile("$pad.new$winopened",redostackdepth) 0; # used to enable/disable the redo menu entry
set listoffile("$pad.new$winopened",progressbar_id) ""; # colorization progressbar identifier
set listoffile("$pad.new$winopened",encoding) $defaultencoding

set chset()                 {} 
set words()                 {}  

# main window settings
eval destroy [winfo children $pad]
wm withdraw $pad ; # $pad will be deiconified after Scipad's startup is completely over
wm iconname $pad $winTitle

# catch the kill of the windowmanager
wm protocol $pad WM_DELETE_WINDOW {idleexitapp}

wm minsize $pad 1 1 

# this makes also the initial buffer too reactive to dnd. 
wm geometry $pad $WMGEOMETRY

# only restore zoomed (maximized) state of the Scipad window
if {$WMSTATE == "zoomed"} {
    wm state $pad $WMSTATE
}

#create main menu
menu $pad.filemenu -tearoff 0
$pad.filemenu configure -font $menuFont

panedwindow $pad.pw0 -orient vertical -opaqueresize true
set pwmaxid 0

# creates the default textarea
# -width 1 and -height 1 ensure that the scrollbars show up
# whatever the size of the textarea
text $textareacur -relief sunken -bd 0 \
    -wrap $wordWrap -width 1 -height 1\
    -fg $FGCOLOR -bg $BGCOLOR  -setgrid 0 -font $textFont \
    -insertwidth $textinsertcursorwidth \
    -insertborderwidth $textinsertcursorborderwidth \
    -insertbackground $CURCOLOR \
    -selectbackground $SELCOLOR -exportselection 1 \
    -undo 1 -autoseparators 1
if {$cursorblink} {
    $textareacur configure -insertofftime 500 -insertontime 500
} else {
    $textareacur configure -insertofftime 0
}

# this is for the status bar at the bottom of the main window
frame $pad.bottom
pack $pad.bottom -side bottom -expand 0 -fill both

set colormen [$pad.filemenu cget -background]

# status indicator
label $pad.statusind -relief groove -state disabled -background $colormen \
   -width 20
# status message
label $pad.statusmes -relief groove -state disabled -background $colormen \
    -width 30
# second status indicator to display the line number in functions
label $pad.statusind2 -relief groove -state disabled -background $colormen \
    -width 24 -anchor w
pack $pad.statusind2 $pad.statusind -in $pad.bottom -side right\
    -expand 0
pack $pad.statusmes -in $pad.bottom -side bottom -expand 0 -fill x

# packing of the bottom line with status info *must* be done before packing
# anything in the panedwindow otherwise the status area can get clipped on
# window resize
packnewbuffer $textareacur $pad.pw0 0

# from now on, keep checking the availability of the scilab prompt 
displaybusystate

# the following update makes the initial textarea reactive to dnd!
update

# Drag and drop feature using TkDnD
tkdndbind $textareacur
