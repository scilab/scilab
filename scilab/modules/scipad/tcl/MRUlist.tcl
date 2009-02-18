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

#################################################################
# generic procedures dealing with a MRU (most recently used) list
#################################################################

# <TODO> would be nice to factorize MRU lists procedures below!


##################################################
# procedures dealing with the recent files list
# displayed in the file menu
##################################################
proc AddRecentFile {filename} {
# add a new recent file item to the file menu
# if there is already the max number of entries, then shift them
# one line down and insert $filename at the top
    global pad listofrecentfiles maxrecentfiles nbrecentfiles
    if {$maxrecentfiles == 0} {return}
    # first check if the new entry is already present
    set present "false"
    for {set i 0} {$i<$nbrecentfiles} {incr i} {
        if {[lindex $listofrecentfiles $i] == $filename} {
            set present "true"
            set pospresent $i
        }
    }
    set rec1ind [GetFirstRecentFileInd]
    if {$present == "false"} {
        # add the new entry
        if {$nbrecentfiles == 0} {
            incr rec1ind
            $pad.filemenu.files insert $rec1ind separator
        }
        # update the file menu
        if {$nbrecentfiles < $maxrecentfiles} {
            incr nbrecentfiles
            # insert new entry
            set listofrecentfiles [linsert $listofrecentfiles 0 $filename]
            # [list [lindex $listofrecentfiles $i]] automatically escapes special characters
            $pad.filemenu.files insert $rec1ind command \
                       -label [file tail [lindex $listofrecentfiles 0] ] \
                       -command "openfileifexists [list [lindex $listofrecentfiles 0]]"
            # update menu entries (required to update the numbers)
            UpdateRecentLabels $rec1ind
        } else {
            # forget last entry of the list and insert new entry
            set listofrecentfiles [lreplace $listofrecentfiles end end]
            set listofrecentfiles [linsert $listofrecentfiles 0 $filename]
            # update menu entries
            UpdateRecentLabels $rec1ind
        }
    } else {
        # move the existing entry to the top of the list
        set listofrecentfiles [lreplace $listofrecentfiles $pospresent $pospresent]
        set listofrecentfiles [linsert $listofrecentfiles 0 $filename]
        UpdateRecentLabels $rec1ind
    }
}

proc GetFirstRecentFileInd {} {
# get index of first recent file item in the file menu
    global FirstMRUFileNameInFileMenu nbrecentfiles
    if {$nbrecentfiles == 0} {
        return [expr {$FirstMRUFileNameInFileMenu - 1}]
    } else {
        return $FirstMRUFileNameInFileMenu
    }
}

proc UpdateRecentLabels {rec1ind} {
# update labels of recent files entries with file tail preceded by a number
    global pad listofrecentfiles nbrecentfiles
    for {set i 0} {$i<$nbrecentfiles} {incr i} {
        if {$i<9} {
            set lab [concat [expr {$i + 1}] [file tail [lindex $listofrecentfiles $i] ] ]
        } else {
            set lab [file tail [lindex $listofrecentfiles $i] ]
        }
        set ind [expr {$rec1ind + $i}]
        # [list [lindex $listofrecentfiles $i]] automatically escapes special characters
        $pad.filemenu.files entryconfigure $ind \
                   -label $lab \
                   -command "openfileifexists [list [lindex $listofrecentfiles $i]]"
        if {$i<9} {
            $pad.filemenu.files entryconfigure $ind -underline 0
        } else {
            $pad.filemenu.files entryconfigure $ind -underline -1
        }
    }
}

proc BuildInitialRecentFilesList {} {
    global pad listofrecentfiles nbrecentfiles
    set nbrecentfiles [llength $listofrecentfiles]
    for {set i 0} {$i<$nbrecentfiles} {incr i} {
        if {$i<9} {
            set lab [concat [expr {$i + 1}] [file tail [lindex $listofrecentfiles $i] ] ]
        } else {
            set lab [file tail [lindex $listofrecentfiles $i] ]
        }
        # [list [lindex $listofrecentfiles $i]] automatically escapes special characters
        $pad.filemenu.files add command \
                   -label $lab \
                   -command "openfileifexists [list [lindex $listofrecentfiles $i]]"
        if {$i<9} {
            set ind [$pad.filemenu.files index end]
            $pad.filemenu.files entryconfigure $ind \
                   -underline 0
        }
    }
    if {$nbrecentfiles > 0} {
        $pad.filemenu.files add separator
    }
}

proc UpdateRecentFilesList {} {
# this proc gets called whenever the maximum number of recent entries
# in the file menu is changed by the user
    global pad listofrecentfiles maxrecentfiles nbrecentfiles
    if {$maxrecentfiles >= [llength $listofrecentfiles]} {
        # nothing to do, maxrecentfiles was just increased
        # this is handled by AddRecentFile
        return
    } else {
        # maxrecentfiles was decreased
        # forget the entries in listofrecentfiles, and update the menu
        set rec1ind [GetFirstRecentFileInd]
        set firstind [expr {$rec1ind + $maxrecentfiles}]
        set lastind  [expr {$rec1ind + [llength $listofrecentfiles] - 1}]
        $pad.filemenu.files delete $firstind $lastind
        set listofrecentfiles [lreplace $listofrecentfiles $maxrecentfiles end]
        incr nbrecentfiles [expr {- ($lastind - $firstind + 1)}]
        if {$maxrecentfiles == 0} {
            # remove the separator
            $pad.filemenu.files delete $firstind
        }
    }
}

