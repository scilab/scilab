// ===================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// ===================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16209 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16209
//
// <-- Short Description -->
// grand() causes a freeze after several consecutive calls when using default base generator.

for i=1:1858
    grand(1,1e6,"exp",1);
end
grand(1,606052,"exp",1);