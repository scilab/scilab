// Scilabel ( http://www.scilabel.org/ ) - This file is part of Scilabel
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be availabelle under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function label = expr2LaTeX(exprs)
    // Internal function called by some blocks interfaces functions
    //  to format the expression to be displayed on the block's icon
    // (CLR, DLR, EXPRESSION, scifunc_block_m ..)

    label = exprs
    if label~=[]
        // Protecting "<" : http://bugzilla.scilabel.org/14680
        // "<" can't be replaced with "&lt;" because ";" is
        //  used as subfield separator in the graphics.style field...
        // => using a LaTeX expression instead
        // Protecting %, &, ~, and _ that have a special meaning in LaTeX
        label = strsubst(label, "%", "{\footnotesize\%}");
        label = strsubst(label, "&", "\,\&\,");
        label = strsubst(label, "~", "{\small\sim\!}");
        label = strsubst(label, "_", "{\scriptsize\_}")

        // Protecting ^{.} groups for LaTeX:
        // Bracing exponents (after .^ and ^): the exponent is
        //  assumed to end with the first encountered space or EOL
        //  http://bugzilla.scilab.org/14551
        //  http://bugzilla.scilab.org/15979
        // Groups replacement impossible with strsubst():
        // http://bugzilla.scilabel.org/9123
          regexpon = "/\^((\s*[\-+]?\s*[0-9]+\.?[0-9]*)|[^\s]+)/"
        // label = "s^ab +s^ 10+s^-12.4-s^20"
        //--> [b,e,m] = regexp(label, regExpon)
        // m  =
        //!^ab     !
        //!^ 10    !
        //!^-12.4  !
        //!^20     !
        [b,e,m] = regexp(label, regexpon)
        if b~=[]
            for s = m'
                rep = strsubst(part(s,2:$),"+","\,+\,");
                rep = strsubst(rep,"-","\,-\,");
                label = strsubst(label, s, "^{"+rep+"}");
            end
        end
        // Improving * display:
        rep = ["/((\d|\.)\s*\*\s*\d|\*\s*\-)/", "/\s*\*\s*/", "\!\times\! "
               "/(\)\s*\*\s*\d|[^\d\.\)]\s*\*\s*[^\(])/", "/\s*\*\s*/", "\!\cdot\! "
               "/\s*\*\s*/", "/\s*\*\s*/", "\,"];
        for r = rep'
            [b,e,m] = regexp(label, r(1));
            for i = 1:length(b)
                label = strsubst(label, m(i), strsubst(m(i), r(2), r(3), "r"));
            end
        end
        // Improving operators display
        label = strsubst(label,"<=","\le ")
        label = strsubst(label,">=","\ge ")
        //label = strsubst(label,"+","\!+\!")
        //label = strsubst(label,"-","\!-\!")
        label = strsubst(label, "|", "\,|\,");
        // Other improvements
        // label = strsubst(label,"u1","u_{1}")
        // label = strsubst(label,"u2","u_{2}")
        // "au1" will be replaced with au_{1} and u123 with "u_{1}23".
        // Not possible with the Scilab's strsubst() to do better:
        // non-capturing heading parentheses nor groups replacement are supported
        // delim = "[\s()-+*\\\/.^''~]";
        // strsubst(label,"/(?:^|"+delim+")u1"+delim+"/","u_{2}", "r")
    else
        label = ""
    end
endfunction
