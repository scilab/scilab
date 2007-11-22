// <-- Non-regression test for bug 1768 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=1768
//
// <-- Short Description -->
//    I just downloaded Scilab-4.0rc1.
//    Whenever I write a new function and try to write the relative help 
//    file, xmltohtml crashes. Apparently, it does not found the whatis file 
//    even if it was just created successfully

// Copyright INRIA
// Scilab Project - Serge Steer
// Copyright INRIA 2005
// Date : 20 janvier 2006

function y=foo1(a,b)
  y=a+b
endfunction
T=help_skeleton('foo1')
mputl(strsubst(T,'<LINK> add a key here</LINK>','<LINK>abs</LINK>'), ...
      TMPDIR+'/foo1.xml')
ok=%t
try
  xmltohtml(TMPDIR)
catch
  ok=%f
end
if ok
  try
    xmltohtml('SCI/modules/elementaries_functions/help/'+getlanguage())
  catch
    ok=%f
  end 
end
affich_result(ok,1768);
