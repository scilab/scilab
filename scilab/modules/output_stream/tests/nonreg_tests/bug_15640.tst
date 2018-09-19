// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 15640 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15640
//
// <-- Short Description -->
// display in mode format("e") is corrupted (regression w.r.t. Scilab 5.5.2)

format("e",25)
256
%pi
nearfloat("succ",0)
format("v",10)
X=testmatrix('magi',4)/3;
X([2 3 4 7 8 12])=%eps*[2 3 4 7 8 12]/8
X=[1 2;3 4]
exp(%i*%pi*X)
exp(%i*%pi*X/2)
exp(%i*%pi*X/4)
X=[1 20;300 4000]
string(X)