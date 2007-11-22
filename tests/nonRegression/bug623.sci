// <-- Non-regression test for bug 623 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=623
//
// <-- Short Description -->
//    Even after applying the patch corresponding to bug report
//    608, integer cast to boolean is still not perfect.


// Non-regression test file for bug 623
// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Copyright INRIA 2006
// Date : 6 juin 2006

uno=int8(1);
1==[uno uno];

test1 = and(1==[uno uno])&and([1 1]==[uno uno])&and([uno uno]==1)&and([uno uno]==[1 1]);
test2 = (execstr("uint8(1) & %t","errcatch") ==  144)&(execstr("%t&uint8(1)","errcatch") ==  144);
test3 = (execstr("int32(1) & %t","errcatch") ==  144)&(execstr("%t&int32(1)","errcatch") ==  144);
test3 = (execstr("uint32(1) & %t","errcatch") ==  144)&(execstr("%t&uint32(1)","errcatch") ==  144);
test4 = (execstr("int16(1) & %t","errcatch") ==  144)&(execstr("%t&int16(1)","errcatch") ==  144);
test5 = (execstr("int8(1) & %f ","errcatch") ==  144)&(execstr("%t&int8(1)","errcatch") ==  144);

if test1 & test2 & test3 & test4 & test5 then
	affich_result(%T,623);
else
	affich_result(%F,623);
end
