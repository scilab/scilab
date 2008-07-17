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
#######################################################
### Breakpoints properties graphical user interface ###
#######################################################

proc showbptgui_bp {} {
# display the breakpoints properties dialog

    global pad bptsgui
    global bptsprops
    global menuFont textFont maxcharinascilabname
    global bptsguicolwidth
    global bptconditiontypes bpthitconditions
    global bptsguigeom
    global currentlyopeningbptsgui

    # currentlyopeningbptsgui is used to prevent from a Tcl error to happen
    # when Ctrl-F9 is pressed continuously: the opening of a new bptsgui window
    # could be launched before the previous bptsgui window has not yet been
    # completely destroyed, thus some sort of duplicate widget name error would
    # fire
    if {![info exists currentlyopeningbptsgui]} {
        set currentlyopeningbptsgui true
    } else {
        # just ignore any binding that fires while Scipad is already currently
        # opening the bpts gui
        return
    }

    # this is used for restoring the previous geometry of the bpts gui at the
    # end of the opening, or to save its initial geometry
    if {[info exists bptsgui]} {
        set firstbptsguiopening false
    } else {
        set firstbptsguiopening true
    }

    set bptsgui $pad.bptsgui
    if {[istoplevelopen bptsgui]} {
        closebptsgui
    }

    # number of lines displayed in the dialog when it opens
    set defaultnbdisplayedlines 4

    # column widths in characters - arrange for a min size, and such that
    # the labels are always entirely visible
    set funcolwidth [mcmaxra "Function"]
    if {$funcolwidth < $maxcharinascilabname} {set funcolwidth $maxcharinascilabname}
    set lincolwidth [mcmaxra "Line"]
    if {$lincolwidth < 4} {set lincolwidth 4}
    set enacolwidth [mcmaxra "Enabled"]
    if {$enacolwidth < 4} {set enacolwidth 4}
    set expcolwidth [mcmaxra "Expression"]
    if {$expcolwidth < 20} {set expcolwidth 20}
    set listofpossible $bptconditiontypes
    lappend listofpossible "Hit if"
    set ctycolwidth [eval mcmaxra $listofpossible]
    if {$ctycolwidth < 4} {set ctycolwidth 4}
    set watcolwidth [mcmaxra "Watched"]
    if {$watcolwidth < 4} {set watcolwidth 4}
    set hctcolwidth [mcmaxra "Hit count"]
    if {$hctcolwidth < 4} {set hctcolwidth 4}
    set listofpossible $bpthitconditions
    lappend listofpossible "Break if"
    set bricolwidth [eval mcmaxra $bpthitconditions]
    if {$bricolwidth < 4} {set bricolwidth 4}
    set hthcolwidth [mcmaxra "Threshold"]
    if {$hthcolwidth < 4} {set hthcolwidth 4}
    set rhccolwidth [mcmaxra "Reset hit"]
    if {$rhccolwidth < 4} {set rhccolwidth 4}
    set bptsguicolwidth [list $funcolwidth $lincolwidth $enacolwidth \
                              $expcolwidth $ctycolwidth $watcolwidth \
                              $hctcolwidth $bricolwidth $hthcolwidth \
                              $rhccolwidth]

    # column weights for the grid geometry manager
    set funcolweight 2
    set lincolweight 1
    set enacolweight 1
    set expcolweight 4
    set ctycolweight 2
    set watcolweight 1
    set hctcolweight 1
    set bricolweight 2
    set hthcolweight 1
    set rhccolweight 1

    toplevel $bptsgui
    wm title $bptsgui [mc "Breakpoints properties"]
    wm withdraw $bptsgui
    wm protocol $bptsgui WM_DELETE_WINDOW {closebptsgui}

    # a frame for the breakpoints list plus the vertical scrollbar
    frame $bptsgui.f1 ;# -bg lightblue ; $bptsgui conf -bg black

    # a frame for the column titles and the breakpoints properties
    set bptsguibptlist $bptsgui.f1.fl
    frame $bptsguibptlist ; # -bg yellow

    # a frame for the column labels
    frame $bptsguibptlist.flabels

    # in order to have the titles be the same width and height than the table
    # content, a fake entry is gridded *in the labels grid*
    # then the real column titles are gridded at the same row 0 as the fakes
    #     so first the fakes
    set cfr $bptsgui.f1.fl.flabels
    set ID fake
    label $cfr.funname$ID -font $textFont -relief groove -takefocus 0 \
            -width $funcolwidth
    label $cfr.funline$ID -font $textFont -relief groove -takefocus 0 \
            -width $lincolwidth
    checkbutton $cfr.enable$ID -font $textFont -relief groove -takefocus 0 \
            -width $enacolwidth
    entry $cfr.expression$ID -font $textFont -takefocus 0\
            -width $expcolwidth
    spinbox $cfr.conditiontype$ID -font $textFont -takefocus 0 \
            -width $ctycolwidth
    checkbutton $cfr.watchit$ID -font $textFont -relief groove -takefocus 0 \
            -width $watcolwidth
    label $cfr.hitcount$ID -font $textFont -relief groove -takefocus 0 \
            -width $hctcolwidth
    spinbox $cfr.hitcountcondition$ID -font $textFont -takefocus 0 \
            -width $bricolwidth
    entry $cfr.hitthreshold$ID -font $textFont -takefocus 0\
            -width $hthcolwidth
    button $cfr.resethitcount$ID -font $textFont -relief groove -takefocus 0 \
            -width $rhccolwidth
    grid $cfr.funname$ID $cfr.funline$ID $cfr.enable$ID $cfr.expression$ID \
            $cfr.conditiontype$ID $cfr.watchit$ID $cfr.hitcount$ID \
            $cfr.hitcountcondition$ID $cfr.hitthreshold$ID $cfr.resethitcount$ID \
            -sticky nsew -row 0
    #     and now the real entries, gridded above the fakes
    label $bptsguibptlist.flabels.fun -font $menuFont -width $funcolwidth \
            -relief groove -takefocus 0 -text [mc "Function"]
    label $bptsguibptlist.flabels.lin -font $menuFont -width $lincolwidth \
            -relief groove -takefocus 0 -text [mc "Line"]
    label $bptsguibptlist.flabels.ena -font $menuFont -width $enacolwidth \
            -relief groove -takefocus 0 -text [mc "Enabled"]
    label $bptsguibptlist.flabels.exp -font $menuFont -width $expcolwidth \
            -relief groove -takefocus 0 -text [mc "Expression"]
    label $bptsguibptlist.flabels.cty -font $menuFont -width $ctycolwidth \
            -relief groove -takefocus 0 -text [mc "Hit if"]
    label $bptsguibptlist.flabels.wad -font $menuFont -width $watcolwidth \
            -relief groove -takefocus 0 -text [mc "Watched"]
    label $bptsguibptlist.flabels.hct -font $menuFont -width $hctcolwidth \
            -relief groove -takefocus 0 -text [mc "Hit count"]
    label $bptsguibptlist.flabels.bri -font $menuFont -width $bricolwidth \
            -relief groove -takefocus 0 -text [mc "Break if"]
    label $bptsguibptlist.flabels.hth -font $menuFont -width $hthcolwidth \
            -relief groove -takefocus 0 -text [mc "Threshold"]
    label $bptsguibptlist.flabels.rhc -font $menuFont -width $rhccolwidth \
            -relief groove -takefocus 0
    # to ensure consistent resizing, the labels must be configured the exact
    # same way as the breakpoint table itself, though in different master windows
    grid $bptsguibptlist.flabels.fun $bptsguibptlist.flabels.lin \
            $bptsguibptlist.flabels.ena $bptsguibptlist.flabels.exp \
            $bptsguibptlist.flabels.cty $bptsguibptlist.flabels.wad \
            $bptsguibptlist.flabels.hct $bptsguibptlist.flabels.bri \
            $bptsguibptlist.flabels.hth $bptsguibptlist.flabels.rhc \
            -sticky nsew -row 0

    grid columnconfigure $bptsguibptlist.flabels 0 -weight $funcolweight
    grid columnconfigure $bptsguibptlist.flabels 1 -weight $lincolweight
    grid columnconfigure $bptsguibptlist.flabels 2 -weight $enacolweight
    grid columnconfigure $bptsguibptlist.flabels 3 -weight $expcolweight
    grid columnconfigure $bptsguibptlist.flabels 4 -weight $ctycolweight
    grid columnconfigure $bptsguibptlist.flabels 5 -weight $watcolweight
    grid columnconfigure $bptsguibptlist.flabels 6 -weight $hctcolweight
    grid columnconfigure $bptsguibptlist.flabels 7 -weight $bricolweight
    grid columnconfigure $bptsguibptlist.flabels 8 -weight $hthcolweight
    grid columnconfigure $bptsguibptlist.flabels 9 -weight $rhccolweight
    pack $bptsguibptlist.flabels -expand 0 -fill x

    # a frame for the vertical scrollbar
    frame $bptsgui.f1.fr

    set scrollf $bptsguibptlist.sf
    set scrolly $bptsgui.f1.fr.yscroll

    # a scrolled frame for the breakpoints properties table itself
    scrolledframe $scrollf -yscrollcommand "$scrolly set" -fill x
    scrollbar $scrolly -command "$scrollf yview" -takefocus 0

    foreach ID [getusedIDsfrombptprops] {
        addbptingui $ID
    }
    grid columnconfigure $scrollf.scrolled 0 -weight $funcolweight
    grid columnconfigure $scrollf.scrolled 1 -weight $lincolweight
    grid columnconfigure $scrollf.scrolled 2 -weight $enacolweight
    grid columnconfigure $scrollf.scrolled 3 -weight $expcolweight
    grid columnconfigure $scrollf.scrolled 4 -weight $ctycolweight
    grid columnconfigure $scrollf.scrolled 5 -weight $watcolweight
    grid columnconfigure $scrollf.scrolled 6 -weight $hctcolweight
    grid columnconfigure $scrollf.scrolled 7 -weight $bricolweight
    grid columnconfigure $scrollf.scrolled 8 -weight $hthcolweight
    grid columnconfigure $scrollf.scrolled 9 -weight $rhccolweight

    pack $scrollf -expand 1 -fill both
    pack $scrolly -expand 1 -fill y -padx 2

    pack $bptsgui.f1.fr -expand 0 -fill y -side right
    pack $bptsguibptlist -expand 1 -fill both 

    frame $bptsgui.fb ; # contains the button at the bottom

    set bestwidth [mcmaxra "&Help" "&Close"]
    set buttonHelp $bptsgui.fb.buttonHelp
    eval "button $buttonHelp [bl "&Help"] \
            -command \"helpbptsgui\" \
            -font \[list $menuFont\] -width $bestwidth"
    set buttonClose $bptsgui.fb.buttonClose
    eval "button $buttonClose [bl "&Close"] \
            -command \"closebptsgui\" \
            -font \[list $menuFont\] -width $bestwidth"
    pack $buttonHelp $buttonClose -padx 40 -side left

    pack $bptsgui.fb -expand 0 -fill none -pady 5 -side bottom
    pack $bptsgui.f1 -expand 1 -fill both

    bind $bptsgui <Alt-[fb $buttonHelp]> "$buttonHelp invoke"

    # after 0 in the following Alt binding is mandatory for Linux only
    # This is Tk bug 1236306 (still unfixed in Tk8.4.15 and Tk 8.5a6)
    bind $bptsgui <Alt-[fb $buttonClose]> "after 0 {$buttonClose invoke}"

    bind $bptsgui <MouseWheel> {$bptsgui.f1.fl.sf yview scroll [expr {- (%D / 120) * 1}] units}

    if {$firstbptsguiopening} {
        update idletasks
        setwingeom $bptsgui
        wm resizable $bptsgui 1 1
    } else {
        wm geometry $bptsgui $bptsguigeom
    }

    wm deiconify $bptsgui

    # this *must* be after wm deiconify, otherwise [winfo height ...] still
    # returns 1 on Linux
    update ; # so that winfo height does not return 0
    set onelineheight [winfo height $bptsguibptlist.flabels.fun]
    $scrollf configure -height [expr {$onelineheight * $defaultnbdisplayedlines}]

    unset -- currentlyopeningbptsgui
}

