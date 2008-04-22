// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Laurent VAYLET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//-- Disable vertical scrolling
lines(0);

//-- Load scicos_nonreg.sci
exec('scicos_nonreg.sci');

//-- Set base directory for non-regression tests, according to version used
if getversion() == 'scilab-4.1.2' | getversion() == 'Scilab-4.1.2-SVN'
  baseDir = '/home/vaylet/dev/scilab-5.0/modules/scicos/tests/nonreg_tests';
elseif getversion() == 'scilab-trunk-SVN' | getversion() == 'scilab-5.0'
  baseDir = fullfile(SCI,'modules','scicos','tests','nonreg_tests');
end

//-- Launch non-regression tests
scicos_nonreg(baseDir);

//-- Exit
exit

