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
proc setScipadVersionString {} {
# set the version string for Scipad, using the information from the
# VERSION file of the Scipad module
    global ScipadVersion
    # empty placeholder for when launched outside of Scilab,
    # or for filling in manually when backporting Scipad from
    # the trunk to the BUILD4 environment (BUILD4 does not
    # understand instruction getversion)
    set ScipadVersion ""
    # try/end so that this code portion can be kept with no change
    # even in a backported version
    set comm1 "try;"
    set comm2   "ScipadVer=getversion(\"scipad\");"
    set comm3   "if ScipadVer(3)==0 then"
    set comm4     "TCL_EvalStr(\"set ScipadVersion \"\"\"+string(ScipadVer(1))+\".\"+string(ScipadVer(2))+\" - \"+getversion(\"scipad\",\"string_info\")+\"\"\"\",\"scipad\");"
    set comm5   "else"
    set comm6     "TCL_EvalStr(\"set ScipadVersion \"\"\"+string(ScipadVer(1))+\".\"+string(ScipadVer(2))+\".\"+string(ScipadVer(3))+\" - \"+getversion(\"scipad\",\"string_info\")+\"\"\"\",\"scipad\");"
    set comm7   "end;"
    set comm8 "end;"
    # update title bar
    set comm9 "TCL_EvalStr(\"modifiedtitle [gettextareacur]\",\"scipad\");"
    # <TODO> SCI_VERSION_REVISION is not used until some automatic way to
    #        fill in this field at commit exists
    #        Well, there is one:  svn:keywords  with  $Revision$
    set fullcomm [concat $comm1 $comm2 $comm3 $comm4 $comm5 $comm6 $comm7 $comm8 $comm9]
    # Warning: "sync" "seq" would have been the natural options to use,
    #          but see proc loadwords for some explanations about why
    #          the sole seq is mandatory
    ScilabEval_lt $fullcomm "seq"
}

proc aboutme {} {
# about
    global winTitle ScipadVersion
    tk_messageBox -title [mc "About"] -type ok -message \
        " $winTitle  $ScipadVersion - Tcl/Tk [info patchlevel] \n [mc aboutme_message] \n\n [mc Scipad_is_released_under_GPL]"
}

proc helpme {} {
# help
    if {[isscilabbusy 0]} {return}
    ScilabEval_lt "help scipad"
}

proc helpword {} {
    if {[isscilabbusy 0]} {return}
    set textareacur [gettextareacur]
    # if there is a block selection, collapse it to its first range
    set selindices [gettaselind $textareacur single]
    if {$selindices == {}} {
        # if there is no selection in the current textarea,
        # select the word at the cursor position
        set i1 [$textareacur index insert]
        $textareacur tag add sel [$textareacur index "$i1 wordstart"] \
                                 [$textareacur index "$i1 wordend"]
        set selindices [gettaselind $textareacur single]
    }
    set cursel [string trim [gettatextstring $textareacur $selindices]]
    # get only the first word of the selection (or a symbol)
    regexp "(\\A\\w*\\M|\\A\\W)" $cursel curterm
    if {[info exists curterm]} {
        set curterm [string trim $curterm]
        set curterm [duplicatechars $curterm "\""]
        set curterm [duplicatechars $curterm "'"]
        if {$curterm!=""} {
            ScilabEval_lt "help \"$curterm\""
        }
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
    wm withdraw $tbox
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
            -font $menuFont
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
    # prevent unwanted Text class bindings from triggering
    bind $tbox.text <Button-3> {break}
    bind $tbox.text <Shift-Button-3> {break}
    bind $tbox.text <Control-Button-3> {break}
    bind $tbox.text <ButtonRelease-2> {break}
    bind $tbox.text <Return> "destroy $tbox;break"
    bind $tbox.text <KP_Enter> "destroy $tbox;break"
    update idletasks
    setwingeom $tbox
    wm resizable $tbox 1 1
    wm deiconify $tbox
}
