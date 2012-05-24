// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 438 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=438
//
// <-- Short Description -->
// dummy =sprintf('%e', 1000) buggy 

dummy = sprintf('%e', 100);
if getos() == 'Windows' then
 if dummy <> "1.000000e+002" then pause, end
else
 if dummy <> "1.000000e+02" then pause, end
end
  
dummy2 =sprintf('%e', 1000);
if getos() == 'Windows' then
  if dummy2 <> "1.000000e+003" then pause, end
else
  if dummy2 <> "1.000000e+03" then pause, end
end  

dummy3 =sprintf('%e', 10000);
if getos() == 'Windows' then
  if dummy3 <> "1.000000e+004" then pause, end
else
  if dummy3 <> "1.000000e+04" then pause, end
end  
