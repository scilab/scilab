proc showwatch_bp {} {
    global pad watch
    global lbvarname lbvarval scrolly
    global watchvars watchvarsvals buttonAddw
    global firsttimeinshowwatch watchgeom watchmins watchminsinit
    global callstackwidget callstackcontent
    global watchwinicons db_butimages
    global showwatchvariablesarea togglewvabutton
    global showcallstackarea togglecsabutton
    global watchvpane1mins watchvpane2mins watchvsashcoord
    global watchhpane1mins watchhpane2mins watchhsashcoord

    # Hardwired size, but how else?
    set heightofwatchwithnoarea 105 ;# 85 enough for windows, 105 for my Linux
    set widthofwatchwithnoarea  504 ;# Anyway, could depend on font size, language...

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

    frame $watch.f.f1

    frame $watch.f.f1.f1l
    set buttonConfigure $watch.f.f1.f1l.configure
    button $buttonConfigure -command "configurefoo_bp" -image [lindex $db_butimages 4] \
           -relief flat -overrelief raised
    set buttonToNextBpt $watch.f.f1.f1l.toNextBpt
    button $buttonToNextBpt -command "tonextbreakpoint_bp" -image [lindex $db_butimages 6] \
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
    pack $buttonConfigure $buttonToNextBpt $buttonRunToCursor \
         $buttonGoOnIgnor $buttonBreakDebug $buttonCancelDebug \
         -padx 2 -pady 2 -side left

    frame $watch.f.f1.f1r
    set buttonshowwatchvariablesarea $watch.f.f1.f1r.showwatchvariablesarea
    button $buttonshowwatchvariablesarea -command "togglewatchvariablesarea" -text $togglewvabutton -width 20
    set buttonshowcallstackarea $watch.f.f1.f1r.showcallstackarea
    button $buttonshowcallstackarea -command "togglecallstackarea" -text $togglecsabutton -width 20

    pack $watch.f.f1.f1r.showwatchvariablesarea $watch.f.f1.f1r.showcallstackarea -pady 2
    pack $watch.f.f1.f1l $watch.f.f1.f1r -side left -padx 20 -anchor w

    pack $watch.f.f1 -anchor w

    set watchwinicons [list "sep" "" "" "sep" $buttonConfigure "sep" $buttonToNextBpt \
                            "" $buttonRunToCursor $buttonGoOnIgnor "sep" "" "sep"\
                            $buttonBreakDebug $buttonCancelDebug ]
    setdbmenuentriesstates_bp
    bind $buttonConfigure   <Enter> {update_bubble_watch enter  4 [winfo pointerxy $watch]}
    bind $buttonConfigure   <Leave> {update_bubble_watch leave  4 [winfo pointerxy $watch]}
    bind $buttonToNextBpt   <Enter> {update_bubble_watch enter  6 [winfo pointerxy $watch]}
    bind $buttonToNextBpt   <Leave> {update_bubble_watch leave  6 [winfo pointerxy $watch]}
    bind $buttonRunToCursor <Enter> {update_bubble_watch enter  8 [winfo pointerxy $watch]}
    bind $buttonRunToCursor <Leave> {update_bubble_watch leave  8 [winfo pointerxy $watch]}
    bind $buttonGoOnIgnor   <Enter> {update_bubble_watch enter  9 [winfo pointerxy $watch]}
    bind $buttonGoOnIgnor   <Leave> {update_bubble_watch leave  9 [winfo pointerxy $watch]}
    bind $buttonBreakDebug  <Enter> {update_bubble_watch enter 13 [winfo pointerxy $watch]}
    bind $buttonBreakDebug  <Leave> {update_bubble_watch leave 13 [winfo pointerxy $watch]}
    bind $buttonCancelDebug <Enter> {update_bubble_watch enter 14 [winfo pointerxy $watch]}
    bind $buttonCancelDebug <Leave> {update_bubble_watch leave 14 [winfo pointerxy $watch]}

    panedwindow $watch.f.vpw -orient vertical -opaqueresize true

    frame $watch.f.vpw.f2 -relief groove -borderwidth 2 -padx 2 -pady 4

    frame $watch.f.vpw.f2.f2l
    set tl [mc "Watch variables:"]
    label $watch.f.vpw.f2.f2l.label -text $tl
    set bl [mc "Add/Change"]
    set buttonAddw $watch.f.vpw.f2.f2l.buttonAdd
    button $buttonAddw -text $bl -width 20
    set bl [mc "Remove"]
    set buttonRemove $watch.f.vpw.f2.f2l.buttonRemove
    button $buttonRemove -text $bl -width 20

    pack $watch.f.vpw.f2.f2l.label $buttonAddw $buttonRemove -pady 4
    pack $watch.f.vpw.f2.f2l -anchor n

    frame $watch.f.vpw.f2.f2r

    panedwindow $watch.f.vpw.f2.f2r.hpw -orient horizontal -opaqueresize true

    frame $watch.f.vpw.f2.f2r.hpw.f
    set lbvarname $watch.f.vpw.f2.f2r.hpw.f.lbvarname
    set scrolly   $watch.f.vpw.f2.f2r.hpw.f.yscroll
    set lbvarval  $watch.f.vpw.f2.f2r.hpw.lbvarval
    $buttonAddw   configure -command {Addarg_bp $watch $buttonAddw $lbvarname $lbvarval; \
                                      closewatch_bp $watch nodestroy}
    $buttonRemove configure -command {Removearg_bp $lbvarname $lbvarval; \
                                      closewatch_bp $watch nodestroy}
    scrollbar $scrolly -command "scrollyboth_bp $lbvarname $lbvarval"
    listbox $lbvarname -height 6 -width 12 -yscrollcommand \
                       "scrollyrightandscrollbar_bp $scrolly $lbvarname $lbvarval" \
                       -takefocus 0
    listbox $lbvarval  -height 6 -yscrollcommand \
                       "scrollyleftandscrollbar_bp $scrolly $lbvarname $lbvarval" \
                       -takefocus 0
    if {[info exists watchvars]} {
        foreach var $watchvars {
            $lbvarname insert end $var
            $lbvarval insert end $watchvarsvals($var)
        }
    }

    pack $lbvarname -side left -expand 1 -fill both -padx 2
    pack $scrolly -side left -expand 0 -fill both -padx 2

    $watch.f.vpw.f2.f2r.hpw add $watch.f.vpw.f2.f2r.hpw.f
    $watch.f.vpw.f2.f2r.hpw add $lbvarval

    pack $watch.f.vpw.f2.f2r.hpw -side left -expand 1 -fill both -padx 2
    pack $watch.f.vpw.f2.f2l $watch.f.vpw.f2.f2r -side left -padx 2
    pack $watch.f.vpw.f2.f2r -fill both -expand 1
    if {$showwatchvariablesarea == "true"} {
        $watch.f.vpw add $watch.f.vpw.f2
    }

    frame $watch.f.vpw.f6 -relief groove -borderwidth 2 -padx 2
    set csl [mc "Call stack:"]
    label $watch.f.vpw.f6.cslabel -text $csl
    pack $watch.f.vpw.f6.cslabel -anchor w -pady 4
    set callstackwidget $watch.f.vpw.f6.callstack
    text $callstackwidget -height 5 -width 81 -state normal -background gray83
    pack $callstackwidget -fill both -expand 1
    if {$showcallstackarea == "true"} {
        $watch.f.vpw add $watch.f.vpw.f6
    }
    $callstackwidget delete 1.0 end
    $callstackwidget insert 1.0 $callstackcontent
    $callstackwidget configure -state disabled

    if {$showwatchvariablesarea == "true" || $showcallstackarea == "true"} {
        pack $watch.f.vpw -fill both -expand yes
    }

    frame $watch.f.f9
    set bl [mc "Close"]
    button $watch.f.f9.buttonClose -text $bl -command "closewatch_bp $watch"\
           -width 10 -height 1
    pack $watch.f.f9.buttonClose
    pack $watch.f.f9 -pady 2

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
        set watchhpane1mins [winfo width  $watch.f.vpw.f2.f2r.hpw.f]
        set watchhpane2mins [winfo width  $lbvarval                ]
        set watchhsashcoord [$watch.f.vpw.f2.f2r.hpw sash coord 0]
        set firsttimeinshowwatch "false"
    }

    if {$showwatchvariablesarea == "true"} {
        $watch.f.vpw paneconfigure $watch.f.vpw.f2 -minsize $watchvpane1mins
        $watch.f.vpw.f2.f2r.hpw paneconfigure $watch.f.vpw.f2.f2r.hpw.f -minsize $watchhpane1mins
        $watch.f.vpw.f2.f2r.hpw paneconfigure $lbvarval                 -minsize $watchhpane2mins
    }
    if {$showcallstackarea == "true"} {
        $watch.f.vpw paneconfigure $watch.f.vpw.f6 -minsize $watchvpane2mins
    }
}

