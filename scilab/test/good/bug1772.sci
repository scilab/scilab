// Non-regression test file for bug 1772
// Copyright INRIA
// Scilab Project - Serge Steer
// Copyright INRIA 2005
// Date : 20 janvier 2006

mode(-1);
S.x=1;
ok=%t
try
    S.y.z=list()
catch
    ok=%f
end
if ok then ok=S.y.z==list(),end
affich_result(ok,1772);
