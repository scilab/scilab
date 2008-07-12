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
proc choosefonts {} {
# pop up the font chooser dialog
    global pad textFont menuFont
    global fontweight fontslant fontunderline
    global textfontsize usefontfor scalefontsize

    # preset values retrieved from $textFont
    set textfontattributes [fontactual_positivesize $textFont]
    if {[lsearch $textfontattributes "-family"] != -1} {
        set fontfamily [lindex $textfontattributes \
                [expr {[lsearch $textfontattributes "-family"] + 1}] ]
    } else {
        set fontfamily ""
    }
    if {[lsearch $textfontattributes "-size"] != -1} {
        set textfontsize [lindex $textfontattributes \
                [expr {[lsearch $textfontattributes "-size"] + 1}] ]
    } else {
        set textfontsize 12
    }
    # positive value for -size is interpreted as a size in points, but
    # $textfontsize is a value in pixels
    # therefore use tk scaling to get the ratio between points and pixels
    set textfontsize [expr {round($textfontsize * [tk scaling])}]
    if {[lsearch $textfontattributes "-weight"] != -1} {
        set fontweight [lindex $textfontattributes \
                [expr {[lsearch $textfontattributes "-weight"] + 1}] ]
    } else {
        set fontweight "normal"
    }
    if {[lsearch $textfontattributes "-slant"] != -1} {
        set fontslant [lindex $textfontattributes \
                [expr {[lsearch $textfontattributes "-slant"] + 1}] ]
    } else {
        set fontslant "roman"
    }
    if {[lsearch $textfontattributes "-underline"] != -1} {
        set fontunderline [lindex $textfontattributes \
                [expr {[lsearch $textfontattributes "-underline"] + 1}] ]
    } else {
        set fontunderline 0
    }
    # overstrike option not retrieved nor used

    # create geometry and widgets
    set fch $pad.fch
    catch {destroy $fch}
    toplevel $fch
    wm title $fch [mc "Choose font"]
    wm withdraw $fch

    frame $fch.f0

    labelframe $fch.f0.lf1 -text [mc "Family"] -font $menuFont
    frame $fch.f0.lf1.f1
    frame $fch.f0.lf1.f5
    scrollbar $fch.f0.lf1.f1.scrollfontsy -command "$fch.f0.lf1.f1.lb yview" -takefocus 0
    scrollbar $fch.f0.lf1.f5.scrollfontsx -command "$fch.f0.lf1.f1.lb xview" -orient horizontal -takefocus 0
    listbox $fch.f0.lf1.f1.lb -height 6 -width 20 -font $textFont \
            -yscrollcommand "$fch.f0.lf1.f1.scrollfontsy set" \
            -xscrollcommand "$fch.f0.lf1.f5.scrollfontsx set" \
            -takefocus 0 -selectmode single -exportselection 0
    foreach family [lsort [font families]] {
        $fch.f0.lf1.f1.lb insert end $family
    }
    set fontexists [lsearch -exact [lsort [font families]] $fontfamily]
    if {$fontexists == -1} {
        set fontexists 0
    }
    $fch.f0.lf1.f1.lb selection set $fontexists
    $fch.f0.lf1.f1.lb see $fontexists
    pack $fch.f0.lf1.f1.lb $fch.f0.lf1.f1.scrollfontsy -side left -expand 1 -fill y
    pack configure $fch.f0.lf1.f1.scrollfontsy -side top
    pack $fch.f0.lf1.f1 -expand 1 -fill both
    pack $fch.f0.lf1.f5.scrollfontsx -expand 1 -fill x
    pack configure $fch.f0.lf1.f5.scrollfontsx -side left
    pack $fch.f0.lf1.f5  -side left -expand 1 -fill x
    pack configure $fch.f0.lf1.f1.lb -expand 1 -fill both

    frame $fch.f0.f2
    labelframe $fch.f0.f2.lf2 -text [mc "Style"] -font $menuFont
    eval "checkbutton $fch.f0.f2.lf2.cbox0 [bl "&Bold"] \
        -variable fontweight -font \[list $menuFont\] -command \"updatesamplefont\" \
        -offvalue normal -onvalue bold  -takefocus 0"
    eval "checkbutton $fch.f0.f2.lf2.cbox1 [bl "&Italic"] \
        -variable fontslant -font \[list $menuFont\] -command \"updatesamplefont\" \
        -offvalue roman -onvalue italic  -takefocus 0"
    eval "checkbutton $fch.f0.f2.lf2.cbox2 [bl "&Underline"] \
        -variable fontunderline -font \[list $menuFont\] -command \"updatesamplefont\" \
        -offvalue 0 -onvalue 1  -takefocus 0"
    pack $fch.f0.f2.lf2.cbox0 $fch.f0.f2.lf2.cbox1 $fch.f0.f2.lf2.cbox2 -anchor w

    scale $fch.f0.f2.fsize -orient horizontal -resolution 1 -bigincrement 5 \
            -sliderlength 20 -from 5 -to 50 -label [mc "Size"] -font $menuFont \
            -length 20 -showvalue yes -variable scalefontsize \
            -command "updatesamplefont" -takefocus 0
    pack $fch.f0.f2.lf2 $fch.f0.f2.fsize -side top -expand 1 -fill x -anchor n
    $fch.f0.f2.fsize set $textfontsize

    frame $fch.f0.f3
    set bestwidth [mcmaxra "OK" \
                           "Cance&l" \
                           "&Defaults"]
    eval "button $fch.f0.f3.button1 [bl "OK"] \
        -command \"updatefont;destroy $fch\" \
        -width $bestwidth -font \[list $menuFont\] "
    eval "button $fch.f0.f3.button2 [bl "Cance&l"] \
        -command \"destroy $fch\" \
        -width $bestwidth -font \[list $menuFont\] \
         -takefocus 0 "
    eval "button $fch.f0.f3.button3 [bl "&Defaults"] \
        -command \"revertfont\" \
        -width $bestwidth -font \[list $menuFont\] \
         -takefocus 0 "
    pack $fch.f0.f3.button1 $fch.f0.f3.button2 $fch.f0.f3.button3 -expand 0 -fill none -padx 10 -pady 5

    labelframe $fch.f0.f3.lf3 -text [mc "Use as..."] -font $menuFont
    eval "radiobutton $fch.f0.f3.lf3.tf [bl "&Text font"] \
        -variable usefontfor -value \"textfont\" \
        -command \"\" -font \[list $menuFont\] -takefocus 0 "
    eval "radiobutton $fch.f0.f3.lf3.mf [bl "&Menu font"] \
        -variable usefontfor -value \"menufont\" \
        -command \"\" -font \[list $menuFont\] -takefocus 0 "
    pack $fch.f0.f3.lf3.tf $fch.f0.f3.lf3.mf -anchor w
    pack $fch.f0.f3.lf3 -pady 10
    $fch.f0.f3.lf3.tf select

    pack $fch.f0.lf1 $fch.f0.f2 $fch.f0.f3 -pady 2 -padx 5 -side left -expand 0 -fill both

    labelframe $fch.lf4 -text [mc "Sample"] -font $menuFont
    entry $fch.lf4.text -background [$fch.lf4 cget -background] \
            -justify center -relief flat -font $textFont -takefocus 0
    $fch.lf4.text insert 0 "ABC abc 123 #~[]"
    pack $fch.lf4.text -expand 1 -fill both

    pack $fch.f0 $fch.lf4 -side top -padx 5 -pady 2 -expand 1 -fill both

    bind $fch.f0.lf1.f1.lb <ButtonRelease-1> {updatesamplefont}

    bind $fch <MouseWheel> {if {%D<0} {scrollarrows_bp [winfo toplevel %W].f0.lf1.f1.lb down} \
                                      {scrollarrows_bp [winfo toplevel %W].f0.lf1.f1.lb up} ; \
                            updatesamplefont }
    bind $fch <Up>   {scrollarrows_bp [winfo toplevel %W].f0.lf1.f1.lb up   ; updatesamplefont}
    bind $fch <Down> {scrollarrows_bp [winfo toplevel %W].f0.lf1.f1.lb down ; updatesamplefont}

    bind $fch <Left>          {set sc [winfo toplevel %W].f0.f2.fsize ; \
                               $sc set [expr {[$sc get] - [$sc cget -resolution]} ]}
    bind $fch <Right>         {set sc [winfo toplevel %W].f0.f2.fsize ; \
                               $sc set [expr {[$sc get] + [$sc cget -resolution]} ]}
    bind $fch <Control-Left>  {set sc [winfo toplevel %W].f0.f2.fsize ; \
                              $sc set [expr {[$sc get] - [$sc cget -bigincrement]} ]}
    bind $fch <Control-Right> {set sc [winfo toplevel %W].f0.f2.fsize ; \
                              $sc set [expr {[$sc get] + [$sc cget -bigincrement]} ]}

    bind $fch <Return> {[winfo toplevel %W].f0.f3.button1 invoke}
    bind $fch <Escape> {[winfo toplevel %W].f0.f3.button2 invoke}
    bind $fch <Alt-[fb $fch.f0.f3.button1]> {[winfo toplevel %W].f0.f3.button1 invoke}
    bind $fch <Alt-[fb $fch.f0.f3.button2]> {[winfo toplevel %W].f0.f3.button2 invoke}
    bind $fch <Alt-[fb $fch.f0.f3.button3]> {[winfo toplevel %W].f0.f3.button3 invoke}
    bind $fch <Alt-[fb $fch.f0.f2.lf2.cbox0]> {[winfo toplevel %W].f0.f2.lf2.cbox0 invoke}
    bind $fch <Alt-[fb $fch.f0.f2.lf2.cbox1]> {[winfo toplevel %W].f0.f2.lf2.cbox1 invoke}
    bind $fch <Alt-[fb $fch.f0.f2.lf2.cbox2]> {[winfo toplevel %W].f0.f2.lf2.cbox2 invoke}
    bind $fch <Alt-[fb $fch.f0.f3.lf3.tf]> {[winfo toplevel %W].f0.f3.lf3.tf invoke}
    bind $fch <Alt-[fb $fch.f0.f3.lf3.mf]> {[winfo toplevel %W].f0.f3.lf3.mf invoke}

    focus $fch.f0.f3.button1
    update idletasks
    setwingeom $fch
    wm deiconify $fch
}

