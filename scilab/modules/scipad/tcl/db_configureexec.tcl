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

proc configurefoo_bp {} {
    global pad conf watch
    global listboxinput listboxinputval listboxscrolly spin buttonAddc
    global funnames funvars funvarsvals funnameargs
    global menuFont textFont
    global bug2789_fixed scipaddebuggerinnerworkingsdescriptionURL

    # the debugger does not work if bug 2789 is not fixed
    # the easiest way to unplug it from Scipad is to prevent configuration
    # of the debugger, thus preventing the debug state to go into
    # ReadyForDebug mode
    if {!$bug2789_fixed} {
        pleaseuseabetterscilabversion 2789 [list $scipaddebuggerinnerworkingsdescriptionURL]
        return
    }

    # configurefoo_bp cannot be executed when colorization is in progress
    # since it uses Obtain_all which in turn needs getallfunsintextarea
    # which needs the colorization results
    if {[colorizationinprogress]} {return}

    # configurefoo_bp cannot be executed if colorization has been turned
    # off in the current buffer
    if {[iscurrentbufnotcolorized]} {return}

    # warn the user about duplicate function names, or unterminated functions
    # possibly found in the opened buffers
    # configure won't execute in that case
    if {[checkforduplicateorunterminatedfuns]} {return}

    set conf $pad.conf
    catch {destroy $conf}
    toplevel $conf
    setscipadicon $conf
    wm title $conf [mc "Configure execution"]
    # the configure box will be deiconified later selectively,
    # depending on the type of file (.sce, .sci, mixed)
    wm withdraw $conf

    # the configure box must be a transient of $watch if it exists
    # and if it is not iconified, otherwise it might be obscured
    # by the watch window if always on top
    if {[istoplevelopen watch]} {
        if {[winfo ismapped $watch]} {
            wm transient $conf $watch
        }
    }

    frame $conf.f

    frame $conf.f.f1
    set tl [mc "Function name:"]
    label $conf.f.f1.label -text $tl -font $menuFont
    set spin $conf.f.f1.spinbox
    spinbox $spin -width 35 -font $textFont -command "spinboxbuttoninvoke" \
                  -values $funnames -state readonly -takefocus 0
    set oppar [string first "\(" $funnameargs]
    set funname [string range $funnameargs 0 [expr {$oppar - 1}]]
    if {$funname != "" } {
        $spin set $funname
    } else {
        $spin set [lindex $funnames 0]
        set funname [$spin get]
    }
    set buttonObtain $conf.f.f1.buttonObtain
    eval "button $buttonObtain [bl "&Obtain"] \
            -command \"Obtainall_bp\" \
            -font \[list $menuFont\] "
    pack $conf.f.f1.label $spin $buttonObtain \
         -side left -padx 4
    pack $conf.f.f1

    frame $conf.f.f2
    frame $conf.f.f2.f2l
    set tl [mc "Input arguments:"]
    label $conf.f.f2.f2l.label -text $tl -font $menuFont
    set buttonAddc $conf.f.f2.f2l.buttonAdd
    eval "button $buttonAddc [bl "Add/Chan&ge"] \
            -font \[list $menuFont\] "
    set buttonRemove $conf.f.f2.f2l.buttonRemove
    eval "button $buttonRemove [bl "&Remove"] \
            -font \[list $menuFont\] "
    grid $conf.f.f2.f2l.label -row 0 -column 0 -sticky we -pady 4
    grid $buttonAddc          -row 1 -column 0 -sticky we -pady 4
    grid $buttonRemove        -row 2 -column 0 -sticky we -pady 4
    frame $conf.f.f2.f2r
    set listboxinput $conf.f.f2.f2r.listboxinput
    set listboxinputval $conf.f.f2.f2r.listboxinputval
    $buttonAddc configure -command {if {[$spin get] != ""} {Addarg_bp $conf $buttonAddc $listboxinput $listboxinputval}}
    $buttonRemove configure -command "Removearg_bp $listboxinput $listboxinputval"
    set listboxscrolly $conf.f.f2.f2r.yscroll
    scrollbar $listboxscrolly -command "scrollyboth_bp $listboxinput $listboxinputval"
    listbox $listboxinput -height 6 -width 12 -font $textFont -yscrollcommand \
                          "scrollyrightandscrollbar_bp $listboxscrolly $listboxinput $listboxinputval" \
                          -takefocus 0 -exportselection 0
    listbox $listboxinputval -height 6 -width 25 -font $textFont -yscrollcommand \
                          "scrollyleftandscrollbar_bp $listboxscrolly $listboxinput $listboxinputval" \
                          -takefocus 0 -exportselection 0
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
           -font $menuFont
#    set bl [mc "Cancel"]
#    button $conf.f.f9.buttonCancel -text $bl -command "Cancelconf_bp $conf"\
#           -font $menuFont
    grid $conf.f.f9.buttonOK     -row 0 -column 0 -sticky we -padx 10
#    grid $conf.f.f9.buttonCancel -row 0 -column 1 -sticky we -padx 10
    grid columnconfigure $conf.f.f9 0 -uniform 1
#    grid columnconfigure $conf.f.f9 1 -uniform 1
    pack $conf.f.f9 -pady 4

    pack $conf.f

    update idletasks
    setwingeom $conf

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
    bind $conf <Shift-Up>   "$spin invoke buttonup"
    bind $conf <Shift-Down> "$spin invoke buttondown"

    bind $conf <Alt-[fb $buttonObtain]> "$buttonObtain invoke"
    bind $conf <Alt-[fb $buttonAddc]>   "$buttonAddc invoke"
    bind $conf <Alt-[fb $buttonRemove]> "$buttonRemove invoke"

    focus $buttonAddc
    grab $conf

    # if no function was previously selected in the dialog, automatically get the
    # list of available functions and treat the .sce case (propose to wrap the
    # file in a function)
    if {$funnames == ""} {Obtainall_bp}

    # if still no function is found here, the user must have a .sce file
    # that he refused to debug as such (otherwise there is at least one
    # function, which is the wrapper)
    # in this case, close the configure box and... that's all, folks!
    if {$funnames == ""} {
        # there is one case where $funnames is "" but the $conf dialog has
        # already been closed:
        # when the exec of the temp buffer containing all the non level zero
        # code produces an error, the cleanup of the buffer has already been
        # done at this point in proc canceldebug_bp that has been called by
        # proc scilaberror from execfile from execfile_bp
        if {[winfo exists $conf]} {
            OKconf_bp $conf
        } else {
            # nothing more to do, the dialog is closed and there was
            # an error during execfile
        }
    } else {
        # a function to debug exists, either a "real" function, or the
        # wrapper created to debug .sce or mixed files
        if {[winfo exists $conf]} {
            # this is a "real" function - the configure box has
            # not been closed, so show it to the user
            wm deiconify $conf
        } else {
            # the function is the wrapper - this must not be shown to
            # the user, and anyway the dialog has already been closed
            # at the end of proc OKconf_bp called by proc Obtainall_bp
            # nothing more to do here
        }
    }
}

