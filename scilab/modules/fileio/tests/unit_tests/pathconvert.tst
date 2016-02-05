//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

ref = 'SCI/modules/fileio/macros/foo.sci';
r = pathconvert('SCI/modules/fileio\macros/foo.sci',%f,%f,'u');
if r <> ref then pause, end

ref = 'SCI\modules\fileio\macros\foo.sci';
r = pathconvert('SCI/modules/fileio\macros/foo.sci',%f,%f,'w');
if r <> ref then pause, end

if getos() == 'Windows' then
  ref = WSCI + '\modules\fileio\macros\cd.sci';
else
  ref = SCI + '\modules\fileio\macros\cd.sci';
  ref = strsubst(ref,filesep(),'\');
end
r = pathconvert('SCI/modules/fileio/macros/cd.sci',%f,%t,'w');
if r <> ref then pause, end

ref = home + '\modules\fileio\macros\foo.sci';
if getos() <> 'Windows' then
 ref = strsubst(ref,filesep(),'\');
end
r = pathconvert('HOME/modules/fileio/macros/foo.sci',%f,%t,'w');
if r <> ref then pause, end

ref = '/cygdrive/c/tmp';
r = pathconvert('c:/tmp',%f,%t,'u');
if r <> ref then pause, end

ref = 'c:\tmp';
r = pathconvert('/cygdrive/c/tmp',%f,%f,'w');
if r <> ref then pause, end
