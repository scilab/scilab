# main window settings
eval destroy [winfo child $pad]
wm title $pad "$winTitle - $listoffile("$pad.textarea",filename)"
wm iconname $pad $winTitle
##ES 4/9/2003
##set taille [font measure $textFont " "]
##wm grid $pad 20 60 20 60 
#boh. "text $pad.textarea -height 1 -width 1" below
wm geometry $pad $WMGEOMETRY
wm minsize $pad 25 1 

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

#ES: instead of several setfontscipadN{}
proc setfontscipad {FontSize} {
    global textFont menuFont pad
    global listoftextarea
    set textFont -Adobe-courier-medium-R-Normal-*-$FontSize-*
    set menuFont -adobe-helvetica-bold-r-normal--$FontSize-*
    $pad.filemenu configure -font $menuFont
    foreach textarea $listoftextarea {$textarea configure -font $textFont}
}


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

# exit app
proc exitapp {} {
    global listoffile
    global listoftextarea
    global pad

    foreach textarea $listoftextarea {
	#inccount $textarea
	closefile $textarea
    }
#    unset pad  ## not needed if closefile unsets it
}


# proc to set child window position
proc setwingeom {wintoset} {
    global pad
    wm resizable $wintoset 0 0
    set myx [expr (([winfo screenwidth $pad]/2) - \
		       ([winfo reqwidth $wintoset]))]
    set myy [expr (([winfo screenheight $pad]/2) - \
		       ([winfo reqheight $wintoset]/2))]
    wm geometry $wintoset +$myx+$myy
}

