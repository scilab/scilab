// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 6682 -->
//
// <-- Short Description -->
// printf functions do not manage octal %o format
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6682
//

printf('%o',8);
mprintf('%o',8);
mfprintf(6,'%o',8);
r = msprintf('%o',8);
if r <> dec2oct(8) then pause,end

r = msprintf('%10o',80);
if r <> '       120' then pause,end

for i = 1:1000
  r = msprintf('%o',i);
  if r <> dec2oct(i) then pause,end
end
