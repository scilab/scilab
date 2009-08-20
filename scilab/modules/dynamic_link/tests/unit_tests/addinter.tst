// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- JVM NOT MANDATORY -->

//================================================
// test addinter
//================================================
if ~c_link('libintertest') then
  curPath = pwd(); 
  copyfile(SCI+filesep()+'modules'+filesep()+'dynamic_link'+filesep()+'tests'+filesep()+'unit_tests'+filesep()+'addinter.c', TMPDIR+filesep()+'addinter.c');
  chdir(TMPDIR); 
  files=['addinter.o'];
  ilib_build('libintertest',['scifun1','intfun1'],files,[]);
  
  // load the shared library 
  exec loader.sce ;
  
  //  check addinter add scifun1
  if ( funptr('scifun1') == 0 ) then pause;end 
  
  if norm(scifun1(%pi)- sin(%pi+1)/%pi ) > %eps then pause;end 
  
  // remove scifun1
  if clearfun('scifun1') <> %T  then pause;end 
  
  // remove dynamic library link
  ulink();
  
  chdir(curPath);
end
//================================================
