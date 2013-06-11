// Non-regression test file for bug 1768
// Copyright INRIA
// Scilab Project - Serge Steer
// Copyright INRIA 2005
// Date : 20 janvier 2006

mode(-1);
clear

function y=foo1(a,b)
    y=a+b
endfunction
T=help_skeleton("foo1")
mputl(strsubst(T,"<LINK> add a key here</LINK>","<LINK>abs</LINK>"), ...
TMPDIR+"/foo1.xml")
ok=%t
try
    xmltohtml(TMPDIR)
catch
    ok=%f
end
if ok
    try
        xmltohtml("SCI/modules/elementaries_functions/help/eng")
    catch
        ok=%f
    end
end
affich_result(ok,1768);
clear