proc addbptingui {ID} {
# insert the breakpoint identified by its ID $ID in the breakpoints gui,
# more precisely inside the container scrolledframe of this gui

    global bptsgui bptsguicolwidth bptconditiontypes bpthitconditions
    global textFont

    # do nothing if the breakpoints properties window is not open
    # it will be refreshed on opening
    if {![istoplevelopen bptsgui]} {
        return
    }

    foreach {funcolwidth lincolwidth enacolwidth \
             expcolwidth ctycolwidth watcolwidth \
             hctcolwidth bricolwidth hthcolwidth \
             rhccolwidth} $bptsguicolwidth {}

    set cfr $bptsgui.f1.fl.sf.scrolled

    label $cfr.funname$ID -font $textFont -relief groove -takefocus 0 \
            -textvariable bptsprops($ID,funname) \
            -width $funcolwidth
    label $cfr.funline$ID -font $textFont -relief groove -takefocus 0 \
            -textvariable bptsprops($ID,funline) \
            -width $lincolwidth
    checkbutton $cfr.enable$ID -font $textFont -relief groove \
            -variable bptsprops($ID,enable) -onvalue true -offvalue false \
            -width $enacolwidth \
            -command "tagconfigurebreakpointbackground $ID"
    entry $cfr.expression$ID -font $textFont \
            -textvariable bptsprops($ID,expression) \
            -width $expcolwidth \
            -validate key -validatecommand "validateconditioneditingui $ID"
    # -textvariable is not used in the spinbox, instead a separate procedure
    # is called through the -command option (see proc updatebptspropscondtype)
    spinbox $cfr.conditiontype$ID -font $textFont \
            -values [listlocalize $bptconditiontypes] \
            -command "updatebptspropscondtype $ID %s" \
            -state readonly -width $ctycolwidth
    updatespinboxexprcondtype $ID $cfr.conditiontype$ID
    checkbutton $cfr.watchit$ID -font $textFont -relief groove \
            -variable bptsprops($ID,watched) -onvalue true -offvalue false \
            -width $watcolwidth \
            -command "togglewatchcondbptexpr $ID"
    label $cfr.hitcount$ID -font $textFont -relief groove -takefocus 0 \
            -textvariable bptsprops($ID,hitcount) \
            -width $hctcolwidth
    # same comment for this spinbox as for $cfr.conditiontype$ID
    spinbox $cfr.hitcountcondition$ID -font $textFont \
            -values [listlocalize $bpthitconditions] \
            -command "updatebptspropshitconds $ID %s" \
            -state readonly -width $bricolwidth
    updatespinboxhitcountcondtype $ID $cfr.hitcountcondition$ID
    entry $cfr.hitthreshold$ID -font $textFont \
            -textvariable bptsprops($ID,hitcountthreshold) \
            -width $hthcolwidth -justify center \
            -validate key -validatecommand "validatehitthresholdeditingui %P"
    button $cfr.resethitcount$ID -font $textFont -relief groove -takefocus 0 \
            -command "resetonehitcount $ID" -text [mc "Reset hit"] \
            -width $rhccolwidth

    # find the row number where new entry should be inserted
    set insertrow [selectrowforsortedbpts $ID]
    # shift the existing rows one line down to make room for the new entry
    set lastrow [expr {[lindex [grid size $cfr] 1] - 1}]
    for {set i $lastrow} {$i >= $insertrow} {incr i -1} {
        set j [expr {$i + 1}]
        foreach aslave [grid slaves $cfr -row $i] {
            grid configure $aslave -row $j
        }
    }

    # grid the new entry in the empty space
    grid $cfr.funname$ID $cfr.funline$ID $cfr.enable$ID $cfr.expression$ID \
            $cfr.conditiontype$ID $cfr.watchit$ID $cfr.hitcount$ID \
            $cfr.hitcountcondition$ID $cfr.hitthreshold$ID $cfr.resethitcount$ID \
            -sticky nsew -row $insertrow

    # when changing something in the breakpoints gui one wants to show where
    # the breakpoint currently modified is located among the opened buffers
    bind $cfr.funname$ID           <Button-1> "showbreakpointedline $ID $cfr.funname$ID"
    bind $cfr.funline$ID           <Button-1> "showbreakpointedline $ID $cfr.funline$ID"
    # but not for check/spin boxes (because blinking delays the update)
    # bind $cfr.enable$ID           <Button-1> "showbreakpointedline $ID $cfr.enable$ID"
    #bind $cfr.expression$ID        <Button-1> "showbreakpointedline $ID $cfr.expression$ID"
    #bind $cfr.conditiontype$ID     <Button-1> "showbreakpointedline $ID $cfr.conditiontype$ID"
    #bind $cfr.watchit$ID           <Button-1> "showbreakpointedline $ID $cfr.watchit$ID"
    bind $cfr.hitcount$ID          <Button-1> "showbreakpointedline $ID $cfr.hitcount$ID"
    #bind $cfr.hitcountcondition$ID <Button-1> "showbreakpointedline $ID $cfr.hitcountcondition$ID"
    #bind $cfr.hitthreshold$ID      <Button-1> "showbreakpointedline $ID $cfr.hitthreshold$ID"
    #bind $cfr.resethitcount$ID     <Button-1> "showbreakpointedline $ID $cfr.resethitcount$ID"
}

