// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2013 - Samuel GOUGEON
// Copyright (C) 2014 - Scilab Enterprises
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// === LICENSE_END ===

function showprofile(fun)
    if argn(2) < 1 then
        error(sprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "showprofile", 1));
    end

    // get profiling results
    lst = macr2lst(fun);
    prof = profile(lst);

    // convert to string profiling results
    nb_calls = string(prof(:,1));
    cpu_time = string(round(100*prof(:,2))/100);
    effort = string(prof(:,3));

    // get function code
    code = fun2string(lst, "fun");

    // line numbers, right justified
    line_numbers = string(1:size(code, "r"))';
    line_numbers = justify(line_numbers, "r");

    // left justify
    nb_calls = justify(nb_calls, "l");
    cpu_time = justify(cpu_time, "l");
    effort = justify(effort, "l");

    printf("|%s|%s|%s| %s: %s\n", nb_calls, cpu_time, effort, line_numbers, code);
endfunction
