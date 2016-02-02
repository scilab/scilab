// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// =============================================================================
// Launch unitary tests
// =============================================================================

function unit_test_run(varargin)

    lhs = argn(1);
    rhs = argn(2);

    if (rhs == 0) then
        test_run([],[],["unit_tests"]);

    elseif rhs == 1 then
        argument_1 = varargin(1);
        test_run(argument_1,[],["unit_tests"]);

    elseif rhs == 2 then
        argument_1 = varargin(1);
        argument_2 = varargin(2);
        test_run(argument_1,argument_2,["unit_tests"]);

    elseif rhs == 3 then
        argument_1 = varargin(1);
        argument_2 = varargin(2);
        argument_3 = varargin(3);
        test_run(argument_1,argument_2,[argument_3,"unit_tests"]);

    end

endfunction
