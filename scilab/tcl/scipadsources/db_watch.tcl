proc showwatch_bp {} {
# Create the watch window
    global pad watch
    global lbvarname lbvarval scrolly
    global watchvars watchvarsvals buttonAddw
    global firsttimeinshowwatch watchgeom watchmins watchminsinit
    global callstackwidget callstackcontent
    global watchwinicons watchwinstepicons db_butimages db_stepbutimages
    global watchalwaysontop
    global showwatchvariablesarea showcallstackarea
    global watchvpane1mins watchvpane2mins watchvsashcoord
    global watchhpane1mins watchhpane2mins watchhsashcoord
    global debugstateindicator
    global menuFont textFont
    global tcl_platform

    # Hardwired size, but how else?
    if {$tcl_platform(platform) == "unix"} {
        # 90 enough for windows, 105 for my Linux Mandrake 10.1
        set heightofwatchwithnoarea 105
    } else {
        set heightofwatchwithnoarea 90
    }
    set widthofwatchwithnoarea  587 ;# Anyway, could depend on font size, language...

    set watch $pad.watch
    catch {destroy $watch}
    toplevel $watch
    wm title $watch [mc "Watch"]

    if { $firsttimeinshowwatch == "true" } {
        setwingeom $watch
        wm resizable $watch 1 1
    } else {
        if {$showwatchvariablesarea == "false" && $showcallstackarea == "false"} {
            set watchgeompos [string trimleft $watchgeom 1234567890x=]
            wm minsize $watch $widthofwatchwithnoarea $heightofwatchwithnoarea
            wm geometry $watch "=$widthofwatchwithnoarea\x$heightofwatchwithnoarea$watchgeompos"
            wm resizable $watch 0 0
        } else {
            wm resizable $watch 1 1
            wm minsize $watch [lindex $watchmins 0] [lindex $watchmins 1]
            wm geometry $watch $watchgeom
        }
    }

    frame $watch.f

    frame $watch.f.f1 ;# -bg pink

    frame $watch.f.f1.f1l ;# -bg red
    set buttonConfigure $watch.f.f1.f1l.configure
    button $buttonConfigure -command "configurefoo_bp" -image [lindex $db_butimages 4] \
           -relief flat -overrelief raised
    set buttonToNextBpt $watch.f.f1.f1l.toNextBpt
    button $buttonToNextBpt -command "tonextbreakpoint_bp" -image [lindex $db_butimages 6] \
           -relief flat -overrelief raised
    set buttonStepInto $watch.f.f1.f1l.stepInto
    button $buttonStepInto -command "stepbystepinto_bp" -image [lindex $db_stepbutimages 1] \
           -relief flat -overrelief raised
    set buttonStepOver $watch.f.f1.f1l.stepOver
    button $buttonStepOver -command "stepbystepover_bp" -image [lindex $db_stepbutimages 2] \
           -relief flat -overrelief raised
    set buttonStepOut $watch.f.f1.f1l.stepOut
    button $buttonStepOut -command "stepbystepout_bp" -image [lindex $db_stepbutimages 3] \
           -relief flat -overrelief raised
    set buttonRunToCursor $watch.f.f1.f1l.runToCursor
    button $buttonRunToCursor -command "runtocursor_bp" -image [lindex $db_butimages 8] \
           -relief flat -overrelief raised
    set buttonGoOnIgnor $watch.f.f1.f1l.goOnIgnor
    button $buttonGoOnIgnor -command "goonwo_bp" -image [lindex $db_butimages 9] \
           -relief flat -overrelief raised
    set buttonBreakDebug $watch.f.f1.f1l.breakDebug
    button $buttonBreakDebug -command "break_bp" -image [lindex $db_butimages 13] \
           -relief flat -overrelief raised
    set buttonCancelDebug $watch.f.f1.f1l.cancelDebug
    button $buttonCancelDebug -command "canceldebug_bp" -image [lindex $db_butimages 14] \
           -relief flat -overrelief raised
    pack $buttonConfigure $buttonToNextBpt $buttonStepInto \
         $buttonStepOver $buttonStepOut \
         $buttonRunToCursor $buttonGoOnIgnor \
         $buttonBreakDebug $buttonCancelDebug \
         -padx 0 -pady 0 -side left

    frame $watch.f.f1.f1r ;# -bg orange
    set checkboxalwaysontop $watch.f.f1.f1r.watchalwaysontop
    checkbutton $checkboxalwaysontop -variable watchalwaysontop \
            -command "managewatchontop_bp" \
            -text [mc "Always on top"] -width 20 -font $menuFont \
            -anchor w -borderwidth 1 -pady 0
    set checkboxshowwatchvariablesarea $watch.f.f1.f1r.showwatchvariablesarea
    checkbutton $checkboxshowwatchvariablesarea -variable showwatchvariablesarea \
            -command "closewatch_bp $watch; showwatch_bp" \
            -onvalue "false" -offvalue "true" \
            -text [mc "Hide watch variables"] -width 20 -font $menuFont \
            -anchor w -borderwidth 1 -pady 0
    set checkboxshowcallstackarea $watch.f.f1.f1r.showcallstackarea
    checkbutton $checkboxshowcallstackarea -variable showcallstackarea \
            -command "closewatch_bp $watch; showwatch_bp" \
            -onvalue "false" -offvalue "true" \
            -text [mc "Hide call stack"] -width 20 -font $menuFont \
            -anchor w -borderwidth 1 -pady 0

    pack $checkboxalwaysontop $checkboxshowwatchvariablesarea \
            $checkboxshowcallstackarea -pady 0
    pack $watch.f.f1.f1l $watch.f.f1.f1r -side left -padx 5 -pady 0 -anchor w
    managewatchontop_bp

    frame $watch.f.f1.f1fr ;# -bg lightblue
    set debugstateindicator $watch.f.f1.f1fr.debugstateindicator
    canvas $debugstateindicator -relief ridge -width 15 -height 15 -borderwidth 1
    updatedebugstateindicator_bp

    pack $watch.f.f1.f1fr $debugstateindicator -expand 1
    pack $watch.f.f1 -anchor w -expand 0 -fill both
    pack $watch.f.f1.f1fr -anchor w -expand 1 -fill both

    set watchwinicons [list "sep" "" "" "sep" $buttonConfigure "sep" $buttonToNextBpt \
                            "" $buttonRunToCursor $buttonGoOnIgnor "sep" "" "sep"\
                            $buttonBreakDebug $buttonCancelDebug ]
    set watchwinstepicons [list "sep" $buttonStepInto $buttonStepOver $buttonStepOut]

    setdbmenuentriesstates_bp
    bind $buttonConfigure   <Enter> {update_bubble_watch enter \
        $MenuEntryId($pad.filemenu.debug.[mcra "&Configure execution..."]) \
        [winfo pointerxy $watch]}
    bind $buttonConfigure   <Leave> {update_bubble_watch leave \
        $MenuEntryId($pad.filemenu.debug.[mcra "&Configure execution..."]) \
        [winfo pointerxy $watch]}
    bind $buttonToNextBpt   <Enter> {update_bubble_watch enter \
        $MenuEntryId($pad.filemenu.debug.[mcra "Go to next b&reakpoint"]) \
        [winfo pointerxy $watch]}
    bind $buttonToNextBpt   <Leave> {update_bubble_watch leave \
        $MenuEntryId($pad.filemenu.debug.[mcra "Go to next b&reakpoint"]) \
        [winfo pointerxy $watch]}
    bind $buttonStepInto    <Enter> {update_bubble_watch_step enter \
        $MenuEntryId($pad.filemenu.debug.step.[mcra "Step &into"]) \
        [winfo pointerxy $watch]}
    bind $buttonStepInto    <Leave> {update_bubble_watch_step leave \
        $MenuEntryId($pad.filemenu.debug.step.[mcra "Step &into"]) \
        [winfo pointerxy $watch]}
    bind $buttonStepOver    <Enter> {update_bubble_watch_step enter \
        $MenuEntryId($pad.filemenu.debug.step.[mcra "Step o&ver"]) \
        [winfo pointerxy $watch]}
    bind $buttonStepOver    <Leave> {update_bubble_watch_step leave \
        $MenuEntryId($pad.filemenu.debug.step.[mcra "Step o&ver"]) \
        [winfo pointerxy $watch]}
    bind $buttonStepOut     <Enter> {update_bubble_watch_step enter \
        $MenuEntryId($pad.filemenu.debug.step.[mcra "Step &out"]) \
        [winfo pointerxy $watch]}
    bind $buttonStepOut     <Leave> {update_bubble_watch_step leave \
        $MenuEntryId($pad.filemenu.debug.step.[mcra "Step &out"]) \
        [winfo pointerxy $watch]}
    bind $buttonRunToCursor <Enter> {update_bubble_watch enter \
        $MenuEntryId($pad.filemenu.debug.[mcra "Run to c&ursor"]) \
        [winfo pointerxy $watch]}
    bind $buttonRunToCursor <Leave> {update_bubble_watch leave \
        $MenuEntryId($pad.filemenu.debug.[mcra "Run to c&ursor"]) \
        [winfo pointerxy $watch]}
    bind $buttonGoOnIgnor   <Enter> {update_bubble_watch enter \
        $MenuEntryId($pad.filemenu.debug.[mcra "G&o on ignoring any breakpoint"]) \
        [winfo pointerxy $watch]}
    bind $buttonGoOnIgnor   <Leave> {update_bubble_watch leave \
        $MenuEntryId($pad.filemenu.debug.[mcra "G&o on ignoring any breakpoint"]) \
        [winfo pointerxy $watch]}
    bind $buttonBreakDebug  <Enter> {update_bubble_watch enter \
        $MenuEntryId($pad.filemenu.debug.[mcra "&Break"]) \
        [winfo pointerxy $watch]}
    bind $buttonBreakDebug  <Leave> {update_bubble_watch leave \
        $MenuEntryId($pad.filemenu.debug.[mcra "&Break"]) \
        [winfo pointerxy $watch]}
    bind $buttonCancelDebug <Enter> {update_bubble_watch enter \
        $MenuEntryId($pad.filemenu.debug.[mcra "Cance&l debug"]) \
        [winfo pointerxy $watch]}
    bind $buttonCancelDebug <Leave> {update_bubble_watch leave \
        $MenuEntryId($pad.filemenu.debug.[mcra "Cance&l debug"]) \
        [winfo pointerxy $watch]}

    panedwindow $watch.f.vpw -orient vertical -opaqueresize true

    frame $watch.f.vpw.f2 -relief groove -borderwidth 2 -padx 2 -pady 4

    frame $watch.f.vpw.f2.f2l ;# -bg lightgrey
    set tl [mc "Watch variables:"]
    label $watch.f.vpw.f2.f2l.label -text $tl -font $menuFont
    set bl [mc "Add/Change"]
    set buttonAddw $watch.f.vpw.f2.f2l.buttonAdd
    button $buttonAddw -text $bl -width 20 -font $menuFont
    set bl [mc "Remove"]
    set buttonRemove $watch.f.vpw.f2.f2l.buttonRemove
    button $buttonRemove -text $bl -width 20 -font $menuFont

    pack $watch.f.vpw.f2.f2l.label $buttonAddw $buttonRemove -pady 4
    pack $watch.f.vpw.f2.f2l -anchor n

    frame $watch.f.vpw.f2.f2r ;# -bg peachpuff

    panedwindow $watch.f.vpw.f2.f2r.hpw -orient horizontal -opaqueresize true

    frame $watch.f.vpw.f2.f2r.hpw.fl
    frame $watch.f.vpw.f2.f2r.hpw.fl.f1
    set lbvarname $watch.f.vpw.f2.f2r.hpw.fl.f1.lbvarname
    set scrollxl  $watch.f.vpw.f2.f2r.hpw.fl.f1.xscroll
    frame $watch.f.vpw.f2.f2r.hpw.fl.f2
    set scrolly   $watch.f.vpw.f2.f2r.hpw.fl.f2.yscroll
    frame $watch.f.vpw.f2.f2r.hpw.fr
    set lbvarval  $watch.f.vpw.f2.f2r.hpw.fr.lbvarval
    set scrollxr  $watch.f.vpw.f2.f2r.hpw.fr.xscroll
    $buttonAddw   configure -command {Addarg_bp $watch $buttonAddw $lbvarname $lbvarval; \
                                      closewatch_bp $watch nodestroy}
    $buttonRemove configure -command {Removearg_bp $lbvarname $lbvarval; \
                                      closewatch_bp $watch nodestroy}
    scrollbar $scrolly  -command "scrollyboth_bp $lbvarname $lbvarval" -takefocus 0
    scrollbar $scrollxl -command "$lbvarname xview" -orient horizontal -takefocus 0
    scrollbar $scrollxr -command "$lbvarval  xview" -orient horizontal -takefocus 0
    listbox $lbvarname -height 6 -width 12 -font $textFont -yscrollcommand \
                       "scrollyrightandscrollbar_bp $scrolly $lbvarname $lbvarval" \
                       -xscrollcommand "$scrollxl set" \
                       -takefocus 0
    listbox $lbvarval  -height 6 -font $textFont -yscrollcommand \
                       "scrollyleftandscrollbar_bp $scrolly $lbvarname $lbvarval" \
                       -xscrollcommand "$scrollxr set" \
                       -takefocus 0
    if {[info exists watchvars]} {
        foreach var $watchvars {
            $lbvarname insert end $var
            $lbvarval insert end $watchvarsvals($var)
        }
    }

    pack $lbvarname -side top  -expand 1 -fill both -padx 2
    pack $scrollxl             -expand 0 -fill x

    pack $scrolly   -side left -expand 0 -fill both -padx 2

    pack $watch.f.vpw.f2.f2r.hpw.fl.f1 -side left -expand 1 -fill both
    pack $watch.f.vpw.f2.f2r.hpw.fl.f2 -side left -expand 0 -fill y

    pack $lbvarval  -side top  -expand 1 -fill both
    pack $scrollxr             -expand 0 -fill x

    $watch.f.vpw.f2.f2r.hpw add $watch.f.vpw.f2.f2r.hpw.fl
    $watch.f.vpw.f2.f2r.hpw add $watch.f.vpw.f2.f2r.hpw.fr

    pack $watch.f.vpw.f2.f2r.hpw -side left -expand 1 -fill both -padx 2
    pack $watch.f.vpw.f2.f2l $watch.f.vpw.f2.f2r -side left -padx 2
    pack $watch.f.vpw.f2.f2r -fill both -expand 1
    if {$showwatchvariablesarea == "true"} {
        $watch.f.vpw add $watch.f.vpw.f2
    }

    frame $watch.f.vpw.f6 -relief groove -borderwidth 2 -padx 2
    set csl [mc "Call stack:"]
    label $watch.f.vpw.f6.cslabel -text $csl -font $menuFont
    pack $watch.f.vpw.f6.cslabel -anchor w -pady 4
    set callstackwidget $watch.f.vpw.f6.callstack
    text $callstackwidget -height 5 -font $textFont -state normal -background gray83
    pack $callstackwidget -fill both -expand 1
    if {$showcallstackarea == "true"} {
        $watch.f.vpw add $watch.f.vpw.f6
    }
    $callstackwidget delete 1.0 end
    $callstackwidget insert 1.0 $callstackcontent
    $callstackwidget configure -state disabled

    frame $watch.f.f9
    set bl [mc "Close"]
    button $watch.f.f9.buttonClose -text $bl -command "closewatch_bp $watch"\
           -width 10 -height 1 -font $menuFont
    pack $watch.f.f9.buttonClose

    # In order to make the Close button visible at all times, it must be packed
    # first with -side bottom, and the panedwindow must be packed after it with
    # -side top. This is a feature of the pack command, it is not a bug.
    # See Tk bug 1217762 (resolved as invalid, btw)
    pack $watch.f.f9 -pady 2 -side bottom
    if {$showwatchvariablesarea == "true" || $showcallstackarea == "true"} {
        pack $watch.f.vpw -fill both -expand yes -side top
    }

    pack $watch.f -fill both -expand 1

    update
    if { $firsttimeinshowwatch != "true" && $showwatchvariablesarea == "true" && $showcallstackarea == "true"} {
        $watch.f.vpw            sash place 0 [lindex $watchvsashcoord 0] [lindex $watchvsashcoord 1]
    }
    if { $firsttimeinshowwatch != "true" && $showwatchvariablesarea == "true"} {
        $watch.f.vpw.f2.f2r.hpw sash place 0 [lindex $watchhsashcoord 0] [lindex $watchhsashcoord 1]
    }

    bind $watch <Return> {Addarg_bp $watch $buttonAddw $lbvarname $lbvarval; \
                          closewatch_bp $watch nodestroy}
    bind $lbvarname <Double-Button-1> {Addarg_bp $watch $buttonAddw $lbvarname $lbvarval; \
                                       closewatch_bp $watch nodestroy}
    bind $watch <Escape>    {closewatch_bp $watch}
    bind $watch <BackSpace> {Removearg_bp $lbvarname $lbvarval; \
                             closewatch_bp $watch nodestroy}
    bind $watch <Delete>    {Removearg_bp $lbvarname $lbvarval; \
                             closewatch_bp $watch nodestroy}
    bind $lbvarval <<ListboxSelect>>  {selectinrightwin_bp $lbvarname $lbvarval}
    bind $lbvarname <ButtonPress-3>   {set itemindex [dragitem_bp $lbvarname %y]}
    bind $lbvarname <ButtonRelease-3> {dropitem_bp $lbvarname $lbvarval "" $itemindex %y}
    bind $watch <Up>   {scrollarrows_bp $lbvarname up  }
    bind $watch <Down> {scrollarrows_bp $lbvarname down}
    bind $watch <Left>  {$lbvarval xview scroll -1 units}
    bind $watch <Right> {$lbvarval xview scroll  1 units}
    bind $watch <MouseWheel> {if {%D<0} {scrollarrows_bp $lbvarname down}\
                                        {scrollarrows_bp $lbvarname up}   }
    bind $watch <Configure> { \
        if {$showwatchvariablesarea == "true" && $firsttimeinshowwatch == "false"} { \
            set watchhsashcoord [$watch.f.vpw.f2.f2r.hpw sash coord 0]; \
            set watchminw [expr [lindex $watchminsinit 0] + [lindex $watchhsashcoord 0] - $watchhpane1mins -4]; \
            set watchmins [lreplace $watchmins 0 0 $watchminw]; \
            if {$showcallstackarea == "true"} { \
                set watchvsashcoord [$watch.f.vpw sash coord 0]; \
                set watchminh [expr [lindex $watchminsinit 1] + [lindex $watchvsashcoord 1] - $watchvpane1mins -4]; \
                set watchmins [lreplace $watchmins 1 1 $watchminh]; \
            } ; \
            wm minsize $watch [lindex $watchmins 0] [lindex $watchmins 1]; \
        }; \
        set watchgeom [wm geometry $watch]; \
    }

    update
    if { $firsttimeinshowwatch == "true" } { 
        getdebuggersciancillaries_bp
        if {$showwatchvariablesarea == "true"} {
            focus $buttonAddw
        }
        wm minsize $watch [winfo width $watch] [winfo height $watch]
        set watchgeom [wm geometry $watch]
        set watchmins [wm minsize $watch]
        set watchminsinit $watchmins
        set watchvpane1mins [winfo height $watch.f.vpw.f2]
        set watchvpane2mins [winfo height $watch.f.vpw.f6]
        set watchvsashcoord [$watch.f.vpw            sash coord 0]
        set watchhpane1mins [winfo width  $watch.f.vpw.f2.f2r.hpw.fl]
        set watchhpane2mins [winfo width  $watch.f.vpw.f2.f2r.hpw.fr]
        set watchhsashcoord [$watch.f.vpw.f2.f2r.hpw sash coord 0]
        set firsttimeinshowwatch "false"
    }

    if {$showwatchvariablesarea == "true"} {
        $watch.f.vpw paneconfigure $watch.f.vpw.f2 -minsize $watchvpane1mins
        $watch.f.vpw.f2.f2r.hpw paneconfigure $watch.f.vpw.f2.f2r.hpw.fl -minsize $watchhpane1mins
        $watch.f.vpw.f2.f2r.hpw paneconfigure $watch.f.vpw.f2.f2r.hpw.fr -minsize $watchhpane2mins
    }
    if {$showcallstackarea == "true"} {
        $watch.f.vpw paneconfigure $watch.f.vpw.f6 -minsize $watchvpane2mins
    }
}

