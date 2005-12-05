proc helpme {} {
# help
    ScilabEval_lt "help scipad"
}

proc aboutme {} {
# about
    global winTitle version
    tk_messageBox -title [mc "About"] -type ok -message \
        " $winTitle $version [mc aboutme_message]"
}

proc helpword {} {
    global textareacur
    set seltexts [selection own]
    if {[catch {selection get -selection PRIMARY} sel] ||$seltexts != $textareacur} {
        # if there is no selection in the current textarea, select the word at the cursor position
        set i1 [$textareacur index insert]
        $textareacur tag add sel [$textareacur index "$i1 wordstart"] \
                                 [$textareacur index "$i1 wordend"]
        set curterm [selection get]
    } else {
        set cursel [string trim [selection get]]
        # get only the first word of the selection (or a symbol)
        regexp "(\\A\\w*\\M|\\A\\W)" $cursel curterm
    }
    if {[info exists curterm]} {
        set curterm [string trim $curterm]
        set curterm [duplicatechars $curterm "\""]
        set curterm [duplicatechars $curterm "'"]
        if {$curterm!=""} { ScilabEval_lt "help \"$curterm\"" }
    }
}

# a generic scrollable messagewindow, which displays the content of a text file
proc textbox {textfile {wtitle ""}} {
    global pad menuFont textFont
    if {$wtitle == ""} {set wtitle $textfile}
    set tbox $pad.textbox
    catch {destroy $tbox}
    toplevel $tbox
    wm title $tbox $wtitle
    setwingeom $tbox
    wm resizable $tbox 1 1
    frame $tbox.f1
    text $tbox.text -font $textFont
    set newnamefile [open $textfile r]
    while {![eof $newnamefile]} {
            $tbox.text insert end [read -nonewline $newnamefile ] 
    }
    close $newnamefile
if {0} {
    set thetext [$tbox.text get 1.0 end]
    regsub -all -- { *\n *([^-\nA-Z=])} $thetext { \1} thetext
    $tbox.text delete 1.0 end
    $tbox.text insert 1.0 $thetext
    $tbox.text configure -wrap word
}
    $tbox.text configure -state disabled -yscrollcommand \
           "managescroll $tbox.sb"
    pack $tbox.text -in $tbox.f1 -side left -expand 1 -fill both
    scrollbar $tbox.sb -command "$tbox.text yview" -takefocus 0
    $tbox.sb set [lindex [$tbox.text yview] 0] [lindex [$tbox.text yview] 1]
    pack $tbox.sb -in $tbox.f1 -side right -expand 0 -fill y \
            -before $tbox.text
    pack $tbox.f1 -expand 1 -fill both
    frame $tbox.f2
    button $tbox.f2.button -text [mc "Close"] \
            -command "destroy $tbox" \
            -width 10 -height 1 -font $menuFont
    pack $tbox.f2.button -in $tbox.f2
    pack configure $tbox.f2 -pady 4 -after $tbox.f1 -expand 0 -fill both
    pack $tbox.f2 -in $tbox -side bottom -before $tbox.f1
    focus $tbox.f2.button
    bind $tbox <Up> "$tbox.text yview scroll -1 units"
    bind $tbox <Down> "$tbox.text yview scroll 1 units"
    bind $tbox <Home> "$tbox.text yview moveto 0"
    bind $tbox <End> "$tbox.text yview moveto 1"
    bind $tbox <Prior> "$tbox.text yview scroll -1 pages"
    bind $tbox <Next> "$tbox.text yview scroll 1 pages"
    bind $tbox <Return> "destroy $tbox"
    bind $tbox <KP_Enter> "destroy $tbox"
    bind $tbox <Escape> "destroy $tbox"
}
