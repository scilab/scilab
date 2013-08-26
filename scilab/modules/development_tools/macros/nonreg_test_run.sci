// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// =============================================================================
// Launch Non-Regression tests
// =============================================================================

function nonreg_test_run(varargin)

    lhs = argn(1);
    rhs = argn(2);

    if (rhs == 0) then
        test_run([],[],["nonreg_tests"]);

    elseif rhs == 1 then
        argument_1 = varargin(1);
        test_run(argument_1,[],["nonreg_tests"]);

    elseif rhs == 2 then
        argument_1 = varargin(1);
        argument_2 = varargin(2);
        test_run(argument_1,argument_2,["nonreg_tests"]);

    elseif rhs == 3 then
        argument_1 = varargin(1);
        argument_2 = varargin(2);
        argument_3 = varargin(3);
        test_run(argument_1,argument_2,[argument_3,"nonreg_tests"]);

    end

endfunction
