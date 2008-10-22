getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/TK_SetVar_data.ref','rb');
TK_EvalStr('toplevel .tst2');
// creates a toplevel TK window.
TK_EvalStr('label .tst2.l -textvariable tvar');
// create a static label
TK_EvalStr('pack .tst2.l');
// pack the label widget. It appears on the screen.
TK_SetVar('tvar', 'This text has been set directly within scilab');

//destroy toplevel  TK window.
%ans = TK_EvalStr('destroy .tst2');
if load_ref('%ans') then   pause,end,



xdel_run(winsid());

mclose(%U);
