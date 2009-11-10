// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- ENGLISH IMPOSED -->

// exec SCI/modules/dynamic_link/tests/unit_tests/addinter_loop.tst;

//================================================
// test addinter
// Create 77 gateways with 2 functions by gateways
// loop_max depends of some system parameters :
// * memory available
// * O.S : example on Windows , we cannot open more
// than 80 dlls simultanously with LoadLibrary
// numbers of files already openend
// scilab minimun 3 LoadLibrary (libscilab.dll,
// jvm.dll,fftw.dll)
lines(0);
//================================================
loop_max = 77; // may be more on linux
//================================================
ulink();
curPath = pwd();
chdir(TMPDIR); 
//================================================
// creates C functions
//================================================
// load .c file 
test_path = get_absolute_file_path('addinter_loop.tst');
template_filename = test_path + filesep() + 'addinter_template.c';
template = mgetl(template_filename);
//================================================
// creates loop_max gateways with 2 functions
// files in TMPDIR
for i = 0 : loop_max
  txtfile = strsubst(template,'interf_template_A0','interf_template_A'+string(i));
  txtfile = strsubst(txtfile,'interf_template_B0','interf_template_B'+string(i));
  txtfile = strsubst(txtfile,'int iValue = 0;','int iValue = '+string(i)+';');
  
  DIR_NAME_ADDINTER = TMPDIR + filesep() + 'addinter_dir_' + string(i);
  
  mkdir(DIR_NAME_ADDINTER);
  mputl(txtfile,DIR_NAME_ADDINTER + filesep() + 'addinter' + string(i) + '.c');
end
//================================================
// link functions and gateways to Scilab
for i = 0 : loop_max
  scilab_functionA = 'scifunA' + string(i);
  scilab_functionB = 'scifunB' + string(i);
  
  c_functionA = 'interf_template_A'+string(i);
  c_functionB = 'interf_template_B'+string(i);
  
  table = [scilab_functionA,c_functionA; ..
           scilab_functionB,c_functionB];
  
  c_file = 'addinter' + string(i) + '.c';
  interf_name = 'interftest_' + string(i);
  
  DIR_NAME_ADDINTER = TMPDIR + filesep() + 'addinter_dir_' + string(i);
  chdir(DIR_NAME_ADDINTER);
  ilib_build(interf_name,table,c_file,[]);

end
//================================================
// checks scilab functions
for i = 0 : loop_max

  DIR_NAME_ADDINTER = TMPDIR + filesep() + 'addinter_dir_' + string(i);
  chdir(DIR_NAME_ADDINTER);
  
  // disable message
  warning_mode = warning('query');
  warning('off');
  
  string_to_execute = 'exec(''loader.sce'');' ;
  ierr = execstr(string_to_execute,'errcatch');
  if ierr <> 0 then pause,end
 
  // enable message
  warning(warning_mode)
 
  scilab_function = 'scifunA' + string(i);
  string_to_execute = 'if ' + scilab_function + '() <> '+ string(i) +' then pause,end; ';
  ierr = execstr(string_to_execute,'errcatch');
  if ierr <> 0 then pause,end
  
  scilab_function = 'scifunB' + string(i);
  string_to_execute = 'if ' + scilab_function + '() <> '+ string(i) +' then pause,end; ';
  ierr = execstr(string_to_execute,'errcatch');
  if ierr <> 0 then pause,end
end;
//================================================
// remove functions in scilab
for i = 0 : loop_max
  scilab_function = 'scifunA' + string(i);
  clearfun(scilab_function);
  scilab_function = 'scifunB' + string(i);
  clearfun(scilab_function);
end
//================================================
// free dynamic librairies
ulink();
chdir(curPath);
//================================================
