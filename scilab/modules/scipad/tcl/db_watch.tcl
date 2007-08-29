proc showwatch_bp {} {
# Create the watch window
    global pad watch
    global lbvarname lbvarval scrolly
    global watchvars watchvarsprops buttonAddw
    global autowatchloc
    global watchgenexps
    global firsttimeinshowwatch watchgeom
    global callstackwidget callstackcontent
    global watchwinicons watchwinstepicons db_butimages db_stepbutimages
    global watchalwaysontop
    global showwatchvariablesarea showgenexparea showcallstackarea
    global watchvsashcoord1 watchvsashcoord2
    global watchhsashcoord
    global genexpwidget
    global led_debugstate led_scilabbusy
    global menuFont textFont
    global bptfunsindic totbptsindic
    global dockwatch

    set watch $pad.watch
    catch {destroy $watch}

    if {$dockwatch} {
        frame $watch
    } else {
        toplevel $watch
        wm title $watch [mc "Watch"]
        # hide the window until all elements are drawn, this avoids a flash
        # when positioning the window on screen
        wm withdraw $watch
    }

    frame $watch.f ;# -bg black

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
    set bestwidth [mcmaxra "Hide watch &variables" \
                           "Hide e&xpressions" \
                           "Hide call &stack"]
    set checkboxshowwatchvariablesarea $watch.f.f1.f1r.showwatchvariablesarea
    eval "checkbutton $checkboxshowwatchvariablesarea \
            -variable showwatchvariablesarea [bl "Hide watch &variables"] \
            -command \"closewatch_bp; showwatch_bp\" \
            -onvalue \"false\" -offvalue \"true\" \
            -width $bestwidth -font \[list $menuFont\] \
            -anchor w -borderwidth 1 -pady 0 "
    if {$dockwatch} {$checkboxshowwatchvariablesarea configure -underline -1}
    set checkboxshowgenexparea $watch.f.f1.f1r.checkboxshowgenexparea
    eval "checkbutton $checkboxshowgenexparea \
            -variable showgenexparea [bl "Hide e&xpressions"] \
            -command \"closewatch_bp; showwatch_bp\" \
            -onvalue \"false\" -offvalue \"true\" \
            -width $bestwidth -font \[list $menuFont\] \
            -anchor w -borderwidth 1 -pady 0 "
    if {$dockwatch} {$checkboxshowgenexparea configure -underline -1}
    set checkboxshowcallstackarea $watch.f.f1.f1r.showcallstackarea
    eval "checkbutton $checkboxshowcallstackarea [bl "Hide call &stack"] \
            -variable showcallstackarea \
            -command \"closewatch_bp; showwatch_bp\" \
            -onvalue \"false\" -offvalue \"true\" \
            -width $bestwidth -font \[list $menuFont\] \
            -anchor w -borderwidth 1 -pady 0 "
    if {$dockwatch} {$checkboxshowcallstackarea configure -underline -1}

    pack $checkboxshowwatchvariablesarea $checkboxshowgenexparea \
            $checkboxshowcallstackarea -pady 0 -anchor w
    pack $watch.f.f1.f1l $watch.f.f1.f1r -side left -padx 5 -pady 0 -anchor w
    managewatchontop_bp

    frame $watch.f.f1.f1fr ;# -bg lightblue
    set bestwidth [mcmaxra "Always on to&p" \
                           "&Dock"]
    set checkboxalwaysontop $watch.f.f1.f1fr.watchalwaysontop
    eval "checkbutton $checkboxalwaysontop [bl "Always on to&p"] \
            -variable watchalwaysontop \
            -command \"managewatchontop_bp\" \
            -width $bestwidth -font \[list $menuFont\] \
            -anchor w -borderwidth 1 -pady 0 "
    if {$dockwatch} {$checkboxalwaysontop configure -underline -1}
    set checkboxdockwatch $watch.f.f1.f1fr.dockwatch
    eval "checkbutton $checkboxdockwatch [bl "&Dock"] \
            -variable dockwatch \
            -command \"managedockwatch_bp\" \
            -onvalue \"true\" -offvalue \"false\" \
            -width $bestwidth -font \[list $menuFont\] \
            -anchor w -borderwidth 1 -pady 0 "
    if {$dockwatch} {$checkboxdockwatch configure -underline -1}

    pack $checkboxalwaysontop $checkboxdockwatch -anchor w
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
                           "A&uto (local variables)" \
                           "Au&to watch globals too"]
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
    set checkboxautowatchglobals $watch.f.vpw.f2.f2l.autowatchglobals
    eval "checkbutton $checkboxautowatchglobals [bl "Au&to watch globals too"] \
            -variable autowatchglo \
            -command \"manageautowatchglo_bp\" \
            -onvalue \"true\" -offvalue \"false\" \
            -width $bestwidth -font \[list $menuFont\] \
            -anchor w -borderwidth 1 -pady 0"
    if {!$autowatchloc} {
        $checkboxautowatchglobals configure -state disabled
    }
    if {$dockwatch} {$checkboxautowatchglobals configure -underline -1}

    pack $watch.f.vpw.f2.f2l.label $buttonAddw $buttonRemove \
            $checkboxautowatchlocals $checkboxautowatchglobals -pady 4
    pack $watch.f.vpw.f2.f2l -anchor n
    # this will also call proc updatewatch_bp, either rather directly, or through
    # manageautowatchloc_bp -> getautowatchnames -> addautosinwatch -> getwatchvarfromshell
    updatewatchvars

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
                       -takefocus 0 -exportselection 0
    listbox $lbvarval  -height 6 -font $textFont -yscrollcommand \
                       "scrollyleftandscrollbar_bp $scrolly $lbvarname $lbvarval" \
                       -xscrollcommand "$scrollxr set" \
                       -takefocus 0 -exportselection 0

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
    pack $watch.f.vpw.f2.f2r -fill both -expand yes
    if {$showwatchvariablesarea} {
        $watch.f.vpw add $watch.f.vpw.f2
    }

    frame $watch.f.vpw.f3 -relief groove -borderwidth 2 -padx 2 -pady 4

    frame $watch.f.vpw.f3.f3l
    set bestwidth [mcmaxra "Generic expressions:" \
                           "&Add" \
                           "Rem&ove" \
                           "&Filter errors"]
    set gel [mc "Generic expressions:"]
    label $watch.f.vpw.f3.f3l.gelabel -text $gel -font $menuFont
    pack $watch.f.vpw.f3.f3l.gelabel -anchor w -pady 4
    set buttonAddge $watch.f.vpw.f3.f3l.buttonAdd
    eval "button $buttonAddge [bl "&Add"] \
            -width $bestwidth -font \[list $menuFont\] "
    if {$dockwatch} {$buttonAddge configure -underline -1}
    set buttonRemovege $watch.f.vpw.f3.f3l.buttonRemove
    eval "button $buttonRemovege [bl "Rem&ove"] \
            -width $bestwidth -font \[list $menuFont\] "
    if {$dockwatch} {$buttonRemovege configure -underline -1}
    set checkboxfiltererrors $watch.f.vpw.f3.f3l.filtererrors
    eval "checkbutton $checkboxfiltererrors [bl "&Filter errors"] \
            -variable filtergenexperrors \
            -onvalue \"true\" -offvalue \"false\" \
            -width $bestwidth -font \[list $menuFont\] \
            -anchor w -borderwidth 1 -pady 0 "
    if {$dockwatch} {$checkboxfiltererrors configure -underline -1}
    pack $watch.f.vpw.f3.f3l.gelabel $buttonAddge $buttonRemovege \
            $checkboxfiltererrors -pady 4

    frame $watch.f.vpw.f3.f3r
    set genexpwidget $watch.f.vpw.f3.f3r.genexp
    set gescrollx $watch.f.vpw.f3.f3r.xscroll
    set gescrolly $watch.f.vpw.f3.f3r.yscroll
    $buttonAddge    configure -command "Addarg_bp $watch $buttonAddge $genexpwidget"
    $buttonRemovege configure -command "Removearg_bp $genexpwidget"
    listbox $genexpwidget -height 6 -font $textFont \
                          -xscrollcommand "$gescrollx set" \
                          -yscrollcommand "$gescrolly set" \
                          -takefocus 0 -exportselection 0 \
                          -listvariable watchgenexps
    scrollbar $gescrollx -command "$genexpwidget xview" -takefocus 0 -orient horizontal
    scrollbar $gescrolly -command "$genexpwidget yview" -takefocus 0
    pack $gescrollx -fill x -expand no -side bottom
    pack $gescrolly $genexpwidget
    pack configure $genexpwidget -fill both -expand yes
    pack configure $gescrolly    -fill y    -expand no -side right  -padx 2
    pack $watch.f.vpw.f3.f3l $watch.f.vpw.f3.f3r -side left -padx 2
    pack configure $watch.f.vpw.f3.f3r -fill both -expand yes
    pack configure $watch.f.vpw.f3.f3l -anchor n
    if {[$genexpwidget size] > 0} {
        $genexpwidget selection set 0
        $genexpwidget see 0
    }
    if {$showgenexparea} {
        $watch.f.vpw add $watch.f.vpw.f3
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
    pack configure $callstackwidget -fill both -expand yes
    pack configure $csscrolly       -fill y    -expand no  -padx 2 -side right
    if {$showcallstackarea} {
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
    frame $watch.f.f9.fc
    set led_scilabbusy $watch.f.f9.fc.ledscilabbusy
    canvas $led_scilabbusy -height [image height led_scilabbusy_ready] \
                           -width  [image width  led_scilabbusy_ready]
    $led_scilabbusy create image 0 0 -anchor nw \
            -image led_scilabbusy_ready
    set led_debugstate $watch.f.f9.fc.leddebugstate
    canvas $led_debugstate -height [image height led_debugstate_NoDebug] \
                           -width  [image width  led_debugstate_NoDebug]
    $led_debugstate create image 0 0 -anchor nw \
            -image led_debugstate_NoDebug
    set buttonClose $watch.f.f9.buttonClose
    eval "button $buttonClose [bl "&Close"] \
            -command \"closewatch_bp\" \
            -font \[list $menuFont\] "
    if {$dockwatch} {$buttonClose configure -underline -1}
    pack $totbptsindic -expand no -fill x
    pack $bptfunsindic -expand no -fill x -pady 2
    pack $led_scilabbusy $led_debugstate -side left
    pack $watch.f.f9.fl $watch.f.f9.fc -side left -padx 10
    pack $buttonClose -side left -padx 10
    pack configure $watch.f.f9.fl -expand yes -fill x
    updatebptcomplexityindicators_bp
    updatedebugstateindicator_bp

    # In order to make the Close button visible at all times, it must be packed
    # first with -side bottom, and the panedwindow must be packed after it with
    # -side top. This is a feature of the pack command, it is not a bug.
    # Another option is to use pack -before or -after with -side option.
    # This way the order reported by pack slaves is correct even if the widgets
    # have been packed in the wrong order
    # See Tk bug 1217762 (resolved as invalid, btw)
    pack $watch.f.f9 -pady 2 -side bottom -fill both -expand no
    if {[llength [$watch.f.vpw panes]] > 0} {
        pack $watch.f.vpw -fill both -expand yes -side top
    }

    pack $watch.f -fill both -expand yes

    bind $led_scilabbusy <Enter> {update_bubble enter $led_scilabbusy [winfo pointerxy $watch] \
            [mc "Scilab busy indicator"] }
    bind $led_scilabbusy <Leave> {update_bubble leave $led_scilabbusy [winfo pointerxy $watch] ""}
    bind $led_debugstate <Enter> {update_bubble enter $led_debugstate [winfo pointerxy $watch] \
            [mc "Debugger state indicator"] }
    bind $led_debugstate <Leave> {update_bubble leave $led_debugstate [winfo pointerxy $watch] ""}

    if {$showwatchvariablesarea} {
        bind $watch <Return> {Addarg_bp $watch $buttonAddw $lbvarname $lbvarval}
        bind $watch <BackSpace> {Removearg_bp $lbvarname $lbvarval;syncwatchvarsfromlistbox}
        bind $watch <Delete>    {Removearg_bp $lbvarname $lbvarval;syncwatchvarsfromlistbox}
        if {$showgenexparea} {
            bind $watch <Shift-Return> "Addarg_bp $watch $buttonAddge $genexpwidget"
            bind $watch <Shift-BackSpace> "Removearg_bp $genexpwidget"
            bind $watch <Shift-Delete>    "Removearg_bp $genexpwidget"
        }
    } elseif {$showgenexparea} {
        bind $watch <Return> "Addarg_bp $watch $buttonAddge $genexpwidget"
        bind $watch <BackSpace> "Removearg_bp $genexpwidget"
        bind $watch <Delete>    "Removearg_bp $genexpwidget"
    } else {
        bind $watch <Return> {}
        bind $watch <BackSpace> {}
        bind $watch <Delete>    {}
    }
    bind $watch <Escape>    {closewatch_bp}

    bind $lbvarname <Double-Button-1> {Addarg_bp $watch $buttonAddw $lbvarname $lbvarval}
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
                                        {scrollarrows_bp $lbvarname up}; }

    bind $genexpwidget <Double-Button-1> "Addarg_bp $watch $buttonAddge $genexpwidget"
    bind $watch <Shift-Up>    "scrollarrows_bp $genexpwidget up"
    bind $watch <Shift-Down>  "scrollarrows_bp $genexpwidget down"
    bind $watch <Shift-Left>  "$genexpwidget xview scroll -1 units"
    bind $watch <Shift-Right> "$genexpwidget xview scroll  1 units"
    bind $watch <Shift-MouseWheel> "if {%D<0} {scrollarrows_bp $genexpwidget down}\
                                              {scrollarrows_bp $genexpwidget up}; "

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
    bind $watch <Alt-[fb $checkboxshowgenexparea]>         "$checkboxshowgenexparea invoke"
    bind $watch <Alt-[fb $checkboxshowcallstackarea]>      "$checkboxshowcallstackarea invoke"
    bind $watch <Alt-[fb $checkboxdockwatch]>              "$checkboxdockwatch invoke"
    bind $watch <Alt-[fb $buttonAddw]>                     "$buttonAddw invoke"
    bind $watch <Alt-[fb $buttonRemove]>                   "$buttonRemove invoke"
    bind $watch <Alt-[fb $checkboxautowatchlocals]>        "$checkboxautowatchlocals invoke"
    bind $watch <Alt-[fb $checkboxautowatchglobals]>       "$checkboxautowatchglobals invoke"
    bind $watch <Alt-[fb $buttonAddge]>                    "$buttonAddge invoke"
    bind $watch <Alt-[fb $buttonRemovege]>                 "$buttonRemovege invoke"
    bind $watch <Alt-[fb $checkboxfiltererrors]>           "$checkboxfiltererrors invoke"
    # after 0 in the following Alt binding is mandatory for Linux only
    # This is Tk bug 1236306 (still unfixed in Tk8.4.15 and Tk 8.5a6)
    bind $watch <Alt-[fb $buttonClose]>                    "after 0 {$buttonClose invoke}"

    update
    if {!$firsttimeinshowwatch} {
        if {[llength [$watch.f.vpw panes]] >= 2} {
            $watch.f.vpw            sash place 0 [lindex $watchvsashcoord1 0] [lindex $watchvsashcoord1 1]
            if {[llength [$watch.f.vpw panes]] >= 3} {
                $watch.f.vpw        sash place 1 [lindex $watchvsashcoord2 0] [lindex $watchvsashcoord2 1]
            }
        }
        if {$showwatchvariablesarea} {
            $watch.f.vpw.f2.f2r.hpw sash place 0 [lindex $watchhsashcoord 0] [lindex $watchhsashcoord 1]
        }
    }

    if {!$dockwatch} {

        bind $watch <Configure> {if {!$firsttimeinshowwatch} {savewatchgeometry 0}}

        # to avoid seeing the window jump on opening, wm geometry must
        # be executed before wm deiconify
        update
        if {$firsttimeinshowwatch} {
            setwingeom $watch
            wm resizable $watch 1 1
        } else {
            # the two statements below seem to set the geometry to what it is already,
            # at least for the position of the window - indeed it prevents the window
            # from jumping in the screen when hiding areas in the watch window or
            # when reopening a closed watch window
            # note: size is managed by the packer, nothing more to do
            set watchgeompos [string trimleft $watchgeom 1234567890x=]
            wm geometry $watch "$watchgeompos"
            if {[llength [$watch.f.vpw panes]] == 0} {
                wm resizable $watch 0 0
            } else {
                wm resizable $watch 1 1
            }
        }
    }

    if {$dockwatch} {
        pack $watch -in $pad -before $pad.pw0 -side bottom -fill x
    } else {
        wm deiconify $watch
    }

    update
    savewatchgeometry 1

    if {$showwatchvariablesarea} {
        focus $buttonAddw
    } elseif {$showgenexparea} {
        focus $buttonAddge
    } else {
        focus $buttonClose
    }

    if {$firsttimeinshowwatch} { 
        set firsttimeinshowwatch false
    }
}

