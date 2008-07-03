//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//


//           CALLING EXTERNAL ROUTINE
txt=['      subroutine foof(a,b,c)';
     '      double precision  a,b,c';
     '      c=a+b ';
     '      return'; '       end'];

// we use TMPDIR for compilation 
	
if ~c_link('foof') then
  path = getcwd(); 
  chdir(TMPDIR); 
  mputl(txt,'foof.f');
  ilib_for_link(['foof'],'foof.f',[],"f");
  exec loader.sce 
  chdir(path) 
end	

//5+7 by C function
call('foof',5,1,'d',7,2,'d','out',[1,1],3,'d')

