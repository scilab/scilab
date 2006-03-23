proc configurefoo_bp {} {
    global pad conf watch
    global listboxinput listboxinputval listboxscrolly spin buttonAddc
    global funnames funvars funvarsvals funnameargs
    global menuFont textFont

    # configurefoo_bp cannot be executed since it uses Obtain_all which
    # in turn needs getallfunsintextarea
    # which needs the colorization results
    if {[colorizationinprogress]} {return}

    set conf $pad.conf
    catch {destroy $conf}
    toplevel $conf
    wm title $conf [mc "Configure execution"]

    # The configure box must be a transient of $watch if it exists
    # otherwise it might be obscured by the watch window if always on top
    if {[info exists watch]} {
        if {[winfo exists $watch]} {
            wm transient $conf $watch
        }
    }

    setwingeom $conf
    frame $conf.f

    frame $conf.f.f1
    set tl [mc "Function name:"]
    label $conf.f.f1.label -text $tl -width 20 -font $menuFont
    set spin $conf.f.f1.spinbox
    spinbox $spin -width 30 -font $textFont -command "spinboxbuttoninvoke" \
                  -values $funnames -state readonly -takefocus 0
    set oppar [string first "\(" $funnameargs]
    set funname [string range $funnameargs 0 [expr $oppar-1]]
    if {$funname != "" } {
        $spin set $funname
    } else {
        $spin set [lindex $funnames 0]
        set funname [$spin get]
    }
    set bl [mc "Obtain"]
    button $conf.f.f1.buttonObtain -text $bl -command "Obtainall_bp"\
           -width 10 -font $menuFont
    pack $conf.f.f1.label $spin $conf.f.f1.buttonObtain \
         -side left -padx 4
    pack $conf.f.f1

    frame $conf.f.f2
    frame $conf.f.f2.f2l
    set tl [mc "Input arguments:"]
    label $conf.f.f2.f2l.label -text $tl -font $menuFont
    set bl [mc "Add/Change"]
    set buttonAddc $conf.f.f2.f2l.buttonAdd
    button $buttonAddc -text $bl -width 20 -font $menuFont
    set bl [mc "Remove"]
    set buttonRemove $conf.f.f2.f2l.buttonRemove
    button $buttonRemove -text $bl -width 20 -font $menuFont
    pack $conf.f.f2.f2l.label $buttonAddc $buttonRemove -pady 4
    frame $conf.f.f2.f2r
    set listboxinput $conf.f.f2.f2r.listboxinput
    set listboxinputval $conf.f.f2.f2r.listboxinputval
    $buttonAddc configure -command {if {[$spin get] != ""} {Addarg_bp $conf $buttonAddc $listboxinput $listboxinputval}}
    $buttonRemove configure -command "Removearg_bp $listboxinput $listboxinputval"
    set listboxscrolly $conf.f.f2.f2r.yscroll
    scrollbar $listboxscrolly -command "scrollyboth_bp $listboxinput $listboxinputval"
    listbox $listboxinput -height 6 -font $textFont -yscrollcommand \
                          "scrollyrightandscrollbar_bp $listboxscrolly $listboxinput $listboxinputval" \
                          -takefocus 0
    listbox $listboxinputval -height 6 -font $textFont -yscrollcommand \
                          "scrollyleftandscrollbar_bp $listboxscrolly $listboxinput $listboxinputval" \
                          -takefocus 0
    if {[info exists funvars($funname)]} {
        foreach var $funvars($funname) {
            $listboxinput insert end $var
            $listboxinputval insert end $funvarsvals($funname,$var)
        }
        $listboxinput selection set 0
        $listboxinput see 0
    }
    pack $listboxinput $listboxscrolly $listboxinputval -side left \
            -expand 1 -fill both -padx 2
    pack $conf.f.f2.f2l $conf.f.f2.f2r -side left -padx 10
    pack $conf.f.f2 -pady 4

    frame $conf.f.f9
    button $conf.f.f9.buttonOK -text "OK" -command "OKconf_bp $conf"\
           -width 10 -height 1 -font $menuFont
#    set bl [mc "Cancel"]
#    button $conf.f.f9.buttonCancel -text $bl -command "Cancelconf_bp $conf"\
#           -width 10
#    pack $conf.f.f9.buttonOK $conf.f.f9.buttonCancel -side left -padx 10
    pack $conf.f.f9.buttonOK
    pack $conf.f.f9 -pady 4

    pack $conf.f
#    bind $conf <Return> "OKconf_bp $conf"
    bind $conf <Return> {if {[$spin get] != ""} {Addarg_bp $conf $buttonAddc $listboxinput $listboxinputval}}
    bind $listboxinput <Double-Button-1> {if {[$spin get] != ""} {Addarg_bp $conf $buttonAddc $listboxinput $listboxinputval}}
#    bind $conf <Escape> "Cancelconf_bp $conf"
    bind $conf <Escape> "OKconf_bp $conf"
    bind $conf <BackSpace> "Removearg_bp $listboxinput $listboxinputval"
    bind $conf <Delete> "Removearg_bp $listboxinput $listboxinputval"
    bind $listboxinputval <<ListboxSelect>> {selectinrightwin_bp $listboxinput $listboxinputval}
    bind $listboxinput <ButtonPress-3> {set itemindex [dragitem_bp $listboxinput %y]}
    bind $listboxinput <ButtonRelease-3> {dropitem_bp $listboxinput $listboxinputval $spin $itemindex %y}
    bind $conf <Up> {scrollarrows_bp $listboxinput up}
    bind $conf <Down> {scrollarrows_bp $listboxinput down}
    bind $conf <MouseWheel> {if {%D<0} {scrollarrows_bp $listboxinput down}\
                                       {scrollarrows_bp $listboxinput up}}
    focus $buttonAddc
    grab $conf
    if {$funnames == ""} {Obtainall_bp}
    if {$funnames == ""} {OKconf_bp $conf;showinfo [mc "Debugger is not yet developed for sce scripts!"]; # <TODO> Do it!}
}

proc scrollyboth_bp {leftwin rightwin args} {
    eval "$leftwin yview $args"
    eval "$rightwin yview $args"
}
proc scrollyleftandscrollbar_bp {lbscrolly leftwin rightwin x y} {
    eval "$lbscrolly set $x $y"
    set firstvisible [expr round([lindex [$rightwin yview] 0] \
                               * [$rightwin size])]
    eval "$leftwin yview $firstvisible"
}
proc scrollyrightandscrollbar_bp {lbscrolly leftwin rightwin x y} {
    eval "$lbscrolly set $x $y"
    set firstvisible [expr round([lindex [$leftwin yview] 0] \
                               * [$leftwin size])]
    eval "$rightwin yview $firstvisible"
}
proc selectinrightwin_bp {leftwin rightwin} {
    set selecteditem [$rightwin curselection]
    if {$selecteditem != ""} {
        $rightwin selection clear $selecteditem
        $leftwin selection set $selecteditem
    }
}
proc spinboxbuttoninvoke {} {
    global spin listboxinput listboxinputval funvars funvarsvals
    $listboxinput delete 0 [$listboxinput size]
    $listboxinputval delete 0 [$listboxinputval size]
    set funname [$spin get]
    if {[info exists funvars($funname)]} {
        foreach var $funvars($funname) {
            $listboxinput insert end $var
            $listboxinputval insert end $funvarsvals($funname,$var)
        }
    $listboxinput selection set 0
    $listboxinput see 0
    }
}

proc scrollarrows_bp {w dir} {
    set ind [$w curselection]
    if {$ind != ""} {
        if {$dir == "down"} {
            if {$ind < [expr [$w size] - 1]} {
                $w selection clear $ind
                $w selection set [expr $ind + 1]
                $w see [expr $ind + 1]
            }
        } else {
            if {$ind > 0} {
                $w selection clear $ind
                $w selection set [expr $ind - 1]
                $w see [expr $ind - 1]
            }
        }
    }
}

proc dragitem_bp {w dragpos} {
    global curdropind dragndroplb
    set se [$w curselection]
    if {$se != "" } {
        $w selection clear $se
        $w selection set @0,$dragpos
        set curdropind [$w curselection]
        set dragndroplb $w
        bind $w <Motion> {set curdropind [showdroppos_bp $dragndroplb $curdropind %y]}
    }
    return [$w curselection]
}

proc dropitem_bp {leftwin rightwin spinwidget dragind droppos} {
    global funvars watchvars curdropind
    if {$dragind != "" } {
        set dropind [$leftwin index @0,$droppos]
        set dragitem [$leftwin get $dragind]
        if {$dragind != $dropind} {
            $leftwin insert $dropind $dragitem
            $rightwin insert $dropind [$rightwin get $dragind]
            if {$spinwidget != ""} {
                set funname [$spinwidget get]
                set funvars($funname) [linsert $funvars($funname) $dropind $dragitem]
            } else {
                set watchvars [linsert $watchvars $dropind $dragitem]
            }
            if {$dropind < $dragind} {
                incr dragind
                incr curdropind
            } else {
                incr dropind -1
            }
            $leftwin delete $dragind
            $leftwin selection set $dropind
            $rightwin delete $dragind
            if {$spinwidget != ""} {
                set funvars($funname) [lreplace $funvars($funname) $dragind $dragind]
            } else {
                set watchvars [lreplace $watchvars $dragind $dragind]
            }
        }
        $leftwin itemconfigure $curdropind -background white
        bind $leftwin <Motion> {}
    }
}

proc showdroppos_bp {w oldcurdropind droppos} {
    if {$droppos < 1} {
        if {$oldcurdropind > 0} {
            $w see [expr $oldcurdropind - 1]
        }
    }
# <TODO>: $maxi computation is unperfect, but should work in most cases
# The best: to get $w widget height in pixels - how?
# winfo fpixels or winfo pixels could be a good idea...
# [winfo height $w] should do it, maybe after update has been called!
    set maxi [$w bbox $oldcurdropind]
    set maxi [expr [lindex $maxi 3] + 1]
    set maxi [expr $maxi * [$w cget -height] + 4]
    if {$droppos > $maxi} {
        if {$oldcurdropind < [expr [$w size] - 1]} {
            $w see [expr $oldcurdropind + 1]
        }
    }
    set dropind [$w index @0,$droppos]
    if {$oldcurdropind != $dropind} {
        $w itemconfigure $oldcurdropind -background white
        $w itemconfigure $dropind -background lightblue
    }
    return $dropind
}

proc OKconf_bp {w} {
    global listboxinput listboxinputval spin
    global funnameargs
    set funname [$spin get]
    if {$funname != ""} {
        foreach {parametersOK varargincase} [checkarglist $funname] {}
        if {$parametersOK == "true"} {
            set strargs ""
            for {set i 0} {$i < [$listboxinput size]} {incr i} {
                set argvalue [$listboxinputval get $i]
# <TODO> If the user configures the debugger successfully and then
#        "switches the varargin mode" by adding or removing the
#        keyword varargin as last argument of the function to debug
#        and then relaunches the debugger, the funnameargs string
#        becomes wrong - this string should be reconstructed on each
#        debug session launch, i.e. on F11, and not on closure of the
#        configure box
                if {$varargincase} {
                    # parameters are passed by value, possibly empty
                    # e.g. foo(1,2,,,9) - strargs will be 1,2,,,9
                    set strargs "$strargs,$argvalue"
                } else {
                    # parameters are passed by name, no empty value
                    # e.g. foo(c=1,a=2,k=9) - strargs will be c=1,a=2,k=9
                    if {$argvalue == ""} continue
                    set argname [$listboxinput get $i]
                    set strargs "$strargs,$argname=$argvalue"
                }
            }
            set strargs [string range $strargs 1 end]
            set funnameargs "$funname\($strargs\)"
            setdbstate "ReadyForDebug"
        } else {
            set funnameargs ""  
            setdbstate "NoDebug"
        }
    } else {       # .sce case
        set funnameargs ""
        setdbstate "NoDebug"
    }
    destroy $w
}

proc checkarglist {funname} {
# Because the user could add input variables (in the text buffer) to the
# currently selected function, checking the argument list cannot just
# rely on the latest Obtainall_bp
    global conf
    global listoftextarea funvars funvarsvals
    global funlineREpat1 funlineREpat2

    # A question mark or dollar sign in the function name must
    # be escaped otherwise the regexp compilation fails
    # (Scilab function names can contain in particular ? or $)
    set escfunname [string map {"\?" "\\\?"} $funname]
    set escfunname [string map {"\$" "\\\$"} $escfunname]

    set pat {}
    append pat $funlineREpat1 {(?:} $escfunname {)} $funlineREpat2

    set varargincase false
    set parametersOK "false"
    set found "false"
    foreach textarea $listoftextarea {
        set allfun [regexp -all -inline -indices -- $pat [$textarea get "1.0" end]]
# <TODO>: if the above returns more than one match that is not in a comment
#         nor in a string, result of checkarglist can be wrong - currently the
#         last match is used (more precisely the last match of the last buffer
#         that contains a match)
        if {$allfun == ""} {
            # have a look in another buffer
            continue
        }
        foreach fullmatch $allfun {
            foreach {i j} $fullmatch {}
            set star [$textarea index "1.0 + $i c"]
            if {[lsearch [$textarea tag names $star] "textquoted"] == -1 && \
                [lsearch [$textarea tag names $star] "rem2"] == -1 } {
                set found "true"
                set stop [$textarea index "1.0 + $j c + 1 c"]
                set funline [$textarea get $star $stop]
                set funline [trimcontandcomments $funline]
                set oppar [string first "\(" $funline]
                set clpar [string first "\)" $funline]
                set listvars [string range $funline [expr $oppar+1] [expr $clpar-1]]
                set listvars [string map {, " "} $listvars]
                set parametersOK "true"
                set orderOK "true"
                foreach {varargincase listvars} [hasvarargin $listvars] {}
                if {$varargincase} {
                    # (true) varargin case
                    # arguments order DOES matter because they will be passed
                    # by value when running a debug session
                    # compare var list of the function line in the textarea
                    # with the dialog content
                    set i 0
                    foreach var $listvars {
                        if {$var != [lindex $funvars($funname) $i]} {
                            set orderOK "false"
                            set parametersOK "false"
                            break
                        } else {
                            incr i
                        }
                    }
                } else {
                    # not in varargin case
                    # arguments order DOES NOT matter because they will be passed
                    # by name when running a debug session
                    # compare var list of the dialog content with the function
                    # line in the textarea
                    foreach var $funvars($funname) {
                        if {[lsearch -exact $listvars $var] == -1} {
                            # the variable name from the dialog does not appear in
                            # the parameters list of the function definition
                            # Scilab accepts this even outside of the varargin
                            # case but I believe it shouldn't (this is bug 1828),
                            # therefore the debugger will not run in this case
                            # moreover, giving more arguments than expected is
                            # error 58 in Scilab (varargin is not used here)
                            set parametersOK "false"
                            break
                        }
                    }
                }
            }
        }
    }
    if {$found == "false"} {
        tk_messageBox -message [concat [mc "The selected function has not been found in the opened files, maybe it was deleted since last configuration of the debugger.\nUse button \""] \
                                       [mc "Obtain"] \
                                       [mc "\" to refresh the list of available functions."] ] \
                      -parent $conf
    } else {
        if {$parametersOK != "true" } {
            if {$orderOK == "false"} {
                set mes [concat [mc "Function name or input arguments do not match definition\
                         of the function"] $funname [mc "in the file!\n\nCheck function\
                         name and arguments (names, order) in the configuration dialog.\
                         \nArguments order can be changed using drag and drop with\
                         right mouse button in the arguments listbox."] ]
                set tit [mc "Error in selected function name or arguments"]
                tk_messageBox -message $mes -icon warning -title $tit -parent $conf
            } else {
                set mes [concat [mc "At least one input argument does not match definition\
                         of the function"] $funname [mc "in the file!\n\nCheck function\
                         arguments (their names) in the configuration dialog."] ]
                set tit [mc "Error in selected function arguments"]
                tk_messageBox -message $mes -icon warning -title $tit -parent $conf
            }
        }
    }
    return [list $parametersOK $varargincase]
}

#proc Cancelconf_bp {w} {
# Better always close the window with OK button. Saves variables management.
#    destroy $w
#}

proc Obtainall_bp {} {
# Get all the functions defined in the current buffer only. Function names and
# argument names are gathered and displayed in the configure box
    global spin listboxinput listboxinputval funnames funvars funvarsvals
    global funsinbuffer
    set textarea [gettextareacur]
    set funsinbuffer($textarea) ""
    set funsinfo [lindex [getallfunsintextarea $textarea] 1]
    if {[lindex $funsinfo 0] != "0NoFunInBuf"} {
        # At least one function definition was found in the buffer
        set funtoset [lindex $funsinfo 0]
        $spin configure -state normal
        $spin delete 0 end
        set spinvalueslist ""
        $listboxinput delete 0 [$listboxinput size]
        $listboxinputval delete 0 [$listboxinputval size]
        foreach {funname funline precfun} $funsinfo {
            # if $funname starts with a question mark, this character
            # must be escaped for lsearch to work as expected (no option defaults
            # to -glob, which matches as string match which in turn uses *?[]\ as
            # special characters - only the question mark must be escaped because
            # the other chars may not appear in Scilab function names),
            # otherwise this ? matches any character
            set escfunname [string map {"\?" "\\\?"} $funname]
            if {[lsearch $spinvalueslist $escfunname] == -1} {
                set spinvalueslist "$spinvalueslist $funname"
            }
            set oppar [string first "\(" $funline]
            set clpar [string first "\)" $funline]
            set listvars [string range $funline [expr $oppar+1] [expr $clpar-1]]
            set listvars [string map {, " "} $listvars]
            foreach {varargincase listvars} [hasvarargin $listvars] {}
            foreach var $listvars {
                set funvarsvals($funname,$var) ""
            }
            set funvars($funname) $listvars
            set funsinbuffer($textarea) "$funsinbuffer($textarea) $funname"
        }
        $spin configure -values $spinvalueslist
        $spin configure -state readonly
        $spin set $funtoset
        set funnames [$spin cget -values]
    } else {
        # No function definition found in the buffer
        set funnames [$spin cget -values]
        $spin set [lindex $funnames 0]
    }
    spinboxbuttoninvoke
    $listboxinput see 0
}

proc hasvarargin {arglist} {
    if {[lindex $arglist end] == "varargin"} {
        # varargin as the last element of the input arguments
        # has a special meaning (see help varargin)
        set hasvarin true
        set arglist [lreplace $arglist end end]
        # however, if varargin also appears a second time before the last
        # argument, then this varargin is a normal argument and the last
        # varargin is ignored by Scilab
        if {[lsearch $arglist "varargin"] != -1} {
            set hasvarin false
        }
        # in any case when varargin is the last element, the returned
        # argument list has no varargin as last element
    } else {
        # either no argument has "varargin" as name, or varargin
        # is not the last argument - in this case varargin is a
        # normal variable and has no special meaning
        set hasvarin false
        # the returned argument list is what was given as input
    }
    return [list $hasvarin $arglist]
}
