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
    global bptfunsindic totbptsindic
    global dockwatch

    set watch $pad.watch
    catch {destroy $watch}
    if {$dockwatch} {
        frame $watch
    } else {
        toplevel $watch
        wm title $watch [mc "Watch"]
        if { $firsttimeinshowwatch == "true" } {
            setwingeom $watch
            wm resizable $watch 1 1
        } else {
            if {$showwatchvariablesarea == "false" && $showcallstackarea == "false"} {
                # the two statements below seem to set the geometry to what it is already,
                # at least for the position of the window - indeed it prevents the window
                # from jumping in the screen when hiding areas in the watch window
                # note: size is managed by the packer, nothing more to do
                set watchgeompos [string trimleft $watchgeom 1234567890x=]
                wm geometry $watch "$watchgeompos"
                wm resizable $watch 0 0
            } else {
                wm resizable $watch 1 1
                wm minsize $watch [lindex $watchmins 0] [lindex $watchmins 1]
                wm geometry $watch $watchgeom
            }
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
    set buttonRunToReturn $watch.f.f1.f1l.runToReturn
    button $buttonRunToReturn -command "runtoreturnpoint_bp" -image [lindex $db_butimages 8] \
           -relief flat -overrelief raised
    set buttonRunToCursor $watch.f.f1.f1l.runToCursor
    button $buttonRunToCursor -command "runtocursor_bp" -image [lindex $db_butimages 9] \
           -relief flat -overrelief raised
    set buttonGoOnIgnor $watch.f.f1.f1l.goOnIgnor
    button $buttonGoOnIgnor -command "goonwo_bp" -image [lindex $db_butimages 10] \
           -relief flat -overrelief raised
    set buttonBreakDebug $watch.f.f1.f1l.breakDebug
    button $buttonBreakDebug -command "break_bp" -image [lindex $db_butimages 14] \
           -relief flat -overrelief raised
    set buttonCancelDebug $watch.f.f1.f1l.cancelDebug
    button $buttonCancelDebug -command "canceldebug_bp" -image [lindex $db_butimages 15] \
           -relief flat -overrelief raised
    pack $buttonConfigure $buttonToNextBpt $buttonStepInto \
         $buttonStepOver $buttonStepOut \
         $buttonRunToReturn $buttonRunToCursor \
         $buttonGoOnIgnor \
         $buttonBreakDebug $buttonCancelDebug \
         -padx 0 -pady 0 -side left

    frame $watch.f.f1.f1r ;# -bg orange
    set bestwidth [mcmaxra "&Always on top" \
                           "Hide watch &variables" \
                           "Hide call &stack"]
    set checkboxalwaysontop $watch.f.f1.f1r.watchalwaysontop
    eval "checkbutton $checkboxalwaysontop [bl "&Always on top"] \
            -variable watchalwaysontop \
            -command \"managewatchontop_bp\" \
            -width $bestwidth -font \[list $menuFont\] \
            -anchor w -borderwidth 1 -pady 0 "
    if {$dockwatch} {$checkboxalwaysontop configure -underline -1}
    set checkboxshowwatchvariablesarea $watch.f.f1.f1r.showwatchvariablesarea
    eval "checkbutton $checkboxshowwatchvariablesarea \
            -variable showwatchvariablesarea [bl "Hide watch &variables"] \
            -command \"closewatch_bp; showwatch_bp\" \
            -onvalue \"false\" -offvalue \"true\" \
            -width $bestwidth -font \[list $menuFont\] \
            -anchor w -borderwidth 1 -pady 0 "
    if {$dockwatch} {$checkboxshowwatchvariablesarea configure -underline -1}
    set checkboxshowcallstackarea $watch.f.f1.f1r.showcallstackarea
    eval "checkbutton $checkboxshowcallstackarea [bl "Hide call &stack"] \
            -variable showcallstackarea \
            -command \"closewatch_bp; showwatch_bp\" \
            -onvalue \"false\" -offvalue \"true\" \
            -width $bestwidth -font \[list $menuFont\] \
            -anchor w -borderwidth 1 -pady 0 "
    if {$dockwatch} {$checkboxshowcallstackarea configure -underline -1}

    pack $checkboxalwaysontop $checkboxshowwatchvariablesarea \
            $checkboxshowcallstackarea -pady 0 -anchor w
    pack $watch.f.f1.f1l $watch.f.f1.f1r -side left -padx 5 -pady 0 -anchor w
    managewatchontop_bp

    frame $watch.f.f1.f1fr ;# -bg lightblue
    set checkboxdockwatch $watch.f.f1.f1fr.dockwatch
    eval "checkbutton $checkboxdockwatch [bl "&Dock"] \
            -variable dockwatch \
            -command \"managedockwatch_bp\" \
            -onvalue \"true\" -offvalue \"false\" \
            -font \[list $menuFont\] \
            -anchor w -borderwidth 1 -pady 0 "
    if {$dockwatch} {$checkboxdockwatch configure -underline -1}
    set debugstateindicator $watch.f.f1.f1fr.debugstateindicator
    canvas $debugstateindicator -relief ridge -width 15 -height 15 -borderwidth 1
    updatedebugstateindicator_bp

    pack $checkboxdockwatch -anchor w
    pack $debugstateindicator -expand 1
    pack $watch.f.f1.f1fr -expand 1
    pack $watch.f.f1 -anchor w -expand 0 -fill both
    pack $watch.f.f1.f1fr -anchor w -expand 1 -fill both

    set watchwinicons [list "sep" "" "" "sep" $buttonConfigure "sep" $buttonToNextBpt \
                            "" $buttonRunToReturn $buttonRunToCursor $buttonGoOnIgnor \
                            "sep" "" "sep"\
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
    bind $buttonRunToReturn <Enter> {update_bubble_watch enter \
        $MenuEntryId($pad.filemenu.debug.[mcra "Run to re&turn point"]) \
        [winfo pointerxy $watch]}
    bind $buttonRunToReturn <Leave> {update_bubble_watch leave \
        $MenuEntryId($pad.filemenu.debug.[mcra "Run to re&turn point"]) \
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
    set bestwidth [mcmaxra "Watch variables:" \
                           "Add/Chan&ge" \
                           "&Remove" \
                           "A&uto (local variables)"]
    set tl [mc "Watch variables:"]
    label $watch.f.vpw.f2.f2l.label -text $tl -font $menuFont
    set buttonAddw $watch.f.vpw.f2.f2l.buttonAdd
    eval "button $buttonAddw [bl "Add/Chan&ge"] \
            -width $bestwidth -font \[list $menuFont\] "
    if {$dockwatch} {$buttonAddw configure -underline -1}
    set buttonRemove $watch.f.vpw.f2.f2l.buttonRemove
    eval "button $buttonRemove [bl "&Remove"] \
            -width $bestwidth -font \[list $menuFont\] "
    if {$dockwatch} {$buttonRemove configure -underline -1}
    set checkboxautowatchlocals $watch.f.vpw.f2.f2l.autowatchlocals
    eval "checkbutton $checkboxautowatchlocals [bl "A&uto (local variables)"] \
            -variable autowatchloc \
            -command \"manageautowatchloc_bp\" \
            -onvalue \"true\" -offvalue \"false\" \
            -width $bestwidth -font \[list $menuFont\] \
            -anchor w -borderwidth 1 -pady 0 "
    if {$dockwatch} {$checkboxautowatchlocals configure -underline -1}

    pack $watch.f.vpw.f2.f2l.label $buttonAddw $buttonRemove $checkboxautowatchlocals -pady 4
    pack $watch.f.vpw.f2.f2l -anchor n
    manageautowatchloc_bp

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
    $buttonAddw   configure -command {Addarg_bp $watch $buttonAddw $lbvarname $lbvarval}
    $buttonRemove configure -command {Removearg_bp $lbvarname $lbvarval; \
                                      syncwatchvarsfromlistbox}
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

    pack $scrollxl             -expand 0 -fill x    -side bottom
    pack $lbvarname -side top  -expand 1 -fill both -padx 2

    pack $scrolly   -side left -expand 0 -fill y    -padx 2

    pack $watch.f.vpw.f2.f2r.hpw.fl.f2 -side right -expand 0 -fill y
    pack $watch.f.vpw.f2.f2r.hpw.fl.f1 -side left  -expand 1 -fill both

    pack $scrollxr             -expand 0 -fill x    -side bottom
    pack $lbvarval  -side top  -expand 1 -fill both

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
    set csscrolly $watch.f.vpw.f6.yscroll
    text $callstackwidget -height 5 -font $textFont -state normal \
            -background gray83 -cursor xterm -yscrollcommand "$csscrolly set" \
            -wrap word
    scrollbar $csscrolly -command "$callstackwidget yview" -takefocus 0
    pack $csscrolly $callstackwidget
    pack configure $callstackwidget -fill both -expand 1
    pack configure $csscrolly       -fill y    -expand 0 -padx 2 -side right
    if {$showcallstackarea == "true"} {
        $watch.f.vpw add $watch.f.vpw.f6
    }
    $callstackwidget delete 1.0 end
    $callstackwidget insert 1.0 $callstackcontent
    updateclickablelinetag
    $callstackwidget configure -state disabled

    frame $watch.f.f9
    frame $watch.f.f9.fl
    set totbptsindic [Progress $watch.f.f9.fl.totbptsindic]
    SetProgressBarNarrow $totbptsindic
    set bptfunsindic [Progress $watch.f.f9.fl.bptfunsindic]
    SetProgressBarNarrow $bptfunsindic
    set buttonClose $watch.f.f9.buttonClose
    eval "button $buttonClose [bl "&Close"] \
            -command \"closewatch_bp\" \
            -font \[list $menuFont\] "
    if {$dockwatch} {$buttonClose configure -underline -1}
    pack $totbptsindic -expand no -fill x
    pack $bptfunsindic -expand no -fill x -pady 2
    pack $watch.f.f9.fl -side left
    pack $buttonClose -side left -padx 40
    pack configure $watch.f.f9.fl -expand yes -fill x
    updatebptcomplexityindicators_bp

    # In order to make the Close button visible at all times, it must be packed
    # first with -side bottom, and the panedwindow must be packed after it with
    # -side top. This is a feature of the pack command, it is not a bug.
    # Another option is to use pack -before or -after with -side option.
    # This way the order reported by pack slaves is correct even if the widgets
    # have been packed in the wrong order
    # See Tk bug 1217762 (resolved as invalid, btw)
    pack $watch.f.f9 -pady 2 -side bottom -fill both -expand no
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

    bind $watch <Return> {Addarg_bp $watch $buttonAddw $lbvarname $lbvarval}
    bind $lbvarname <Double-Button-1> {Addarg_bp $watch $buttonAddw $lbvarname $lbvarval}
    bind $watch <Escape>    {closewatch_bp}
    bind $watch <BackSpace> {Removearg_bp $lbvarname $lbvarval;syncwatchvarsfromlistbox}
    bind $watch <Delete>    {Removearg_bp $lbvarname $lbvarval;syncwatchvarsfromlistbox}
    bind $lbvarval <<ListboxSelect>> {selectinrightwin_bp $lbvarname $lbvarval}
    bind $lbvarname <Enter> "set go_on_update_bubble_watchvar true ; update_bubble_watchvar %W enter \[winfo pointerxy $pad\]"
    bind $lbvarname <Leave> "set go_on_update_bubble_watchvar false; update_bubble_watchvar %W leave \[list \]"
    bind $lbvarname <ButtonPress-3>   {set itemindex [dragitem_bp $lbvarname %y]}
    bind $lbvarname <ButtonRelease-3> {dropitem_bp $lbvarname $lbvarval "" $itemindex %y}
    bind $watch <Up>   {scrollarrows_bp $lbvarname up  }
    bind $watch <Down> {scrollarrows_bp $lbvarname down}
    bind $watch <Left>  {$lbvarval xview scroll -1 units}
    bind $watch <Right> {$lbvarval xview scroll  1 units}
    bind $watch <MouseWheel> {if {%D<0} {scrollarrows_bp $lbvarname down}\
                                        {scrollarrows_bp $lbvarname up}   }

    bind $callstackwidget <Double-Button-1> {openpointedstacklevel %W %x %y ; break}

    # prevent unwanted Text class bindings from triggering
    bind $callstackwidget <Button-3> {break}
    bind $callstackwidget <Shift-Button-3> {break}
    bind $callstackwidget <Control-Button-3> {break}
    bind $callstackwidget <ButtonRelease-2> {break}

    $callstackwidget tag bind clickableline <Enter> {%W configure -cursor hand2}
    $callstackwidget tag bind clickableline <Leave> {%W configure -cursor xterm}

    bind $watch <Alt-[fb $checkboxalwaysontop]>            "$checkboxalwaysontop invoke"
    bind $watch <Alt-[fb $checkboxshowwatchvariablesarea]> "$checkboxshowwatchvariablesarea invoke"
    bind $watch <Alt-[fb $checkboxshowcallstackarea]>      "$checkboxshowcallstackarea invoke"
    bind $watch <Alt-[fb $checkboxdockwatch]>              "$checkboxdockwatch invoke"
    bind $watch <Alt-[fb $buttonAddw]>                     "$buttonAddw invoke"
    bind $watch <Alt-[fb $buttonRemove]>                   "$buttonRemove invoke"
    bind $watch <Alt-[fb $checkboxautowatchlocals]>        "$checkboxautowatchlocals invoke"
    bind $watch <Alt-[fb $buttonClose]>                    "$buttonClose invoke"

    if {!$dockwatch} {
        bind $watch <Configure> { \
            if {$showwatchvariablesarea == "true" && $firsttimeinshowwatch == "false"} { \
                set watchhsashcoord [$watch.f.vpw.f2.f2r.hpw sash coord 0]; \
                set watchminw [expr {[lindex $watchminsinit 0] + [lindex $watchhsashcoord 0] - $watchhpane1mins - 4}]; \
                set watchmins [lreplace $watchmins 0 0 $watchminw]; \
                if {$showcallstackarea == "true"} { \
                    set watchvsashcoord [$watch.f.vpw sash coord 0]; \
                    set watchminh [expr {[lindex $watchminsinit 1] + [lindex $watchvsashcoord 1] - $watchvpane1mins - 4}]; \
                    set watchmins [lreplace $watchmins 1 1 $watchminh]; \
                } ; \
                wm minsize $watch [lindex $watchmins 0] [lindex $watchmins 1]; \
            }; \
            set watchgeom [wm geometry $watch]; \
        }
    }

    focus $watch
    update
    if { $firsttimeinshowwatch == "true" } { 
        if {$showwatchvariablesarea == "true"} {
            focus $buttonAddw
        }
        if {!$dockwatch} {
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
        }
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

    if {$dockwatch} {
        pack $watch -in $pad -before $pad.pw0 -side bottom -fill x
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
            updateclickablelinetag
            $callstackwidget configure -state disabled
            updatebptcomplexityindicators_bp
        }
    }
}

proc closewatch_bp {} {
    global watch
    syncwatchvarsfromlistbox
    destroy $watch
}

proc managewatchontop_bp {} {
# tell the windows manager to keep or not the watch window on top
# of the other windows
#   Windows: use wm attributes -topmost
#   Linux:   use wm transient if Tk version is before 8.5
#            use wm topmost if Tk version is 8.5 or above
# TIP #231 implements wm topmost for X11 environments and is included in
# Tk starting from Tcl/Tk8.5a4
# wm transient seems to work equally well on Windows, but since
# there is -topmost for win, let's use the latter
    global watch watchalwaysontop tcl_platform pad
    global Tk85
    if {[info exists watch]} {
        if {[$watch cget -class] == "Toplevel"} {
            # the watch window is a separate toplevel (not packed in $pad)
            # then Always on top applies to $watch
            set w $watch
        } else {
            # [$watch cget -class] == "Frame"
            # i.e. the watch window is docked (packed inside $pad)
            # then Always on top applies to $pad
            set w $pad
        }
        if {$watchalwaysontop} {
            if {$tcl_platform(platform) == "windows"} {
                wm attributes $w -topmost 1
            } else {
                # Linux case
                if {$Tk85} {
                    wm attributes $w -topmost 1
                } else {
                    # the following is minimal but could be enough
                    # more is difficult, see http://wiki.tcl.tk/3014
                    # note: catched because when $w is $pad this leads
                    # to an error
                    catch {wm transient $w $pad}
                }
            }
        } else {
            if {$tcl_platform(platform) == "windows"} {
                wm attributes $w -topmost 0
            } else {
                if {$Tk85} {
                    wm attributes $w -topmost 0
                } else {
                    wm transient $w
                }
            }
        }
    }
}

proc managedockwatch_bp {} {
# redraws the watch window in the new (docked or toplevel) state
    global watch watchalwaysontop

    set checkboxalwaysontop $watch.f.f1.f1r.watchalwaysontop

    set saved_state $watchalwaysontop

    # remove the always on top setting
    $checkboxalwaysontop deselect
    managewatchontop_bp

    # redraw the watch window with the new watchontop value
    closewatch_bp
    showwatch_bp
    managewatchontop_bp

    # set again the always on top setting if it was set before
    if {$saved_state} {
        $checkboxalwaysontop select
        managewatchontop_bp
    }
}

proc manageautowatchloc_bp {} {
# switch on or off the local variables automatic display in the watch window
    global watch autowatchloc
    global callstackfuns
    if {[info exists watch]} {
        if {[getdbstate] == "DebugInProgress"} {
            # retrieve the local variables names
            getlocalsnames
            if {$autowatchloc} {
                # add all local variables to the watch window
                set fullcomm "TCL_EvalStr(\"addlocalsinwatch\",\"scipad\");"
            } else {
                # remove all local variables to the watch window
                set fullcomm "TCL_EvalStr(\"removelocalsfromwatch\",\"scipad\");"
            }
             ScilabEval_lt $fullcomm "seq"
        } else {
            # not in a debug session - nothing to do, this will be handled
            # during the debug
        }
    }
}

proc getlocalsnames {} {
# fill in the varsforautowatchloc global array with the names of
# the local variables of function $curfun
# this is done by retrieving the result of macrovar for the current
# function, more precisely if vars=macrovar(foo), vars(1), vars(2),
# and vars(5), which are respectively the input, output, and local
# variables of the first item in $callstackfuns
# the matrix elements of each element of list varsforautowatchloc_db
# are concatenated using strcat with a space in between, so that
# $varsforautowatchloc(...) can be interpreted later as a Tcl list

    # note the complication below to provide a function (type 13)
    # and not a string to macrovar, and to let the expression be
    # evaluated at the correct time...!
    
    # Moreover, the test on the emptiness of db_curfunname allows
    # to deal easily with the case where the debugger finished
    # the debug: [lindex $callstackfuns 0] is empty and would throw
    # an error in macrovar(""). In this case no local variable is
    # to be considered and the elements of $varsforautowatchloc are
    # reset to empty lists
 
    # finally, to ease backporting, the result of TCL_EvalStr is not
    # used directly, the usual detour with TCL_GetVar is used

# this could be used on trunk only
#    set comm1 "db_curfunname = TCL_EvalStr(\"lindex \$callstackfuns 0\",\"scipad\");"
# this is compatible with both trunk and BUILD4 branches
    set comm1 "TCL_EvalStr(\"set db_curfunname_Tcl \[lindex \$callstackfuns 0\]\",\"scipad\");db_curfunname = TCL_GetVar(\"db_curfunname_Tcl\",\"scipad\");"

    set comm2 "varsforautowatchloc_db = \"macrovar(\"+db_curfunname+\")\";"
    set comm3 "if db_curfunname <> \"\" then"
    set comm4     "execstr(\"varsforautowatchloc_db=\"+varsforautowatchloc_db);"
    set comm5     "TCL_EvalStr(\"set varsforautowatchloc(in)     \"\"\"+strcat(varsforautowatchloc_db(1),\" \")+\"\"\"\",\"scipad\");"
    set comm6     "TCL_EvalStr(\"set varsforautowatchloc(out)    \"\"\"+strcat(varsforautowatchloc_db(2),\" \")+\"\"\"\",\"scipad\");"
    set comm7     "TCL_EvalStr(\"set varsforautowatchloc(locals) \"\"\"+strcat(varsforautowatchloc_db(5),\" \")+\"\"\"\",\"scipad\");"
    set comm8 "else"
    set comm9     "TCL_EvalStr(\"set varsforautowatchloc(in)     \"\"\"\"\",\"scipad\");"
    set commA     "TCL_EvalStr(\"set varsforautowatchloc(out)    \"\"\"\"\",\"scipad\");"
    set commB     "TCL_EvalStr(\"set varsforautowatchloc(locals) \"\"\"\"\",\"scipad\");"
    set commC "end;"
    set fullcomm [concat $comm1 $comm2 $comm3 $comm4 $comm5 $comm6 $comm7 $comm8 $comm9 $commA $commB $commC]
    ScilabEval_lt $fullcomm "seq"
}

proc addlocalsinwatch {} {
# add the variables whose names are stored in $varsforautowatchloc
# in the watch window
    global varsforautowatchloc
    global watchvars watchvarsvals unklabel
    foreach typ {in out locals} {
        foreach avar $varsforautowatchloc($typ) {
            # there is no point in adding the "ans" local variable,
            # which is always present in the 5th element of the macrovar output
            # on next debug step, the debugger will [ans]=resume(ans)
            # which means that a new variable "ans" will be created
            # which was not present in the debugged script - the
            # original ans is special, and cannot be watched because
            # of this
            # to let ans be watched, I could just avoid to include it
            # in the resume list in proc createsetinscishellcomm
            # however, this is not enough: if ans is added here,
            # it gets filled by the result of FormatWhereForWatch,
            # which is not the intent -> better completely forbid
            # watching ans
            if {$avar != "ans"} {
                # don't add if already present
                if {[lsearch $watchvars $avar] == -1} {
                    lappend watchvars $avar
                    set watchvarsvals($avar) $unklabel
                }
            }
        }
    }
    getfromshell
}

proc removelocalsfromwatch {} {
# remove the variables whose names are stored in $varsforautowatchloc
# from the watch window
    global varsforautowatchloc
    global watchvars watchvarsvals
    foreach typ {in out locals} {
        foreach avar $varsforautowatchloc($typ) {
            # the test below is almost always true, but might also
            # be false (e.g. for $avar == "ans")
            set avarindex [lsearch $watchvars $avar]
            if {$avarindex != -1} {
                set watchvars [lreplace $watchvars $avarindex $avarindex]
                unset watchvarsvals($avar)
            }
        }
    }
    getfromshell
}

proc addlocalvars {} {
# add the local vars in the watch window if the user
# checked the corresponding "Auto add locals" checkbox
    global autowatchloc
    if {$autowatchloc} {
        manageautowatchloc_bp
    }
}

proc removelocalvars {} {
# remove the local vars from the watch window if the user
# checked the corresponding "Auto add locals" checkbox
    global autowatchloc
    if {$autowatchloc} {
        set autowatchloc false
        manageautowatchloc_bp
        set autowatchloc true
    }
}

proc getfromshell { {startitem 3} } {
# Update the watched variables content by getting their values from Scilab
# Also update the call stack area and update the watch window display
    global watchvars callstackcontent
    foreach var $watchvars {
        getonefromshell $var
    }
    set fullcomm "TCL_EvalStr(\"set callstackcontent \"\"\"+FormatWhereForWatch($startitem)+\"\"\"\",\"scipad\");"
    ScilabEval_lt $fullcomm "seq"
    set fullcomm "TCL_EvalStr(\"updatewatch_bp\",\"scipad\");"
    ScilabEval_lt $fullcomm "seq"
}

proc getonefromshell {wvar {opt "seq"}} {
# Update one single watched variable content by getting its value from Scilab
# The watch window display is not updated
    global unklabel
    # certain special characters are allowed in Scilab names,
    # these must be escaped
    set escwvar [escapespecialchars $wvar]
    set fullcomm ""
    set comm1 "if ext_exists(\"$wvar\"),"
    set comm2 "\[db_svar,db_tysi\]=FormatStringsForWatch($wvar);"
    set comm3 "TCL_EvalStr(\"set watchvarsvals($escwvar) \"\"\"+db_svar+\"\"\"\",\"scipad\");"
    set comm4 "TCL_EvalStr(\"set watchvarstysi($escwvar) \"\"\"+db_tysi+\"\"\"\",\"scipad\");"
    set comm5 "else"
    set comm6 "TCL_EvalStr(\"set watchvarsvals($escwvar) \"\"$unklabel\"\"\",\"scipad\");"
    set comm7 "TCL_EvalStr(\"set watchvarstysi($escwvar) \"\"$unklabel\"\"\",\"scipad\");"
    set comm8 "end;"
    set fullcomm [concat $comm1 $comm2 $comm3 $comm4 $comm5 $comm6 $comm7 $comm8]
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
                set varset [concat $varset [string range $var 0 [expr {$oppar - 1}]]]
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
                set listtype [string range $watchvarsvals($var) 0 [expr {$oppar - 1}]]
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
                set start [expr {$oppar + 1}]
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
                    set curval [string range $watchvarsvals($var) $start [expr {$i - 1}]]
                    if {$curval != $unklabel} { 
                        set onecomm [duplicatechars "$var\($curind\)=$curval;" "\""]
                        set onecomm [duplicatechars $onecomm "'"]
                        set fullcomm [concat $fullcomm $onecomm]
                    }
                    set start [expr {$i + 1}]
                    set oppar [string first "\(" $var]
                    if {$oppar == -1} {
                        set varset [concat $varset $var]
                    } else {
                        set varset [concat $varset [string range $var 0 [expr {$oppar - 1}]]]
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

proc updateclickablelinetag {} {
    global callstackwidget
    global callstackfuns callstacklines 
    global errfunc errline
    if {[getdbstate] == "DebugInProgress"} {
        # there is no error displayed in the call stack area
        # the call stack area looks like this:
        #     Breakpoint called at line 4 of macro %foo
        #     %foo       called at line 4 of macro foo
        #     foo        called at line 5 of macro b_test
        #     b_test     called at line 101 of macro atest
        $callstackwidget tag add clickableline 1.0 "end - 1c"
    } else {
        # no debug in progress
        # either there was no error (the call stack area is empty), and in
        # this case $errfunc is "" and $errline is 0
        # or the debug stopped due to an error and the call stack area looks
        # like this:
        #     Error 4 -- << undefined variable : A >>
        #     at line 14 of atest
        #
        #     Scilab is back at the main level now.
        # note that only line 2 is relevant to click in this case, and that
        # $errfunc and $errline set in proc checkexecutionerror_bp contain
        # the target destination
        $callstackwidget tag remove clickableline 1.0 end
        if {$errfunc != ""} {
            # non empty call stack area
            $callstackwidget tag add clickableline 2.0 3.0
        }
    }
}

proc openpointedstacklevel {w x y} {
# display the line in the file pointed by the mouse in the call stack area
    global callstackwidget
    global callstackfuns callstacklines
    global errfunc errline
    global SELCOLOR

    # if the cursor is not a hand, then the line is not clickable
    if {[$callstackwidget cget -cursor] != "hand2"} {return}

    set indincstackwidget [$w index "@$x,$y"]
    scan $indincstackwidget "%d.%d" lin col

    if {[getdbstate] == "DebugInProgress"} {
        # there is no error displayed in the call stack area
        # the call stack area looks like this:
        #     Breakpoint called at line 4 of macro %foo
        #     %foo       called at line 4 of macro foo
        #     foo        called at line 5 of macro b_test
        #     b_test     called at line 101 of macro atest
        set nametoopen  [lindex $callstackfuns  [expr {$lin - 1}]]
        set loglinetogo [lindex $callstacklines [expr {$lin - 1}]]
    } else {
        # no debug in progress
        # either there was no error (the call stack area is empty), and in
        # this case $errfunc is "" and $errline is 0
        # or the debug stopped due to an error and the call stack area looks
        # like this:
        #     Error 4 -- << undefined variable : A >>
        #     at line 14 of atest
        #
        #     Scilab is back at the main level now.
        # note that only line 2 is relevant to click in this case, and that
        # $errfunc and $errline set in proc checkexecutionerror_bp contain
        # the target destination - however, there is no need to check that
        # {$errfunc != ""} and {$lin == 2} since this is done by the hand2
        # cursor
        set nametoopen  $errfunc
        set loglinetogo $errline
    }

    set keywtype [gettagfromkeyword $nametoopen]
    if {$keywtype != "error"} {
        doopenfunsource $keywtype $nametoopen
        # things must be done through ScilabEval "seq" here because
        # doopenfunsource opens files using ScilabEval "seq"
        ScilabEval_lt "TCL_EvalStr(\"blinkline $loglinetogo $nametoopen 1\",\"scipad\")" "seq"
    } else {
        set mes [mc "Scipad cannot find source code for the selected function.\nPlease open the adequate file manually and try again."]
        set tit [mc "Missing source code"]
        # here ScilabEval is used in order to blink the line in
        # the call stack area before the message box opening
        ScilabEval_lt "TCL_EvalStr(\"tk_messageBox -message \"\"$mes\"\" -icon warning -title \"\"$tit\"\"\",\"scipad\")" "seq"
    }

   # blink once the clicked line in the call stack area
    $w tag add clickedline "$indincstackwidget linestart" "$indincstackwidget lineend + 1c"
    set curbg [$w tag cget clickedline -background]
    $w tag configure clickedline -background $SELCOLOR
    update idletasks
    after 500
    $w tag configure clickedline -background $curbg
    $w tag delete clickedline
}

proc updatebptcomplexityindicators_bp {{NbBreakpointedMacros -1} {NbBreakpoints -1}} {
# update the indicators describing the complexity of the debug in
# terms of number of breakpoints and number of breakpointed functions
    global ScilabCodeMaxBreakpointedMacros ScilabCodeMaxBreakpoints
    global bptfunsindic totbptsindic
    global watch
    if {[info exists watch]} {
        if {[winfo exists $watch]} {
            if {$NbBreakpointedMacros == -1} {
                set NbBreakpointedMacros [countallbreakpointedmacros]
                set NbBreakpoints [countallbreakpointedlines]
            }
            SetProgress $totbptsindic $NbBreakpoints $ScilabCodeMaxBreakpoints [mc "Breakpoints"] {0.6 0.8}
            SetProgress $bptfunsindic $NbBreakpointedMacros $ScilabCodeMaxBreakpointedMacros [mc "Breakpointed functions"] {0.6 0.8}
        }
    }
}

proc duplicatechars {st ch} {
# Duplicate character $ch in input string $st
# This is used ease construction of Scilab command strings. Duplication of quotes for
# instance is done at once after the command string has been constructed.
# Warning: $ch must be a single character string (but it works also for the string "\"")
    set indquot [string first $ch $st 0]
    while {$indquot != -1} {
        set st [string replace $st $indquot $indquot "$ch$ch"]
        set indquot [string first $ch $st [expr {$indquot + 2}]]
    }
    return $st
}
