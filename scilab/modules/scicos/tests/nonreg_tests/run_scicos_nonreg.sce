// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Laurent VAYLET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// This file can be used to launch Scicos non-regression tests from a
// shell script:
//
//  $ cd /home/vaylet/dev/scilab-5.0/modules/scicos/tests/nonreg_tests
//  $ ../../../../bin/scilab -nw -nb -f run_scicos_nonreg.sce

//-- Disable vertical scrolling
lines(0);

//-- Load scicos_nonreg.sci
getf('scicos_nonreg.sci');

//-- Launch non-regression tests
scicos_nonreg();
