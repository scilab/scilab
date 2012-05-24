// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

//================================================
// test addinter
//================================================
ilib_verbose(0);
if ~c_link('libintertest') then
  curPath = pwd(); 
  // creates a subdirectory in TMPDIR
  // dynamic link on linux copy some files in TMPDIR
  // then to not have some conflict 
  // we create a subdirectory in TMPDIR with sources
  // previously we had :
  // lib_gen_Make: Did not copy libintertest.c: Source and target directories are the same (/tmp/SD_28668_).

  chdir(TMPDIR); 
  mkdir('addinter');
  chdir('addinter'); 
  src_file = SCI+filesep()+'modules'+filesep()+'dynamic_link'+filesep()+'tests'+filesep()+'unit_tests'+filesep()+'addinter.c';
  dst_file = TMPDIR + filesep() + 'addinter' + filesep() + 'addinter.c';
  copyfile(src_file, dst_file);

  files=['addinter.c'];
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