proc updatewatch_bp {} {
# update the content of the watch window without redrawing it
    global watch watchvars lbvarname lbvarval watchvarsvals
    global callstackwidget callstackcontent
    if {[info exists watch]} {
        if {[winfo exists $watch]} {
            if {[info exists watchvars]} {
                $lbvarname delete 0 end
                $lbvarval delete 0 end
                foreach var $watchvars {
                    $lbvarname insert end $var
                    $lbvarval insert end $watchvarsvals($var)
                }
                $lbvarname selection set 0
                $lbvarname see 0
            }
            $callstackwidget configure -state normal
            $callstackwidget delete 1.0 end
            $callstackwidget insert 1.0 $callstackcontent
            $callstackwidget configure -state disabled
        }
    }
}

proc closewatch_bp {w {dest "destroy"}} {
    global lbvarname lbvarval
    global watchvars watchvarsvals
    set watchvars ""
    array set watchvarsvals {}
    for {set i 0} {$i < [$lbvarname size]} {incr i} {
        set wvarname [$lbvarname get $i]
        set watchvars "$watchvars $wvarname"
        set wvarvalue [$lbvarval get $i]
        set watchvarsvals($wvarname) $wvarvalue
    }
    if {$dest == "destroy"} {destroy $w}
}

proc getfromshell { {startitem 3} } {
# Update the watched variables content by getting their values from Scilab
# Also update the call stack area and update the watch window display
    global watchvars callstackcontent
    foreach var $watchvars {
        getonefromshell $var
    }
    set fullcomm "TCL_EvalStr(\"scipad eval {set callstackcontent \"\"\"+FormatWhereForDebugWatch($startitem)+\"\"\"}\");"
    ScilabEval_lt $fullcomm "seq"
    set fullcomm "TCL_EvalStr(\"scipad eval {updatewatch_bp}\");"
    ScilabEval_lt $fullcomm "seq"
}

