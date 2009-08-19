//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
// This file is distributed under the same license as the Scilab package.
//

//           CALLING EXTERNAL C FUNCTION

if haveacompiler() then

 
// we use TMPDIR for compilation 
	
  foo=['void foo(double *a,double *b,double *c)';
     '{ *c = *a + *b; }'  ];
     
  mprintf('\n');
  mprintf(gettext('C Function to call in scilab.\n'));
  
  disp(foo);
  	
if ~c_link('foo') then
  cur_ilib_verbose = ilib_verbose();
  ilib_verbose(1);
  path = pwd(); 
  chdir(TMPDIR); 
  mputl(foo,'foo.c');
  
  mprintf('\n');
  mprintf(gettext('Call ilib_for_link to build C function.\n'));
  ilib_for_link(['foo'],'foo.c',[],"c");
  exec loader.sce ;
  chdir(path);
  ilib_verbose(cur_ilib_verbose);
end	

//Z = X+Y by C function
X = 5;
Y = 7;
mprintf('\n');
mprintf(gettext('Call C function. Z = X+Y'));
mprintf('\n');
mprintf(gettext('with X = %d'), X);
mprintf('\n');
mprintf(gettext('with Y = %d'), Y);
mprintf('\n');
mprintf("Z = call(''foo'', X, 1, ''d'', Y, 2, ''d'', ''out'', [1,1], 3, ''d'');");
mprintf('\n');
Z = call('foo', X, 1, 'd', Y, 2, 'd', 'out', [1,1], 3, 'd');
mprintf(gettext('result Z = %d'), Z);
mprintf('\n');

end