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

proc showinfo {message} {
# Temporarily display a string in the message area
    global pad
    $pad.statusmes configure -state normal
    $pad.statusmes configure -text " "
    $pad.statusmes configure -text "$message"
    after 5000 catch delinfo
}

proc delinfo {} {
    global pad
    $pad.statusmes configure -state normal
    $pad.statusmes configure -text " " 
}

proc keyposn {textarea} {
# launch a keyposn command in background
# dokeyposn is a bit long for very large buffers (execution time is
# around 300 ms for 15000 lines buffers when cursor is at the buffer end),
# entirely due to the inefficient whichfun)
# if many keyposn are waiting for execution, e.g. when the user types
# quicker than the position can be updated by Scipad, there can be many
# keyposn commands pending -> first delete them since they are now pointless
# and launch only the latest command
# dokeyposn is catched to deal more easily with buffers that were closed
# before the command could be processed
    global backgroundtasksallowed
    if {$backgroundtasksallowed} {
        after cancel [list after 1 "catch \"dokeyposn $textarea\""]
        after idle   [list after 1 "catch \"dokeyposn $textarea\""]
    } else {
        dokeyposn $textarea
    }
}

proc dokeyposn {textarea} {
# this proc gets the posn and sets the statusbar, enables context menues, etc.
    global pad listoffile
    global MenuEntryId
    global Scilab5

    # update the status bars data
    $pad.statusind configure -state normal
    set indexin [$textarea index insert]
    $pad.statusind configure -text " "
    scan $indexin "%d.%d" ypos xpos
    incr xpos
    $pad.statusind configure -text [concat [mc "Line:"] $ypos [mc "Column:"] $xpos]

    # <DEBUG>: uncomment this to display the undo and redo stack depths
    #$pad.statusind configure -text [concat "undo:" $listoffile("$textarea",undostackdepth) "redo:" $listoffile("$textarea",redostackdepth)]

    set infun [whichfun $indexin $textarea]
    $pad.statusind2 configure -state normal
    $pad.statusind2 configure -text " "
    if {$listoffile("$textarea",language) == "scilab" } {
        if {$infun != {} } {
            # display logical line number in current function
            set funname   [lindex $infun 0]
            set lineinfun [lindex $infun 1]
            $pad.statusind2 configure -text [concat [mc "Line"]\
                 $lineinfun [mc "in"] $funname]
            # create help skeleton enabled since we're in a Scilab function
            $pad.filemenu.files entryconfigure $MenuEntryId($pad.filemenu.files.[mcra "Create help s&keleton..."]) -state normal
            if {$Scilab5} {
                # help from head comments only available in Scilab 5
                # there is no function help_from_sci in Scilab-4.x or Scilab-gtk
                $pad.filemenu.files entryconfigure $MenuEntryId($pad.filemenu.files.[mcra "Create help from hea&d comments..."]) -state normal
            } else {
                # nothing to do, the menu entry is always disabled
                # since the above condition is the only place where it can be enabled
            }
        } else {
            # display logical line number in current buffer
            set contlines [countcontlines $textarea 1.0 $indexin]
            set logicline [$textarea index "$indexin - $contlines l"]
            scan $logicline "%d.%d" ylogicpos xlogicpos
            $pad.statusind2 configure -text [concat [mc "Logical line:"] $ylogicpos]
            # create help skeleton disabled since we're outside any Scilab function
            $pad.filemenu.files entryconfigure $MenuEntryId($pad.filemenu.files.[mcra "Create help s&keleton..."]) -state disabled
            $pad.filemenu.files entryconfigure $MenuEntryId($pad.filemenu.files.[mcra "Create help from hea&d comments..."]) -state disabled
        }
    }

    # enable Open function source contextually
    if {[lsearch [$textarea tag names $indexin] "libfun"]!=-1 || \
        [lsearch [$textarea tag names $indexin] "scicos"]!=-1 || \
        [lsearch [$textarea tag names $indexin] "userfun"]!=-1 } {
        $pad.filemenu.files entryconfigure $MenuEntryId($pad.filemenu.files.[mcra "Open &function source"]) -state normal
    } else {
        $pad.filemenu.files entryconfigure $MenuEntryId($pad.filemenu.files.[mcra "Open &function source"]) -state disabled
    }

    # enable Undo if the undo stack is not empty
    # (and not when the buffer was modified, see also bug 3476)
    if {$listoffile("$textarea",undostackdepth) != 0} {
        $pad.filemenu.edit entryconfigure $MenuEntryId($pad.filemenu.edit.[mcra "&Undo"]) -state normal
        bindenable Text {undo [gettextareacur]}
    } else {
        $pad.filemenu.edit entryconfigure $MenuEntryId($pad.filemenu.edit.[mcra "&Undo"]) -state disabled
        binddisable Text {undo [gettextareacur]}
    }

    # enable Redo if the redo stack is not empty
    if {$listoffile("$textarea",redostackdepth) == 0} {
        $pad.filemenu.edit entryconfigure $MenuEntryId($pad.filemenu.edit.[mcra "&Redo"]) -state disabled
        binddisable Text {redo [gettextareacur]}
    } else {
        $pad.filemenu.edit entryconfigure $MenuEntryId($pad.filemenu.edit.[mcra "&Redo"]) -state normal
        bindenable Text {redo [gettextareacur]}
    }

    # enable Cut and Copy and Evaluate selection if there is a selection
    if {[gettaselind $textarea any] == {}} {
        $pad.filemenu.edit entryconfigure $MenuEntryId($pad.filemenu.edit.[mcra "Cu&t"]) -state disabled
        $pad.filemenu.edit entryconfigure $MenuEntryId($pad.filemenu.edit.[mcra "B&lock cut"]) -state disabled
        $pad.filemenu.edit entryconfigure $MenuEntryId($pad.filemenu.edit.[mcra "&Copy"]) -state disabled
        $pad.filemenu.exec entryconfigure $MenuEntryId($pad.filemenu.exec.[mcra "&Evaluate selection"]) -state disabled
        binddisable Text {cuttext normal}
        binddisable Text {cuttext block}
        binddisable $pad copytext
        binddisable $pad execselection
    } else {
        $pad.filemenu.edit entryconfigure $MenuEntryId($pad.filemenu.edit.[mcra "Cu&t"]) -state normal
        $pad.filemenu.edit entryconfigure $MenuEntryId($pad.filemenu.edit.[mcra "B&lock cut"]) -state normal
        $pad.filemenu.edit entryconfigure $MenuEntryId($pad.filemenu.edit.[mcra "&Copy"]) -state normal
        $pad.filemenu.exec entryconfigure $MenuEntryId($pad.filemenu.exec.[mcra "&Evaluate selection"]) -state normal
        bindenable Text {cuttext normal}
        bindenable Text {cuttext block}
        bindenable $pad copytext
        bindenable $pad execselection
   }

    # enable Paste if the clipboard contains something
    if {[catch {clipboard get}] == 1} {
        $pad.filemenu.edit entryconfigure $MenuEntryId($pad.filemenu.edit.[mcra "&Paste"]) -state disabled
        $pad.filemenu.edit entryconfigure $MenuEntryId($pad.filemenu.edit.[mcra "&Block paste"]) -state disabled
        binddisable Text {pastetext normal}
        binddisable Text {pastetext block}
    } else {
        $pad.filemenu.edit entryconfigure $MenuEntryId($pad.filemenu.edit.[mcra "&Paste"]) -state normal
        $pad.filemenu.edit entryconfigure $MenuEntryId($pad.filemenu.edit.[mcra "&Block paste"]) -state normal
        bindenable Text {pastetext normal}
        bindenable Text {pastetext block}
    }
}

