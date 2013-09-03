// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [sci_equiv]=getvar2sci(var,lhslist)
    // Translate the named variable acquisition

    // Global variable for M2SCI
    global("varslist")

    rhs=argn(2)
    if rhs<>2 then
        error(gettext("Wrong number of inputs."))
    end

    // Search variable name in variable name table
    [boolval,index]=isdefinedvar(var)
    varname=var.name

    // Variable is not defined yet
    if ~boolval then
        scinam=varname

        // If Matlab variable name is a function name in Scilab
        if funptr(varname)<>0 then
            scinam="%"+varname
        end

        // Undefined variable may be a M-file or a global variable
        // Check if a corresponding M-file exists here
        if isanmfile(varname) then
            // A M-file without parameter
            if verbose_mode<0 then
                m2sci_info(msprintf(gettext("L.%d: Unknown variable %s is a M-file."),nblines,varname),-1);
            end
            sci_equiv=Funcall(varname,size(lhslist),list(),lhslist)
        elseif exists("sci_"+varname)==1 then
            // A translated function without parameter
            if verbose_mode<0 then
                m2sci_info(msprintf(gettext("L.%d: Unknown variable %s is a M-file (sci_%s exists)."),nblines,varname,varname),-1);
            end
            sci_equiv=Funcall(varname,size(lhslist),list(),lhslist)
        elseif or(varname==["i","j"]) then
            if verbose_mode<0 then
                m2sci_info(msprintf(gettext("Variable %s supposed to be the Imaginary unit."),varname),-1);
            end
            sci_equiv=Variable("%i",Infer(list(1,1),Type(Double,Complex)))
        else
            // Try to find what is 'varname'
            sci_equiv=get_unknown(varname,lhslist)
            if verbose_mode<0 then
                m2sci_info(msprintf(gettext("L.%d: Unknown variable %s."),nblines,varname),-1);
            end
        end
    else
        sci_equiv=Variable(varname,Infer(varslist(index).dims,varslist(index).type))
    end
endfunction
