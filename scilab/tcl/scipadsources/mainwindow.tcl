toplevel $pad

set fileName " "
set textareacur $pad.textarea
set saveTextMsg 0

# added by Matthieu PHILIPPE
if { ![info exists lang] } { set lang "eng" }
set chset()                 {} 
set words()                 {}  
set listoffile("$pad.textarea",filename)   "[pwd]/Untitled.sce"
set listoffile("$pad.textarea",save) 0; 
# set flag to know if the editing file was change by user
set listoffile("$pad.textarea",new) 1; # is not an opened file from disk
set listoffile("$pad.textarea",thetime) 0; # set the time of the last modify
# set the scheme for colorization and allowed operations
set listoffile("$pad.textarea",language) "scilab"; 
set listoftextarea $pad.textarea
set winopened 0
set radiobuttonvalue 0


# main window settings
eval destroy [winfo child $pad]
wm title $pad "$winTitle - $listoffile("$pad.textarea",filename)"
wm iconname $pad $winTitle

#create main menu
menu $pad.filemenu -tearoff 0

# start by setting default font sizes
if [ expr [string compare $tcl_platform(platform) "unix"] ==0] {
	set textFont -Adobe-courier-medium-r-Normal-*-$FontSize-*
	set menuFont -adobe-helvetica-bold-r-normal--$FontSize-*-75-75-*-*-*-*
} else {
	set textFont -Adobe-Courier-medium-R-Normal-*-$FontSize-*
	#set menuFont -adobe-helvetica-bold-r-normal--12-*-75-75-*-*-*-*
	set menuFont [$pad.filemenu cget -font]
}

$pad.filemenu configure -font $menuFont

# added by Matthieu PHILIPPE 18/12/2001
# to have the possibility to launch multiple
# browser from scilab with independant menu
# a little uggly !!!


# create frames for widget layout
# this is for the text widget and the y scroll bar
frame $pad.bottomTopMenu
pack $pad.bottomTopMenu  -side top -expand 1 -fill both
# where the text widget is packed
frame $pad.bottomleftmenu
pack $pad.bottomleftmenu -in $pad.bottomTopMenu  -side left -expand 1 \
    -fill both
# where the y scrollbar is packed
frame $pad.bottomrightmenu 
pack  $pad.bottomrightmenu -in $pad.bottomTopMenu  -side right -expand 0 \
    -fill both 
# this is for the x scroll bar at the bottom of the window
frame $pad.bottombottommenu
pack $pad.bottombottommenu -side bottom -expand 0 -fill both


#create textarea
set taille [expr [font measure $textFont " "] *3]

# creates the default textarea 
##ES was here: added insertofftime 0 and exportselection
## Francois Vogel, 21/04/04: changed insertofftime to 500, and added 
##  insertontime 500
## FV 13/05/04: added test with tk_version on ES request
text $pad.textarea -relief sunken -bd 2 -xscrollcommand "$pad.xscroll set" \
	-yscrollcommand "$pad.yscroll set" -wrap $wordWrap -width 1 -height 1 \
        -fg $FGCOLOR -bg $BGCOLOR  -setgrid 1 -font $textFont -tabs $taille \
        -insertwidth 3 -insertborderwidth 2 -insertbackground $CURCOLOR \
        -selectbackground $SELCOLOR -exportselection 1
set textareacur $pad.textarea  
if {[expr $tk_version] >= 8.4} {
    $textareacur configure -insertofftime 500 -insertontime 500
} else {
    $textareacur configure -insertofftime 0
}

scrollbar $pad.yscroll -command "$textareacur yview"
scrollbar $pad.xscroll -command "$textareacur xview" -orient horizontal
pack $textareacur  -in  $pad.bottomleftmenu -side left -expand 1 -fill both
pack $pad.yscroll -in $pad.bottomrightmenu -side right -expand 1 -fill both
pack $pad.xscroll -in $pad.bottombottommenu -expand 1 -fill x 
focus $textareacur
TextStyles $textareacur

## the following comes from infomessages.tcl v2.5

#to get the line number
#included by Matthieu PHILIPPE 21/11/2001 from linenum.pth
set colormen [$pad.filemenu cget -background]
label $pad.statusind -relief groove -state disabled -background $colormen \
   -width 20
# added by Matthieu PHILIPPE 07/12/2001
# this addes an entry widget to display information !
label $pad.statusmes -relief groove -state disabled -background $colormen \
    -foreground blue -width 30
# FV 21/05/04, added a second statusind to display the line number in functions
label $pad.statusind2 -relief groove -state disabled -background $colormen \
    -width 24
pack $pad.statusind2 $pad.statusind -in $pad.bottombottommenu -side right\
    -expand 0
pack $pad.statusmes -in $pad.bottombottommenu -side bottom -expand 0 -fill x

##ES 13/6/2004
wm minsize $pad 1 1 
#strange: this on corrects reopen size
#tk_messageBox -message $WMGEOMETRY;
wm geometry $pad $WMGEOMETRY


#the following comes from undoredo.tcl v2.5
###################################################################
#set zed_dir [file dirname [info script]] 
# here is where the undo stuff begins
if {![info exists classNewId]} {
    # work around object creation between multiple include of this file problem
    set classNewId 0
}
# end of where youd source in undo.tcl

set undo_id [new textUndoer $textareacur]
#
# added by Matthieu PHILIPPE 14/12/2001
set listundo_id("$textareacur") $undo_id