proc updatewatch_bp {} {
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
    global watchvars callstackcontent
    foreach var $watchvars {
        getonefromshell $var
    }
    set fullcomm "TCL_EvalStr(\"scipad eval {set callstackcontent \"\"\"+FormatWhereForDebugWatch($startitem)+\"\"\"}\");"
    ScilabEval $fullcomm "seq"
    set fullcomm "TCL_EvalStr(\"scipad eval {updatewatch_bp}\");"
    ScilabEval $fullcomm "seq"
}

proc getonefromshell {wvar {opt "seq"}} {
    global watchvars watchvarsvals unklabel
    set fullcomm ""
    set comm1 "if exists(\"$wvar\"),"
    set comm2 "TCL_EvalStr(\"scipad eval {set watchvarsvals($wvar) \"\"\"+FormatStringsForDebugWatch($wvar)+\"\"\"}\");"
    set comm3 "else"
    set comm4 "TCL_EvalStr(\"scipad eval {set watchvarsvals($wvar) \"\"$unklabel\"\"}\");"
    set comm5 "end;"
    set fullcomm [concat $comm1 $comm2 $comm3 $comm4 $comm5]
    ScilabEval $fullcomm $opt
}

proc createsetinscishellcomm {} {
    global watchvars watchvarsvals unklabel
    set fullcomm ""
    set varset ""
    set retcomm ""
    foreach var $watchvars {
        if {$watchvarsvals($var) != $unklabel} {
            set onecomm [duplicatechars "$var=$watchvarsvals($var);" "\""]
            set onecomm [duplicatechars $onecomm "'"]
            set fullcomm [concat $fullcomm $onecomm]
            set varset [concat $varset $var]
        }
    }
    if {$fullcomm != ""} {
        set fullcomm "execstr(\"$fullcomm\",\"errcatch\",\"m\");"
        foreach var $varset {
            set retcomm "$retcomm,$var"
        }
        set retcomm [string range $retcomm 1 end]
        set retcomm "\[$retcomm\]=resume($retcomm);"
    }
    return [list $fullcomm $retcomm]
}

