toplevel $pad

set winopened 1
set radiobuttonvalue 1
set textareacur $pad.new$winopened
set listoftextarea $pad.new$winopened

set listoffile("$pad.new$winopened",fullname) "[mc "Untitled"]$winopened.sce"
set listoffile("$pad.new$winopened",displayedname) "[mc "Untitled"]$winopened.sce"
set listoffile("$pad.new$winopened",save) 0; # file is unmodified
set listoffile("$pad.new$winopened",new)  1; # is not an opened file from disk
set listoffile("$pad.new$winopened",thetime) 0; # set the time of the last modify
set listoffile("$pad.new$winopened",readonly) 0; # file can be written
# set the scheme for colorization and allowed operations
set listoffile("$pad.new$winopened",language) "scilab"; 

set chset()                 {} 
set words()                 {}  

# main window settings
eval destroy [winfo child $pad]
wm title $pad "$winTitle - $listoffile("$pad.new$winopened",displayedname)"
wm iconname $pad $winTitle

# catch the kill of the windowmanager
wm protocol $pad WM_DELETE_WINDOW {exitapp yesnocancel}
## geometry in what units? The width is more than 65 columns, though it's 
## resized proportionally
#wm geometry $pad 65x24 
wm minsize $pad 1 1 
# strange: this on corrects reopen size when in setgrid 1 mode
#  and also, makes the initial buffer too reactive to dnd. 
#tk_messageBox -message $WMGEOMETRY;
wm geometry $pad $WMGEOMETRY

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

set taille [expr [font measure $textFont " "] *3]

# creates the default textarea 
text $pad.new$winopened -relief sunken -bd 2 -xscrollcommand "$pad.xscroll set" \
        -yscrollcommand "$pad.yscroll set" -wrap $wordWrap -width 1 -height 1 \
        -fg $FGCOLOR -bg $BGCOLOR  -setgrid 0 -font $textFont -tabs $taille \
        -insertwidth 3 -insertborderwidth 2 -insertbackground $CURCOLOR \
        -selectbackground $SELCOLOR -exportselection 1
if {$cursorblink == "true"} {
    $textareacur configure -insertofftime 500 -insertontime 500
} else {
    $textareacur configure -insertofftime 0
}

scrollbar $pad.yscroll -command "$textareacur yview" -takefocus 0
scrollbar $pad.xscroll -command "$textareacur xview" -orient horizontal\
       -takefocus 0
pack $textareacur  -in $pad.bottomleftmenu   -side left  -expand 1 -fill both
pack $pad.yscroll  -in $pad.bottomrightmenu  -side right -expand 1 -fill both
pack $pad.xscroll  -in $pad.bottombottommenu -expand 1 -fill x 
focus $textareacur
TextStyles $textareacur

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
pack $pad.statusind2 $pad.statusind -in $pad.bottombottommenu -side right\
    -expand 0
pack $pad.statusmes -in $pad.bottombottommenu -side bottom -expand 0 -fill x

$textareacur mark set insert "1.0"

# here is where the undo stuff begins
if {![info exists classNewId]} {
    # work around object creation between multiple include of this file problem
    set classNewId 0
}

set undo_id [new textUndoer $textareacur]
set listundo_id("$textareacur") $undo_id

# the following update makes the initial textarea reactive to dnd!
update

# Drag and drop feature using TkDnD
tkdndbind $textareacur