proc delbptingui {ID} {
# remove breakpoint identified by its ID $ID from the breakpoints gui
    global bptsgui

    # do nothing if the breakpoints properties window is not open
    # it will be refreshed on opening
    if {![istoplevelopen bptsgui]} {
        return
    }

    set cfr $bptsgui.f1.fl.sf.scrolled

    # grid forget and shift upwards needed so that grid size is OK,
    # which in turn is the condition for the downwards shifting algorithm
    # in proc addbptingui to work
    set deleterow [getrowingridfromID $ID]
    set lastrow [expr {[lindex [grid size $cfr] 1] - 1}]
    grid forget $cfr.funname$ID $cfr.funline$ID $cfr.expression$ID $cfr.watchit$ID
    for {set i $deleterow} {$i <= $lastrow} {incr i} {
        set j [expr {$i + 1}]
        foreach aslave [grid slaves $cfr -row $j] {
            grid configure $aslave -row $i
        }
    }

    # note: destroy instruction mandatory here - see comments in proc deletebreakpoint
    destroy $cfr.funname$ID $cfr.funline$ID $cfr.enable$ID $cfr.expression$ID \
            $cfr.conditiontype$ID $cfr.watchit$ID $cfr.hitcount$ID \
            $cfr.hitcountcondition$ID $cfr.hitthreshold$ID $cfr.resethitcount$ID
}

