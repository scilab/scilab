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
set listoffile("$pad.new$winopened",redostackdepth) 0; # used to enable/disable the redo menu entry
set listoffile("$pad.new$winopened",progressbar_id) ""; # colorization progressbar identifier

set chset()                 {} 
set words()                 {}  

# main window settings
eval destroy [winfo children $pad]
wm withdraw $pad ; # $pad will be deiconified after Scipad's startup is completely over
wm iconname $pad $winTitle

# give Scipad an icon in the task bar and in its toplevel
if {$tcl_platform(platform) == "windows" && $tcl_platform(osVersion) == "5.0"} {
    # special case for Windows 2000: just do nothing
    # we cannot rely on catch {wm iconphoto ...} because this produces
    # a black square icon on Win 2000 (bug 3416)
    # <TODO> the proper fix would be to use
    #          wm iconbitmap $pad -default $iconicofile
    #        but the question is how to create the ico file from the existing
    #        icon in GIF format
    # doing nothing here means Scipad will receive the Tk icon, which is fine
} else {
    # other platforms or other Windows versions (see bug 3105)
    # wm iconphoto is officially implemented in Tk 8.5 only
    # however it is also in Tk 8.4.8 onwards
    # see http://groups.google.com/group/comp.lang.tcl/browse_thread/thread/8c6e1a59ea384573
    # to avoid testing for complicated versions and platforms, I lazily catch this...
    # there is anyway no fallback when wm iconphoto is not supported
    # (wm iconbitmap with an ico file, on Windows only... well, let's wait for bug reports, if any!)
    set iconimage [image create photo $pad.icon1 -format gif \
                        -file [file join $iconsdir scipad-editor.gif]]
    catch {wm iconphoto $pad -default $iconimage}
}

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
