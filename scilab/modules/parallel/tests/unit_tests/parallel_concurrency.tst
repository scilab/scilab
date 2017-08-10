// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Bernard HUGUENEY
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


// <-- CLI SHELL MODE -->
function r=dummy_arg(n)
  r= parallel_concurrency();
endfunction;
if parallel_concurrency() <> 0 then pause, end
// parallel_run requires a function taking at least one argument
r= parallel_run(1:2,dummy_arg);
// all results must be <>0 (hence %T)
if ~and(r) then pause, end