proc getrowingridfromID {ID} {
# return the row number in which the breakpoint $ID is gridded in the
# breakpoints gui
# if $ID is not gridded, then return -1
    global bptsgui

    set cfr $bptsgui.f1.fl.sf.scrolled

    set isingrid [lsearch -exact [grid slaves $cfr] $cfr.funname$ID]

    if {$isingrid != -1} {
        set infos [grid info $cfr.funname$ID]
        set therow [lindex $infos [expr {[lsearch $infos "-row"] + 1}]]
    } else {
        set therow -1
    }
    return $therow    
}

proc selectrowforsortedbpts {ID} {
# from a breakpoint ID, return the row number in which this breakpoint should
# be gridded so that the grid is sorted alphabetically by funname and then by
# increasing funline
    global bptsprops bptsgui

    set fname $bptsprops($ID,funname)
    set fline $bptsprops($ID,funline)

    set row 0

    set allfunnames [getfunnamesfrombptsprops]
    set funlinesoffname [getfunlinesfrombptsprops $fname]

    # remove what we're searching from the lists of line numbers
    # and function names
    # this is necessary since when proc selectrowforsortedbpts
    # is called the bptsprops array has already been augmented
    # with the new breakpoint, and the algorithm below supposes
    # that (fname,fline) is not (yet) part of bptsprops
    set flinepos [lsearch -exact $funlinesoffname $fline]
    set funlinesoffname [lreplace $funlinesoffname $flinepos $flinepos]

    # same thing: $funlinesoffname must only contain the breakpointed
    # lines already displayed in the bpts gui
    set linestokeep [list ]
    foreach thisfline $funlinesoffname {
        set rowingrid [getrowingridfromID [where2bptID $fname $thisfline]]
        if {$rowingrid != -1} {
            lappend linestokeep $thisfline
        }
    }
    set funlinesoffname $linestokeep

    # now, if there is no remaining breakpointed line for $fname,
    # remove also $fname from $allfunnames
    if {$funlinesoffname == {}} {
        set fnamepos [lsearch -exact $allfunnames $fname]
        set allfunnames [lreplace $allfunnames $fnamepos $fnamepos]
    }

    foreach nameoffun $allfunnames {
        switch -- [string compare $nameoffun $fname] {
            -1 {
                 incr row [llength [getfunlinesfrombptsprops $nameoffun]]
                 continue
               }
             0 {
                 set i 0
                 while {([lindex $funlinesoffname $i] < $fline) && ($i < [llength $funlinesoffname])} {incr i}
                 incr row $i
                 break
               }
             1 {
                 break
               }
        }
    }

    return $row
}

