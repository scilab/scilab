set funnameargs ""
set funnames ""
set unklabel "<?>" ;# Warning: if this is changed it must be changed accordingly in FormatStringsForWatch.sci
set curdropind 0
set dragndroplb ""
set watchvars ""
array unset watchvarsvals
array unset watchvarstysi          ; # type and size of watch variables
array unset varsforautowatchloc    ; # list of current input, output and local variables for the auto watch feature
set callstackfuns ""
set callstacklines ""
set callstackcontent ""
set firsttimeinshowwatch "true"
set showwatchvariablesarea "true"
set showcallstackarea "true"
set watchalwaysontop 0
set autowatchloc false
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

# list of functions names that cannot be debugged
# because they are debugger ancillaries
set debugger_fun_ancillaries [list \
    "LocalizeForScipad" \
    "FormatStringsForWatch" \
    "MatFormatStringsForWatch" \
    "FormatWhereForWatch" \
    "ext_exists" \
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
set db_butimages [list "sep" butsetbptimage butremoveallimage "sep" butconfigureimage \
                       "sep" butnextimage butstepimage \
                       butruntoreturnimage butruntocursorimage \
                       butgoonignorimage "sep" butwatchimage "sep" butbreakimage \
                       butcancelimage]
set db_stepbutimages [list "sep" butstepenterimage butstepoverimage butstepexitimage]
# Divide size again by 2 for display in the debug menu
image create photo menubutsetbptimage
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