proc updatesamplefont {{notused ""}} {
# update the sample text font in the font chooser dialog
# return value: the new font described as a {-option value ... } list
    global pad
    global fontweight fontslant fontunderline scalefontsize
    set fch $pad.fch
    set newfont [list "-family" [$fch.f0.lf1.f1.lb get [$fch.f0.lf1.f1.lb curselection]]]
    # positive value for -size is interpreted as a size in points, but
    # $scalefontsize is a value in pixels
    # therefore use tk scaling to get the ratio between points and pixels
    lappend newfont "-size" [expr {round($scalefontsize / [tk scaling])}]
    lappend newfont "-weight" $fontweight
    lappend newfont "-slant" $fontslant
    lappend newfont "-underline" $fontunderline
    $fch.lf4.text configure -font $newfont
    return $newfont
}

proc updatefont {{fontsource "newfont"}} {
# if $fontsource == "newfont", apply the font selected in the chooser,
# otherwise update all fonts taking into account their new sizes
    global textFont menuFont actbptextFont textsmallerFont
    global pad listoftextarea watch firsttimeinshowwatch
    global showwatchvariablesarea showcallstackarea    
    global usefontfor textfontsize menufontsize
    global linenumbersmargins

    if {$fontsource == "newfont"} {
        if {$usefontfor == "textfont"} {
            set textFont [updatesamplefont]
            if {[lsearch $textFont "-size"] != -1} {
                set textfontsize [lindex $textFont \
                        [expr {[lsearch $textFont "-size"] + 1}] ]
                set textfontsize [expr {round($textfontsize * [tk scaling])}]
            } else {
                set textfontsize 12
            }
       } else {
            # $usefontfor == "menufont"
            set menuFont [updatesamplefont]
            if {[lsearch $menuFont "-size"] != -1} {
                set menufontsize [lindex $menuFont \
                        [expr {[lsearch $menuFont "-size"] + 1}] ]
                set menufontsize [expr {round($menufontsize * [tk scaling])}]
            } else {
                set menufontsize 12
            }
        }
    } else {
        # $fontsource == "all"
        # this is used when updating the size only
        set sizeinpoints [expr {round($textfontsize / [tk scaling])}]
        set textFont [fontactual_positivesize $textFont]
        set sizevalueindex [expr {[lsearch $textFont "-size"] + 1}]
        set textFont [lreplace $textFont $sizevalueindex $sizevalueindex $sizeinpoints]
        set sizeinpoints [expr {round($menufontsize / [tk scaling])}]
        set menuFont [fontactual_positivesize $menuFont]
        set sizevalueindex [expr {[lsearch $menuFont "-size"] + 1}]
        set menuFont [lreplace $menuFont $sizevalueindex $sizevalueindex $sizeinpoints]
    }

    # $actbptextFont is $textFont in bold and with a larger size
    set actbptextFont [fontactual_positivesize $textFont]
    set sizevalueindex [expr {[lsearch $actbptextFont "-size"] + 1}]
    set actbptextFont [lreplace $actbptextFont $sizevalueindex $sizevalueindex \
            [expr {round( ($textfontsize + round(2*[tk scaling])) / [tk scaling] )} ]]
    set weightvalueindex [expr {[lsearch $actbptextFont "-weight"] + 1}]
    set actbptextFont [lreplace $actbptextFont $weightvalueindex $weightvalueindex "bold"]

    # $textsmallerFont is $textFont with reduced size
    set textsmallerFont [fontactual_positivesize $textFont]
    set sizevalueindex [expr {[lsearch $textsmallerFont "-size"] + 1}]
    set textsmallerFont [lreplace $textsmallerFont $sizevalueindex $sizevalueindex \
            [expr {round( ($textfontsize - round(2*[tk scaling])) / [tk scaling] )} ]]

    # change the font of all of the entries in the menu tree and status bar
    set allmenus1 "$pad.statusind $pad.statusind2 $pad.statusmes $pad.filemenu"
    set allmenus ""
    while {$allmenus != $allmenus1} {
        set allmenus $allmenus1
        foreach m "$allmenus" { append allmenus1 { } [winfo children $m] }
        set allmenus1 [lsort -unique $allmenus1]
    }
    foreach m "$allmenus" {$m configure -font $menuFont}

    # this must be done for all textareas, including peers
    foreach textarea $listoftextarea {
        $textarea configure -font $textFont
        $textarea tag configure activebreakpoint -font $actbptextFont
        if {[isdisplayed $textarea]} {
            set tapwfr [getpaneframename $textarea]
            $tapwfr.panetitle configure -font $menuFont
            $tapwfr.clbutton  configure -font $menuFont
            $tapwfr.hibutton  configure -font $menuFont
            if {$linenumbersmargins != "hide"} {
                $tapwfr.margin configure -font $textFont
            }
        }
    }
    settabsize

    # This sets the font used in all dialogs (Unix only - on Windows
    # native platform dialogs are used by Tk)
    # $dialogfont is $menuFont in normal weight (non-bold)
    set dialogFont [fontactual_positivesize $menuFont]
    set weightvalueindex [expr {[lsearch $dialogFont "-weight"] + 1}]
    set dialogFont [lreplace $dialogFont $weightvalueindex $weightvalueindex "normal"]
    option add *Dialog.msg.font    $dialogFont userDefault  ; # for all tk_messageBox and tk_dialog
    option add *TkFDialog*Font     $dialogFont userDefault  ; # file open / save as dialogs
    option add *TkColorDialog*Font $dialogFont userDefault  ; # color picker dialog

    # If the watch window was open, refresh it
    if {[istoplevelopen watch]} {
        set saved_showwatchvariablesarea $showwatchvariablesarea
        set saved_showcallstackarea $showcallstackarea
        set firsttimeinshowwatch true
        # the two following flags must be reset,
        # otherwise Tcl error "invalid sash index"
        # in proc showwatch_bp
        set showwatchvariablesarea true
        set showcallstackarea true
        showwatch_bp
        # restore hide/show state for each area
        if {!$saved_showwatchvariablesarea} {
            $watch.f.f1.f1r.showwatchvariablesarea invoke
        }
        if {!$saved_showcallstackarea} {
            $watch.f.f1.f1r.showcallstackarea invoke
        }
    }

    # If the breakpoints properties window was open, refresh it
    if {[istoplevelopen bptsgui]} {
        showbptgui_bp
    }

    showinfo [concat [mc "Font size"] $textfontsize ]
}

