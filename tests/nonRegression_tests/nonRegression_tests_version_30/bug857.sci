// Non-regression test file for bug 857
// Copyright INRIA
// Scilab Project - V. Couvert

mode(-1);
clear all;

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
"    end";
"  end";
"end";
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
"    end";
"  end";
"end"]

MFILE=TMPDIR+"/bug857.m"
SCIFILE=TMPDIR+"/bug857.sci"

fd=mopen(MFILE,"w");
mputl(MFILECONTENTS,fd);
mclose(fd);

mfile2sci(MFILE,TMPDIR);

fd=mopen(SCIFILE,"r");
SCIFILECONTENTS=mgetl(fd,-1);
mclose(fd);

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
"    end";
"  end";
"end";
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
"    end";
"  end";
"end"]

correct=%T
if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
  correct=%F
end

affich_result(correct,857);

clear all