proc getonefromshell {wvar {opt "seq"}} {
# Update one single watched variable content by getting its value from Scilab
# The watch window display is not updated
    global watchvars watchvarsvals unklabel
    # certain special characters are allowed in Scilab names,
    # these must be escaped
    set escwvar [escapespecialchars $wvar]
    set fullcomm ""
    set comm1 "if ext_exists(\"$wvar\"),"
    set comm2 "TCL_EvalStr(\"scipad eval {set watchvarsvals($escwvar) \"\"\"+FormatStringsForDebugWatch($wvar)+\"\"\"}\");"
    set comm3 "else"
    set comm4 "TCL_EvalStr(\"scipad eval {set watchvarsvals($escwvar) \"\"$unklabel\"\"}\");"
    set comm5 "end;"
    set fullcomm [concat $comm1 $comm2 $comm3 $comm4 $comm5]
    ScilabEval_lt $fullcomm $opt
}

proc createsetinscishellcomm {setofvars} {
# Create three command strings used outside of this proc to send to Scilab new values for variables
# Input:  a list of variable names to consider
# Output: 1. execstr("var1=var1_value;...;varN=varN_value","errcatch","m");
#         2. [var1,...,varN]=resume(var1_value,...,varN_value);
#         3. execstr("var1",...,varN","errcatch","n");
    global watchvars watchvarsvals unklabel
    set fullcomm ""
    set varset ""
    set retcomm ""
    set viscomm ""
    foreach var $setofvars {
        if {[string first $unklabel $watchvarsvals($var)] == -1} {
            # Variable is fully known and defined
            set onecomm [duplicatechars "$var=$watchvarsvals($var);" "\""]
            set onecomm [duplicatechars $onecomm "'"]
            set fullcomm [concat $fullcomm $onecomm]
            set oppar [string first "\(" $var]
            if {$oppar == -1} {
                set varset [concat $varset $var]
            } else {
                set varset [concat $varset [string range $var 0 [expr $oppar-1]]]
            }
        } else {
            if {$watchvarsvals($var) == $unklabel} {
                # Variable is fully undefined, nothing to do
            } else {
                # Variable is partially undefined (ex: certain elements of a list)
                # In this case, we're dealing with list(elt1,..,eltn,$unklabel,eltm,..,eltp)
                # and $unklabel can appear any number of times >1 in the elements list
                # Result: variable is split into:
                # $var=list();$var($curind)=elt1; and so on, forgetting the undefined elements
                # marked as $unklabel. This recreates truly undefined elements in Scilab
                set oppar [string first "\(" $watchvarsvals($var)]
                set listtype [string range $watchvarsvals($var) 0 [expr $oppar-1]]
                if {$listtype != "list"} {
                    # Undefined elements are forbidden in any variable of type different than "list"
                    tk_messageBox -message [concat \
                        [mc "Undefined elements are not legal in variable"] $var \
                        [mc ".\nThis variable will not be updated in Scilab."] ]\
                        -icon warning -type ok \
                        -title [mc "Illegal undefined element found"]
                    continue
                }
                set onecomm "$var=[string range $watchvarsvals($var) 0 $oppar]);"
                set fullcomm [concat $fullcomm $onecomm]
                set start [expr $oppar+1]
                set anotherelt "true"
                set curind 0
                while {$anotherelt == "true"} {
                    # Parse for next element
                    set i $start
                    set nestlevel 0
                    set quotenest 0
                    set curchar [string index $watchvarsvals($var) $i]
                    while { !( ($curchar == "," && $nestlevel == 0) || \
                               $nestlevel == -1 ) } {
                        if {$curchar == "\"" || $curchar == "'"} {
                            if {$quotenest == 0} {
                                incr quotenest
                            } else {
                                incr quotenest -1
                            }
                        }
                        if { ($curchar == "\(" || $curchar == "\[") && \
                              $quotenest == 0 } {incr nestlevel}
                        if { ($curchar == "\)" || $curchar == "\]") && \
                              $quotenest == 0 } {incr nestlevel -1}
                        incr i
                        set curchar [string index $watchvarsvals($var) $i]
                    }
                    if {$nestlevel == -1} {
                        # We went out of the while because $nestlevel == -1, which means
                        # that we have just reached the end of the elements list
                        set anotherelt "false"
                        incr i -1
                    }
                    incr curind
                    set curval [string range $watchvarsvals($var) $start [expr $i-1]]
                    if {$curval != $unklabel} { 
                        set onecomm [duplicatechars "$var\($curind\)=$curval;" "\""]
                        set onecomm [duplicatechars $onecomm "'"]
                        set fullcomm [concat $fullcomm $onecomm]
                    }
                    set start [expr $i+1]
                    set oppar [string first "\(" $var]
                    if {$oppar == -1} {
                        set varset [concat $varset $var]
                    } else {
                        set varset [concat $varset [string range $var 0 [expr $oppar-1]]]
                    }
                }
            }
        }
    }
    if {$fullcomm != ""} {
        set fullcomm "execstr(\"$fullcomm\",\"errcatch\",\"m\");"
        foreach var $varset {
            set retcomm "$retcomm,$var"
        }
        set retcomm [string range $retcomm 1 end] ;# remove leading comma
        # purpose of viscomm: to create a copy that is local to the current context,
        # otherwise variable visibility problems arise
        set viscomm "execstr(\"$retcomm\",\"errcatch\",\"n\");"
        set retcomm "\[$retcomm\]=resume($retcomm);"
    }
    return [list $fullcomm $retcomm $viscomm]
}