proc revertfont {} {
# reset fonts to default ones
    global pad
    set answ [tk_messageBox -icon question -type yesno -parent $pad.fch \
            -title [mc "Revert fonts"] \
            -message [mc "Are you sure you want to revert all fonts to defaults?"] ]
    switch -- $answ {
        yes {
              setdefaultfonts
              updatefont all
              destroy $pad.fch
            }
        no  { }
    }
}

proc setdefaultfonts {} {
    global textfontsize menufontsize textFont menuFont tcl_platform

    # $textfontsize is in pixels, not in points
    set textfontsize 12
    set menufontsize $textfontsize

    if {$tcl_platform(platform) == "unix"} {
        # format is  -foundry-family-weight-slant-setwidth-addstyle-pixel-point-resx-resy-spacing-width-charset-encoding
        set textFont [fontactual_positivesize -Adobe-courier-medium-r-Normal-*-$textfontsize-*]
        set menuFont [fontactual_positivesize -adobe-helvetica-bold-r-normal--$menufontsize-*-75-75-*-*-*-*]
    } else {
        set textFont [fontactual_positivesize -Adobe-Courier-medium-R-Normal-*-$textfontsize-*]
        set menuFont [fontactual_positivesize -adobe-helvetica-bold-r-normal--$menufontsize-*-75-75-*-*-*-*]
    }
}

