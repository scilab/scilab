# procedure to setup the printer
proc printseupselection {} {
    global printCommand
    global pad
    global lang
    
    set print $pad.print
    catch {destroy $print}
    toplevel $print
    if {$lang == "eng"} {
	wm title $print "Print Setup"
    } else {
	wm title $print "Mise en page"
    }
    setwingeom $print
    frame $print.top 
    frame $print.bottom
    if {$lang == "eng"} {
	label $print.top.label -text "Print Command: "
    } else {
	label $print.top.label -text "Commande d'impression : "
    }
    entry $print.top.print -textvariable printsetupnew -width 40
    $print.top.print delete 0 end
    set printvar $printCommand 
    $print.top.print insert 0 $printvar
    if {$lang == "eng"} {
	button $print.bottom.ok -text "OK" -command "addtoprint $print"
	button $print.bottom.cancel -text "Cancel" -command "destroy $print"
    } else {
	button $print.bottom.ok -text "OK" -command "addtoprint $print"
	button $print.bottom.cancel -text "Annuler" -command "destroy $print"
    }
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

# procedure to print
proc selectprint {textarea} {
    set TempPrintFile [open /tmp/tkpadtmpfile w]
    puts -nonewline $TempPrintFile [$textarea get 0.0 end]
    close $TempPrintFile
    global printCommand
    set prncmd $printCommand	
    eval exec $prncmd /tmp/tkpadtmpfile
    eval exec rm -f /tmp/tkpadtmpfile
}