proc scrollyboth_bp {leftwin rightwin args} {
    eval "$leftwin yview $args"
    eval "$rightwin yview $args"
}
proc scrollyleftandscrollbar_bp {lbscrolly leftwin rightwin x y} {
    eval "$lbscrolly set $x $y"
    set firstvisible [expr {round([lindex [$rightwin yview] 0] \
                               * [$rightwin size]) } ]
    eval "$leftwin yview $firstvisible"
}
proc scrollyrightandscrollbar_bp {lbscrolly leftwin rightwin x y} {
    eval "$lbscrolly set $x $y"
    set firstvisible [expr {round([lindex [$leftwin yview] 0] \
                               * [$leftwin size]) } ]
    eval "$rightwin yview $firstvisible"
}
proc selectinrightwin_bp {leftwin rightwin} {
    $leftwin selection clear 0 end
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
            if {$ind < [expr {[$w size] - 1}]} {
                $w selection clear $ind
                $w selection set [expr {$ind + 1}]
                $w see [expr {$ind + 1}]
            }
        } else {
            if {$ind > 0} {
                $w selection clear $ind
                $w selection set [expr {$ind - 1}]
                $w see [expr {$ind - 1}]
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
            $w see [expr {$oldcurdropind - 1}]
        }
    }
# <TODO>: $maxi computation is unperfect, but should work in most cases
# The best: to get $w widget height in pixels - how?
# winfo fpixels or winfo pixels could be a good idea...
# [winfo height $w] should do it, maybe after update has been called!
    set maxi [$w bbox $oldcurdropind]
    set maxi [expr {[lindex $maxi 3] + 1}]
    set maxi [expr {$maxi * [$w cget -height] + 4}]
    if {$droppos > $maxi} {
        if {$oldcurdropind < [expr {[$w size] - 1}]} {
            $w see [expr {$oldcurdropind + 1}]
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
#        configure box ...hmmm, to be thought further. What if the
#        user changes something else in the function definition line
#        that was used for debugger configuration, e.g. changes the name
#        of the function? $funnameargs becomes also wrong. The real
#        problem would come when the user changes a variable name for
#        instance since what was configured in the configure box and
#        what the function to debug needs as inputs is no longer
#        consistent...
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
    } else {
        # .sce case
        # with the wrapper implementation, this does only happen
        # when the user refused to debug its .sce file as 
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
    foreach textarea [filteroutpeers $listoftextarea] {
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
                set listvars [string range $funline [expr {$oppar + 1}] [expr {$clpar - 1}]]
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
        set tit [mc "Error in selected function name or arguments"]
        tk_messageBox -message [concat [mc "The selected function has not been found in the opened files, maybe it was deleted since last configuration of the debugger.\nUse button \""] \
                                       [mc "Obtain"] \
                                       [mc "\" to refresh the list of available functions."] ] \
                      -icon warning -title $tit -parent $conf
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
# <TODO> what's happening if the user closes the configure dialog with the upper-right red cross? It's not forbidden!
#    destroy $w
#}

proc Obtainall_bp {} {
# Get all the functions defined in the current buffer only. Function names and
# argument names are gathered and displayed in the configure box
    global spin listboxinput listboxinputval funnames funvars funvarsvals
    global funsinbuffer
    global debugassce pad conf
    global debugger_fun_ancillaries

    set textarea [gettextareacur]
    set funsinbuffer($textarea) ""
    set funsinfo [lindex [getallfunsintextarea $textarea] 1]

    # the debugger ancillaries cannot be debugged and shouldn't be configured
    # for debug, therefore remove them from the list of functions found
    for {set i [expr {[llength $funsinfo] - 3}]} {$i >= 0} {incr i -3} {
        set funname [lindex $funsinfo $i]
        if {[lsearch -exact $debugger_fun_ancillaries $funname] != -1} {
            set funsinfo [lreplace $funsinfo $i [expr {$i + 2}]]
        }
    }

    # $debugger_fun_ancillaries is supposed to be maintained in sync with the
    # Scipad ancillary files, which cannot be debugged, therefore when no
    # function remains in $funsinfo after the ancillaries filtering above,
    # it means that the current buffer is indeed a Scipad ancillary file
    if {[llength $funsinfo] == 0} {
        tk_messageBox -icon warning -type ok -title [mc "Scipad ancillary file"] \
                -parent $conf \
                -message [append messagestring \
                    [mc "The current file contains only Scipad ancillaries that cannot be debugged in Scipad."] "\n\n" \
                    [mc "Reserved function names are:\n"] \
                    [string map {" " ", "} $debugger_fun_ancillaries] ".\n\n" \
                    [mc "See help scipad for further details."] ]
        set debugassce false
        return
    }

    if {[lindex $funsinfo 0] != "0NoFunInBuf"} {
        # At least one function definition was found in the buffer
        if {[bufferhaslevelzerocode $textarea]} {
            # Mixed .sce/.sci
            set answ [tk_messageBox -icon question -type yesno -parent $conf \
                    -title [mc "Main level code found"] \
                    -message [mc "This file contains main level code.\nDebug as a single .sce file?"] ]
            switch -- $answ {
                yes {set treatassce true }
                no  {set treatassce false}
            }
        } else {
            # Pure .sci file (or endfunction missing, see proc bufferhaslevelzerocode)
            set treatassce false
        }
    } else {
        # No function definition found in the buffer
        # Pure .sce file
        set answ [tk_messageBox -icon question -type yesno -parent $conf \
                -title [mc "No function found"] \
                -message [concat [mc "This file contains no function and will be treated"] \
                                 [mc "as a single .sce file.\nStart debugging?"] ] ]
        switch -- $answ {
            yes {set treatassce true}
            no  {set debugassce false ; return}
        }
    }

    if {!$treatassce} {
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
            set listvars [string range $funline [expr {$oppar + 1}] [expr {$clpar - 1}]]
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
        spinboxbuttoninvoke
        $listboxinput see 0
        set debugassce false

    } else {
        # not a pure .sci file
        # wrap in a function, and run the game again

        # wrapper instructions insertion cannot be undone
        # and must not change the modified flag
        $textarea configure -undo 0
        set mflag [ismodified $textarea]

        # add function header
        set txt "function db_wrapper_"
        set taid [gettaidfromwidgetname $textarea]
        append txt $taid "_db()\n"
        $textarea mark set insert 1.0
        puttext $textarea $txt "forceinsert"
        $textarea tag add db_wrapper 1.0 insert

        # add function return instructions
        # note: if these instructions are changed then the number of lines
        # to adjust $lastlogicalline in proc getlogicallinenumbersranges
        # could need to be updated too
        # the leading \n is very important to avoid concatenation of text from
        # the debugged file with this wrapper text. See also proc runtoreturnpoint_bp
        # about the importance of the leading \n
        set txt "\ndb_nam=who(\"local\");db_nam=strcat(db_nam(1:size(db_nam,1)-predef()),\",\")\n"
        append txt "execstr(\"\[\" + db_nam + \"\]=resume(\" + db_nam + \")\")\n"
        # the trailing \n below is also very important and related to
        # adjustment of $lastlogicalline in proc getlogicallinenumbersranges
        append txt "endfunction\n"
        $textarea mark set insert end
        set oldinsert [$textarea index insert]
        puttext $textarea $txt "forceinsert"
        $textarea tag add db_wrapper $oldinsert insert

        # restore the undo capability in the buffer
        $textarea configure -undo 1
        if {!$mflag} {
            # catched to avoid errors when quickly closing Scipad during debug
            catch {after idle [list resetmodified $textarea "clearundoredostacks"]}
        }

        $textarea tag configure db_wrapper -background gray40
        showwrappercode

        Obtainall_bp
        set debugassce true

# the wrapper code must not be elided for the debug commands to work
#        hidewrappercode
    }

    # if the file is a pure or mixed .sce that the user agreed to debug as
    # a .sce file, the wrapper function has already been inserted and there
    # is nothing more to configure (no input argument), therefore simply
    # close the dialog and start debugging
    if {$debugassce} {OKconf_bp $conf;stepbystepover_bp}
}

proc showwrappercode {} {
    global listoftextarea
    foreach w [filteroutpeers $listoftextarea] {
        $w tag configure db_wrapper -elide false
    }
}

proc hidewrappercode {} {
    global listoftextarea
    foreach w [filteroutpeers $listoftextarea] {
        $w tag configure db_wrapper -elide true
    }
}

proc scedebugcleanup_bp {} {
# suppress wrapper data added in the buffer by proc Obtainall_bp
# in order to debug a .sce file, and clean the content of the
# configure box so that the next F10 will not show remaining
# db_wrapper_... functions
# this proc actually deletes any text tagged as db_wrapper from
# all the textareas
    global listoftextarea debugassce
    global funnames funnameargs logicallinenumbersranges

    # if the debug in progress was not a .sce debug, do nothing
    if {!$debugassce} {return}

    # remove wrapper data previously added in the buffer
    foreach w [filteroutpeers $listoftextarea] {
        # wrapper data removal cannot be undone
        # and must not change the modified flag
        $w configure -undo 0
        set mflag [ismodified $w]

        set wrapperrange [$w tag nextrange db_wrapper "1.0"]
        while {$wrapperrange != {}} {
            $w delete [lindex $wrapperrange 0] [lindex $wrapperrange 1]
            set wrapperrange [$w tag nextrange db_wrapper "1.0"]
        }

        # restore the undo capability in the buffer
        $w configure -undo 1
        if {!$mflag} {
            catch {after idle [list resetmodified $w "clearundoredostacks"]}
        }

    }

    # reset configure box settings for the next configuration
    # because the wrapper info is removed after the debug session
    # the user cannot restart a new debug session without
    # configuring execution again
    set funnameargs ""
    set funnames ""
    set debugassce false
    # -nocomplain to prevent Tcl error when canceling the debug
    # before the first step (configure as .sce, and then immediately
    # cancel)
    unset -nocomplain logicallinenumbersranges

    setdbstate "NoDebug"

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

proc checkforduplicateorunterminatedfuns {} {
# check if the opened buffers define duplicate function names
# if it is the case, then warn the user through a message box
# and return true
# check also if the opened buffers contain unterminated functions, i.e.
# functions without an endfunction keyword
# otherwise (no duplicate found), return false

    global listoffile pad

    set allfuns [getallfunsinalltextareas]

    set listoffunnames [list ]

    foreach {textarea funsinthatta} $allfuns {
        set funsto 1.0
        foreach {funnam funlin funsta} $funsinthatta {
            if {$funnam == "0NoFunInBuf"} {
                break
            }
            if {[$textarea compare $funsta >= $funsto]} {
                set funsto [getendfunctionpos $textarea $funsta]
                if {$funsto == -1} {
                    # unterminated function (i.e. function keyword with
                    # no balanced endfunction keyword) -> warn the user and return true
                    # <TODO>: It happens in well-formed functions containing a string
                    #         containing the word "function", the string being quoted
                    #         with single quotes when these strings are not colorized
                    #         (options menu)
                    #         It happens also when debugging functions not terminated
                    #         by an endfunction
                    #         The former case should be handled (separate the strings
                    #         detection from their colorization)
                    set mes ""
                    append mes [mc "Warning!\n\n \
                            A function missing the closing \"endfunction\" has been found in the currently opened files.\n \
                            Such syntax is now obsolete.\n\n \
                            Please terminate function "] $funnam [mc " and try again."] \
                            "\n\n " [mc "Note: This message might also be triggered by a string\n \
                            quoted with single quotes and containing the word \"function\",\n \
                            when \"Colorize \'strings\'\" is unchecked in the Options menu."] "\n " \
                            [mc "This known bug can be worked around simply by checking \"Colorize \'&strings\'\"."]
                    set tit [mc "Unterminated function definition found"]
                    tk_messageBox -message $mes -icon warning -title $tit -parent $pad
                    return true
                }
                lappend listoffunnames $funnam "$listoffile(\"$textarea\",fullname)\n"
            } else {
                # this {funnam funlin funsta} item denotes a function
                # nested in another one already copied -> ignore it
            }
        }
    }

    set dupfunfilesstr ""
    foreach {funnam funfile} $listoffunnames {
        set duppos [lsearch -all -exact $listoffunnames $funnam]
        if {[llength $duppos] > 1} {
            # duplicate function name found among the opened buffers
            append dupfunfilesstr [mc "\n--> Definition of function "] $funnam [mc " found in:\n"]
            foreach dupind $duppos {
                append dupfunfilesstr [lindex $listoffunnames [expr {$dupind + 1}] ]
            }
            foreach dupind [lreverse $duppos] {
                set listoffunnames [lreplace $listoffunnames $dupind [expr {$dupind + 1}]]
            }
        }
    }

    if {$dupfunfilesstr != ""} {
        set mes ""
        append mes [mc "Warning!\n\n \
                 One or more function is defined more than once in the currently opened files.\n \
                 The debugger cannot cope with multiple copies of functions.\n \
                 The following duplicates were detected:\n"] $dupfunfilesstr
        set tit [mc "Duplicate function definitions found"]
        tk_messageBox -message $mes -icon warning -title $tit -parent $pad
        return true
    } else {
        return false
    }
}
