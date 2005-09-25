proc Addarg_bp {w focusbut leftwin rightwin} {
# Create the add argument dialog
# This dialog can be called from the watch window or from the configure dialog
    global argname argvalue
    global adda getvaluefromscilab
    global textFont menuFont
    set pos [$leftwin curselection]
    if {$pos == ""} {set pos -1}
    set adda $w.adda
    toplevel $adda
    wm title $adda [mc "Add/Change"]
    setwingeom $adda
    wm resizable $adda 1 0
    set selecteditem [$leftwin curselection]
    if {$selecteditem != ""} {
        set argname [$leftwin get $selecteditem]
        set argvalue [$rightwin get $selecteditem]
    } else {
        set argname ""
        set argvalue ""
    }
    frame $adda.f
    frame $adda.f.f1
    set tl [mc "Variable:"]
    label $adda.f.f1.label -text $tl -width 10 -font $menuFont
    entry $adda.f.f1.entry -textvariable argname -width 0 -font $textFont
    pack $adda.f.f1.label $adda.f.f1.entry -side left
    pack $adda.f.f1.entry -expand 1 -fill x
    $adda.f.f1.entry selection range 0 end
    pack $adda.f.f1 -expand 1 -fill x
    frame $adda.f.f2
    set tl [mc "Value:"]
    label $adda.f.f2.label -text $tl -width 10 -font $menuFont
    entry $adda.f.f2.entry -textvariable argvalue -width 0 -font $textFont
    pack $adda.f.f2.label $adda.f.f2.entry -side left
    pack $adda.f.f2.entry -expand 1 -fill x
    $adda.f.f2.entry selection range 0 end
    pack $adda.f.f2 -expand 1 -fill x
    if {[string first listboxinput $leftwin] == -1} {
        # This checkbutton is only displayed when the dialog is used with the watch window,
        # not with the configure box
        eval "checkbutton $adda.f.cbox1 [bl "Get current value from Scilab"] -variable getvaluefromscilab \
                                                                     -command togglegetvaluefromscilab \
                                                                     -font $menuFont"
        pack $adda.f.cbox1 -expand 0 -fill none -anchor w -padx 6
        $adda.f.cbox1 deselect
    }
    frame $adda.f.f9
    button $adda.f.f9.buttonOK -text "OK" \
           -command "OKadda_bp $pos $leftwin $rightwin ; destroy $adda"\
           -width 10 -height 1 -font $menuFont
    set bl [mc "Cancel"]
    button $adda.f.f9.buttonCancel -text $bl \
           -command "Canceladda_bp $adda $pos $leftwin"\
           -width 10 -height 1 -font $menuFont
    pack $adda.f.f9.buttonOK $adda.f.f9.buttonCancel -side left -padx 10
    pack $adda.f.f9 -pady 4
    pack $adda.f -expand 1 -fill x
    bind $adda <Return> "OKadda_bp $pos $leftwin $rightwin ; destroy $adda"
    bind $adda <Escape> "Canceladda_bp $adda $pos $leftwin"
    if {$selecteditem != ""} {
        focus $adda.f.f2.entry
    } else {
        focus $adda.f.f1.entry
    }
    focus $focusbut
    # This update is required for the width and height to be taken into account in minsize
    # It also prevents a "grab failed" error to occur on Linux when double clicking on the
    # variable name in the watch or configure window
    update
    grab $adda
    wm minsize $adda [winfo width $adda] [winfo height $adda]
}

proc OKadda_bp {pos leftwin rightwin {forceget "false"}} {
# Close the add argument dialog and take the values of its fields into account
    global unklabel
    global argname argvalue
    global spin funvars funvarsvals
    global watchvars watchvarsvals
    global getvaluefromscilab
    if {$argname != ""} {

        # 1. Check whether the variable was already entered in the calling (watch or configure) box
        set leftwinelts [$leftwin get 0 end]
        set alreadyexists "false"
        set eltindex 0
        foreach elt $leftwinelts {
            if {$argname == $elt} {
                set alreadyexists "true"
                break
            } else {
                incr eltindex
            }
        }

        # 2. add or change this variable in the calling box
        # next line is a bit dirty... it is used to differentiate processing for the
        # add argument box used with the watch window from the one use with the configure box
        if {[string first listboxinput $leftwin] != -1} {
            set funname [$spin get]
        }
        # certain special characters are allowed in Scilab names,
        # these must be escaped
        set escargname [escapespecialchars $argname]
        if {$alreadyexists == "false"} {
            # a new variable was added in the add box
            set pos [expr $pos + 1]
            if {[string first listboxinput $leftwin] != -1} {
                # the proc was called from configure box
                set funvars($funname) [linsert $funvars($funname) $pos $argname]
                set funvarsvals($funname,$argname) $argvalue
            } else {
                # the proc was called from the watch window
                if {$argvalue == "" || $getvaluefromscilab == 1} {set argvalue $unklabel}
                set watchvars [linsert $watchvars $pos $argname]
                set watchvarsvals($argname) $argvalue
                if {$argvalue == $unklabel} {
                    getonefromshell $escargname "sync"
                    set argvalue $watchvarsvals($argname)
                }
            }
            $leftwin insert $pos $argname
            $rightwin insert $pos $argvalue
            $leftwin selection set $pos
            $leftwin see $pos
        } else {
            # an existing variable was modified in the add box
            set nextone [expr $eltindex + 1]
            if {$nextone >= [$leftwin size]} {
                set nextone 0
            }
            if {[string first listboxinput $leftwin] != -1} {
                # the proc was called from configure box
                set funvarsvals($funname,$argname) $argvalue
            } else {
                # the proc was called from the watch window
                if {$argvalue == ""} {set argvalue $unklabel}
                if {$getvaluefromscilab == 1} {set forceget "true"}
                set watchvarsvals($argname) $argvalue
                if {$forceget == "true"} {
                    getonefromshell $escargname "sync"
                    set argvalue $watchvarsvals($argname)
                }
            }
            $leftwin selection set $nextone
            $leftwin see $nextone
            $rightwin delete $eltindex
            $rightwin insert $eltindex $argvalue
        }

        # 3. Perform a roundtrip to Scilab to:
        #   a. update the new value of the modified variable in Scilab, and
        #   b. get back the new values of all the watched variables from Scilab
        # This is required only for the watch window, in case the user watches a variable
        # var and elements of var such as var(i) or var(i:j) at the same time
        # The new or changed variable must first be updated in Scilad, and then the
        # watched variables must be get back from Scilab
        if {[string first listboxinput $leftwin] == -1 && \
            [getdbstate] == "DebugInProgress" && \
            $getvaluefromscilab == 0 } {
            # the proc was called from the watch window, during a debug session,
            # and the "get from Scilab" box was not checked
            setinscishellone_bp $argname  ; # update the changed var only
            getfromshell                  ; # get new value of all the watched variables
        }

    }
}

