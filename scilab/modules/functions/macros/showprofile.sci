// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2013 - Samuel GOUGEON
// Copyright (C) 2014 - Scilab Enterprises
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
