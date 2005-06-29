getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/TK_EvalStr_data.ref','r');
//with one call
%ans = TK_EvalStr(['toplevel .foo1';
  'label .foo1.l -text ""TK married Scilab !!!""';
  'pack .foo1.l';
  'button .foo1.b -text close -command {destroy .foo1}';
  'pack .foo1.b']);
if load_ref('%ans') then   pause,end,


//step by step (debugging)
TK_EvalStr('toplevel .foo2');
// creates a toplevel TK window.
TK_EvalStr('label .foo2.l -text ""TK married Scilab !!!""');
// create a static label
TK_EvalStr('pack .foo2.l');
// pack the label widget. It appears on the screen.
text = 'button .foo2.b -text close -command {destroy .foo2}';
TK_EvalStr(text);
TK_EvalStr('pack .foo2.b');

//kill the windows by program
TK_EvalStr('destroy .foo1');
TK_EvalStr('destroy .foo2');
xdel_run(winsid());

mclose(%U);