proc savewatchgeometry {wminstructionsvalid} {
# $wminstructionsvalid is used to prevent the triggering of further
# <Configure> events, which would then lead to endless execution of
# this proc
    global watch watchgeom
    global dockwatch firsttimeinshowwatch
    global showwatchvariablesarea showgenexparea showcallstackarea
    global watchhpane1mins watchhpane2mins
    global watchhsashcoord
    global watchvpane1mins watchvpane2mins watchvpane3mins
    global watchvsashcoord1 watchvsashcoord2

    if {!$dockwatch} {
        # minimum size of each pane must be saved as being the size
        # when the watch opens the first time
        if {$firsttimeinshowwatch && 1} {
            if {$showwatchvariablesarea} {
                set watchhpane1mins [winfo width  $watch.f.vpw.f2.f2r.hpw.fl]
                set watchhpane2mins [winfo width  $watch.f.vpw.f2.f2r.hpw.fr]
                set watchvpane1mins [winfo height $watch.f.vpw.f2]
           }
            if {$showgenexparea} {
                set watchvpane2mins [winfo height $watch.f.vpw.f3]
            }
            if {$showcallstackarea} {
                set watchvpane3mins [winfo height $watch.f.vpw.f6]
            }
        }

        # sash coordinates must be saved avery time it's moved, not only
        # when the watch opens the first time
        if {$showwatchvariablesarea} {
            set watchhsashcoord [$watch.f.vpw.f2.f2r.hpw sash coord 0]
        }
        if {[llength [$watch.f.vpw panes]] >= 2} {
            set watchvsashcoord1 [$watch.f.vpw sash coord 0]
            if {[llength [$watch.f.vpw panes]] >= 3} {
                set watchvsashcoord2 [$watch.f.vpw sash coord 1]
            }
        }
    }

    if {$wminstructionsvalid} {
        if {$showwatchvariablesarea} {
            $watch.f.vpw.f2.f2r.hpw paneconfigure $watch.f.vpw.f2.f2r.hpw.fl -minsize $watchhpane1mins
            $watch.f.vpw.f2.f2r.hpw paneconfigure $watch.f.vpw.f2.f2r.hpw.fr -minsize $watchhpane2mins
            $watch.f.vpw paneconfigure $watch.f.vpw.f2 -minsize $watchvpane1mins
        }
        if {$showgenexparea} {
            $watch.f.vpw paneconfigure $watch.f.vpw.f3 -minsize $watchvpane2mins
        }
        if {$showcallstackarea} {
            $watch.f.vpw paneconfigure $watch.f.vpw.f6 -minsize $watchvpane3mins
        }
    }

    if {!$dockwatch} {
        if {$wminstructionsvalid} {
            # minimum size of the watch window
            if {[llength [$watch.f.vpw panes]] > 0} {
                wm minsize $watch [winfo width $watch] [winfo height $watch]
            }
        }
        set watchgeom [wm geometry $watch]
    }
}

