proc montretext {textarea} {
    global pad listoffile Scheme
# Next line added by Francois VOGEL 16/04/04, to fix the silent clearing of 
# selection
    selection clear
    pack forget [gettextareacur]
    settextareacur $textarea
    $pad.yscroll configure -command "[gettextareacur] yview"
    $pad.xscroll configure -command "[gettextareacur] xview"
# Next two lines added by Francois VOGEL, 21/04/04, to fix size/position bug of sliders
    $pad.xscroll set [lindex [[gettextareacur] xview] 0] [lindex [[gettextareacur] xview] 1]
    $pad.yscroll set [lindex [[gettextareacur] yview] 0] [lindex [[gettextareacur] yview] 1]
#    settitle $listoffile("$textarea",filename)
    modifiedtitle $textarea
    pack  $textarea -in  $pad.bottomleftmenu -side left -expand 1 -fill both
    focus $textarea
    keyposn $textarea
#ES 27/5/04
    set Scheme $listoffile("$textarea",language)
    schememenus $textarea
}


# Francois VOGEL, 16/04/04, corrected and expanded by ES
proc nextbuffer {} {
    global pad listoftextarea listoffile radiobuttonvalue
    set textarea [gettextareacur]
#    set curbuf [$pad.filemenu.wind index $listoffile("$textarea",filename)]    
    set curbuf [expr [lsearch $listoftextarea $textarea]+1]
    set curbuf [expr $curbuf+1]
    set nbuf [llength $listoftextarea]

    if {$curbuf>$nbuf} {
        set curbuf 1
    }
#    showinfo $curbuf
    set radiobuttonvalue [$pad.filemenu.wind entrycget $curbuf -value]
    montretext [lindex $listoftextarea [expr $curbuf-1]]
}

proc prevbuffer {} {
   global pad listoftextarea listoffile radiobuttonvalue
    set textarea [gettextareacur]
#    set curbuf [$pad.filemenu.wind index $listoffile("$textarea",filename)]    
    set curbuf [expr [lsearch $listoftextarea $textarea]+1]
    set curbuf [expr $curbuf-1]
    set nbuf [llength $listoftextarea]

    if {$curbuf<1} {
        set curbuf $nbuf
    }
#    showinfo $curbuf
    set radiobuttonvalue [$pad.filemenu.wind entrycget $curbuf -value]
    montretext [lindex $listoftextarea [expr $curbuf-1]]
}

# add gotoline option included by Matthieu PHILIPPE 21/11/2001 from gotoline.pth
proc gotoline {} {
	global textareacur pad lang
	set gotln $pad.gotln
	catch {destroy $gotln}
	toplevel $gotln
	if {$lang == "eng"} {
	    wm title $gotln "Goto Line?"
	} else {
	    wm title $gotln "Aller à la ligne"
	}
	setwingeom $gotln
	frame $gotln.top 
	frame $gotln.bottom
	if {$lang == "eng"} {
	    label $gotln.top.label -text "Goto Line: "
	} else {
	    label $gotln.top.label -text "Aller à la ligne : "
	}
	entry $gotln.top.gotln -textvariable gotlnsetupnew -width 10
	$gotln.top.gotln delete 0 end 
	button $gotln.bottom.ok -text "OK" -command "addtogotln $gotln"
	if {$lang == "eng"} {
	    button $gotln.bottom.cancel -text "Cancel" \
		-command "destroy $gotln"
	} else {
	    button $gotln.bottom.cancel -text "Annuler" \
	         -command "destroy $gotln"
	}
	focus $gotln.top.gotln
	pack $gotln.top -side top -expand 0 
	pack $gotln.bottom -side bottom -expand 0 
	pack $gotln.top.label $gotln.top.gotln -in $gotln.top -side left \
	    -fill x -fill y
	pack $gotln.bottom.ok $gotln.bottom.cancel -in $gotln.bottom \
	    -side left -fill x -fill y
	bind $gotln <Return> "addtogotln $gotln"
	bind $gotln <Escape> "destroy $gotln"

    proc addtogotln {prnt} {
	global textareacur
	global gotlnCommand
	set gotlnCommand [$prnt.top.gotln get]
#	tkTextSetCursor [gettextareacur] "$gotlnCommand.0"
	[gettextareacur] mark set insert "$gotlnCommand.0"
	catch {keyposn [gettextareacur]}
        [gettextareacur] see insert
	destroy $prnt
    }
}

