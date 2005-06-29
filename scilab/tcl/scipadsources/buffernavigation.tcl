proc montretext {textarea} {
# Display a textarea
# This textarea becomes the current one
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
# Pop up the go to line dialog
    global pad textFont menuFont unklabel
    global physlogic linetogo curfileorfun funtogoto

    # Obtain all the functions defined in all the opened buffers, and create a list
    # containing function name, textarea where this function is defined and physical
    # line number where the function definition starts. Format is:
    # { funcname1 textarea1 funstartline1 funcname2 textarea2 funstartline2 ...}
    set funtogotolist {}
    set fundefs [getallfunsinalltextareas]
    foreach {ta fundefsinta} $fundefs {
        foreach {funcname funcline funstartline} $fundefsinta {
            if {$funcname != "0NoFunInBuf"} {
                lappend funtogotolist $funcname $ta $funstartline
            }
        }
    }
    # Set default values for the other the choices of the box
    if {![info exists linetogo]} {set linetogo 1}
    if {![info exists funtogoto]} {
        set funtogoto [list $unklabel 0 0]
    } else {
        # Do not propose a function defined in a closed buffer (in case the goto box is reopened
        # after a buffer has been closed)
        set stillexists "false"
        foreach {fn ta fsl} $funtogotolist {
            if {$fn == [lindex $funtogoto 0]} {
                set stillexists true
                break
            }
        }
        if {$stillexists == "false"} {
            set funtogoto [list $unklabel 0 0]
        }
    }

    # Create dialog geometry and pack widgets
    set gotln $pad.gotln
    catch {destroy $gotln}
    toplevel $gotln
    wm title $gotln [mc "Goto Line?"]
    setwingeom $gotln

    label $gotln.l1 -text [mc "Go to:"] -font $menuFont
    pack $gotln.l1 -anchor w -pady 5

    frame $gotln.f1
    eval "radiobutton $gotln.f1.cbox1 [bl "logical line"]  \
        -variable physlogic -value \"logical\"  -font $menuFont \
        -command \"updateOKbuttonstategoto $gotln\" "
    eval "radiobutton $gotln.cbox2 [bl "physical line"] \
        -variable physlogic -value \"physical\" -font $menuFont \
        -command \"updateOKbuttonstategoto $gotln\" "
    entry $gotln.f1.en1 -textvariable linetogo -width 8 -font $textFont
    pack $gotln.f1.cbox1 $gotln.f1.en1 -side left
    pack $gotln.f1 $gotln.cbox2 -anchor w

    frame $gotln.f2
    eval "radiobutton $gotln.f2.cbox3 [bl "in function"]  \
        -variable curfileorfun -value \"function\"     -font $menuFont \
        -command \"updateOKbuttonstategoto $gotln\" "
    eval "radiobutton $gotln.cbox4 [bl "in current file"] \
        -variable curfileorfun -value \"current_file\" -font $menuFont \
        -command \"updateOKbuttonstategoto $gotln\" "
    menubutton $gotln.f2.mb -text [lindex $funtogoto 0] -indicatoron 1 \
        -font $textFont

    menu $gotln.f2.mb.om1 -tearoff 0 -font $menuFont
    $gotln.f2.mb configure -menu $gotln.f2.mb.om1
    foreach {funname ta funstartline} $funtogotolist {
        $gotln.f2.mb.om1 add radiobutton -label $funname -font $menuFont \
            -variable funtogoto -value [list $funname $ta $funstartline] \
            -command "updatemenubutlabelgoto $gotln"
    }
    pack $gotln.f2.cbox3 $gotln.f2.mb -side left
    pack $gotln.f2 $gotln.cbox4 -anchor w

    button $gotln.ok -text [mc "OK"] -font $menuFont -width 8 \
        -command "dogotoline ; destroy $gotln"
    button $gotln.cancel -text [mc "Cancel"] -font $menuFont -width 8 \
        -command "destroy $gotln"
    pack $gotln.ok $gotln.cancel -side left -padx 5 -pady 5 -fill none -expand yes

    focus $gotln.f1.en1
    $gotln.f1.en1 selection range 0 end

    bind $gotln <Return> {if {[[winfo toplevel %W].ok cget -state] == "normal"} { \
                              dogotoline ; destroy [winfo toplevel %W] \
                          }}
    bind $gotln <Escape> {destroy [winfo toplevel %W]}
 
    # Default choices
    if {$physlogic    == ""} {$gotln.f1.cbox1 invoke}
    if {$curfileorfun == ""} {$gotln.f2.cbox3 invoke}

    # Validation of the entry to prevent the user to enter nasty things
    $gotln.f1.en1 configure -validate all -vcmd "updateOKbuttonstategoto $gotln %P"
}

