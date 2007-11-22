// <-- Non-regression test for bug 1180 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=1180
//
// <-- Short Description -->
//    mtlb_filter lines 
//
//    b=b*z^(max(degree(a)-degree(b),0))
//    a=a*z^(max(degree(b)-degree(a),0))
//
//    should be replaced by :
//
//    da = degree(a)
//    db = degree(b)
//    if (da-db) > 0 then b=b*z^(da-db); end 
//    if (db-da) > 0 then a=a*z^(db-da); end


// Non-regression test file for bug 1180
// Pierre MARECHAL - Scilab Project
// Copyright INRIA
// 1er juin 2005

mode(-1)
clear

x = zeros(1:10);
x(5) = 1;
b = [ 0 1 0 ]
y = mtlb_filter(b,1,x);

if y==[0,0,0,0,0,1,0,0,0,0] then
	affich_result(%T,1180);
else
	affich_result(%F,1180);
end