proc modifiedtitle {textarea {panesonly "false"}} {
# Set the Scipad window title to the name of the file displayed in $textarea
# and add tags (modified, readonly, encoding (if different from the system
# encoding at Scipad startup))
# Do the same for the pane title if it exists (i.e. if not maximized)
# Update also the visual indications of the modified state of the buffer.
# This includes title bar, colorization of the windows menu entry and
# colorization of an area in the status bar
    global pad winTitle ScipadVersion listoffile
    global MenuEntryId
    global defaultencoding

    if {$listoffile("$textarea",readonly) == 1} { 
        set mod1 [mc " \[ReadOnly\]"]
    } else {
        set mod1 ""
    }

    if {[isanymodified]} {
        $pad.statusind configure -background PeachPuff
    } else {
        $pad.statusind configure -background [$pad.filemenu cget -background]
    }

    set fname $listoffile("$textarea",displayedname)
    set ind [extractindexfromlabel $pad.filemenu.wind $fname]
    if {[ismodified $textarea]} {
        set mod2 [mc " (modified)"]
        if {$ind !=-1} {
            $pad.filemenu.wind entryconfigure $ind -background Salmon \
              -activebackground LightSalmon
        }
        $pad.statusind configure -background Salmon
    } else {  
        set mod2 "" 
        if {$ind !=-1} {
            $pad.filemenu.wind entryconfigure $ind -background "" \
               -activebackground ""
        }
    }

    if {$listoffile("$textarea",encoding) != $defaultencoding} {
        set mod3 " ($listoffile("$textarea",encoding))"
    } else {
        set mod3 ""
    }

    if {$panesonly == "false"} {
        # catched because scan will fail when launched from wish
        if {[catch {
            scan $ScipadVersion "%s - %s" ScipadVersionNumber ScipadVersionString
            wm title $pad "$winTitle $ScipadVersionNumber - $fname$mod1$mod2$mod3"
                   }] } {
            wm title $pad "$winTitle - $fname$mod1$mod2$mod3"
        }
    }

    if {[isdisplayed $textarea]} {
        [getpaneframename $textarea].panetitle configure \
          -text "$fname$mod1$mod2$mod3"
    }

    if {[ismodified $textarea] && \
          $listoffile("$textarea",thetime) !=0} { 
        $pad.filemenu.files entryconfigure \
          $MenuEntryId($pad.filemenu.files.[mcra "&Revert..."]) -state normal
        bindenable $pad {revertsaved \[gettextareacur\]}
    } else {
        $pad.filemenu.files entryconfigure \
          $MenuEntryId($pad.filemenu.files.[mcra "&Revert..."]) -state disabled
        binddisable $pad {revertsaved \[gettextareacur\]}
    }
}

