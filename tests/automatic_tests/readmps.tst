getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/readmps_data.ref','rb');

//Let the LP problem:
//objective:
//   min     XONE + 4 YTWO + 9 ZTHREE
//constraints:
//  LIM1:    XONE +   YTWO            < = 5
//  LIM2:    XONE +            ZTHREE > = 10
// MYEQN:         -   YTWO  +  ZTHREE   = 7
//Bounds
//  0 < = XONE < = 4
// -1 < = YTWO < = 1

//Generate MPS file
txt = ['NAME          TESTPROB';
  'ROWS';
  ' N  COST';
  ' L  LIM1';
  ' G  LIM2';
  ' E  MYEQN';
  'COLUMNS';
  '    XONE      COST                 1   LIM1                 1';
  '    XONE      LIM2                 1';
  '    YTWO      COST                 4   LIM1                 1';
  '    YTWO      MYEQN               -1';
  '    ZTHREE    COST                 9   LIM2                 1';
  '    ZTHREE    MYEQN                1';
  'RHS';
  '    RHS1      LIM1                 5   LIM2                10';
  '    RHS1      MYEQN                7';
  'BOUNDS';
  ' UP BND1      XONE                 4';
  ' LO BND1      YTWO                -1';
  ' UP BND1      YTWO                 1';
  'ENDATA'];
%ans = mputl(txt, TMPDIR + '/test.mps');
if load_ref('%ans') then   pause,end,

//Read the MPS file
P = readmps(TMPDIR + '/test.mps', [0,10^30]);
if load_ref('P') then   pause,end,

//Convert it to linpro format
LP = mps2linpro(P);
if load_ref('LP') then   pause,end,

//Solve it with linpro
[x,lagr,f] = linpro(LP(2:$));
if load_ref('f') then   pause,end,
if load_ref('lagr') then   pause,end,
if load_ref('x') then   pause,end,

xdel_run(winsid());

mclose(%U);
