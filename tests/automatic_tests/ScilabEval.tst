getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/ScilabEval_data.ref','rb');
//Callbacks and "seq" option usage

//create tcl instructions
tcl_script = ['toplevel .w1';
  'button .w1.b -text ""Click here to execute without seq option""  -command WithoutSeq';
  'button .w1.b1 -text ""Click here to execute with seq option""  -command  WithSeq';
  'pack .w1.b .w1.b1';
  'proc  WithoutSeq {} { ';
  '  ScilabEval ""cont=%f;;cont=%t;"" ';
  '  ScilabEval ""if cont then disp(''ok''),else disp(''wrong'');end;cont=%f;"" ';
  '}';
  'proc WithSeq {} { ';
  '  ScilabEval ""cont=%f;;cont=%t;""  ""seq""';
  '  ScilabEval ""if cont then disp(''ok''),else disp(''wrong'');end;cont=%f;"" ';
  '}'];
%ans = mputl(tcl_script, TMPDIR + '/test.tcl');
if load_ref('%ans') then   pause,end,
//write them to a file
// Execute the tcl script
cont = %f;
TK_EvalFile(TMPDIR + '/test.tcl');

//scripts and "sync" option usage

//----------------without "sync"----------------
tcl_script = ['  set t ""0""';
  '  while {$t != ""10""} {';
  '     ScilabEval ""a=$t;mprintf(''%d '',a);""';
  '     incr t';
  '  }'];

%ans = mputl(tcl_script, TMPDIR + '/test.tcl');
if load_ref('%ans') then   pause,end,
//write them to a file
// Execute the tcl script
TK_EvalFile(TMPDIR + '/test.tcl');mprintf('TK_EvalFile finished\n');
// The ScilabEval are executed  after the and of TK_EvalFile

//----------------with "sync"----------------
tcl_script = ['  set t ""0""';
  '  while {$t != ""10""} {';
  '     ScilabEval ""a=$t;mprintf(''%d '',a);"" ""sync""';
  '     incr t';
  '  }'];

%ans = mputl(tcl_script, TMPDIR + '/test.tcl');
if load_ref('%ans') then   pause,end,
//write them to a file
// Execute the tcl script
TK_EvalFile(TMPDIR + '/test.tcl');mprintf('TK_EvalFile finished\n');
// The ScilabEval are executed  synchronously with TK_EvalFile

xdel_run(winsid());

mclose(%U);
