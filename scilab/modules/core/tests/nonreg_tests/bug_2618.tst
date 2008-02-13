
// <-- Non-regression test for bug 2618  -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=2618
//
// <-- Short Description -->
// try/catch statement makes Scilab unstable, 

// Copyright INRIA
// Scilab Project - Serge Steer
// Date : 12 feb 2008

 
//catched error in a compiled function in a try
function r=ev(str), execstr('r='+str),endfunction
r=%f;a = 1;
try
   ev('a*z');
catch
   r=%t
end
if ~r then pause,end
[str,n]=lasterror();
if n<>4 then pause,end

//catched error in a uncompiled function in a try
clear ev
deff(' r=ev(str)','execstr(''r=''+str),','n')
r=%f;a = 1;
try
   ev('a*z');
catch
   r=%t
end
if ~r then pause,end
[str,n]=lasterror();
if n<>4 then pause,end


//catched error in a compiled function in a try in a compiled function
clear ev foo
function r=ev(str), execstr('r='+str),endfunction
function r=foo()
r=%f;a=1;
try
   ev('a*z');
catch
   r=%t;
end
endfunction
r=foo();
if ~r then pause,end
[str,n]=lasterror();
if n<>4 then pause,end

//catched error in a exec'ed compiled function in a try in a compiled function
clear ev foo
function r=ev(str), execstr('r='+str),endfunction
function r=foo()
r=%f;a=1;
try
   str='a*z';
   exec(ev);
catch
   r=%t;
end
endfunction
r=foo();
if ~r then pause,end
[str,n]=lasterror();
if n<>4 then pause,end

//catched error in a exec'ed uncompiled function in a try in a compiled function
clear ev foo
deff(' r=ev(str)','execstr(''r=''+str),','n')
function r=foo()
r=%f;a=1;
try
   str='a*z';
   exec(ev);
catch
   r=%t;
end
endfunction
r=foo();
if ~r then pause,end
[str,n]=lasterror();
if n<>4 then pause,end


//catched error in an uncompiled function in a try in a compiled function
clear ev foo
deff(' r=ev(str)','execstr(''r=''+str),','n')
function r=foo()
r=%f;
try
   ev('a*z')
catch
   r=%t
end
endfunction
r=foo();
if ~r then pause,end
[str,n]=lasterror();
if n<>4 then pause,end


//catched error in a compiled function in a try in a uncompiled function
clear ev foo
function r=ev(str), execstr('r='+str),endfunction
deff('r=foo()',[
'r=%f;a = 1;'
'try'
'   ev(a*z);'
'catch'
'   r=%t'
'end'],'n')
r=foo();
if ~r then pause,end
[str,n]=lasterror();
if n<>4 then pause,end



//catched error in a uncompiled function in a try in a uncompiled function
clear ev foo
deff(' r=ev(str)','execstr(''r=''+str),','n')
deff('r=foo()',[
'r=%f;a = 1;'
'try'
'   ev(a*z);'
'catch'
'   r=%t'
'end'],'n')
r=foo();
if ~r then pause,end
[str,n]=lasterror();
if n<>4 then pause,end


//catched error in a execstr in a try in a compiled function
clear  foo
function r=foo()
r=%f;a=1;
try
   execstr('r=a*z')
catch
   r=%t;
end
endfunction
r=foo();
if ~r then pause,end
[str,n]=lasterror();
if n<>4 then pause,end


//catched error in a exec in a try in a compiled function
clear  foo
mputl('r=a*z',TMPDIR+'/ev')
function r=foo()
r=%f;a=1;
try
   exec(TMPDIR+'/ev')
catch
   r=%t;
end
endfunction
r=foo();
if ~r then pause,end
[str,n]=lasterror();
if n<>4 then pause,end
