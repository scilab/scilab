// exec("bug539.sce")

function foo(); a_wrong_string; endfunction


tcl_script=['toplevel .w1'
'button .w1.b -text ""Click here to see a new Scilab Graphic Window""\'
'  -command {ScilabEval ""foo()""}'
'pack .w1.b ']
mputl(tcl_script,TMPDIR+'/test.tcl')
// Execute the tcl script
TK_EvalFile(TMPDIR+'/test.tcl')
