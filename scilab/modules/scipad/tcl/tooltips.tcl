proc update_bubble_panetitle {type widgetname mousexy textarea} {
# Manage the popup bubbles that display the fullname of a textarea packed in a pane
    global listoffile filenamesdisplaytype
    if {$filenamesdisplaytype != "full"} {
        update_bubble $type $widgetname $mousexy $listoffile("$textarea",fullname)
    }
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
        after 2000 $cmd
    }
}
