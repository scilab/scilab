set funnameargs ""
set funnames ""
set unklabel "<?>"
set curdropind 0
set dragndroplb ""
set watchvars ""
array set watchvarsvals {}
set callstackcontent ""
set firsttimeinshowwatch "true"
# FV, 17/06/04, better do it in scipad.sci
#ScilabEval "TK_EvalStr(\"scipad eval {set tmpdir \"+TMPDIR+\"}\");"
if {$lang=="eng"} {
    set waitmessage "Please wait..."
} else {
    set waitmessage "Veuillez patienter..."
}
