// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Vincent Couvert
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function C=rhs2code(rhs)
    // Translate rhs list of an instruction to macro code (called by tree2code)
    // Input:
    // - rhs: list of rhs arguments
    // Output:
    // - C: Scilab code corresponding to rhs
    // V.C.

    n=size(rhs)

    C=""
    if typeof(rhs)<>"list" then
        C=expression2code(rhs)
    else
        for k=1:n
            therhs = expression2code(rhs(k));
            if k==1 then
                C = therhs;
            else
                if therhs == "(EOL)" then
                    C = [C;""];
                else
                    if C($)=="" then
                        // Previous rhs was a EOL
                        C = [C(1:($-2));
                        C($-1)+"..";
                        C($) + therhs(1);
                        therhs(2:$)];
                    else
                        C = [C(1:($-1));
                        C($) + "," + therhs(1);
                        therhs(2:$)];
                    end
                end
            end
        end
    end


endfunction
