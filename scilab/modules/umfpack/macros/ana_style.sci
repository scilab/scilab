//   Copyright Bruno Pinçon, ESIAL-IECN, Inria CORIDA project
//   <bruno.pincon@iecn.u-nancy.fr>
//
// This set of scilab 's macros provide a few sparse utilities.
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [col, mark] = ana_style(style)
    //
    //  an utility for PlotSparse
    //
    tab_col  = ["k" "b" "r" "g" "c" "m" "y" "t" "G"]
    num_col  = [ 1   2   5   3   4   6  32  16  13 ]
    tab_mark = ["." "+" "x" "*" "D" "d" "^" "v" "o"]
    num_mark = [ 0   1   2   3   4   5   6   7   9 ]

    n = length(style)
    if n >= 1 then
        c = part(style,1) ; ic = 1
        ind = grep(tab_col, c)
        if ind == [] then
            if isdigit(c) then
                while %t
                    ic = ic+1
                    if ic <= n then
                        c = part(style,ic)
                        if ~isdigit(c) then , break, end
                    else
                        break
                    end
                end
                col = evstr(part(style,1:ic-1))
                nb_col = xget("lastpattern")
                if col < 1  |  col > nb_col then
                    col = default_markColor
                end
            else
                col = default_markColor
            end
        else
            ic = 2
            col = num_col(ind)
        end
        reste = part(style,ic:n)
        if reste == "" then
            mark = default_markId
        else
            ind = grep(tab_mark, part(style,ic:n))
            if ind == [] then
                mark = default_markId
            else
                mark = num_mark(ind)
            end
        end
    else
        col = default_markColor ; mark = default_markId
    end

endfunction
