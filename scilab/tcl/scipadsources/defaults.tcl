set winTitle "SciPad"
set version "Version 3.0"
set wordWrap none
##ES: default options which can be overridden
if { ![info exists BGCOLOR] } {set BGCOLOR "snow1"}
if { ![info exists FGCOLOR] } {set FGCOLOR "black"}
if { ![info exists CURCOLOR] } {set CURCOLOR "red"}
if { ![info exists PARCOLOR] } {set PARCOLOR "magenta3"}
if { ![info exists BRAKCOLOR] } {set BRAKCOLOR "DarkGoldenrod4"}
if { ![info exists BRACCOLOR] } {set BRACCOLOR "red"}
if { ![info exists PUNCOLOR] } {set PUNCOLOR "turquoise4"}
if { ![info exists KEYWCOLOR] } {set KEYWCOLOR "blue2"}
if { ![info exists OPCOLOR] } {set OPCOLOR "blue4"}
if { ![info exists TXTCOLOR] } {set TXTCOLOR $FGCOLOR}
if { ![info exists QTXTCOLOR] } {set QTXTCOLOR "darkred"}
if { ![info exists REMCOLOR] } {set REMCOLOR "green4"}
if { ![info exists XMLCOLOR] } {set XMLCOLOR "orange"}
if { ![info exists NUMCOLOR] } {set NUMCOLOR "yellow4"}
if { ![info exists SELCOLOR] } {set SELCOLOR "PaleGreen"}
# FV 13/05/04
if { ![info exists BREAKPOINTCOLOR] } {set BREAKPOINTCOLOR "pink"}
##ES: remember fontsize, pad geometry
if { ![info exists FontSize] } {set FontSize 12 }
if { ![info exists WMGEOMETRY] } { set WMGEOMETRY 32x36 }
if { ![info exists printCommand] } { set printCommand lpr }
set Scheme scilab

# FV 14/06/04, relocated here the mainlevel code of debugger.tcl
set funnameargs ""
set funnames ""
set unklabel "<?>"
set curdropind 0
set dragndroplb ""
set watchvars ""
array set watchvarsvals {}
set firsttimeinshowwatch "true"
# FV, 17/06/04, better do it in scipad.sci
#ScilabEval "     TK_EvalStr(\"scipad eval {set tmpdir \"+TMPDIR+\"}\");"
