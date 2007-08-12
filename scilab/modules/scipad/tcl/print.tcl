proc printsetup {} {
    global tcl_platform
    if {$tcl_platform(platform) != "unix"} {
        printsetup_win
    } else {
        printsetup_unix
    }
}

proc selectprint {textarea} {
    global tcl_platform
    if {$tcl_platform(platform) != "unix"} {
        selectprint_win $textarea
    } else {
        selectprint_unix $textarea
    }
}

proc printsetup_unix {} {
# procedure to setup the printer on unix systems
    global printCommand pad menuFont textFont
    set print $pad.print
    catch {destroy $print}
    toplevel $print
    wm title $print [mc "Print Setup"]
    wm withdraw $print

    frame $print.top 
    frame $print.bottom
    label $print.top.label -text [mc "Print Command: "] -font $menuFont
    entry $print.top.print -textvariable printsetupnew -width 40 -font $textFont
    $print.top.print delete 0 end
    set printvar $printCommand 
    $print.top.print insert 0 $printvar
    set bestwidth [mcmaxra "OK" \
                           "Cancel"]
    button $print.bottom.ok -text [mc "OK"] \
            -command "addtoprint $print" \
            -width $bestwidth -font $menuFont
    button $print.bottom.cancel -text [mc "Cancel"] \
            -command "destroy $print" \
            -width $bestwidth -font $menuFont
    pack $print.top -side top -expand 0 -pady 2
    pack $print.bottom -side bottom -expand 0 -pady 2
    pack $print.top.label $print.top.print -in $print.top -side left -fill x \
            -fill y
    pack $print.bottom.ok $print.bottom.cancel -in $print.bottom -side left \
            -fill x -fill y -padx 10
    bind $print <Return> "addtoprint $print"
    bind $print <Escape> "destroy $print"

    update idletasks
    setwingeom $print
    wm deiconify $print

    proc addtoprint {prnt} {
        global printCommand
        set printCommand [$prnt.top.print get]
        destroy $prnt
    }
}

proc selectprint_unix {textarea} {
# procedure to print on unix systems
    global printCommand listoffile
    # testing for file exists needed because Ctrl+n followed by print
    # prints garbage or leads to an error otherwise
    if {[ismodified $textarea] ||
        ![file exists $listoffile("$textarea",fullname)]} {
        set TempPrintFile [open /tmp/SciPadtmpfile w]
        puts -nonewline $TempPrintFile [$textarea get 1.0 end]
        close $TempPrintFile
        catch {eval exec "$printCommand /tmp/SciPadtmpfile"} result
    } else {
        catch {eval exec "$printCommand $listoffile("$textarea",fullname)"} result
    }
    if {$result != ""} {tk_messageBox -message $result}
    eval exec rm -f /tmp/SciPadtmpfile
}

proc printsetup_win {} {
# procedure to setup the printer on windows systems
    if {[isscilabbusy 0]} {return}
    ScilabEval_lt "printsetupbox()"
}

proc selectprint_win {textarea} {
# procedure to print on windows systems
    global tmpdir listoffile
    if {[isscilabbusy 0]} {return}
    # testing for file exists needed because Ctrl+n followed by print
    # prints garbage or leads to an error otherwise
    if {[ismodified $textarea] ||
        ![file exists $listoffile("$textarea",fullname)]} {
        set fname [file join $tmpdir SciPadtmpfile]
        set TempPrintFile [open $fname w]
        puts -nonewline $TempPrintFile [$textarea get 1.0 end]
        close $TempPrintFile
        ScilabEval_lt "toprint(\"$fname\")" "sync"
        file delete $fname
    } else {
        ScilabEval_lt "toprint(\"$listoffile("$textarea",fullname)\")"
    }
}
