// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 119 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=119
//
// <-- Short Description -->
//    when running sample from readmps help it crashes complete SCILAB when readmps is invoked.

//Generate MPS file 
txt=['NAME          TESTPROB'
     'ROWS'
     ' N  COST'
     ' L  LIM1'
     ' G  LIM2'
     ' E  MYEQN'
     'COLUMNS'
     '    XONE      COST                 1   LIM1                 1'
     '    XONE      LIM2                 1'
     '    YTWO      COST                 4   LIM1                 1'
     '    YTWO      MYEQN               -1'
     '    ZTHREE    COST                 9   LIM2                 1'
     '    ZTHREE    MYEQN                1'
     'RHS'
     '    RHS1      LIM1                 5   LIM2                10'
     '    RHS1      MYEQN                7'
     'BOUNDS'
     ' UP BND1      XONE                 4'
     ' LO BND1      YTWO                -1'
     ' UP BND1      YTWO                 1'
     'ENDATA'];
mputl(txt,TMPDIR+'/test.mps')
//Read the MPS file
P=readmps(TMPDIR+'/test.mps',[0 10^30])
//Convert it to linpro format
LP=mps2linpro(P)
//Solve it with linpro
[x,lagr,f]=linpro(LP(2:$))
