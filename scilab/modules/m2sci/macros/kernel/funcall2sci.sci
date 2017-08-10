// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [sci_equiv]=funcall2sci(mtlb_expr)
    // M2SCI function
    // Convert a function call in an instruction or in an expression from Matlab to Scilab
    // Input:
    // - mtlb_instr: Matlab instr or expression to convert
    // Output:
    // - sci_instr: Scilab equivalent for mtlb_instr

    rhslist=mtlb_expr.rhs
    if rhslist==[] then // Function called as a command
        rhsnb=-1
        rhslist=list()
        mtlb_expr.rhs=list()
    else
        rhsnb=size(rhslist)
    end
    // Init returned value
    sci_expr=mtlb_expr

    rhsind=1
    while rhsind<=rhsnb
        [sci_equiv]=expression2sci(rhslist(rhsind));
        mtlb_expr.rhs(rhsind)=sci_equiv;
        rhsind=rhsind+1;
    end

    // Performs the conversion of function call
    lhs=mtlb_expr.lhsnb
    if rhsnb==-1 then
        rhs=-1
    else
        rhs=size(mtlb_expr.rhs)
    end
    funname=mtlb_expr.name

    // If a translation function exists
    if exists("sci_"+funname)==1 then
        execstr("[sci_equiv]=sci_"+funname+"(mtlb_expr)");
        // If I don't know where I can search other M-files
    elseif res_path==[] then
        sci_equiv=default_trad(mtlb_expr)
    else
        sci_tmpfile =pathconvert(TMPDIR)+pathconvert(fnam)+"sci_"+funname+".sci"
        tmpierr=execstr("exec(sci_tmpfile,-1)","errcatch");errclear();
        sci_file=res_path+"sci_"+funname+".sci"
        ierr=execstr("exec(sci_file,-1)","errcatch");errclear();
        if tmpierr==0 then
            execstr("[sci_equiv]=sci_"+mtlb_expr.name+"(mtlb_expr)");
            // If a translation function exists
        elseif ierr==0 then
            execstr("[sci_equiv]=sci_"+mtlb_expr.name+"(mtlb_expr)");
            // If no translation indication given
        elseif Recmode then
            // Check if the M-file exists in the given paths
            path=mfile_path(funname)
            if path==[] then
                sci_equiv=default_trad(mtlb_expr)
            elseif or(funname==nametbl)
                sci_equiv=sci_generic(mtlb_expr)
            else
                fnam=funname
                scipath=res_path+fnam+".sci"
                scepath=res_path+fnam+".sce"
                catpath=res_path+fnam+".cat"
                res=0
                if newest(path,scipath,scepath)==1 then
                    res=mfile2sci(path,res_path,%F,%T)
                end
                if res==1 then
                    exec(sci_file, -1)
                    ierr=execstr("[sci_equiv]=sci_"+mtlb_expr.name+"(mtlb_expr)","errcatch");
                    if ierr<>0 then
                        error(msprintf(gettext("Error while executing : [sci_equiv]=sci_%s(mtlb_expr)."),mtlb_expr.name));
                    end
                else
                    sci_equiv=default_trad(mtlb_expr)
                end
            end
        else  // Default translation
            sci_equiv=default_trad(mtlb_expr)
        end
    end

    // If equivalent is a funcall, number of lhs can have changed
    if typeof(sci_equiv)=="funcall" then
        sci_equiv.lhsnb=size(sci_equiv.lhs)
    end

endfunction