proc validateconditioneditingui {ID} {
# this proc is used to maintain the display in the main Scipad window in sync
# with the changes made in the expression area of the breakpoints properties gui
    global bptsprops

    # maintain consistency between the breakpoints gui and the watch window
    # with no headache! if the expression was currently watched, then remove
    # the watching first and let the user set it again afterwards if he still
    # wants it
    if {$bptsprops($ID,watched)} {
        set bptsprops($ID,watched) false
        togglewatchcondbptexpr $ID
    }

    # the new condition value needs to be updated in bptsprops($ID,curexpvalue)
    # so that the next check in proc checkendofdebug_bp will use the correct
    # "value at previous stop"
    # executing this is fast enough, even with validation happening on each
    # keypress in the condition area of the bptsgui
    saveoneconditionvalue $ID

    # the after 0 trick is needed to have the validation result (i.e. true)
    # be sent to the entry widget first, so that the edit gets validated
    # and the textvariable bptsprops(...) receives the new value before
    # the tagging stuff occurs
    after 0 "tagconfigurebreakpointbackground $ID"

    # any content is allowed, error checking happens
    # when executing the statement at each stop (see proc checkendofdebug_bp)
    return true
}

proc updatebptspropscondtype {ID newconditiontype} {
# update bptsprops($ID,conditiontype)
# this proc is called whenever a button is clicked in the condition type
# spinbox of the bpts gui
# one could have obtained the same effect through -textvariable in the
# spinbox definition, but then one would have had to deal with the locale
# whenever bptsprops($ID,conditiontype) is used thoughout the code
# with this approach only the display in the spinbox is localized, and
# the internal value of bptsprops($ID,conditiontype) has only to be used
# in english (and with strings that do not need to be identical to the
# displayed ones)
    global bptsprops bptconditiontypes

    set pos [lsearch -exact [listlocalize $bptconditiontypes] $newconditiontype]
    # $pos == -1 should not happen, i.e. the espression condition type newly selected
    # in the spinbox should be found in the localized list of esxpression condition types

    set bptsprops($ID,conditiontype) [lindex $bptconditiontypes $pos]
}

