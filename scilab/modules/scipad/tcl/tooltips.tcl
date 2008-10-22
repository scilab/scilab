#  Scipad - programmer's editor and debugger for Scilab
#
#  Copyright (C) 2002 -      INRIA, Matthieu Philippe
#  Copyright (C) 2003-2006 - Weizmann Institute of Science, Enrico Segre
#  Copyright (C) 2004-2008 - Francois Vogel
#
#  Localization files ( in tcl/msg_files/) are copyright of the 
#  individual authors, listed in the header of each file
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# See the file scipad/license.txt
#
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
# Manage the popup bubbles that display the name and accelerator of the watch
# window icons
    set butname [lindex $watchwiniconslist $butnum]
    set txt [$menutosearchin entrycget $butnum -label]
    set acc [$menutosearchin entrycget $butnum -accelerator]
    if {$acc != ""} { set txt "$txt ($acc)" }
    update_bubble $type $butname $mousexy $txt
}

proc update_bubble_watchvar {w type mousexy} {
# Manage the popup bubbles that display the type and size of the watched
# variables
# Because there is no <Enter> and <Leave> events for listbox individual
# entries, some gymnastics must be done here to update the bubble when
# the mouse is hovering over a new entry
    global pad watchvarsprops
    global go_on_update_bubble_watchvar hovereditem_update_bubble_watchvar

    if {$type == "enter" && $go_on_update_bubble_watchvar} {

        # no bubble should show up if the mouse is below the last watched var
        set ycoordinw [expr {[winfo pointery $pad] - [winfo rooty $w]}]
        set bboxoflast [$w bbox end]
        if  {$bboxoflast == ""} {
            # last element not visible, i.e. the listbox area is full - nothing to do
        } else {
            set ylast [lindex $bboxoflast 1]
            set hlast [lindex $bboxoflast 3]
            if {$ycoordinw > [expr {$ylast + $hlast}]} {
                # delete bubble now
                update_bubble_watchvar $w leave [list ]
                # and check again later
                after 100 "update_bubble_watchvar $w $type \[winfo pointerxy $pad\]"
                return
            }
        }
        set hovereditem [$w nearest $ycoordinw]

        # if the new hovered item is the same as in the previous call to this
        # proc, just reschedule a check for later
        if {[info exists hovereditem_update_bubble_watchvar]} {
            if {$hovereditem == $hovereditem_update_bubble_watchvar} {
                after 100 "update_bubble_watchvar $w $type \[winfo pointerxy $pad\]"
                return
            } else {
                set hovereditem_update_bubble_watchvar $hovereditem
            }
        } else {
            set hovereditem_update_bubble_watchvar $hovereditem
        }

        # a new item is hovered, retrieve the corresponding watch variable name
        set wvar [$w get $hovereditem]
        if {$wvar == ""} {
            return
        }

        # prevent from bubble vanishing because of a previous call to destroy
        # while the mouse was hovering something else
        cancel_bubble_deletion $w

        # destroy the previous bubble and create the new one
        update_bubble leave $w [list ] ""
        update_bubble enter $w $mousexy $watchvarsprops($wvar,tysi)

        # wait a bit and play the game again
        after 100 "update_bubble_watchvar $w $type \[winfo pointerxy $pad\]"

    } else {
        # $type == "leave" or $go_on_update_bubble_watchvar is false,
        # then delete the bubble immediately
        update_bubble leave $w [list ] ""
        unset -nocomplain -- hovereditem_update_bubble_watchvar
    }
}

proc update_bubble {type widgetname mousexy bubbletxt} {
# generic bubble window handler
    global menuFont
    set bubble $widgetname.bubble
    catch {destroy $bubble}
    if {$type=="enter"} {
        update idletasks
        after 100
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

proc cancel_bubble_deletion {widgetname} {
    after cancel [list destroy $widgetname.bubble]
}
