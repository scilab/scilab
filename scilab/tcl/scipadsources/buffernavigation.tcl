proc montretext {textarea} {
    global pad listoffile Scheme
    selection clear
    pack forget [gettextareacur]
    settextareacur $textarea
    $pad.yscroll configure -command "[gettextareacur] yview"
    $pad.xscroll configure -command "[gettextareacur] xview"
    $pad.xscroll set [lindex [[gettextareacur] xview] 0] [lindex [[gettextareacur] xview] 1]
    $pad.yscroll set [lindex [[gettextareacur] yview] 0] [lindex [[gettextareacur] yview] 1]
    modifiedtitle $textarea
    pack  $textarea -in  $pad.bottomleftmenu -side left -expand 1 -fill both
    focus $textarea
    keyposn $textarea
    set Scheme $listoffile("$textarea",language)
    schememenus $textarea
}


proc nextbuffer {} {
    global pad listoftextarea listoffile radiobuttonvalue
    set textarea [gettextareacur]
    set curbuf [expr [lsearch $listoftextarea $textarea]+1]
    set curbuf [expr $curbuf+1]
    set nbuf [llength $listoftextarea]
    if {$curbuf>$nbuf} {
        set curbuf 1
    }
    set radiobuttonvalue [$pad.filemenu.wind entrycget $curbuf -value]
    montretext [lindex $listoftextarea [expr $curbuf-1]]
    #keypress must replace the selection if buffers are switched
    set existsSel [[gettextareacur] tag nextrange sel 1.0]
    if {$existsSel != {}} {
        [gettextareacur] tag add sel [lindex $existsSel 0] [lindex $existsSel 1]
    }
}

proc prevbuffer {} {
   global pad listoftextarea listoffile radiobuttonvalue
    set textarea [gettextareacur]
    set curbuf [expr [lsearch $listoftextarea $textarea]+1]
    set curbuf [expr $curbuf-1]
    set nbuf [llength $listoftextarea]
    if {$curbuf<1} {
        set curbuf $nbuf
    }
    set radiobuttonvalue [$pad.filemenu.wind entrycget $curbuf -value]
    montretext [lindex $listoftextarea [expr $curbuf-1]]
    #keypress must replace the selection if buffers are switched
    set existsSel [[gettextareacur] tag nextrange sel 1.0]
    if {$existsSel != {}} {
        [gettextareacur] tag add sel [lindex $existsSel 0] [lindex $existsSel 1]
    }
}

proc gotoline {} {
	global textareacur pad
	set gotln $pad.gotln
	catch {destroy $gotln}
	toplevel $gotln
    wm title $gotln [mc "Goto Line?"]
	setwingeom $gotln
	frame $gotln.top 
	frame $gotln.bottom
    label $gotln.top.label -text [mc "Goto Line: "]
	entry $gotln.top.gotln -textvariable gotlnsetupnew -width 10
	$gotln.top.gotln delete 0 end 
	button $gotln.bottom.ok -text "OK" -command "addtogotln $gotln"
    button $gotln.bottom.cancel -text [mc "Cancel"] \
		-command "destroy $gotln"
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
	[gettextareacur] mark set insert "$gotlnCommand.0"
	catch {keyposn [gettextareacur]}
        [gettextareacur] see insert
	destroy $prnt
    }
}