proc settabsize {} {
# update the tab size in all the existing text widgets
    global textfontsize tabsizeinchars listoftextarea
    set tabsizeinpix [expr {$textfontsize / [tk scaling] / [tk scaling] * $tabsizeinchars}]
    # this must be done for all textareas, including peers
    foreach textarea $listoftextarea {
        $textarea configure -tabs $tabsizeinpix
    }
}

proc fontactual_positivesize {afont} {
# this proc applies font actual to $afont and then ensures that the value
# of the -size parameter is positive (negate it if it's negative, and
# convert from pixels into points by dividing by [tk scaling])
# this is needed because throughout the Scipad code it is supposed that the
# size option of any font is positive, i.e. in points (not in pixels)
# fonts are lists of options which are parsed at a number of places in the
# code, and negative sizes are not taken into account
# this proc is to fix bug 2879
    set fa [font actual $afont]
    set sizeoptpos [lsearch $fa "-size"]
    if {$sizeoptpos != -1} {
        set sizevalpos [expr {$sizeoptpos + 1}]
        set fsize [lindex $fa $sizevalpos]
        if {$fsize < 0} {
            set fa [lreplace $fa $sizevalpos $sizevalpos [expr {round(- $fsize / [tk scaling ])}]]
        } else {
            # size is already positive, nothing to do
        }
    } else {
        # no -size option - nothing to do
    }
    return $fa
}