proc updatepanestitles {} {
    global listoftextarea
    # update file names in panes only (not in Scipad title bar)
    foreach ta $listoftextarea {
        if {[isdisplayed $ta]} {
            modifiedtitle $ta "panesonly"
        }
    }
    # update file name in Scipad title bar (and in the current textarea pane)
    modifiedtitle [gettextareacur]
}

proc displaybusystate {} {
# a visual indicator of the availability of the scilab prompt, for the
# main window and for the debug watch window. This proc keeps rescheduling
# itself as long as $pad.statusmes (hence the main window) exists
    global sciprompt pad led_scilabbusy
    if {[info exists pad]} {
        if {[string compare $sciprompt -1] == 0} {
            set bok [catch {$pad.statusmes configure -background orange}]
            if {[istoplevelopen watch]} { 
                $led_scilabbusy itemconfigure all -image led_scilabbusy_busy
            }
        } else {
            set bok [catch {$pad.statusmes configure -background lightblue}]
            if {[istoplevelopen watch]} { 
                $led_scilabbusy itemconfigure all -image led_scilabbusy_ready
            }
        }
        if {$bok == 0} {
            after 100 {displaybusystate}
        }
    }
}

proc pleaseuseabetterscilabversion {bugnumber {furtherrefs {}}} {
# Many thanks to the Operational Team of the Scilab Consortium
# Scilab 5 definitely rocks
    global ScilabBugzillaURL latestbackportedscipadversionURL
    global pad
    set bugzillabugURL {}
    append bugzillabugURL $ScilabBugzillaURL "/show_bug.cgi?id=$bugnumber"
    set mes {}
    append mes [mc "This feature is no longer available in Scilab 5 due to bug "] $bugnumber "." "\n\n" \
               [mc "See details at "] $bugzillabugURL "\n\n"
    if {$furtherrefs != {}} {
        append mes [mc "See also:"] "\n"
        foreach elt $furtherrefs {
            append mes "    " $elt "\n"
        }
        append mes "\n"
    }
    append mes [mc "Note: Scilab 4.x and Scilab-gtk are offering this feature."] "\n" \
               [mc "A solution is to use a backported version of Scipad inside one of these environments, for instance:"] "\n" $latestbackportedscipadversionURL
    set tit [mc "Feature missing from Scilab 5"]
    tk_messageBox -message $mes -icon error -title $tit -parent $pad
}

proc setscipadicon {toplevelname} {
# give $toplevelname the Scipad icon in the task bar and in its titlebar
    global tcl_platform pad iconsdir
    if {$tcl_platform(platform) == "windows" && $tcl_platform(osVersion) == "5.0"} {
        # special case for Windows 2000: just do nothing
        # we cannot rely on catch {wm iconphoto ...} because this produces
        # a black square icon on Win 2000 (bug 3416)
        # <TODO> the proper fix would be to use
        #          wm iconbitmap $toplevelname $iconicofile
        #        but the question is how to create the ico file from the existing
        #        icon in GIF format
        # doing nothing here means Scipad will receive the Tk icon, which is fine
    } else {
        # other platforms or other Windows versions (see bug 3105)
        # wm iconphoto is officially implemented in Tk 8.5 only
        # however it is also in Tk 8.4.8 onwards
        # see http://groups.google.com/group/comp.lang.tcl/browse_thread/thread/8c6e1a59ea384573
        # to avoid testing for complicated versions and platforms, I lazily catch this...
        # there is anyway no fallback when wm iconphoto is not supported
        # (wm iconbitmap with an ico file, on Windows only... well, let's wait for bug reports, if any!)
        set iconimage [image create photo $pad.icon1 -format gif \
                            -file [file join $iconsdir scipad-editor.gif]]
        catch {wm iconphoto $toplevelname $iconimage}
    }
}
