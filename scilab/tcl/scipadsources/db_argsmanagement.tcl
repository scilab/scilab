proc Addarg_bp {w leftwin rightwin} {
    global lang
    global argname argvalue
    global buttonAdd
    set pos [$leftwin curselection]
    if {$pos == ""} {set pos -1}
    set adda $w.adda
    toplevel $adda
    if {$lang == "eng"} {
        wm title $adda "Add/Change"
    } else {
        wm title $adda "Ajouter/Modifier"
    }
    setwingeom $adda
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
    if {$lang == "eng"} {
        set tl "Variable:"
    } else {
        set tl "Variable :"
    }
    label $adda.f.f1.label -text $tl -width 10
    entry $adda.f.f1.entry  -textvariable argname -width 20 
    pack $adda.f.f1.label $adda.f.f1.entry -side left
    $adda.f.f1.entry selection range 0 end
    pack $adda.f.f1
    frame $adda.f.f2
    if {$lang == "eng"} {
        set tl "Value:"
    } else {
        set tl "Valeur :"
    }
    label $adda.f.f2.label -text $tl -width 10
    entry $adda.f.f2.entry  -textvariable argvalue -width 20 
    pack $adda.f.f2.label $adda.f.f2.entry -side left
    $adda.f.f2.entry selection range 0 end
    pack $adda.f.f2
    frame $adda.f.f9
    button $adda.f.f9.buttonOK -text "OK" -command "OKadda_bp $adda $pos $leftwin $rightwin"\
           -width 10 -height 1 -underline 0
    if {$lang == "eng"} {
        set bl "Cancel"
    } else {
        set bl "Annuler"
    }
    button $adda.f.f9.buttonCancel -text $bl -command "Canceladda_bp $adda $pos $leftwin"\
           -width 10 -underline 0
    pack $adda.f.f9.buttonOK $adda.f.f9.buttonCancel -side left -padx 10
    pack $adda.f.f9 -pady 4
    pack $adda.f
    bind $adda <Return> "OKadda_bp $adda $pos $leftwin $rightwin"
    bind $adda <Escape> "Canceladda_bp $adda $pos $leftwin"
    if {$selecteditem != ""} {
        focus $adda.f.f2.entry
    } else {
        focus $adda.f.f1.entry
    }
    grab $adda
    focus $buttonAdd
}

proc OKadda_bp {w pos leftwin rightwin} {
    global lang unklabel
    global argname argvalue
    global spin funvars funvarsvals
    global watchvars watchvarsvals
    if {$argname != ""} {
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
        # next line is a bit dirty...
        if {[string first listboxinput $leftwin] != -1} {
            set funname [$spin get]
        }
        if {$alreadyexists == "false"} {
            set pos [expr $pos + 1]
            if {[string first listboxinput $leftwin] != -1} {
                set funvars($funname) [linsert $funvars($funname) $pos $argname]
                set funvarsvals($funname,$argname) $argvalue
            } else {
                if {$argvalue == ""} {set argvalue $unklabel}
                set watchvars [linsert $watchvars $pos $argname]
                set watchvarsvals($argname) $argvalue
            }
            $leftwin insert $pos $argname
            $rightwin insert $pos $argvalue
            $leftwin selection set $pos
            $leftwin see $pos
       } else {
            set nextone [expr $eltindex + 1]
            if {$nextone >= [$leftwin size]} {
                set nextone 0
            }
            if {[string first listboxinput $leftwin] != -1} {
                set funvarsvals($funname,$argname) $argvalue
            } else {
                if {$argvalue == ""} {set argvalue $unklabel}
                set watchvarsvals($argname) $argvalue
            }
            $leftwin selection set $nextone
            $leftwin see $nextone
            $rightwin delete $eltindex
            $rightwin insert $eltindex $argvalue
        }
    }
    destroy $w
}

proc Canceladda_bp {w pos leftwin} {
    destroy $w
    $leftwin selection set $pos
}

proc Removearg_bp {leftwin rightwin} {
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

proc removefuns_bp {textarea} {
    global funsinbuffer funvars funvarsvals funnames funnameargs
    if {[info exists funsinbuffer($textarea)]} {
        foreach fun $funsinbuffer($textarea) {
            if {[info exists funvars($fun)]} {
                foreach arg $funvars($fun) {
                    unset funvarsvals($fun,$arg)
                }
                unset funvars($fun)
            }
            set pos [lsearch $funnames $fun]
            set funnames [lreplace $funnames $pos $pos]
            set oppar [expr [string first "\(" $funnameargs] - 1]
            set curfunname [string range $funnameargs 0 $oppar]
            if {$curfunname == $fun} {
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
        }
    }
}
