// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Vincent COUVERT <vincent.couvert@inria.fr>
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 857 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=857
//
// <-- Short Description -->
//    Imaginary unit i and index loop i in an M-file are both
//    converted to %i in Scilab...
//
//    for i=1:10
//    disp(i)
//    end
//
//    is converted as follows :
//
//    for i=1:10
//    disp(%i)
//    end
//
//    TOO BAD !

MFILECONTENTS=["% i and j do not exist as variables -> Imaginary unit";
		"a=1+2*i";
		"b=1+2*j";
		"% Two complex values";
		"icplxnumber=1+2i;";
		"jcplxnumber=1+2j;";
		"chgt_rampe=[];";
		"for k=1:10";
		"  for i=1:10";
		"    for j=1:10";
		"      chgt_rampe(k)=2*i-j+1;";
		"    end;";
		"  end;";
		"end;";
		"% Two complex values";
		"icplxnumber2=1+3i;";
		"jcplxnumber2=1+3j;";
		"% Two real values";
		"irealnumber=1+4*i;";
		"jrealnumber=1+4*j;";
		"nbrserie=30";
		"for i=1:nbrserie";
		"  if nbrserie==24";
		"    switch i";
		"     case 1";
		"      disp(i)";
		"    else";
		"      disp(''abcd'')";
		"    end;";
		"  end;";
		"end;"];

MFILE=TMPDIR+"/bug857.m";
SCIFILE=TMPDIR+"/bug857.sci";

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
		"// i and j do not exist as variables -> Imaginary unit";
		"a = 1+2*%i";
		"b = 1+2*%i";
		"// Two complex values";
		"icplxnumber = 1+2*%i;";
		"jcplxnumber = 1+2*%i;";
		"chgt_rampe = [];";
		"for k = 1:10";
		"  for i = 1:10";
		"    for j = 1:10";
		"      chgt_rampe(1,k) = 2*i-j+1;";
		"    end;";
		"  end;";
		"end;";
		"// Two complex values";
		"icplxnumber2 = 1+3*%i;";
		"jcplxnumber2 = 1+3*%i;";
		"// Two real values";
		"irealnumber = 1+4*i;";
		"jrealnumber = 1+4*j;";
		"nbrserie = 30";
		"for i = 1:nbrserie";
		"  if nbrserie==24 then";
		"    select i";
		"      case 1 then";
		"        disp(i)";
		"      else";
		"        disp(""abcd"")";
		"    end;";
		"  end;";
		"end;"];
		
if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then pause,end
