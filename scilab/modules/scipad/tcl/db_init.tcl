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
set funnameargs ""
set funnames ""
set unklabel "<?>" ;# Warning: if this is changed it must be changed accordingly in FormatStringsForWatch.sci
set curdropind 0
set dragndroplb ""
set watchvars ""
array unset watchvarsprops
array unset varsforautowatch       ; # list of current variables for the auto watch feature input, output, globals and locals)
set watchgenexps {}
set callstackfuns ""
set callstacklines ""
set callstackcontent ""
set firsttimeinshowwatch true
set showwatchvariablesarea true
set showgenexparea true
set showcallstackarea true
set watchalwaysontop 0
set autowatchloc false
set autowatchglo false
set dockwatch false
set errmsg ""
set errnum 0
set errline 0
set errfunc ""
set uabpt_opened_a_file false ;# see proc updateactbreakpointtag
array set cursorsinwidgets {}
set debuggerbusycursor false
set initprevdbpauselevel 4                  ;# see proc checkendofdebug_bp
set prevdbpauselevel $initprevdbpauselevel
set afilewasopenedbyuabpt false             ;# see proc checkendofdebug_bp
set displayruntoreturnwarning true
set breakcommandtriggered false
array set bptsprops {}
unset -nocomplain bptsgui    :# needed to reset initial state of the bptsgui as seen by proc showbptgui_bp when Scipad is closed and reopened
set bptconditiontypes [list istrue haschanged]  ; # intentionally not localized, see proc updatebptspropscondtype
set bpthitconditions  [list always equalto multipleof greaterthan lessthan]  ; # intentionally not localized, see proc updatebptspropshitconds

# list of functions names that cannot be debugged
# because they are debugger ancillaries
# note: with Scilab 5 one could have thought about using
#       libraryinfo("scipadinternalslib"), but this instruction:
#         - does not provide macro names that do not match the file name,
#           i.e. if file1.sci contains functions file1 and file2, only
#           file1 is returned (this is bug 2465)
#         - will never provide evstr (that cannot be debugged because it
#           is directly used in a ScilabEval for generic expressions), nor
#           macrovar and ancillaries (used for the autowatched variables)
set debugger_fun_ancillaries [list \
    "LocalizeForScipad" \
    "FormatStringsForWatch" \
    "FormatInfNanForWatch" \
    "StringMatrixForWatch" \
    "FormatWhereForWatch" \
    "ext_exists" \
    "evstr" \
    "macrovar" "listvars" "addvar" "addget" \
    ]

# list of variables that cannot be watched for various reasons
# ans: "ans" local variable is always present in the 5th element of the macrovar output
#      On next debug step, the debugger will [ans]=resume(ans) which means that a new
#      variable "ans" will be created which was not present in the debugged script - the
#      original ans is special, and cannot be watched because of this
#      To let ans be watched, I could just avoid to include it in the resume list in
#      proc createsetinscishellcomm, i.e. make ans a non editable watch variable.
#      However this is not enough: if ans is added as a watch variable it gets filled
#      during debug by the result of FormatWhereForWatch, which is not the intent!
#      -> better completely forbid watching ans
# db_nam: is used in the wrapper of a .sce debug
set debugger_unwatchable_vars [list \
    "ans" \
    "db_nam" \
    ]

# Set the Scilab limits in terms of max number of breakpointed functions
# and of max number of breakpoints (see help setbpt)
set ScilabCodeMaxBreakpointedMacros 100
set ScilabCodeMaxBreakpoints 1000

set waitmessage [mc "Please wait..."]

