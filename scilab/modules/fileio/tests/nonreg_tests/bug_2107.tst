// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2107 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2107
//

// Creates file

LINE = "AAAAAAAAAAAAAA	2005/01/06	10:55:50.0000	4	BBBBBBBBBBBBBBBB	2005/01/11 00:00:50.0000	2005/01/02 00:00:00.0000	2005/01/02 20:28:10.0000";
STRINGS = string(ones(800,1));
STRINGS = strsubst(STRINGS,string(1),LINE);

filename = TMPDIR+'/test_2107.txt';
fid = mopen(filename,'wt');
mputl(STRINGS,fid);
mclose(fid);

clear LINE;
clear STRINGS;

// checks file

fid = mopen(filename,'rt');
[n,a,b,c,d,e,f,g,h,i,j,k] = mfscanf(-1,fid,'%s\t%s %s\t%s\t%s\t%s %s\t%s %s\t%s%s\n');
mclose(fid);

if n <> 11 then pause,end
if and(a <> 'AAAAAAAAAAAAAA') then pause,end
if and(b <> '2005/01/06') then pause,end
if and(c <> '10:55:50.0000') then pause,end
if and(d <> '4') then pause,end
if and(e <> 'BBBBBBBBBBBBBBBB') then pause,end
if and(f <> '2005/01/11') then pause,end
if and(g <> '00:00:50.0000') then pause,end
if and(h <> '2005/01/02') then pause,end
if and(i <> '00:00:00.0000') then pause,end
if and(j <> '2005/01/02') then pause,end
if and(k <> '20:28:10.0000') then pause,end

mdelete(filename);