proc Canceladda_bp {w pos leftwin} {
# Close the add argument dialog and ignore the values of its fields
    destroy $w
    $leftwin selection set $pos
}

proc Removearg_bp {leftwin rightwin} {
# Suppress a watch variable (if called from the watch window)
# or a function argument (if called from the configure box)
# from their respective lists and update the calling dialog
    global spin
    global funvars funvarsvals
    set selecteditem [$leftwin curselection]
    if {$selecteditem != ""} {
        if {[string first listboxinput $leftwin] != -1} {
            set funname [$spin get]
            set argname [$leftwin get $selecteditem]
            unset funvarsvals($funname,$argname)
            set funvars($funname) [lreplace $funvars($funname) \
                                            $selecteditem $selecteditem]
        }
        $leftwin delete $selecteditem
        $leftwin see $selecteditem
        $rightwin delete $selecteditem
        if {$selecteditem < [expr [$leftwin size] ]} {
            $leftwin selection set $selecteditem
        } else {
            $leftwin selection set [expr [$leftwin size] - 1]
        }
    }
}

proc togglegetvaluefromscilab {} {
# Toggle the get from Scilab checkbox of the add argument dialog
    global adda getvaluefromscilab
    if {$getvaluefromscilab == 1} {
        $adda.f.f2.entry configure -state disabled
        focus $adda.f.f1.entry
    } else {
        $adda.f.f2.entry configure -state normal
        focus $adda.f.f2.entry
    }
}

proc quickAddWatch_bp {watchvar} {
# Quickly add a vatch variable
# Variable name is passed to this proc as an input
# Variable value is always got from Scilab
    global watch argname argvalue lbvarname lbvarval
    global getvaluefromscilab
    set watchalreadyopen "false"
    if {[info exists watch]} {
        if {[winfo exists $watch]} {
            set watchalreadyopen "true"
        }
    }
    if {$watchalreadyopen == "false"} {
        showwatch_bp
    }
    set argname $watchvar
    # set value to "" so that OKadda_bp will get it from the shell
    set argvalue ""
    set getvaluefromscilab 1
    OKadda_bp -1 $lbvarname $lbvarval "true"
}

proc removefuns_bp {textarea} {
    global funsinbuffer funvars funvarsvals funnames funnameargs
    if {[info exists funsinbuffer($textarea)]} {
        set oppar [expr [string first "\(" $funnameargs] - 1]
        set curfunname [string range $funnameargs 0 $oppar]
        foreach fun $funsinbuffer($textarea) {
            if {[info exists funvars($fun)]} {
                foreach arg $funvars($fun) {
                    unset funvarsvals($fun,$arg)
                }
                unset funvars($fun)
            }
            set pos [lsearch $funnames $fun]
            set funnames [lreplace $funnames $pos $pos]
            if {$curfunname == $fun} {
                # debug session is aborted if the buffer containing the debugged function is closed
                if {[getdbstate] == "DebugInProgress"} canceldebug_bp
                set funnameargs "[lindex $funnames 0]("
            }
        }
        if {$funnameargs != "("} {
            set funname [string range $funnameargs 0 [expr [string length $funnameargs] - 2]]
            set strargs ""
            if {[info exists funvars($funname)]} {
                foreach var $funvars($funname) {
                    set argvalue $funvarsvals($funname,$var)
                    set strargs "$strargs,$argvalue"
                }
                set strargs [string range $strargs 1 end]
            }
            set funnameargs "$funname\($strargs\)"
        } else {
            set funnameargs ""
            setdbstate "NoDebug"
        }
    }
}

proc setinscishellone_bp {var} {
# Update in Scilab the value of one watched variable named $var
    global funnameargs waitmessage
    if {[checkscilabbusy] == "OK"} {
        showinfo $waitmessage
        if {$funnameargs != ""} {
            set commnvars [createsetinscishellcomm $var]
            set watchsetcomm [lindex $commnvars 0]
            set visibilitycomm [lindex $commnvars 2]
            if {$watchsetcomm != ""} {
                ScilabEval_lt "$visibilitycomm;$watchsetcomm" "seq"
            }
        } else {
            # <TODO> .sce case
 #           ScilabEval_lt " " "seq"
        }
    }
}
