set funnameargs ""
set funnames ""
set unklabel "<?>"
set curdropind 0
set dragndroplb ""
set watchvars ""
array set watchvarsvals {}
set callstackcontent ""
set firsttimeinshowwatch "true"
set showwatchvariablesarea "true"
set togglewvabutton [mc "Hide watch variables"]
set showcallstackarea "true"
set togglecsabutton [mc "Hide call stack"]
# FV, 17/06/04, better do it in scipad.sci
#ScilabEval "TCL_EvalStr(\"scipad eval {set tmpdir \"+TMPDIR+\"}\");"
set waitmessage [mc "Please wait..."]

# Read debugger icons from gif files on disk and resize them independently of their
# original size
set newsizex 32
set newsizey 32
set iconspath [file join $env(SCIPATH) tcl scipadsources icons]
image create photo butsetbptimage_o      -file [file join $iconspath setbp.gif]
image create photo butremoveallimage_o   -file [file join $iconspath removeallbp.gif]
image create photo butconfigureimage_o   -file [file join $iconspath arrowframe.gif]
image create photo butnextimage_o        -file [file join $iconspath arrowin.gif]
image create photo butstepimage_o        -file [file join $iconspath stepstep.gif]
image create photo butruntocursorimage_o -file [file join $iconspath arrowcursor.gif]
image create photo butgoonignorimage_o   -file [file join $iconspath arrowignore.gif]
image create photo butwatchimage_o       -file [file join $iconspath arrowwatch.gif]
image create photo butbreakimage_o       -file [file join $iconspath arrowstop.gif]
image create photo butcancelimage_o      -file [file join $iconspath arrowend2.gif]
set orig_w [image width  butconfigureimage_o]
set orig_h [image height butconfigureimage_o]
set subsamplex [expr $orig_w / $newsizex]
set subsampley [expr $orig_h / $newsizey]
image create photo butsetbptimage
image create photo butremoveallimage
image create photo butconfigureimage
image create photo butnextimage
image create photo butstepimage
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
butruntocursorimage copy butruntocursorimage_o -subsample $subsamplex $subsampley
butgoonignorimage   copy butgoonignorimage_o   -subsample $subsamplex $subsampley
butwatchimage       copy butwatchimage_o       -subsample $subsamplex $subsampley
butbreakimage       copy butbreakimage_o       -subsample $subsamplex $subsampley
butcancelimage      copy butcancelimage_o      -subsample $subsamplex $subsampley
set db_butimages [list "sep" butsetbptimage butremoveallimage "sep" butconfigureimage \
                       "sep" butnextimage butstepimage butruntocursorimage \
                       butgoonignorimage "sep" butwatchimage "sep" butbreakimage \
                       butcancelimage]
# Divide size again by 2 for display in the debug menu
image create photo menubutsetbptimage
image create photo menubutremoveallimage
image create photo menubutconfigureimage
image create photo menubutnextimage
image create photo menubutstepimage
image create photo menubutruntocursorimage
image create photo menubutgoonignorimage
image create photo menubutwatchimage
image create photo menubutbreakimage
image create photo menubutcancelimage
set subsamplex [expr $orig_w / ($newsizex / 2)]
set subsampley [expr $orig_h / ($newsizey / 2)]
menubutsetbptimage      copy butsetbptimage_o      -subsample $subsamplex $subsampley
menubutremoveallimage   copy butremoveallimage_o   -subsample $subsamplex $subsampley
menubutconfigureimage   copy butconfigureimage_o   -subsample $subsamplex $subsampley
menubutnextimage        copy butnextimage_o        -subsample $subsamplex $subsampley
menubutstepimage        copy butstepimage_o        -subsample $subsamplex $subsampley
menubutruntocursorimage copy butruntocursorimage_o -subsample $subsamplex $subsampley
menubutgoonignorimage   copy butgoonignorimage_o   -subsample $subsamplex $subsampley
menubutwatchimage       copy butwatchimage_o       -subsample $subsamplex $subsampley
menubutbreakimage       copy butbreakimage_o       -subsample $subsamplex $subsampley
menubutcancelimage      copy butcancelimage_o      -subsample $subsamplex $subsampley

# Committed versions should have this attribute set to false
# In that case, the Run to Cursor and Break commands are hidden
# since there are issues with them in the Scilab parsers
set dev_debug "false"
