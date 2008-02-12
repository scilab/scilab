// <-- Non-regression test for bug 2611 (part 1) -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=2611
//
// <-- Short Description -->
// for loop variable not removed when an error arises within a for called in try context

// Copyright INRIA
// Scilab Project - Serge Steer
// Date : 12 feb 2008

  	

function r=test()
  r=%f
  try
    for j=1:3, error('test',1001); end
  catch
    r=%t
  end
endfunction
if ~test() then pause,end
if lasterror()<>'test' then pause,end


deff('r=test()',['r=%f'
'try,'
'    for j=1:3, error(''test'',1001); end,'
'catch'
'    r=%t'
'end'],'n')
if ~test() then pause,end
if lasterror()<>'test' then pause,end

