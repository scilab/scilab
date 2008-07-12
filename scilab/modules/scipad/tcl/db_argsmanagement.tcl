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
proc Addarg_bp {w focusbut leftwin {rightwin "norightwin"}} {
# Create the add argument dialog
# This dialog can be called from the watch window or from the configure dialog
# Calling this proc with $leftwin being an empty listbox or a non empty listbox
# with no selected item will result in creating a new element (unless the user
# hits Cancel)
    global argname argvalue
    global adda getvaluefromscilab
    global textFont menuFont

    if {[$leftwin index end] == 0} {
        set emptylistbox true
    } else {
        set emptylistbox false
    }

    foreach {calledfromwatch_vars calledfromwatch_exps calledfromconfigure} [findaddargcallingcontext $leftwin $rightwin] {}

    set selecteditem [$leftwin curselection]
    if {!$emptylistbox && $selecteditem != ""} {
        set argname [$leftwin get $selecteditem]
        if {$rightwin ne "norightwin"} {
            set argvalue [$rightwin get $selecteditem]
        } else {
            # the generic expressions area in the watch window has no twin listbox
            # so to avoid repeated tests on !$calledfromwatch_exps an empty default
            # value is set for $argvalue
            set argvalue ""
        }
        # check that what the user selected for edit is actually editable
        # (this check only concerns the variables area of the watch window,
        # not the configure box)
        if {$calledfromwatch_vars} {
            set editable [isvareditable $argname]
            if {!$editable} {
                tk_messageBox -message \
                    [concat [mc "This variable can be watched but cannot be edited!"]\
                            [mc "A new watch variable will be created."] ] \
                    -icon warning -type ok \
                    -title [mc "Non editable variable"]
                # run the add variable dialog again after having cleared the selection,
                # which is a special case in proc Addarg_bp: listbox is not empty but
                # has no currently selected item
                $leftwin selection clear $selecteditem
                Addarg_bp $w $focusbut $leftwin $rightwin
                return
            }
        }
    } else {
        set argname ""
        set argvalue ""
        set selecteditem -1
    }

    set adda $w.adda
    toplevel $adda
    if {!$calledfromwatch_exps} {
        wm title $adda [mc "Add/Change"]
    } else {
        wm title $adda [mc "Add"]
    }
    wm withdraw $adda

    # The add argument dialog must be a transient of $watch or $conf
    # otherwise it might be obscured by the watch window if always on top
    wm transient $adda $w

    frame $adda.f

    frame $adda.f.f1
    if {!$calledfromwatch_exps} {
        set firstentrylabelname "Variable:"
        set bestwidth [mcmaxra $firstentrylabelname \
                               "Value:"]
    } else {
        set firstentrylabelname "Expression:"
        set bestwidth [mcmaxra $firstentrylabelname]
    }
    set tl [mc $firstentrylabelname]
    label $adda.f.f1.label -text $tl -width $bestwidth -font $menuFont
    entry $adda.f.f1.entry -textvariable argname -width 0 -font $textFont
    pack $adda.f.f1.label $adda.f.f1.entry -side left
    pack $adda.f.f1.entry -expand 1 -fill x
    $adda.f.f1.entry selection range 0 end
    pack $adda.f.f1 -expand 1 -fill x

    frame $adda.f.f2
    set tl [mc "Value:"]
    label $adda.f.f2.label -text $tl -width $bestwidth -font $menuFont
    entry $adda.f.f2.entry -textvariable argvalue -width 0 -font $textFont
    pack $adda.f.f2.label $adda.f.f2.entry -side left
    pack $adda.f.f2.entry -expand 1 -fill x
    $adda.f.f2.entry selection range 0 end
    if {!$calledfromwatch_exps} {
        pack $adda.f.f2 -expand 1 -fill x
    }
    if {$calledfromwatch_vars} {
        # This checkbutton is only displayed when the dialog is used with the variables area
        # of the watch window only
        eval "checkbutton $adda.f.cbox1 [bl "&Get current value from Scilab"] \
                -variable getvaluefromscilab \
                -command togglegetvaluefromscilab \
                -font \[list $menuFont\] "
        pack $adda.f.cbox1 -expand 0 -fill none -anchor w -padx 6
        $adda.f.cbox1 deselect
    }

    frame $adda.f.f9
    set bestwidth [mcmaxra "OK" \
                           "Cance&l"]
    button $adda.f.f9.buttonOK -text "OK" \
           -command "OKadda_bp $selecteditem $leftwin $rightwin ; destroy $adda"\
           -width $bestwidth -font $menuFont
    eval "button $adda.f.f9.buttonCancel [bl "Cance&l"] \
           -command \"Canceladda_bp $adda $selecteditem $leftwin\"\
           -width $bestwidth -font \[list $menuFont\] "
    pack $adda.f.f9.buttonOK $adda.f.f9.buttonCancel -side left -padx 10
    pack $adda.f.f9 -pady 4
    pack $adda.f -expand 1 -fill x

    bind $adda <Return> "OKadda_bp $selecteditem $leftwin $rightwin ; destroy $adda"
    bind $adda <Escape> "Canceladda_bp $adda $selecteditem $leftwin"

    bind $adda <Alt-[fb $adda.f.cbox1]>           "$adda.f.cbox1 invoke"
    bind $adda <Alt-[fb $adda.f.f9.buttonCancel]> "$adda.f.f9.buttonCancel invoke"

    if {$selecteditem != -1 && !$calledfromwatch_exps} {
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
    setwingeom $adda
    wm resizable $adda 1 0
    wm minsize $adda [winfo width $adda] [winfo height $adda]
    wm deiconify $adda
}

proc OKadda_bp {pos leftwin rightwin {forceget "false"}} {
# Close the add argument dialog and take the values of its fields into account
    global unklabel
    global argname argvalue
    global spin funvars funvarsvals
    global watchvars watchvarsprops
    global getvaluefromscilab
    global debugger_unwatchable_vars

    if {$argname != ""} {

        foreach {calledfromwatch_vars calledfromwatch_exps calledfromconfigure} [findaddargcallingcontext $leftwin $rightwin] {}

        # 1. Some variables cannot be watched
        if {[lsearch -exact $debugger_unwatchable_vars $argname] != -1} {
            set mes [concat [mc "Sorry, variable"] $argname [mc "is special and cannot be watched."]]
            set tit [mc "Non watchable variable"]
            tk_messageBox -message $mes -icon error -title $tit
            if {$pos == -1} {set pos 0}
            $leftwin selection clear 0 end
            $leftwin selection set $pos
            return
        }

        # 2. Check whether the variable was already entered in the calling (watch or configure) box
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

        # 3. In certain cases one cannot add variables when Scilab is busy
        if {$alreadyexists == "false" && \
            $calledfromwatch_vars && \
            ($argvalue == "" || $getvaluefromscilab == 1 || $argvalue == $unklabel) } {
            # the proc was called from the variables area of the watch window,
            # $argname does not yet exist and $argvalue will in fine be set
            # to $unklabel
            # -> the value will be retrieved from Scilab
            if {[isscilabbusy 5]} {
                if {$pos == -1} {set pos 0}
                $leftwin selection clear 0 end
                $leftwin selection set $pos
                return
            }
        }
        if {$alreadyexists == "true" && \
            $calledfromwatch_vars && \
            ($getvaluefromscilab == 1 || $forceget == "true") } {
            # the proc was called from variables area of the the watch window,
            # $argname does already exist and $argvalue will in fine be forced
            # to Scilab's value
            # -> the value will be retrieved from Scilab
            if {[isscilabbusy 5]} {
                if {$pos == -1} {set pos 0}
                $leftwin selection clear 0 end
                $leftwin selection set $pos
                return
            }
        }

        # 4. Attempts to update a global auto watched variable must be killed.
        # Note that in principle only attempts to redefine globals must be
        # killed, but not necessarily attempts to redefine other non editable
        # variables. This is because redefining globals would mess the
        # visibility assumptions in the debugged function, i.e. the debug tool
        # would potentially change the behavior of what is debugged. This is
        # however not true for non global non editable variables that could
        # be redefined to some editable types with no such side effect. However
        # allowing this wouldn't make a lot of sense, especially since the user
        # previously got the non editable warning message box, so all non
        # editable variables are prevented from being redefined, not only the
        # global ones
        if {$alreadyexists == "true" && \
            $calledfromwatch_vars && \
            ![isvareditable $argname]} {
            if {$pos == -1} {set pos 0}
            $leftwin selection clear 0 end
            $leftwin selection set $pos
            return
        }

        # 5. Decide whether a roundtrip to Scilab is needed, so that when
        # variable var has been modified, the watched var(1:2) (for instance)
        # will also be updated
        set mustdoScilabroundtrip false
        if {$calledfromwatch_vars && \
            [getdbstate] == "DebugInProgress" && \
            $getvaluefromscilab == 0 } {
            # the proc was called from the watch window, during a debug session,
            # and the "get from Scilab" box was not checked
            # -> a round trip to Scilab is required
            set mustdoScilabroundtrip true
            if {[isscilabbusy 5]} {
                if {$pos == -1} {set pos 0}
                $leftwin selection clear 0 end
                $leftwin selection set $pos
                return
            }
        }

        # 6. Add or change this variable in the calling box
        if {$calledfromconfigure} {
            set funname [$spin get]
        }
        if {$alreadyexists == "false"} {
            # a new variable was added in the add box
            $leftwin selection clear $pos
            # set insert position after the currently selected item
            incr pos
            if {$calledfromconfigure} {
                # the proc was called from the configure box
                set funvars($funname) [linsert $funvars($funname) $pos $argname]
                set funvarsvals($funname,$argname) $argvalue
            } elseif {$calledfromwatch_vars} {
                # the proc was called from the variables area of the watch window
                if {$argvalue == "" || $getvaluefromscilab == 1} {set argvalue $unklabel}
                set watchvars [linsert $watchvars $pos $argname]
                set watchvarsprops($argname,value) $argvalue
                set watchvarsprops($argname,tysi) $unklabel
                set watchvarsprops($argname,editable) true
                if {$argvalue == $unklabel} {
                    getonewatchvarfromshell $argname
                    set fullcomm "TCL_EvalStr(\"updatewatch_bp\",\"scipad\");"
                    ScilabEval_lt $fullcomm "seq"
                    set argvalue $watchvarsprops($argname,value)
                }
            } else {
                #the proc was called from the expressions area of the watch window
                # nothing to do: updating the listbox below will update the listvariable
            }
            $leftwin insert $pos $argname
            if {$rightwin ne "norightwin"} {
                $rightwin insert $pos $argvalue
            }
            $leftwin selection clear 0 end
            $leftwin selection set $pos
            $leftwin see $pos
        } else {
            # an existing variable was modified in the add box
            $leftwin selection clear $eltindex
            if {$calledfromconfigure} {
                # the proc was called from the configure box
                set funvarsvals($funname,$argname) $argvalue
                set nextone [expr {$eltindex + 1}]
                if {$nextone >= [$leftwin size]} {
                    set nextone 0
                }
                set selitem $nextone
            } elseif {$calledfromwatch_vars} {
                # the proc was called from the variables area of the watch window
                if {$argvalue == ""} {set argvalue $unklabel}
                if {$getvaluefromscilab == 1} {set forceget "true"}
                set watchvarsprops($argname,value) $argvalue
                set watchvarsprops($argname,tysi) $unklabel
                set watchvarsprops($argname,editable) true
                if {$forceget == "true"} {
                    getonewatchvarfromshell $argname
                    set fullcomm "TCL_EvalStr(\"updatewatch_bp\",\"scipad\");"
                    ScilabEval_lt $fullcomm "seq"
                    set argvalue $watchvarsprops($argname,value)
                }
                set selitem $eltindex
            } else {
                #the proc was called from the expressions area of the watch window
                # nothing to do: updating the listbox below will update the listvariable
                set selitem $eltindex
            }
            $leftwin selection clear 0 end
            $leftwin selection set $selitem
            $leftwin see $selitem
            if {$rightwin ne "norightwin"} {
                $rightwin delete $eltindex
                $rightwin insert $eltindex $argvalue
            }
        }

        # 7. Perform a roundtrip to Scilab to:
        #   a. update the new value of the modified variable in Scilab, and
        #   b. get back the new values of all the watched variables from Scilab
        # This is required only for the watch window, in case the user watches a variable
        # var and elements of var such as var(i) or var(i:j) at the same time
        # The new or changed variable must first be updated in Scilab, and then the
        # watched variables must be retrieved from Scilab
        if {$mustdoScilabroundtrip} {
            setinscishellone_bp $argname  ; # update the changed var only
            getwatchvarfromshell          ; # get new value of all the watched variables
        }

    } else {
        # $argname is empty
        if {$pos == -1} {set pos 0}
        $leftwin selection clear 0 end
        $leftwin selection set $pos
    }
}

proc Canceladda_bp {w pos leftwin} {
# Close the add argument dialog and ignore the values of its fields
    destroy $w
    if {$pos == -1} {set pos 0}
    $leftwin selection clear 0 end
    $leftwin selection set $pos
}

proc Removearg_bp {leftwin {rightwin "norightwin"}} {
# Suppress a watch variable (if called from the variables area of the watch window)
# or a generic expression previously watched (if called for this area of the watch window)
# or a function argument (if called from the configure box)
# from their respective lists and update the calling dialog
    global spin
    global funvars funvarsvals
    set selecteditem [$leftwin curselection]
    foreach {calledfromwatch_vars calledfromwatch_exps calledfromconfigure} [findaddargcallingcontext $leftwin $rightwin] {}
    if {$selecteditem != ""} {
        if {$calledfromconfigure} {
            set funname [$spin get]
            set argname [$leftwin get $selecteditem]
            unset funvarsvals($funname,$argname)
            set funvars($funname) [lreplace $funvars($funname) \
                                            $selecteditem $selecteditem]
        }
        # uncheck the "watched" checkbox for all the conditional breakpoints concerned
        unwatchallcondbptexpr [$leftwin get $selecteditem]
        # thanks to the -listvariable argument used for the generic expressions listbox,
        # $watchgenexps is updated automatically by Tcl when deleting in the listbox
        $leftwin delete $selecteditem
        $leftwin see $selecteditem
        if {$rightwin ne "norightwin"} {
            $rightwin delete $selecteditem
        }
        $leftwin selection clear 0 end
        if {$selecteditem < [$leftwin size]} {
            $leftwin selection set $selecteditem
        } else {
            $leftwin selection set [expr {[$leftwin size] - 1}]
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

proc findaddargcallingcontext {leftwin rightwin} {
# determine what is the calling context of the Add argument dialog box
# based on the analysis of the widget (a listbox) name passed
# return a list of three booleans, among which at most one can be true
# in case something unexpected happens, then return {false false false}
    set calledfromwatch_vars false
    set calledfromwatch_exps false
    set calledfromconfigure  false
    if {[string first "lbvarname" $leftwin] != -1} {
        set calledfromwatch_vars true
    } elseif {[string first "genexp" $leftwin] != -1} {
        set calledfromwatch_exps true
    } elseif {[string first "listboxinput" $leftwin] != -1} {
        set calledfromconfigure true
    } else {
        tk_messageBox -message "Unexpected widget was received by proc findaddargcallingcontext: $leftwin - Please report"
    }
    if {$calledfromwatch_exps && ($rightwin ne "norightwin")} {
        tk_messageBox -message "Inconsistency between calledfromwatch_exps and rightwin in proc findaddargcallingcontext ($calledfromwatch_exps;$rightwin) - Please report"
    }
    return [list $calledfromwatch_vars $calledfromwatch_exps $calledfromconfigure]
}

proc quickAddWatch_bp {texttoadd typeofquickadd} {
# Quickly add a vatch variable
# Variable name is passed to this proc as an input
# Variable value is always got from Scilab
    global watch argname argvalue lbvarname lbvarval
    global getvaluefromscilab
    global dockwatch
    global genexpwidget
    if {![istoplevelopen watch]} {
        showwatch_bp
    } else {
        if {!$dockwatch} {
            wm deiconify $watch
        }
    }
    set argname $texttoadd
    if {$typeofquickadd eq "watchvariable"} {
        # set value to "" so that OKadda_bp will get it from the shell
        set argvalue ""
        set getvaluefromscilab 1
        OKadda_bp -1 $lbvarname $lbvarval "true"
    } else {
        # $typeofquickadd eq "genericexpression"
        OKadda_bp -1 $genexpwidget "norightwin"
    }
}

proc syncwatchvarsfromlistbox {} {
    global lbvarname lbvarval
    global watchvars watchvarsprops
    foreach var $watchvars {
        unset watchvarsprops($var,value)
    }
    set watchvars ""
    for {set i 0} {$i < [$lbvarname size]} {incr i} {
        set wvarname [$lbvarname get $i]
        set watchvars "$watchvars $wvarname"
        set wvarvalue [$lbvarval get $i]
        set watchvarsprops($wvarname,value) $wvarvalue
    }
    # <TODO>: here a slight memory leak: watchvarsprops(...,tysi & editable)
    #         are not sync'ed to the watch window content (this proc is called
    #         when closing the watch: no leak, or when removing an item: leak!)
}

proc removefuns_bp {textarea} {
    global funsinbuffer funvars funvarsvals funnames funnameargs
    if {[info exists funsinbuffer($textarea)]} {
        set oppar [expr {[string first "\(" $funnameargs] - 1}]
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
            set funname [string range $funnameargs 0 [expr {[string length $funnameargs] - 2}]]
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
    if {[isscilabbusy 5]} {return}
    showinfo $waitmessage
    if {$funnameargs != ""} {
        set commnvars [createsetinscishellcomm $var]
        set watchsetcomm [lindex $commnvars 0]
        set visibilitycomm [lindex $commnvars 2]
        if {$watchsetcomm != ""} {
            ScilabEval_lt "$visibilitycomm;$watchsetcomm" "seq"
        }
    } else {
        # <TODO> .sce case if some day the parser uses pseudocode noops
 #       ScilabEval_lt " " "seq"
    }
}
