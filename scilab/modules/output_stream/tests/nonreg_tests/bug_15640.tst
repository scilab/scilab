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