# Read debugger icons from gif files on disk and resize them independently of their
# original size
set newsizex 32
set newsizey 32
image create photo butsetbptimage_o      -file [file join $iconsdir setbp.gif]
image create photo buteditbptimage_o     -file [file join $iconsdir editbp.gif]
image create photo butremoveallimage_o   -file [file join $iconsdir removeallbp.gif]
image create photo butconfigureimage_o   -file [file join $iconsdir arrowframe.gif]
image create photo butnextimage_o        -file [file join $iconsdir arrowin.gif]
image create photo butstepimage_o        -file [file join $iconsdir stepstep.gif]
image create photo butstepenterimage_o   -file [file join $iconsdir arrowenter.gif]
image create photo butstepoverimage_o    -file [file join $iconsdir arrowover.gif]
image create photo butstepexitimage_o    -file [file join $iconsdir arrowexit.gif]
image create photo butruntoreturnimage_o -file [file join $iconsdir arrowtoreturn.gif]
image create photo butruntocursorimage_o -file [file join $iconsdir arrowcursor.gif]
image create photo butgoonignorimage_o   -file [file join $iconsdir arrowignore.gif]
image create photo butwatchimage_o       -file [file join $iconsdir arrowwatch.gif]
image create photo butbreakimage_o       -file [file join $iconsdir arrowstop.gif]
image create photo butcancelimage_o      -file [file join $iconsdir arrowend2.gif]
set orig_w [image width  butconfigureimage_o]
set orig_h [image height butconfigureimage_o]
set subsamplex [expr {$orig_w / $newsizex}]
set subsampley [expr {$orig_h / $newsizey}]
image create photo butsetbptimage
image create photo buteditbptimage
image create photo butremoveallimage
image create photo butconfigureimage
image create photo butnextimage
image create photo butstepimage
image create photo butstepenterimage
image create photo butstepoverimage
image create photo butstepexitimage
image create photo butruntoreturnimage
image create photo butruntocursorimage
image create photo butgoonignorimage
image create photo butwatchimage
image create photo butbreakimage
image create photo butcancelimage
butsetbptimage      copy butsetbptimage_o      -subsample $subsamplex $subsampley
buteditbptimage     copy buteditbptimage_o     -subsample $subsamplex $subsampley
butremoveallimage   copy butremoveallimage_o   -subsample $subsamplex $subsampley
butconfigureimage   copy butconfigureimage_o   -subsample $subsamplex $subsampley
butnextimage        copy butnextimage_o        -subsample $subsamplex $subsampley
butstepimage        copy butstepimage_o        -subsample $subsamplex $subsampley
butstepenterimage   copy butstepenterimage_o   -subsample $subsamplex $subsampley
butstepoverimage    copy butstepoverimage_o    -subsample $subsamplex $subsampley
butstepexitimage    copy butstepexitimage_o    -subsample $subsamplex $subsampley
butruntoreturnimage copy butruntoreturnimage_o -subsample $subsamplex $subsampley
butruntocursorimage copy butruntocursorimage_o -subsample $subsamplex $subsampley
butgoonignorimage   copy butgoonignorimage_o   -subsample $subsamplex $subsampley
butwatchimage       copy butwatchimage_o       -subsample $subsamplex $subsampley
butbreakimage       copy butbreakimage_o       -subsample $subsamplex $subsampley
butcancelimage      copy butcancelimage_o      -subsample $subsamplex $subsampley
set db_butimages [list "sep" butsetbptimage buteditbptimage butremoveallimage "sep" \
                       butconfigureimage "sep" butnextimage butstepimage \
                       butruntoreturnimage butruntocursorimage \
                       butgoonignorimage "sep" butwatchimage "sep" butbreakimage \
                       butcancelimage]
set db_stepbutimages [list "sep" butstepenterimage butstepoverimage butstepexitimage]
# Divide size again by 2 for display in the debug menu
image create photo menubutsetbptimage
image create photo menubuteditbptimage
image create photo menubutremoveallimage
image create photo menubutconfigureimage
image create photo menubutnextimage
image create photo menubutstepimage
image create photo menubutstepenterimage
image create photo menubutstepoverimage
image create photo menubutstepexitimage
image create photo menubutruntoreturnimage
image create photo menubutruntocursorimage
image create photo menubutgoonignorimage
image create photo menubutwatchimage
image create photo menubutbreakimage
image create photo menubutcancelimage
set subsamplex [expr {$orig_w / ($newsizex / 2)}]
set subsampley [expr {$orig_h / ($newsizey / 2)}]
menubutsetbptimage      copy butsetbptimage_o      -subsample $subsamplex $subsampley
menubuteditbptimage     copy buteditbptimage_o     -subsample $subsamplex $subsampley
menubutremoveallimage   copy butremoveallimage_o   -subsample $subsamplex $subsampley
menubutconfigureimage   copy butconfigureimage_o   -subsample $subsamplex $subsampley
menubutnextimage        copy butnextimage_o        -subsample $subsamplex $subsampley
menubutstepimage        copy butstepimage_o        -subsample $subsamplex $subsampley
menubutstepenterimage   copy butstepenterimage_o   -subsample $subsamplex $subsampley
menubutstepoverimage    copy butstepoverimage_o    -subsample $subsamplex $subsampley
menubutstepexitimage    copy butstepexitimage_o    -subsample $subsamplex $subsampley
menubutruntoreturnimage copy butruntoreturnimage_o -subsample $subsamplex $subsampley
menubutruntocursorimage copy butruntocursorimage_o -subsample $subsamplex $subsampley
menubutgoonignorimage   copy butgoonignorimage_o   -subsample $subsamplex $subsampley
menubutwatchimage       copy butwatchimage_o       -subsample $subsamplex $subsampley
menubutbreakimage       copy butbreakimage_o       -subsample $subsamplex $subsampley
menubutcancelimage      copy butcancelimage_o      -subsample $subsamplex $subsampley

# Watch window LEDs
image create photo led_debugstate_NoDebug         -file [file join $iconsdir dbs_NoDebug_48x24.gif]
image create photo led_debugstate_ReadyForDebug   -file [file join $iconsdir dbs_ReadyForDebug_48x24.gif]
image create photo led_debugstate_DebugInProgress -file [file join $iconsdir dbs_DebugInProgress_48x24.gif]
image create photo led_scilabbusy_busy  -file [file join $iconsdir prompt_busy_48x24.gif]
image create photo led_scilabbusy_ready -file [file join $iconsdir prompt_ready_48x24.gif]
