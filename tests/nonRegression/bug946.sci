// <-- Non-regression test for bug 946 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=946
//
// <-- Short Description -->
//    Code badly converted by mfile2sci when using particular 
//    column vector initialization / assignment (see attachments).

// Copyright INRIA
// Scilab Project - V. Couvert

// Modified by Pierre MARECHAL
// Copyright INRIA
// Date : 18 Mar 2005

MFILECONTENTS=["x=zeros(3,2);";"y=zeros(3,1);";"x=[1 0;0 0;0 0];";"y(1:3)=x(:,1);"]

MFILE=TMPDIR+"/bug946.m"
SCIFILE=TMPDIR+"/bug946.sci"

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
		"x = zeros(3,2);";
		"y = zeros(3,1);";
		"x = [1,0;0,0;0,0];";
		"y(1:3) = x(:,1);"]

if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
	affich_result(%F,946);
else
	affich_result(%T,946);
end