proc updatespinboxexprcondtype {ID w} {
# update condition type displayed in the spinbox so that it matches
# $bptsprops($ID,conditiontype)
    global bptsprops

    $w set [mc $bptsprops($ID,conditiontype)]
}

proc validatehitthresholdeditingui {proposedcontent} {
# return true if and only if the proposed new full content $newcontent that
# the user tries to have in the threshold entry area is a positive or zero
# integer number
# moreover, the user is not allowed to delete completely the entry
# note: string is integer -strict $newcontent is not enough because
#       this allows for negative integers, and also for leading zeroes

    global strictlypositiveintegerREpat

    return [regexp $strictlypositiveintegerREpat $proposedcontent]
}

proc updatebptspropshitconds {ID newhitcondition} {
# this is the same as proc updatebptspropscondtype, but for the hit count
# condition type (while proc updatebptspropscondtype deals with the
# breakpoint expression condition type)
# <TODO> factorize this code with proc updatebptspropscondtype
    global bptsprops bpthitconditions

    set pos [lsearch -exact [listlocalize $bpthitconditions] $newhitcondition]
    # $pos == -1 should not happen, i.e. the hit condition type newly selected
    # in the spinbox should be found in the localized list of hit condition types

    set bptsprops($ID,hitcountcondition) [lindex $bpthitconditions $pos]
}

