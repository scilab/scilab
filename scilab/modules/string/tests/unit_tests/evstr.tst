// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// =============================================================================
// unit tests evstr
// =============================================================================
a = 1;
b = 2;
Z = ['a','b'];
res = evstr(Z);
if res(1) <> a then pause, end
if res(2) <> b then pause, end
// =============================================================================
a = 1;
b = 2;
Z = list(['%(1)','%(1)-%(2)'], ['a+1','b+1']);
res = evstr(Z);
if or(res <> [2 -1]) then pause, end
// =============================================================================
if isnan(evstr(%nan)) <> %t then pause, end
if isinf(evstr(%inf)) <> %t then pause, end
if isnan(evstr('%nan')) <> %t then pause, end
if isinf(evstr('%inf')) <> %t then pause, end
if isnan(evstr('Nan')) <> %t then pause, end
if isinf(evstr('Inf')) <> %t then pause, end
if isnan(evstr('NaN')) <> %t then pause, end
if isinf(evstr(-%inf)) <> %t then pause, end
if isinf(evstr('-%inf')) <> %t then pause, end
if isinf(evstr('-Inf')) <> %t then pause, end
// =============================================================================
Scilab_value = 3;
if Scilab_value <> evstr('Scilab_value') then pause, end 
// =============================================================================
clear toto;
if execstr("evstr(''toto'')", "errcatch") <> 4 then pause, end
if execstr("evstr(toto)", "errcatch") <> 4 then pause, end
// =============================================================================
s = rand(3,3,3);
if execstr("evstr(s)", "errcatch") <> 10000 then pause, end
// =============================================================================
