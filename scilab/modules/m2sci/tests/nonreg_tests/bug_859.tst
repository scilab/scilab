// <-- Non-regression test for bug 859 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=859
//
// <-- Short Description -->
//    M2SCI convert Matlab expression such as a(n,:)=b(m,:) (with
//    a==[])by a(n,:)=b(m,:) but execution crashes.

// Copyright INRIA
// Scilab Project - V. Couvert

// Modified by Pierre MARECHAL
// Copyright INRIA
// Date : 18 Mar 2005

MFILECONTENTS=["result=ones(10,10);";
		"m=1;";
		"for h=1:10";
		"  result_bis(m,:) = result(h,:);";
		"  m = m + 1;";
		"end"]

MFILE=TMPDIR+"/bug859.m"
SCIFILE=TMPDIR+"/bug859.sci"

mputl(MFILECONTENTS,MFILE);
mfile2sci(MFILE,TMPDIR);
SCIFILECONTENTS=mgetl(SCIFILE);

SCIFILECONTENTSREF=["";
		"// Display mode";
		"mode(0);";
		"";
		"// Display warning for floating point exception";
		"ieee(1);";
		"";
		"result = ones(10,10);";
		"m = 1;";
		"for h = 1:10";
		"  result_bis(m,1:length(result(h,:))) = result(h,:);";
		"  m = m+1;";
		"end;"]

if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then pause,end
