proc showwatch_bp {} {
    global pad watch lang
    global lbvarname lbvarval scrolly
    global watchvars watchvarsvals buttonAddw
    global firsttimeinshowwatch watchgeom
    global callstackwidget callstackcontent
    global watchwinicons db_butimages
    set watch $pad.watch
    catch {destroy $watch}
    toplevel $watch
    wm title $watch "Watch"
    if { $firsttimeinshowwatch == "true" } { 
        setwingeom $watch
    } else {
        wm resizable $watch 0 0
        wm geometry $watch $watchgeom
    }
    frame $watch.f

    frame $watch.f.f1
    set buttonConfigure $watch.f.f1.configure
    button $buttonConfigure -command "configurefoo_bp" -image [lindex $db_butimages 4] \
           -relief flat -overrelief raised
    set buttonToNextBpt $watch.f.f1.toNextBpt
    button $buttonToNextBpt -command "tonextbreakpoint_bp" -image [lindex $db_butimages 6] \
           -relief flat -overrelief raised
    set buttonGoOnIgnor $watch.f.f1.goOnIgnor
    button $buttonGoOnIgnor -command "goonwo_bp" -image [lindex $db_butimages 8] \
           -relief flat -overrelief raised
    set buttonCancelDebug $watch.f.f1.cancelDebug
    button $buttonCancelDebug -command "canceldebug_bp" -image [lindex $db_butimages 12] \
           -relief flat -overrelief raised
    pack $buttonConfigure $buttonToNextBpt $buttonGoOnIgnor $buttonCancelDebug \
         -padx 2 -pady 2 -side left
    pack $watch.f.f1 -anchor w
    set watchwinicons [list "sep" "" "" "sep" $buttonConfigure "sep" $buttonToNextBpt \
                            "" $buttonGoOnIgnor "sep" "" "sep" $buttonCancelDebug ]
    setdbmenuentriesstates_bp

    frame $watch.f.f2 -relief groove -borderwidth 2 -padx 2 -pady 4
    frame $watch.f.f2.f2l
    if {$lang == "eng"} {
        set tl "Watch variables:"
    } else {
        set tl "Variables espions :"
    }
    label $watch.f.f2.f2l.label -text $tl
    if {$lang == "eng"} {
        set bl "Add/Change"
    } else {
        set bl "Ajouter/Modifier"
    }
    set buttonAddw $watch.f.f2.f2l.buttonAdd
    button $buttonAddw -text $bl -width 20 -underline 0
    if {$lang == "eng"} {
        set bl "Remove"
    } else {
        set bl "Supprimer"
    }
    set buttonRemove $watch.f.f2.f2l.buttonRemove
    button $buttonRemove -text $bl -width 20 -underline 0
    pack $watch.f.f2.f2l.label $buttonAddw $buttonRemove -pady 4
    frame $watch.f.f2.f2r
    set lbvarname $watch.f.f2.f2r.lbvarname
    set lbvarval $watch.f.f2.f2r.lbvarval
    $buttonAddw configure -command {Addarg_bp $watch $buttonAddw $lbvarname $lbvarval; \
                                    closewatch_bp $watch nodestroy}
    $buttonRemove configure -command {Removearg_bp $lbvarname $lbvarval; \
                                      closewatch_bp $watch nodestroy}
    set scrolly $watch.f.f2.f2r.yscroll
    scrollbar $scrolly -command "scrollyboth_bp $lbvarname $lbvarval"
    listbox $lbvarname -height 6 -width 12 -yscrollcommand \
                       "scrollyrightandscrollbar_bp $scrolly $lbvarname $lbvarval" \
                       -takefocus 0
    listbox $lbvarval -height 6 -width 40 -yscrollcommand \
                      "scrollyleftandscrollbar_bp $scrolly $lbvarname $lbvarval" \
                      -takefocus 0
    if {[info exists watchvars]} {
        foreach var $watchvars {
            $lbvarname insert end $var
            $lbvarval insert end $watchvarsvals($var)
        }
        $lbvarname selection set 0
        $lbvarname see 0
    }
    pack $lbvarname $scrolly $lbvarval -side left \
            -expand 1 -fill both -padx 2
    pack $watch.f.f2.f2l $watch.f.f2.f2r -side left -padx 2
    pack $watch.f.f2 -pady 2

    frame $watch.f.f6 -relief groove -borderwidth 2 -padx 2
    if {$lang == "eng"} {
        set csl "Call stack:"
    } else {
        set csl "Pile des appels:"
    }
    label $watch.f.f6.cslabel -text $csl
    pack $watch.f.f6.cslabel -anchor w -pady 4
    set callstackwidget $watch.f.f6.callstack
    text $callstackwidget -height 5 -width 81 -state normal -background gray83
    pack $callstackwidget
    pack $watch.f.f6 -pady 2
    $callstackwidget delete 1.0 end
    $callstackwidget insert 1.0 $callstackcontent
    $callstackwidget configure -state disabled

    frame $watch.f.f9
    if {$lang == "eng"} {
        set bl "Close"
    } else {
        set bl "Fermer"
    }
    button $watch.f.f9.buttonClose -text $bl -command "closewatch_bp $watch"\
           -width 10 -height 1 -underline 0
    pack $watch.f.f9.buttonClose
    pack $watch.f.f9 -pady 2

    pack $watch.f
    bind $watch <Return> {Addarg_bp $watch $buttonAddw $lbvarname $lbvarval; \
                          closewatch_bp $watch nodestroy}
    bind $lbvarname <Double-Button-1> {Addarg_bp $watch $buttonAddw $lbvarname $lbvarval; \
                                       closewatch_bp $watch nodestroy}
    bind $watch <Escape> {set watchgeom [string trimleft [eval {wm geometry $watch}] 1234567890x=]; \
                          closewatch_bp $watch}
    bind $watch <BackSpace> {Removearg_bp $lbvarname $lbvarval; \
                             closewatch_bp $watch nodestroy}
    bind $watch <Delete> {Removearg_bp $lbvarname $lbvarval; \
                          closewatch_bp $watch nodestroy}
    bind $lbvarval <<ListboxSelect>> {selectinrightwin_bp $lbvarname $lbvarval}
    bind $lbvarname <ButtonPress-3> {set itemindex [dragitem_bp $lbvarname %y]}
    bind $lbvarname <ButtonRelease-3> {dropitem_bp $lbvarname $lbvarval "" $itemindex %y}
    bind $watch <Up> {scrollarrows_bp $lbvarname up}
    bind $watch <Down> {scrollarrows_bp $lbvarname down}
    bind $watch <MouseWheel> {if {%D<0} {scrollarrows_bp $lbvarname down}\
                                       {scrollarrows_bp $lbvarname up}}
    bind $watch <Configure> {set watchgeom [string trimleft [eval {wm geometry $watch}] 1234567890x=]}
    if { $firsttimeinshowwatch == "true" } { 
        focus $buttonAddw
        set watchgeom [string trimleft [eval {wm geometry $watch}] 1234567890x=]
        set firsttimeinshowwatch "false"
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
                $callstackwidget configure -state normal
                $callstackwidget delete 1.0 end
                $callstackwidget insert 1.0 $callstackcontent
                $callstackwidget configure -state disabled
            }
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

proc getfromshell {} {
    global watchvars watchvarsvals unklabel callstackcontent
    set fullcomm ""
    foreach var $watchvars {
        set comm1 "if exists(\"$var\"),"
        set comm2 "TK_EvalStr(\"scipad eval {set watchvarsvals($var) \"\"\"+FormatStringsForDebugWatch($var)+\"\"\"}\");"
        set comm3 "else"
        set comm4 "TK_EvalStr(\"scipad eval {set watchvarsvals($var) \"\"$unklabel\"\"}\");"
        set comm5 "end;"
        set fullcomm [concat $comm1 $comm2 $comm3 $comm4 $comm5]
        ScilabEval $fullcomm "seq"
    }
    set fullcomm "TK_EvalStr(\"scipad eval {set callstackcontent \"\"\"+FormatWhereForDebugWatch()+\"\"\"}\");"
    ScilabEval $fullcomm "seq"
    set fullcomm "TK_EvalStr(\"scipad eval {updatewatch_bp}\");"
    ScilabEval $fullcomm "seq"
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
# warning: $ch must be a single character string (but it works for the string "\"")
    set indquot [string first $ch $st 0]
    while {$indquot != -1} {
        set st [string replace $st $indquot $indquot "$ch$ch"]
        set indquot [string first $ch $st [expr $indquot + 2]]
    }
    return $st
}
