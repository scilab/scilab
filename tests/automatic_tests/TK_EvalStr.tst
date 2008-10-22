getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/TK_EvalStr_data.ref','rb');
//with one call
%ans = TCL_EvalStr(['toplevel .foo1';
  'label .foo1.l -text ""TK married Scilab !!!""';
  'pack .foo1.l';
  'button .foo1.b -text close -command {destroy .foo1}';
  'pack .foo1.b']);
if load_ref('%ans') then   pause,end,


//step by step (debugging)
TCL_EvalStr('toplevel .foo2');
// creates a toplevel TK window.
TCL_EvalStr('label .foo2.l -text ""TK married Scilab !!!""');
// create a static label
TCL_EvalStr('pack .foo2.l');
// pack the label widget. It appears on the screen.
text = 'button .foo2.b -text close -command {destroy .foo2}';
TCL_EvalStr(text);
TCL_EvalStr('pack .foo2.b');

//kill the windows by program
TCL_EvalStr('destroy .foo1');
TCL_EvalStr('destroy .foo2');

//with one call, and in a slave interpreter
TCL_CreateSlave('TCLSlave');
TCL_EvalStr('set test ""in Slave TCL Interp""', 'TCLSlave');
%ans = TCL_GetVar('test', 'TCLSlave');
if load_ref('%ans') then   pause,end,


%ans = TCL_DeleteInterp('TCLSlave');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
