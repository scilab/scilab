// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function sci_equiv = getvar2sci(var,lhslist)
    // Translate the named variable acquisition

    // Global variable for M2SCI
    global("varslist")

    if argn(2) <> 2 then
        error(gettext("Wrong number of inputs."))
    end

    isFunctionHandle = part(var.name,1)=="£" // set by m2sci_syntax() for @entries
    if isFunctionHandle then
        var.name = part(var.name, 2:$)
        [boolval, index] = isdefinedvar(var)
    end
    // Search variable name in variable name table
    [boolval, index] = isdefinedvar(var)
    varname = var.name

    // Variable is not defined yet
    if ~boolval then
        scinam = varname

        // If Matlab variable name is a function name in Scilab
        if funptr(varname)<>0 then
            scinam = "%" + varname
        end

        // Undefined variable may be a M-file or a global variable
        // Check if a corresponding M-file exists here
        if isanmfile(varname) then
            // A M-file without parameter
            if verbose_mode<0 then
                m2sci_info(msprintf(gettext("L.%d: Unknown variable %s is a M-file."),nblines,varname),-1);
            end
            if isFunctionHandle
                sci_equiv = Variable(varname, Infer(list(1,1),Type(Function, Unknown)))           else
                sci_equiv = Funcall(varname, size(lhslist), list(), lhslist)
            end

        elseif exists("sci_"+varname)==1 then
            // A translated function without parameter
            if verbose_mode<0 then
                m2sci_info(msprintf(gettext("L.%d: Unknown variable %s is a M-file (sci_%s exists)."),nblines,varname,varname),-1);
            end
            sci_equiv = Funcall(varname, size(lhslist), list(), lhslist)
            if isFunctionHandle
                sci_equiv = Variable(varname, Infer(list(1,1),Type(Function, Unknown)))           else
                sci_equiv = Funcall(varname, size(lhslist), list(), lhslist)
            end

        elseif or(varname==["i","j"]) then
            if verbose_mode<0 then
                m2sci_info(msprintf(gettext("Variable %s supposed to be the Imaginary unit."),varname),-1);
            end
            sci_equiv=Variable("%i", Infer(list(1,1), Type(Double,Complex)))

        else
            // Try to find what is 'varname'
            sci_equiv = get_unknown(varname,lhslist)
            if verbose_mode<0 then
                m2sci_info(msprintf(gettext("L.%d: Unknown variable %s."),nblines,varname),-1);
            end
        end
    else
        sci_equiv = Variable(varname, Infer(varslist(index).dims,varslist(index).type))
    end
endfunction

// -----------------------------------------------------------------------------

function sci_equiv = get_unknown(varname,lhslist)
    // Private function called only within getvar2sci()
    //
    // Handle cases where varname appear in an expression while it is not known.

    // nam may be:
    // - a variable created in another clause part
    // - a M-file called without args
    // - a variable created by an eval

    // Check clause !!!

    // Check if it is a Matlab function not converted yet
    if or(varname==not_yet_converted()) then
        set_infos(msprintf(gettext("Matlab function %s not yet converted."),varname),2)
        tmpvar = Variable(varname, Infer())
        sci_equiv = Funcall("mtlb", 1, Rhs_tlist(tmpvar), lhslist)
    else
        // Other cases: I am not able to determine what is nam
        set_infos(msprintf(gettext("mtlb(%s) can be replaced by %s() or %s whether %s is an M-file or not."), varname, varname, varname, varname), 1)
        tmpvar = Variable(varname, Infer())
        sci_equiv = Funcall("mtlb", 1, Rhs_tlist(tmpvar), lhslist)
    end
endfunction
