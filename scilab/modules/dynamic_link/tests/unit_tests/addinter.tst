// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->

//================================================
// test addinter
//================================================
if ~c_link('libintertest') then
  curPath = getcwd(); 
  copyfile(SCI+filesep()+'modules'+filesep()+'dynamic_link'+filesep()+'tests'+filesep()+'unit_tests'+filesep()+'addinter.c', TMPDIR+filesep()+'addinter.c');
  chdir(TMPDIR); 
  files=['addinter.o'];
  ilib_build('libintertest',['scifun1','intfun1'],files,[]);
  
  // disable message
  warning_mode = warning('query');
  warning('off');
  // load the shared library 
  exec loader.sce ;
  
  // enable message
  warning(warning_mode);
  
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