##################################################
# procedures dealing with the recent encodings list
# displayed in the options/encoding menu
##################################################
proc AddRecentEncoding {encod} {
# add a new recent encoding item to the options/encoding menu
# if there is already the max number of entries, then shift them
# one line down and insert $encod at the top
    global pad listofrecentencodings maxrecentencodings nbrecentencodings
    if {$maxrecentencodings == 0} {return}
    # do nothing if the encoding passed here does not exist
    # (should never happen, unless encoding files *.enc are missing
    # from the Tcl distribution)
    if {[lsearch -exact [encoding names] $encod] == -1} {
        return
    }
    # first check if the new entry is already present
    set present "false"
    for {set i 0} {$i<$nbrecentencodings} {incr i} {
        if {[lindex $listofrecentencodings $i] eq $encod} {
            set present "true"
            set pospresent $i
        }
    }
    set rec1ind [GetFirstRecentEncodingInd]
    if {$present == "false"} {
        # add the new entry
        if {$nbrecentencodings == 0} {
            incr rec1ind
            $pad.filemenu.options.encodings insert $rec1ind separator
        }
        # update the file menu
        if {$nbrecentencodings < $maxrecentencodings} {
            incr nbrecentencodings
            # insert new entry
            set listofrecentencodings [linsert $listofrecentencodings 0 $encod]
            # [list [lindex $listofrecentencodings $i]] automatically escapes special characters
            $pad.filemenu.options.encodings insert $rec1ind radiobutton \
                       -label [lindex $listofrecentencodings 0] \
                       -command "setencoding" \
                       -value [lindex $listofrecentencodings 0] -variable currentencoding
            # update menu entries (required to update the numbers)
            UpdateRecentEncodingsLabels $rec1ind
        } else {
            # forget last entry of the list and insert new entry
            set listofrecentencodings [lreplace $listofrecentencodings end end]
            set listofrecentencodings [linsert $listofrecentencodings 0 $encod]
            # update menu entries
            UpdateRecentEncodingsLabels $rec1ind
        }
    } else {
        # move the existing entry to the top of the list
        set listofrecentencodings [lreplace $listofrecentencodings $pospresent $pospresent]
        set listofrecentencodings [linsert $listofrecentencodings 0 $encod]
        UpdateRecentEncodingsLabels $rec1ind
    }
}

proc GetFirstRecentEncodingInd {} {
# get index of first recent file item in the file menu
    global FirstMRUEncodingInOptEncMenu nbrecentencodings
    if {$nbrecentencodings == 0} {
        return [expr {$FirstMRUEncodingInOptEncMenu - 1}]
    } else {
        return $FirstMRUEncodingInOptEncMenu
    }
}

proc UpdateRecentEncodingsLabels {rec1ind} {
# update labels of recent encodings entries preceded by a number
    global pad listofrecentencodings nbrecentencodings
    for {set i 0} {$i<$nbrecentencodings} {incr i} {
        if {$i<9} {
            set lab [concat [expr {$i + 1}] [lindex $listofrecentencodings $i] ]
        } else {
            set lab [lindex $listofrecentencodings $i]
        }
        set ind [expr {$rec1ind + $i}]
        $pad.filemenu.options.encodings entryconfigure $ind \
                   -label $lab \
                   -command "setencoding" \
                   -value [lindex $listofrecentencodings $i] -variable currentencoding
        if {$i<9} {
            $pad.filemenu.options.encodings entryconfigure $ind -underline 0
        } else {
            $pad.filemenu.options.encodings entryconfigure $ind -underline -1
        }
    }
}

proc BuildInitialRecentEncodingsList {} {
    global pad listofrecentencodings nbrecentencodings
    set nbrecentencodings [llength $listofrecentencodings]
    for {set i 0} {$i<$nbrecentencodings} {incr i} {
        if {$i<9} {
            set lab [concat [expr {$i + 1}] [lindex $listofrecentencodings $i] ]
        } else {
            set lab [lindex $listofrecentencodings $i]
        }
        $pad.filemenu.options.encodings add radiobutton \
                   -label $lab \
                   -command "setencoding" \
                   -value [lindex $listofrecentencodings $i] -variable currentencoding
        if {$i<9} {
            set ind [$pad.filemenu.options.encodings index end]
            $pad.filemenu.options.encodings entryconfigure $ind \
                   -underline 0
        }
    }
    if {$nbrecentencodings > 0} {
        $pad.filemenu.options.encodings add separator
    }
}

proc UpdateRecentEncodingsList {} {
# this proc gets called whenever the maximum number of recent entries
# in the options/encodings menu is changed by the user
    global pad listofrecentencodings maxrecentencodings nbrecentencodings
    if {$maxrecentencodings >= [llength $listofrecentencodings]} {
        # nothing to do, maxrecentencodings was just increased
        # this is handled by AddRecentEncoding
        return
    } else {
        # maxrecentencodings was decreased
        # forget the entries in listofrecentencodings, and update the menu
        set rec1ind [GetFirstRecentEncodingInd]
        set firstind [expr {$rec1ind + $maxrecentencodings}]
        set lastind  [expr {$rec1ind + [llength $listofrecentencodings] - 1}]
        $pad.filemenu.options.encodings delete $firstind $lastind
        set listofrecentencodings [lreplace $listofrecentencodings $maxrecentencodings end]
        incr nbrecentencodings [expr {- ($lastind - $firstind + 1)}]
        if {$maxrecentencodings == 0} {
            # remove the separator
            $pad.filemenu.options.encodings delete $firstind
        }
    }
}
