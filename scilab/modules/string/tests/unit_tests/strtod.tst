// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

//===============================
// strtod
//===============================
N = 666.666;
STREND = ' is a double';
STR = string(N)+STREND;
d = strtod(STR);
if d <> N then pause,end
[d,s] = strtod(STR);
if d <> N pause,end
if s <> STREND then pause,end
//===============================
STR ='1';
[d,s] = strtod(STR);
if s<>'' then pause,end
if d<>1 then pause,end
//===============================
STR = 'string';
[d,s] = strtod(STR);
if s<>STR then pause,end
if d<>0 then pause,end
//===============================
STR1 = '1A';
STR2 = '2B';
STR3 = '3C';
STR4 = '4d';
STR = [STR1,STR2;STR3,STR4];
[d,s] = strtod(STR);
if s<>['A','B';'C','d'] then pause,end
if d<>[1,2;3,4] then pause,end
//===============================
[d,s] = strtod('');
if s<>'' then pause,end
if d<>0 then pause,end
//===============================