proc updatespinboxhitcountcondtype {ID w} {
# update hit count condition type displayed in the spinbox so that it matches
# $bptsprops($ID,hitcountcondition)
    global bptsprops

    $w set [mc $bptsprops($ID,hitcountcondition)]
}

proc togglewatchcondbptexpr {ID} {
# toggle the watching of the conditional expression of breakpoint identified
# by $ID
# the watching consists in adding a generic expression in the watch window
# and in maintaining consistency between the expression displayed in the
# breakpoints gui and in the watch window (the latter is done throughout
# the code, not in this proc)
    global bptsprops
    global watchgenexps

    if {$bptsprops($ID,watched)} {
        # now watch expression from breakpoint $ID
        # duplicates are dealt with in proc OKadda_bp
        quickAddWatch_bp $bptsprops($ID,expression) genericexpression

    } else {
        # stop watching expression from breakpoint $ID
        # before removing the expression from the watch window, check that
        # no other breakpoint currently watched has the same expression
        set mustremove true
        foreach otherID [getusedIDsfrombptprops] {
            if {$otherID == $ID} {continue}
            if {$bptsprops($otherID,watched) &&
                $bptsprops($otherID,expression) == $bptsprops($ID,expression)} {
                set mustremove false
                break
            }
        }
        if {$mustremove} {
            # remove the expression from the watch
            set posinwatchgenexps [lsearch -exact $watchgenexps $bptsprops($ID,expression)]
            # $posinwatchgenexps should never be -1 if consistency is maintained correctly
            # between the watch window and the breakpoints gui, which should be the case
            set watchgenexps [lreplace $watchgenexps $posinwatchgenexps $posinwatchgenexps]
        }
    }
}

proc unwatchallcondbptexpr {exp} {
# uncheck the "Watched" checkbox of all breakpoints having $exp
# as conditional expression
    global bptsprops
    foreach ID [getusedIDsfrombptprops] {
        if {$bptsprops($ID,expression) == $exp} {
            set bptsprops($ID,watched) false
            # here don't call proc togglewatchcondbptexpr, because the current
            # proc is already called by proc Removearg_bp, i.e. the corresponding
            # expression is already in the process of being deleted from the
            # watch window
        }
    }
}

