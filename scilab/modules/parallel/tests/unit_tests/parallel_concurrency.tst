// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Bernard HUGUENEY
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


// <-- CLI SHELL MODE -->
function r=dummy_arg(n)
  r= parallel_concurrency();
endfunction;
if parallel_concurrency() <> 0 then pause, end
// parallel_run requires a function taking at least one argument
r= parallel_run(1:2,dummy_arg);
// all results must be <>0 (hence %T)
if ~and(r) then pause, end
