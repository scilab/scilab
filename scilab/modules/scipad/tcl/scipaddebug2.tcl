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

#################################
#    Debug settings - Part 2    #
#################################


#############
# Code related to the Scipad debug log file

if {$debuglog} {

    # list of Scipad procs that won't be logged
    # this is basically for convenience (in order to clutter the logs a bit less)
    # for some procs it can however be mandatory, e.g. when the proc is used
    # in proc log such as proc timestamp
    set excludedScipadprocs [list \
    timestamp \
    tk_optionMenu \
    escapespecialchars \
    mc amp mcra mcmax mcmaxra bl \
    keyposn dokeyposn ismodified whichfun modifiedtitle \
    showinfo delinfo showinfo_menu_file showinfo_menu_wind \
    GetFirstRecentInd extractindexfromlabel \
    checkifanythingchangedondisk checkiffilechangedondisk \
    TextStyles managescroll \
    schememenus setdbmenuentriesstates_bp getdbstate pbind \
    Button1BindTextArea IsBufferEditable changedmodified \
    gettextareacur settextareacur countcontlines reshape_bp \
    undoredo sreverse lreverse commonPrefix \
    iscontinuedline getstartofcontline getendofcontline \
    colorize remalltags tagcontline \
    colorizestringsandcomments_sd colorizestringsandcomments \
    backgroundcolorize dobackgroundcolorize \
    getstartofcolorization getendofcolorization \
    Progress SetProgress \
    backgroundcolorizeuserfun docolorizeuserfun \
    colorizetag \
    getallfunsintextarea \
    trimcontandcomments extractfunnamefromfunline \
    bindenable binddisable findbinding \
    setwindowsmenuentrylabel addwindowsmenuentry sortwindowsmenuentries \
    update_bubble cancel_bubble_deletion update_bubble_watchvar \
    generic_update_bubble_watch update_bubble_watch_step update_bubble_watch \
    update_bubble_panetitle \
    displaybusystate \
    istoplevelopen \
    ]
#    set excludedScipadprocs [list ]

    # delete procs that were renamed during the previous Scipad session
    # that was launched from the same Scilab session
    foreach pr [info procs] {
        if {[info procs ScipadLog_$pr] != ""} {
            rename ScipadLog_$pr ""
        }
    }    

    # for each Scipad proc not excluded in the list above, this surrounds the existing
    # proc with log info: proc called with input arguments, and proc return value
    set logid 0
    foreach pr [info procs] {
        if {[lsearch $nologprocs $pr] == -1 && \
            [lsearch $excludedScipadprocs $pr] == -1 } {
            # add log info to the proc
            rename $pr ScipadLog_$pr
            eval "proc $pr {args} {global logid; \
                                   incr logid; set thislogid \$logid; \
                                   log \"# \$thislogid - Level \[info level\]: \[info level 0\]\"; \
                                   set ret \[uplevel 1 ScipadLog_$pr \$args\]; \
                                   log \"# \$thislogid - Return value (level \[info level\] - \[info level 0\]):!!\$ret!!\"; \
                                   return \$ret \
                                  }"
        }
    }

    # for each Scipad menu item, this surrounds the existing command with log info
    proc logmenues {men} {
        foreach w [winfo children $men] {
            for {set i 0} {$i<=[$w index last]} {incr i} {
                if {[$w type $i] != "separator" && [$w type $i] != "tearoff" && [$w type $i] != "cascade"} {
                    $w entryconfigure $i -command " \
                            log \"\n-----------------------------------------\" ; \
                            log \[concat \"Menu command:\" [list [$w entrycget $i -label]]\]; \
                            [$w entrycget $i -command]; \
                            log \[concat \"End of menu command:\" [list [$w entrycget $i -label]]\];
                            log \"\n-----------------------------------------\n\" "
                }
            }
            if {[$w type $i] == "cascade"} {
                catch {logmenues $w}
            }
        }
    }
    logmenues $pad.filemenu

    # for each Scipad binding, this surrounds the existing command with log info
    if {$logdetailedbindings} {
        set scipadwidgets [winfo children $pad]
        lappend scipadwidgets $pad
    } else {
        set scipadwidgets $pad
    }
    if {$logTextclassbindings} {
        lappend scipadwidgets Text
    }
    foreach wid $scipadwidgets {
        foreach sequ [bind $wid] {
            set script [bind $wid $sequ]
            bind $wid $sequ "log \"\n----------------------\" ; \
                             log \"Bind $wid $sequ triggered!\"; \
                             \n$script;\n\
                             log \"End of bind $wid $sequ\"; \
                             log \"\n----------------------\n\" "
        }
    }

}

# End of code related to the Scipad debug log file
#############


##################################
# End of debug settings - Part 2 #
##################################
