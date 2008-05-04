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
proc showpopup2 {} {
    global pad mouseoversel snRE
    set numx [winfo pointerx $pad]
    set numy [winfo pointery $pad]
    # if there is no debug session, popup menu is the edit menu
    # otherwise it is the debug menu
    if {[getdbstate] == "NoDebug"} {
        tk_popup $pad.filemenu.edit $numx $numy
    } else {
        set ta [gettextareacur]
        # If there is a selection, and if the selected trimmed string matches
        # a regexp (constructed from help names in Scilab) then the selection
        # is probably a valid variable to watch and the quick add watch menu
        # should pop up
        # If the selected trimmed string doesn't match the regexp, then it is
        # proposed for addition in the generic expression area
        # about block selections: they are collapsed to their first range
        # (line) before trying to match them against the regexp
        if {$mouseoversel == "true"} {
            set selindices [gettaselind $ta single]
            set trimmedseltext [string trim [gettatextstring $ta $selindices]]
            regexp "\\A$snRE\\Z" $trimmedseltext validwatchvar
            if {[info exists validwatchvar]} {
                showpopupdebugwsel $validwatchvar "watchvariable"
            } else {
                showpopupdebugwsel $trimmedseltext "genericexpression"
            }
        } else {
            tk_popup $pad.filemenu.debug $numx $numy
        }
    }
}

proc showpopup3 {} {
    global pad 
    set numx [winfo pointerx $pad]
    set numy [winfo pointery $pad]
    tk_popup $pad.filemenu.exec $numx $numy
}

proc showpopupfont {} {
    global pad
    set numx [winfo pointerx $pad]
    set numy [winfo pointery $pad]
    tk_popup $pad.filemenu.options $numx $numy
}

proc showpopupsource {ind} {
    global pad menuFont words
    set textareacur [gettextareacur]
    set numx [winfo pointerx $pad]
    set numy [winfo pointery $pad]
    catch {destroy $pad.popsource}
    set tagname ""
    if {[lsearch [$textareacur tag names $ind] "libfun"] != -1} {
        set tagname libfun
    } elseif {[lsearch [$textareacur tag names $ind] "scicos"] != -1} {
        set tagname scicos
    } elseif {[lsearch [$textareacur tag names $ind] "userfun"] != -1} {
        set tagname userfun
    } else {
        return
    }
    set lrange [$textareacur tag prevrange $tagname "$ind+1c"]
    if {$lrange==""} {set lrange [$textareacur tag nextrange $tagname $ind]}
    set curterm [$textareacur get [lindex $lrange 0] [lindex $lrange 1]]
    if {[info exists curterm]} {
        set curterm [string trim $curterm]
        if {$curterm!=""} {
            if {$tagname == "userfun"} {
                set nameinitial [string range $curterm 0 0]
                set candidates $words(scilab.$tagname.$nameinitial)
                for {set i 0} {$i<[llength $candidates]} {incr i} {
                    if {[lindex [lindex $candidates $i] 0] == $curterm} {
                        set plabel [concat [mc "Jump to"] $curterm ]
                        set sourcecommand \
                            "dogotoline \"physical\" 1 \"function\" [list [lindex $candidates $i]]"
                        break
                    }
                }
            } else {
                # scicos or libfun
                set plabel [concat [mc "Open the source of"] $curterm ]
                set sourcecommand "opensourcecommand $tagname $curterm"
            }
            menu $pad.popsource -tearoff 0 -font $menuFont
            $pad.popsource add command -label $plabel -command $sourcecommand
            tk_popup $pad.popsource $numx $numy
       }
    }
}

proc opensourcecommand {tagname curterm} {
    if {![isscilabbusy 0]} {
        doopenfunsource $tagname $curterm
    }
}

proc showpopupdebugwsel {texttoadd typeofquickadd} {
    global pad menuFont
    set numx [winfo pointerx $pad]
    set numy [winfo pointery $pad]
    catch {destroy $pad.popdebugwsel}
    menu $pad.popdebugwsel -tearoff 0 -font $menuFont
    if {$typeofquickadd eq "watchvariable"} {
        set plabel [mc AddWatch $texttoadd]
    } else {
        # $typeofquickadd eq "genericexpression"
        set plabel [mc AddGenExp $texttoadd]
    }
    $pad.popdebugwsel add command -label $plabel\
        -command "quickAddWatch_bp {$texttoadd} $typeofquickadd"
    tk_popup $pad.popdebugwsel $numx $numy
}

proc istoplevelopen {toplevelvarname} {
# return true if the toplevel window whose *name* is given in argument is open
# return false otherwise
# Warning: it's the _name_ of the variable containing the toplevel name that must
# be provided to this proc, not the toplevel name itself, i.e. call it with:
#   istoplevelopen bptsgui      but not    istoplevelopen $bptsgui
    global $toplevelvarname
    if {[info exists $toplevelvarname]} {
        append toplevelreference {$} $toplevelvarname
        if {[eval "winfo exists $toplevelreference"]} {
            return true
        }
    }
    return false
}