proc closebptsgui {} {
    global bptsgui bptsguigeom

    # save size and position of the window for future restoration
    set bptsguigeom [wm geometry $bptsgui]

    # one must delete the widgets (see proc deletebreakpoint for an
    # approximate reason)
    foreach ID [getusedIDsfrombptprops] {
        delbptingui $ID
    }

    destroy $bptsgui
}

proc helpbptsgui {} {
# a dialog explaining a bit the vocabulary about breakpoints conditions
    global bptsgui
    set tit [mc "Breakpoints vocabulary"]
    set mes [concat [mc "A breakpoint is identified by its location, i.e. its position (a line number) in a function."] \
                    [mc "When the execution process encounters an enabled breakpoint, then this breakpoint is said to be reached."] \
                    [mc "When the breakpoint is reached and its associated conditional expression is true (or changed),\
                         then the breakpoint is said to be hit."] \
                    [mc "The hit count is the number of times the breakpoint has been hit."] \
                    [mc "Execution will stop at a breakpoint if the hit count satisfies the selectable break condition."] \
            ]
    tk_messageBox -message $mes -icon info -title $tit -parent $bptsgui
}

proc showbreakpointedline {ID focuswidget} {
# show the piece of code that contains the breakpoint identified by $ID
    global bptsprops
    after idle "blinkline $bptsprops($ID,funline) $bptsprops($ID,funname) 1 ; focus $focuswidget"
}

proc highlightbptinbptsgui {ID} {
# colorize in red the line in $bptsgui that contains the informations related to 
# breakpoint with ID $ID
    global bptsgui
    global oldcolorfunnameinbptsgui oldcolorfunlineinbptsgui

    # do nothing if the breakpoints properties window is not open
    if {![istoplevelopen bptsgui]} {
        return
    }

    set cfr $bptsgui.f1.fl.sf.scrolled

    set oldcolorfunnameinbptsgui [$cfr.funname$ID cget -foreground]
    set oldcolorfunlineinbptsgui [$cfr.funline$ID cget -foreground]

    $cfr.funname$ID configure -foreground red
    $cfr.funline$ID configure -foreground red

    # <TODO> Should it scroll the bpts list so that breakpoint $ID
    #        becomes visible if it was hidden? Sort of a $ID see...
    #        I'd say no...
}

proc unhighlightbptinbptsgui {ID} {
# colorize in normal color the line in $bptsgui that contains the informations 
# related to breakpoint with ID $ID
    global bptsgui
    global oldcolorfunnameinbptsgui oldcolorfunlineinbptsgui

    # do nothing if the breakpoints properties window is not open
    if {![istoplevelopen bptsgui]} {
        return
    }

    set cfr $bptsgui.f1.fl.sf.scrolled

    # when the breakpoints gui is opened the first time with the mouse cursor
    # in a breakpoint, then the old colors are undefined
    # therefore catch this
    catch {
        $cfr.funname$ID configure -foreground $oldcolorfunnameinbptsgui
        $cfr.funline$ID configure -foreground $oldcolorfunlineinbptsgui
    }
}

proc sortbptsgui {nameoffun} {
# sort the entries in the breakpoints properties gui
# this is not a real sorting algorithm, in the sense that is won't sort
# an arbitrarily messed up breakpoints gui
# when entering this proc the gui is supposed to be perfectly sorted,
# the only entries that might be not sorted are those having $nameoffun
# as function name

    # do nothing if the breakpoints properties window is not open
    # it will be refreshed on opening
    if {![istoplevelopen bptsgui]} {
        return
    }

    # algorithm :
    #   . retrieve the IDs of entries having $nameoffun
    #   . delete these entries from the gui
    #   . add them again (this looks for the right insert place)
    foreach aline [getfunlinesfrombptsprops $nameoffun] {
        set ID [where2bptID $nameoffun $aline]
        delbptingui $ID
        addbptingui $ID
    }
}
