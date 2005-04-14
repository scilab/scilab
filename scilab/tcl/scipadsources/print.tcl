proc printseupselection {} {
# procedure to setup the printer
    global printCommand pad
    set print $pad.print
    catch {destroy $print}
    toplevel $print
    wm title $print [mc "Print Setup"]
    setwingeom $print
    frame $print.top 
    frame $print.bottom
    label $print.top.label -text [mc "Print Command: "]
    entry $print.top.print -textvariable printsetupnew -width 40
    $print.top.print delete 0 end
    set printvar $printCommand 
    $print.top.print insert 0 $printvar
    button $print.bottom.ok -text [mc "OK"] -command "addtoprint $print"
    button $print.bottom.cancel -text [mc "Cancel"] -command "destroy $print"
    pack $print.top -side top -expand 0 
    pack $print.bottom -side bottom -expand 0 
    pack $print.top.label $print.top.print -in $print.top -side left -fill x \
            -fill y
    pack $print.bottom.ok $print.bottom.cancel -in $print.bottom -side left \
            -fill x -fill y
    bind $print <Return> "addtoprint $print"
    bind $print <Escape> "destroy $print"

    proc addtoprint {prnt} {
        global printCommand
        set printCommand [$prnt.top.print get]
        destroy $prnt
    }
}

proc selectprint {textarea} {
# procedure to print
    global printCommand listoffile
    if {$listoffile("$textarea",save) == 1} {
      set TempPrintFile [open /tmp/tkpadtmpfile w]
      puts -nonewline $TempPrintFile [$textarea get 0.0 end]
      close $TempPrintFile
      catch {eval exec "$printCommand /tmp/tkpadtmpfile"} result
    } else {
      catch {eval exec "$printCommand $listoffile("$textarea",fullname)"}\
               result
    }
    if {$result != ""} {tk_messageBox -message $result}
    eval exec rm -f /tmp/tkpadtmpfile
}
