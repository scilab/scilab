//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
// This file is distributed under the same license as the Scilab package.
//

if haveacompiler() then

//           CALLING EXTERNAL FORTRAN SUBROUTINE

foof = ['      subroutine foof(a,b,c)';
        '      double precision  a,b,c';
        '      c=a+b ';
        '      return'; '       end'];

  mprintf('\n');
  mprintf(gettext('Fortran subroutine to call in scilab.\n'));
  
  disp(foof);

// we use TMPDIR for compilation 
	
if ~c_link('foof') then
  path = pwd(); 
  chdir(TMPDIR); 
  mputl(foof,'foof.f');
  
  mprintf('\n');
  mprintf(gettext('Call ilib_for_link to build fortran subroutine.\n'));
  
  ilib_for_link(['foof'],'foof.f',[],"f");
  exec loader.sce ;
  chdir(path) 
end	

//Z = X+Y by fortran subroutine
X = 5;
Y = 7;

mprintf('\n');
mprintf(gettext('Call Fortran subroutine. Z = X+Y'));
mprintf('\n');
mprintf(gettext('with X = %d'), X);
mprintf('\n');
mprintf(gettext('with Y = %d'), Y);
mprintf('\n');
mprintf("Z = call(''foof'', X, 1, ''d'', Y, 2, ''d'', ''out'', [1,1], 3, ''d'');");
mprintf('\n');
Z = call('foof', X, 1, 'd', Y, 2, 'd', 'out', [1,1], 3, 'd');
mprintf(gettext('result Z = %d'), Z);
mprintf('\n');

end