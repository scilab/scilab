// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function browsevar_seeSpecial(Var, varname)
    // Internal function called by browsevar
    tv = type(Var)
    if tv==9 then   // graphic handles
        // We call ged() for the figure of the first component
        Var = Var(1) // In case of vector, we consider only the 1st element
        if Var.type=="Figure"
            ged(8, Var.figure_id)
        elseif Var.type=="Axes"
            prevAxes = gca()
            sca(Var)
            ged(9, Var.parent.figure_id)
            sca(prevAxes)
        else
            while and(Var.type <> ["Figure" "Axes"])
                Var = Var.parent
            end
            browsevar_seeSpecial(Var, varname)
        end

    elseif tv==13   // function
        edit(varname)

    elseif tv==14   // library of functions
        tmp = string(Var)
        path = tmp(1)
        funs = tmp(2:$)
        [?, k] = gsort(convstr(funs), "g", "i")
        funs = funs(k);
        n = size(funs,"*")
        nr = ceil(sqrt(n*1.5))
        nc = floor(n/nr)
        nr = ceil(n/nc)
        funs = [funs ; emptystr(nr*nc-n,1)]
        funs = matrix(funs,nr,-1)
        funs = prettyprint(funs,"html","",%f,%f)
        funs = strsubst(funs, " align=""center""", "")
        nl = ascii(10)
        tmp = "<tr><td colspan=""%d"">%s%s%s%s</td></tr>"
        funs = "<table cellspacing=""0"" cellpadding=""2"">" + nl + ..
            msprintf(tmp, nc,"<b>",_("Library "), "</b>", varname) + nl + ..
            msprintf(tmp, nc,"<b>",_("Path = "),"</b>", path) + nl + ..
            msprintf(tmp, nc,"<b>",_("Functions:"),"</b>", "") + nl + ..
            funs + ..
            "</table>";
        messagebox(funs, varname)

    elseif typeof(Var)=="st"
        tree_show(Var, varname)

    elseif or(typeof(Var)==["ce" "syslin"])
        p = prettyprint(Var, "html", "_")
        messagebox("<html>" + p, varname)

    elseif or(tv==[15 16 17])
        tree_show(Var, varname)
    end

endfunction