proc duplicatechars {st ch} {
# Duplicate character $ch in input string $st
# This is used ease construction of Scilab command strings. Duplication of quotes for
# instance is done at once after the command string has been constructed.
# Warning: $ch must be a single character string (but it works also for the string "\"")
    set indquot [string first $ch $st 0]
    while {$indquot != -1} {
        set st [string replace $st $indquot $indquot "$ch$ch"]
        set indquot [string first $ch $st [expr $indquot + 2]]
    }
    return $st
}

proc update_bubble_watch {type butnum mousexy} {
# Wrapper for generic_update_bubble_watch (all but step by step icons)
    global pad watchwinicons
    generic_update_bubble_watch $type $butnum $mousexy \
            $watchwinicons $pad.filemenu.debug
}

proc update_bubble_watch_step {type butnum mousexy} {
# Wrapper for generic_update_bubble_watch (step by step icons)
    global pad watchwinstepicons
    generic_update_bubble_watch $type $butnum $mousexy \
            $watchwinstepicons $pad.filemenu.debug.step
}

proc generic_update_bubble_watch {type butnum mousexy watchwiniconslist menutosearchin} {
# Manage the popup bubbles that display the name and accelerator of the watch window icons
    set butname [lindex $watchwiniconslist $butnum]
    set txt [$menutosearchin entrycget $butnum -label]
    set acc [$menutosearchin entrycget $butnum -accelerator]
    if {$acc != ""} { set txt "$txt ($acc)" }
    update_bubble $type $butname $mousexy $txt
}