proc updatewatch_bp {} {
# update the content of the watch window without redrawing it
    global watch watchvars lbvarname lbvarval watchvarsprops
    global callstackwidget callstackcontent
    if {[info exists watch]} {
        if {[winfo exists $watch]} {
            if {[info exists watchvars]} {
                set curlbsel [$lbvarname curselection]
                $lbvarname delete 0 end
                $lbvarval delete 0 end
                foreach var $watchvars {
                    $lbvarname insert end $var
                    $lbvarval  insert end $watchvarsprops($var,value)
                    if {[isvareditable $var]} {
                        $lbvarname itemconfigure end -background white
                        $lbvarval  itemconfigure end -background white
                    } else {
                        if {[isglobalautowatchvar $var]} {
                            # non editable because a global
                            $lbvarname itemconfigure end -background thistle2
                            $lbvarval  itemconfigure end -background thistle2
                        } else {
                            # non editable local
                            $lbvarname itemconfigure end -background grey90
                            $lbvarval  itemconfigure end -background grey90
                        }
                    }
                    if {[hasvarchangedsincelaststop $var]} {
                        $lbvarname itemconfigure end -foreground red
                        $lbvarval  itemconfigure end -foreground red
                        $lbvarname itemconfigure end -selectforeground red
                    } else {
                        $lbvarname itemconfigure end -foreground black
                        $lbvarval  itemconfigure end -foreground black
                        $lbvarname itemconfigure end -selectforeground white
                    }
                }
                if {$curlbsel != ""} {
                    $lbvarname selection set $curlbsel
                    $lbvarname see $curlbsel
                } else {
                    $lbvarname selection set 0
                    $lbvarname see 0
                }
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

    set checkboxalwaysontop $watch.f.f1.f1fr.watchalwaysontop

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
    manageautowatchlocglo_bp loc
}

proc manageautowatchglo_bp {} {
# switch on or off the global variables automatic display in the watch window
    manageautowatchlocglo_bp glo
}

proc manageautowatchlocglo_bp {locorglo} {
# switch on or off the local and/or global variables automatic display
# in the watch window
    global watch autowatchloc autowatchglo
    global callstackfuns
    if {[info exists watch]} {
        if {[winfo exists $watch]} {
            if {$locorglo == "loc"} {
                if {$autowatchloc} {
                    $watch.f.vpw.f2.f2l.autowatchglobals configure -state normal
                } else {
                    $watch.f.vpw.f2.f2l.autowatchglobals deselect
                    $watch.f.vpw.f2.f2l.autowatchglobals configure -state disabled
                }
                set checkboxtocheck $autowatchloc
                set opt "globalstoo"
            } else {
                set checkboxtocheck $autowatchglo
                set opt "globalsonly"
            }
            if {[getdbstate] == "DebugInProgress"} {
                # retrieve the local variables names
                getautowatchnames
                if {$checkboxtocheck} {
                    # add auto watch variables to the watch window
                    set fullcomm "TCL_EvalStr(\"addautosinwatch\",\"scipad\");"
                } else {
                    # remove auto watch variables from the watch window
                    set fullcomm "TCL_EvalStr(\"removeautosfromwatch $opt\",\"scipad\");"
                }
                ScilabEval_lt $fullcomm "seq"
            } else {
                # not in a debug session - nothing to do, this will be handled
                # during the debug
            }
        }
    }
}

proc getautowatchnames {} {
# fill in the varsforautowatch global array with the names of
# the local and global variables of function $curfun
# this is done by retrieving the result of macrovar for the current
# function, more precisely if vars=macrovar(foo), vars(1), vars(2),
# vars(3) and vars(5), which are respectively the input, output,
# global and local variables of the function whose name is the first
# item from $callstackfuns
# the matrix elements of each element of list varsforautowatch_db
# are concatenated using strcat with a space in between, so that
# $varsforautowatch(...) can be interpreted later as a Tcl list

    # note the complication below to provide a function (type 13)
    # and not a string to macrovar, and to let the expression be
    # evaluated at the correct time...!

    # Moreover, the test on the emptiness of db_curfunname allows
    # to deal easily with the case where the debugger finished
    # the debug: [lindex $callstackfuns 0] is empty and would throw
    # an error in macrovar(""). In this case no local variable is
    # to be considered and the elements of $varsforautowatch are
    # reset to empty lists

    # finally, to ease backporting, the result of TCL_EvalStr is not
    # used directly, the usual detour with TCL_GetVar is used

# this could be used on trunk only
#    set comm1 "db_curfunname = TCL_EvalStr(\"lindex \$callstackfuns 0\",\"scipad\");"
# this is compatible with both trunk and BUILD4 branches
    set comm1 "TCL_EvalStr(\"set db_curfunname_Tcl \[lindex \$callstackfuns 0\]\",\"scipad\");db_curfunname = TCL_GetVar(\"db_curfunname_Tcl\",\"scipad\");"

    set comm2 "varsforautowatch_db = \"macrovar(\"+db_curfunname+\")\";"
    set comm3 "if db_curfunname <> \"\" then"
    set comm4     "execstr(\"varsforautowatch_db=\"+varsforautowatch_db);"
    set comm5     "TCL_EvalStr(\"set varsforautowatch(in)      \[escapespecialchars {\"+strcat(varsforautowatch_db(1),\" \")+\"}\]\",\"scipad\");"
    set comm6     "TCL_EvalStr(\"set varsforautowatch(out)     \[escapespecialchars {\"+strcat(varsforautowatch_db(2),\" \")+\"}\]\",\"scipad\");"
    set comm7     "TCL_EvalStr(\"set varsforautowatch(globals) \[escapespecialchars {\"+strcat(varsforautowatch_db(3),\" \")+\"}\]\",\"scipad\");"
    set comm8     "TCL_EvalStr(\"set varsforautowatch(locals)  \[escapespecialchars {\"+strcat(varsforautowatch_db(5),\" \")+\"}\]\",\"scipad\");"
    set comm9 "else"
    set commA     "TCL_EvalStr(\"set varsforautowatch(in)      \"\"\"\"\",\"scipad\");"
    set commB     "TCL_EvalStr(\"set varsforautowatch(out)     \"\"\"\"\",\"scipad\");"
    set commC     "TCL_EvalStr(\"set varsforautowatch(locals)  \"\"\"\"\",\"scipad\");"
    set commD     "TCL_EvalStr(\"set varsforautowatch(globals) \"\"\"\"\",\"scipad\");"
    set commE "end;"
    set fullcomm [concat $comm1 $comm2 $comm3 $comm4 $comm5 $comm6 $comm7 $comm8 $comm9 $commA $commB $commC $commD $commE]
    ScilabEval_lt $fullcomm "seq"
}

proc addautosinwatch {} {
# add all variables whose names are stored in all elements of the
# $varsforautowatch array in the watch window
    global varsforautowatch autowatchglo
    global watchvars watchvarsprops unklabel
    global debugger_unwatchable_vars

    set listoftypes [list in out locals]
    if {$autowatchglo} {
        lappend listoftypes globals
    }

    foreach typ $listoftypes {
        foreach avar $varsforautowatch($typ) {
            # don't add an unwatchable variable, such as "ans"
            # which is always present in the 5th element of the
            # macrovar output
            if {[lsearch -exact $debugger_unwatchable_vars $avar] == -1} {
                # don't add if already present
                if {[lsearch $watchvars $avar] == -1} {
                    lappend watchvars $avar
                    set watchvarsprops($avar,value) $unklabel
                }
            }
        }
    }

    getwatchvarfromshell
}

proc removeautosfromwatch {{whichautos all}} {
# remove all variables whose names are stored in $varsforautowatch
# from the watch window
# if $whichautos == "globalsonly" : globals
# if $whichautos == "globalstoo"  : in, out, locals, globals
# if $whichautos == "all", it depends on the "Auto (globals too)" checkbox status:
#     if $autowatchglo is true    : in, out, locals, globals
#     if $autowatchglo is false   : in, out, locals
    global varsforautowatch autowatchglo
    global watchvars watchvarsprops

    if {$whichautos != "globalsonly"} {
        set listoftypes [list in out locals]
    } else {
        set listoftypes [list ]
    }
    if {$autowatchglo || $whichautos == "globalsonly" || $whichautos == "globalstoo"} {
        lappend listoftypes globals
    }

    foreach typ $listoftypes {
        foreach avar $varsforautowatch($typ) {
            # the test below is almost always true, but might also
            # be false (e.g. for $avar == "ans")
            set avarindex [lsearch $watchvars $avar]
            if {$avarindex != -1} {
                set watchvars [lreplace $watchvars $avarindex $avarindex]
                unset watchvarsprops($avar,value)
            }
        }
    }

    getwatchvarfromshell
}

proc updatewatchvars {} {
# add the local vars in the watch window if the user
# checked the corresponding "Auto add locals" checkbox
# otherwise just update values of the existing watch variables
    global autowatchloc
    if {$autowatchloc} {
        # add locals and get locals + user variables values from the shell
        manageautowatchloc_bp
    } else {
        # get user variables values from the shell
        getwatchvarfromshell
    }
}

proc removeautovars {} {
# remove the local vars from the watch window if the user
# checked the corresponding "Auto add locals" checkbox
    global watch autowatchloc autowatchglo
    if {$autowatchloc} {
        set saved_autowatchglo $autowatchglo
        set autowatchloc false
        manageautowatchloc_bp
        set autowatchloc true
        if {[info exists watch]} {
            if {[winfo exists $watch]} {
                $watch.f.vpw.f2.f2l.autowatchglobals configure -state normal
            }
        }
        set autowatchglo $saved_autowatchglo
    }
}

proc isglobalautowatchvar {var} {
# return true if $var is a purely global auto watched variable,
# and false otherwise
    global varsforautowatch
    if {![info exists varsforautowatch]} {
        return false
    } elseif {[lsearch -exact $varsforautowatch(globals) $var] == -1} {
        set retval false
    } else {
        if {[lsearch -exact $varsforautowatch(in)     $var] == -1 && \
            [lsearch -exact $varsforautowatch(out)    $var] == -1 && \
            [lsearch -exact $varsforautowatch(locals) $var] == -1 \
            } {
            set retval true
        } else {
            set retval false
        }
    }
    return $retval
}

proc isvareditable {varname} {
# A variable is editable if it is flagged as such in FormatStringsForWatch,
# and if it's not a global variable
    global watchvarsprops
    if {$watchvarsprops($varname,editable) && ![isglobalautowatchvar $varname]} {
        return true
    } else {
        return false
    }
}

proc isunknownelementinvar {var} {
# return true if the content of watch variable $var (seen as a character string)
# contains at least an undefined element which is not in a string or matrix of strings
# return false otherwise
    global watchvarsprops unklabel
    global ssmsRE
    # get positions of matches of $unklabel against the watch variable value
    set value $watchvarsprops($var,value)
    set allunkmatches [regexp -all -inline -indices -- [escapespecialchars $unklabel] $value]
    # get positions of matches of strings and matrices of strings against the watch variable value
    set quote [list {"} {'}]
    set allstrmatches [regexp -all -inline -indices -- $ssmsRE $value]
    # if there is at least one $unklabel match that is not in a string then $var
    # contains at least one undefined element: return true in this case
    set hasanundefined false
    foreach unkmatch $allunkmatches {
        foreach {unk1 unk2} $unkmatch {}
        set isinstring false
        foreach strmatch $allstrmatches {
            foreach {str1 str2} $strmatch {}
            if {$str1 <= $unk1 && $unk2 <= $str2} {
                set isinstring true
                break
            }
        }
        if {!$isinstring} {
            set hasanundefined true
            break
        }
    }
    return $hasanundefined
}

proc savecurrentwatchvarsvalues {} {
# copy value of all the watch variables into their prevvalue
    global watchvars watchvarsprops
    foreach var $watchvars {
        set watchvarsprops($var,prevvalue) $watchvarsprops($var,value)
    }
}

proc hasvarchangedsincelaststop {var} {
# return true if previous value of $var is different from current value of $var,
# otherwise return false
# this proc also takes care of non initialized prevvalue, which happens at the
# beginning of a debug session
    global watchvarsprops
    set haschanged false
    if {[info exists watchvarsprops($var,prevvalue)]} {
        if {$watchvarsprops($var,prevvalue) ne $watchvarsprops($var,value)} {
            set haschanged true
        }
    }
    return $haschanged
}

proc getcallstackfromshell {} {
# update the call stack area with the textual call stack content retrieved
# from Scilab, and set the global variables callstackfuns and callstacklines
# (this is done in the Scilab function FormatWhereForWatch)
    set fullcomm "TCL_EvalStr(\"set callstackcontent \"\"\"+FormatWhereForWatch(3)+\"\"\"\",\"scipad\");"
    ScilabEval_lt $fullcomm "seq"
    set fullcomm "TCL_EvalStr(\"updatewatch_bp\",\"scipad\");"
    ScilabEval_lt $fullcomm "seq"
}

proc getwatchvarfromshell {} {
# Update the watched variables content by getting their values from Scilab
    global watchvars callstackcontent
    foreach var $watchvars {
        getonewatchvarfromshell $var
    }
    set fullcomm "TCL_EvalStr(\"updatewatch_bp\",\"scipad\");"
    ScilabEval_lt $fullcomm "seq"
}

proc getonewatchvarfromshell {wvar} {
# Update one single watched variable content by getting its value from Scilab
# The watch window display is not updated
    global unklabel
    # certain special characters are allowed in Scilab names,
    # these must be escaped
    set escwvar [escapespecialchars $wvar]
    set fullcomm ""
    set comm1 "if ext_exists(\"$wvar\"),"
    set comm2 "\[db_svar,db_tysi,db_edit\]=FormatStringsForWatch($wvar);"
    set comm3 "TCL_EvalStr(\"set watchvarsprops($escwvar,value) \"\"\"+db_svar+\"\"\"\",\"scipad\");"
    set comm4 "TCL_EvalStr(\"set watchvarsprops($escwvar,tysi) \"\"\"+db_tysi+\"\"\"\",\"scipad\");"
    set comm5 "TCL_EvalStr(\"set watchvarsprops($escwvar,editable) \"\"\"+db_edit+\"\"\"\",\"scipad\");"
    set comm6 "else"
    set comm7 "TCL_EvalStr(\"set watchvarsprops($escwvar,value) \"\"$unklabel\"\"\",\"scipad\");"
    set comm8 "TCL_EvalStr(\"set watchvarsprops($escwvar,tysi) \"\"$unklabel\"\"\",\"scipad\");"
    set comm9 "TCL_EvalStr(\"set watchvarsprops($escwvar,editable) true\",\"scipad\");"
    set commA "end;"
    set fullcomm [concat $comm1 $comm2 $comm3 $comm4 $comm5 $comm6 $comm7 $comm8 $comm9 $commA]
    ScilabEval_lt $fullcomm "seq"
}

proc createsetinscishellcomm {setofvars} {
# Create three command strings used outside of this proc to send to Scilab
# new values for variables
# Input:  a list of variable names to consider
# Output: 1. execstr("var1=var1_value;...;varN=varN_value","errcatch","m");
#         2. [var1,...,varN]=resume(var1_value,...,varN_value);
#         3. execstr("var1",...,varN","errcatch","n");
# A non editable variable (either because flagged as such in FormatStringsForWatch
# or because it is a global) or a variable whose value is $unklabel is not part
# of any of the three returned commands (execstr("...=..."), resume command
# and visibility command execstr("..."),
# so that:
#   . Scipad does not attempt to update really non editable variables
#     (libraries for instance)
#   . Scipad does not transform globals into locals, which would otherwise mess
#     up the visibility assumptions in the debugged function
    global watchvars watchvarsprops unklabel
    set fullcomm ""
    set varset ""
    set retcomm ""
    set viscomm ""
    foreach var $setofvars {
        # ignore non editable variables
        if {![isvareditable $var]} {
            continue
        }
        # ignore fully undefined variables
        if {$watchvarsprops($var,value) == $unklabel} {
            continue
        }
        if {![isunknownelementinvar $var]} {
            # Variable is fully defined
            set onecomm [duplicatechars "$var=$watchvarsprops($var,value);" "\""]
            set onecomm [duplicatechars $onecomm "'"]
            set fullcomm [concat $fullcomm $onecomm]
            set oppar [string first "\(" $var]
            if {$oppar == -1} {
                set varset [concat $varset $var]
            } else {
                set varset [concat $varset [string range $var 0 [expr {$oppar - 1}]]]
            }
        } else {
            # Variable is partially undefined (ex: certain elements of a list)
            # In this case, we're dealing with list(elt1,..,eltn,$unklabel,eltm,..,eltp)
            # and $unklabel can appear any number of times >1 in the elements list
            # Result: variable is split into:
            # $var=list();$var($curind)=elt1; and so on, forgetting the undefined
            # marked as $unklabel
            # This recreates truly undefined elements in Scilab
            # Note that this method does not support lists with non editable
            # elements. For this reason, if a list contains a non editable
            # element, then the entire list is non editable (this is set in
            # FormatStringsFor Watch)
            set oppar [string first "\(" $watchvarsprops($var,value)]
            set listtype [string range $watchvarsprops($var,value) 0 [expr {$oppar - 1}]]
            if {$listtype != "list"} {
                # Undefined elements are forbidden in any variable
                # of type different than "list"
                tk_messageBox -message [concat \
                    [mc "Undefined elements are not legal in variable"] $var \
                    [mc ".\nThis variable will not be updated in Scilab."] ]\
                    -icon warning -type ok \
                    -title [mc "Illegal undefined element found"]
                continue
            }
            set onecomm "$var=[string range $watchvarsprops($var,value) 0 $oppar]);"
            set fullcommlist $onecomm
            set start [expr {$oppar + 1}]
            set anotherelt "true"
            set curind 0
            while {$anotherelt == "true"} {
                # Parse for next element
                set i $start
                set nestlevel 0
                set quotenest 0
                set curchar [string index $watchvarsprops($var,value) $i]
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
                    set curchar [string index $watchvarsprops($var,value) $i]
                }
                if {$nestlevel == -1} {
                    # We went out of the while because $nestlevel == -1, which means
                    # that we have just reached the end of the elements list
                    set anotherelt "false"
                    incr i -1
                }
                incr curind
                set curval [string range $watchvarsprops($var,value) $start [expr {$i - 1}]]
                if {$curval != $unklabel} {
                    set onecomm [duplicatechars "$var\($curind\)=$curval;" "\""]
                    set onecomm [duplicatechars $onecomm "'"]
                    set fullcommlist [concat $fullcommlist $onecomm]
                } else {
                    # undefined element - ignore it
                }
                set start [expr {$i + 1}]
                set oppar [string first "\(" $var]
                # <TODO> here a test for duplicates in varset is missing
                #        watching a list of n elements, varset contains
                #        n times the name of the list - not a big deal
                if {$oppar == -1} {
                    set varset [concat $varset $var]
                } else {
                    set varset [concat $varset [string range $var 0 [expr {$oppar - 1}]]]
                }
            }
            set fullcomm [concat $fullcomm $fullcommlist]
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

proc evalgenericexpinshell {} {
# evaluate in the Scilab shell the generic expressions listed in the
# corresponding area of the watch window
    global watchgenexps
    global filtergenexperrors
    set formattingstring1 [mc "evaluates to:"]
    set formattingstring2 [mc "produces error"]
    set formattingstring3 [mc "returns no result"]
    foreach genexp $watchgenexps {
        set onegenexp [duplicatechars $genexp "\""]
        set percentescagenexp [duplicatechars $onegenexp "%"]
        set comm1      "try;"
        set comm2          "lines(0);\[db_evstrresult,db_evstrierr\]=evstr(\"$onegenexp\");"
        set comm3          "if db_evstrierr==0 then"
        set comm4              "mprintf(\"\n$percentescagenexp $formattingstring1\");"
        set comm5              "disp(db_evstrresult);"
        if {!$filtergenexperrors} {
            # don't filter errors from the shell display
            set comm6      "else"
            set comm7          "lasterror(%t);mprintf(\"\n$percentescagenexp $formattingstring2\"+\"(\"+string(db_evstrierr)+\")\n\");"
            set comm8      "end;"
            set comm9  "catch;"
            set comm10     "lasterror(%t);mprintf(\"\n$percentescagenexp $formattingstring3\n\");"
            set comm11 "end;"
        } else {
            # generic expressions evaluating to an error or to no result are silent
            set comm6      "else"
            set comm7          "lasterror(%t);"
            set comm8      "end;"
            set comm9  "catch;"
            set comm10     "lasterror(%t);"
            set comm11  "end;"
        }
        set fullcomm [concat $comm1 $comm2 $comm3 $comm4 $comm5 $comm6 $comm7 $comm8 $comm9 $comm10 $comm11]
        ScilabEval_lt $fullcomm "seq"
    }
}

proc updateclickablelinetag {} {
    global callstackwidget
    global errfunc
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