proc duplicatechars {st ch} {
# warning: $ch must be a single character string (but it works also for the string "\"")
    set indquot [string first $ch $st 0]
    while {$indquot != -1} {
        set st [string replace $st $indquot $indquot "$ch$ch"]
        set indquot [string first $ch $st [expr $indquot + 2]]
    }
    return $st
}

proc update_bubble_watch {type butnum mousexy} {
    global pad watchwinicons
    set butname [lindex $watchwinicons $butnum]
    set txt [$pad.filemenu.debug entrycget $butnum -label]
    set acc [$pad.filemenu.debug entrycget $butnum -accelerator]
    if {$acc != ""} { set txt "$txt ($acc)" }
    update_bubble $type $butname $mousexy $txt
}

proc update_bubble {type widgetname mousexy bubbletxt} {
# generic bubble window handler
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
                          -highlightthickness 0 -bg PaleGoldenrod
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

proc togglewatchvariablesarea {} {
    global watch
    global showwatchvariablesarea togglewvabutton
    closewatch_bp $watch
    if {$showwatchvariablesarea == "true"} {
        set showwatchvariablesarea "false"
        set togglewvabutton [mc "Show watch variables"]
    } else {
        set showwatchvariablesarea "true"
        set togglewvabutton [mc "Hide watch variables"]
    }
    showwatch_bp
}

proc togglecallstackarea {} {
    global watch
    global showcallstackarea togglecsabutton
    closewatch_bp $watch
    if {$showcallstackarea == "true"} {
        set showcallstackarea "false"
        set togglecsabutton [mc "Show call stack"]
    } else {
        set showcallstackarea "true"
        set togglecsabutton [mc "Hide call stack"]
    }
    showwatch_bp
}