proc update_bubble {type widgetname mousexy bubbletxt} {
# generic bubble window handler
    global menuFont
    set bubble $widgetname.bubble
    catch {destroy $bubble}
    if {$type=="enter"} {
        update idletasks
        after 200
        toplevel $bubble -relief solid -bg PaleGoldenrod -bd 1
        wm overrideredirect $bubble 1
        wm transient $bubble
        wm withdraw $bubble
        catch {wm attributes $bubble -topmost 1}
        label $bubble.txt -text $bubbletxt -relief flat -bd 0 \
                          -highlightthickness 0 -bg PaleGoldenrod \
                          -font $menuFont
        if {[$widgetname cget -state] == "disabled"} {
            $bubble.txt configure -state disabled
        }
        pack $bubble.txt -side left
        update idletasks
        if {![winfo exists $bubble]} {return}
        set  scrwidth  [winfo vrootwidth  .]
        set  scrheight [winfo vrootheight .]
        set  width     [winfo reqwidth  $bubble]
        set  height    [winfo reqheight $bubble]
        set x [lindex $mousexy 0]
        set y [lindex $mousexy 1]
        incr y 12
        incr x 8
        if { $x+$width > $scrwidth } {
            set x [expr {$scrwidth - $width}]
        }
        if { $y+$height > $scrheight } {
            set y [expr {$y - 12 - $height}]
        }
        wm geometry  $bubble "+$x+$y"
        update idletasks
        if {![winfo exists $bubble]} {return}
        wm deiconify $bubble
        raise $bubble
        set cmd [list destroy $bubble]
        after 1000 $cmd
    }
}
