// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Vincent Couvert
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function C = tree2code(P, bprettyprintformat)
    // Convert a tree returned by macr2tree() to a macro code
    // Input:
    // - P: 'program' tlist
    // - bprettyprintformat: boolean value, if FALSE (default value), generated code is not formatted else it is
    // Output:
    // - C: character string matrix (colum vector)

    // Verify good type for input
    if typeof(P)<>"program" then
        msg = gettext("%s: Wrong type for input argument #%d: ''%s'' expected.\n")
        error(msprintf(msg, "tree2code", 2, "program tree"));
    end

    // Default value
    rhs = argn(2);
    if rhs==1 then
        bprettyprintformat = %F
    end

    C=""

    // Function prototype
    lhsstr = ""
    rhsstr = ""
    if P.name<>"" then // Not a batch file
        for k = 1:size(P.outputs)
            lhsstr = [lhsstr, expression2code(P.outputs(k))]
        end
        if size(lhsstr,2)>1
            lhsstr = "["+strcat(lhsstr(2:$),",")+"] = "
        end

        for k = 1:size(P.inputs)
            rhsstr = [rhsstr, expression2code(P.inputs(k))]
        end
        if size(rhsstr,2)>1
            rhsstr = strcat(rhsstr(2:$),",")
        end
        rhsstr = "(" + rhsstr + ")"

        C = "function " + lhsstr + P.name + rhsstr;
    end

    I = P.statements

    // To avoid to have some instructions on the first line (function prototype)
    if I(1)<>list("EOL") then
        C = cat_code(C, instruction2code(list("EOL"),%T));
    end

    // For each statement, generate corresponding code
    header = ""
    if bprettyprintformat then
        File = "SCIHOME/XConfiguration.xml"
        if isfile(File)
            path = "//interface/scinotes/display/body/scinotes-display"
            tmp = evstr(xmlGetValues(path, "indent-size", File))
        else
            tmp = 4
        end
        header = blanks(tmp)
    end
    for i = 1:size(I)-2 // -2 to ignore last return+EOL
        C = cat_code(C, header + instruction2code(I(i), bprettyprintformat));
        C = format_txt(C, I(i), bprettyprintformat, I(i+1));
    end

    if P.name<>"" then // Not a batch file
        C = cat_code(C, "endfunction"); // Replace last return
        C = cat_code(C, "");
    end
endfunction