proc updatemenubutlabelgoto {w} {
# Set value of -text for the menu button label of the go to line box
# It contains the currently selected function name to go to
    global funtogoto
    $w.f2.mb configure -text [lindex $funtogoto 0]
    # if a function has been selected it is likely the goto will use it, hence invoke this choice
    $w.f2.cbox3 invoke
    updateOKbuttonstategoto $w
}

proc updateOKbuttonstategoto {w {entryfieldvalue "not_given"}} {
# Prevent from launching gotos with inconsistent choices, and do some
# minimal controls on the user input
# Note: Scheme or $listoffile("$textarea",language) does not need to be checked since
# getallfunsintextarea deals with it and does not return functions from anything else
# than a Scilab scheme buffer
    global unklabel physlogic curfileorfun linetogo Scheme
    if {$entryfieldvalue == "not_given"} {set entryfieldvalue $linetogo}
    if {($curfileorfun == "current_file" && $physlogic == "logical" && $Scheme != "scilab") || \
        ($curfileorfun == "function" && [$w.f2.mb cget -text] == $unklabel) || \
        ($entryfieldvalue <= 0) || ![string is integer -strict $entryfieldvalue] } {
        $w.ok configure -state disabled
    } else {
        $w.ok configure -state normal
    }
    # Validation of the entry widget always succeeds, so that the textvariable
    # is updated. The validation result is made known by the OK button state
    return 1
}

proc dogotoline {} {
# Actually perform a go to line number ... taking into account all the possible
# choices from the user in the dialog box. These choices are known by the four
# global variables below (this is required and sufficient)

    global physlogic linetogo curfileorfun funtogoto

    if {$curfileorfun == "current_file"} {
        if {$physlogic == "physical"} {
            # go to physical line in current file
            set ta [gettextareacur]
            $ta mark set insert "$linetogo.0"
            catch {keyposn $ta}
            $ta see insert

        } else {
            # go to logical line in current file
            # this option enabled in the goto line dialog only for scilab scheme files
            set ta [gettextareacur]
            set endpos [$ta index end]
            set offset 0
            set curphysline 1.0
            set curlogicline $curphysline
            while {$linetogo != $curlogicline && [$ta compare $curphysline < $endpos]} {
                incr offset
                set curphysline [$ta index "$offset.0"]
                set contlines [countcontlines $ta 1.0 $curphysline]
                set curlogicline [$ta index "$curphysline - $contlines l"]
            }
            $ta mark set insert $curphysline
            catch {keyposn $ta}
            $ta see insert

        }

    } else {
        if {$physlogic == "physical"} {
            # go to physical line in function
            set textarea [lindex $funtogoto 1]
            set absoluteline [$textarea index "[lindex $funtogoto 2] + 1c + $linetogo lines -1l"]
            # check that the end of the function is after the position to go to
            set infun [whichfun $absoluteline $textarea]
            if {$infun == {}} {
                # target line is before function definition line - should never happen since
                # negative or null line numbers are forbidden in the entry widget!
                showinfo [mc "Outside of function definition"]
                set absoluteline [$textarea index [lindex $funtogoto 2]]
            } else {
                # target line is after the beginning of the function definition
                if {[lindex $infun 0] != [lindex $funtogoto 0]} {
                    # target line is after the end of the function definition,
                    # we will jump to the start of the function definition instead
                    showinfo [mc "Outside of function definition"]
                    set absoluteline [$textarea index [lindex $funtogoto 2]]
                } else {
                    # target position is between function and endfunction lines (inclusive)
                    set absoluteline [$textarea index "[lindex $funtogoto 2] + $linetogo lines -1l"]
                }
            }
            montretext $textarea
            $textarea mark set insert $absoluteline
            catch {keyposn $textarea}
            $textarea see insert

        } else {
            # go to logical line in function
            set textarea [lindex $funtogoto 1]
            set funstart [lindex $funtogoto 2]
            set infun [whichfun [$textarea index "$funstart + 1c"] $textarea]
            set offset 0
            # <TODO> This while loop could be improved (proc whichfun takes time to execute)
            # Its purpose is to make the line number in the buffer correspond to $linetogo
            while {$infun != "" && [lindex $infun 1] != $linetogo} {
                incr offset
                set infun [whichfun [$textarea index "$funstart + $offset l"] $textarea]
            }
            if {[lindex $infun 0] == [lindex $funtogoto 0]} {
                # target logical line is between function and endfunction
                set targetline [$textarea index "$funstart + $offset l"]
            } else {
                # target logical line is after the endfunction,
                # we will jump to the start of the function definition instead
                showinfo [mc "Outside of function definition"]
                set targetline [$textarea index $funstart]
            }
            montretext $textarea
            $textarea mark set insert $targetline
            catch {keyposn $textarea}
            $textarea see insert

        }
    }

}
