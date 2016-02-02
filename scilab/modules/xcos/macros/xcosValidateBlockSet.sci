//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
// Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//
//
function [status, msg] = xcosValidateBlockSet(interfFunctionName)
    status = %t;
    msg = "";

    if typeof(interfFunctionName) <> "string" | size(interfFunctionName) <> [1, 1]
        error(999, sprintf(_("%s: Wrong type for argument #%d: A String expected."), "xcosValidateBlockSet", 1))
    end

    // Check function is defined
    ierr = execstr("funType = typeof("+interfFunctionName+");", "errcatch")
    if ierr <> 0 | and(funType <> ["fptr", "function"])
        status = %f;
        msg = _("Interface function does not exist or can not be called.");
        return
    end

    // Check for signature
    vars=macrovar(evstr(interfFunctionName));
    if or([size(vars(1)) <> [3 1] , size(vars(2)) <> [3 1]]) then
        status = %f;
        msg = sprintf(_("%s is not a valid block descriptor."), interfFunctionName);
        continue;
    end

    // Overload usefull functions
    // Stubbing the x_mdialog method
    // checking it's arguments size only
    function [result]=x_mdialog(title, labelsv, labelsh, default_inputs_vector)
        [lhs, rhs] = argn();
        if rhs == 3 then
            default_inputs_vector = labelsh;
            result = x_dialog(labelsv, default_inputs_vector);
        elseif rhs == 4 then
            vSize = size(labelsv, "*");
            hSize = size(labelsh, "*");
            if size(default_inputs_vector) <> [vSize, hSize] then
                error(999, sprintf(_("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"), "x_mdialog", 4, vSize, hSize));
            end;
            result = default_inputs_vector;
        else
            error(999, sprintf(_("%s: Wrong number of input arguments: %d or %d expected.\n"), "x_mdialog", 3, 4));
        end
    endfunction

    // Stubbing the x_dialog method
    // checking it's arguments size only
    function [result]=x_dialog(labels, default_inputs_vector)
        result = default_inputs_vector;
    endfunction

    // Stubbing the edit_curv method
    function [xx, yy, ok, gc] = edit_curv(xx, yy,  axis, args, gc)
        ok = %T;
        if ~exists("gc", "l") then
            rect=[0 0 1 1];
            axisdata=[2 10 2 10];
            gc = list(rect, axisdata);
        end
    endfunction

    // Stubbing the msgbox method
    function [btn] = msgbox(msg, msgboxtitle, msgboxicon, buttons, ismodal)
        btn=1;
    endfunction

    // Stubbing the message method
    function [btn] = message(strings ,buttons, modal)
        btn=1;
    endfunction

    ierr = execstr("scs_m = "+interfFunctionName+"(""define"", [], [])", "errcatch");
    if ierr <> 0
        status = %f;
        msg = sprintf(_("Block definition with function [%s] failed."), interfFunctionName);
        return
    end

    ierr = execstr("scs_m = "+interfFunctionName+"(""set"", scs_m, [])", "errcatch")
    if ierr <> 0
        errmsg = lasterror();
        status = %f;
        msg = sprintf(_("Block configuration with function [%s] failed."), interfFunctionName);
        msg = [msg;errmsg];
        return
    end

endfunction
