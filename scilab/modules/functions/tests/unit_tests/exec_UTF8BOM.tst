// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//<-- CLI SHELL MODE -->
﻿
tab_ref = [
"世界您好",
"азеазея",
"ハロー・ワールド",
"เฮลโลเวิลด์",
"حريات وحقوق",
"תוכנית"];

str_exec = 'disp(''OK'');';

for i = 1 : size(tab_ref,'*')
  mkdir(TMPDIR + filesep() + tab_ref(i));
  fd = mopen(TMPDIR + filesep() + tab_ref(i) + filesep() + tab_ref(i) + '.sce','wt');
  mputl(str_exec,fd);
  mclose(fd);
  exec(TMPDIR + filesep() + tab_ref(i) + filesep() + tab_ref(i) + '.sce');
  mdelete(TMPDIR + filesep() + tab_ref(i) + filesep() + tab_ref(i) + '.sce');
  rmdir(TMPDIR + filesep() + tab_ref(i));
end

// create a script file
mputl('a=1;b=2',TMPDIR+'/myscript')
// execute it
exec(TMPDIR+'/myscript')
if isdef('a')<> %t then pause,end
if isdef('b')<> %t then pause,end
if a <> 1 then pause,end
if b <> 2 then pause,end


// create a function
deff('y=foo(x)','a=x+1;y=a^2');
clear a b
// call the function
k = foo(1);
if isdef('k')<> %t then pause,end
if isdef('a')<> %f then pause,end
if k <> 4 then pause,end

x=4; //create x to make it known by the script foo
clear k y
exec(foo);
if isdef('y')<> %t then pause,end
if y <> 25 then pause,end

