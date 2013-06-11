function ext_ex = ext_exists(varargin)
    // Extend the exists instruction so as to be able to check whether a matrix
    // or list element exists.
    // This is required for the Scipad debugger since if a matrix d exists in Scilab,
    // exists(d) is true but exists(d(1)) is false
    // Author: François Vogel, 2005

    if argn(2) == 0 then error(39); else var = varargin(1); end

    if exists(var,"nolocal") then
        ext_ex = %t;
    else
        var = string(var);
        if prod(size(var)) > 1 then
            ext_ex = %f;
        else
            var_tok = tokens(var,["(",")"]);
            if prod(size(var_tok)) <> 2 then
                ext_ex = %f;
            else
                if ~exists(var_tok(1),"nolocal") then
                    ext_ex = %f;
                else
                    accesstryresult = execstr(var_tok(1) + "(" + var_tok(2) + ")","errcatch");
                    if accesstryresult == 0 then
                        // this element can be addressed, therefore it exists
                        ext_ex = %t;
                    else
                        // element does not exist. This includes:
                        // error   4: undefined variable - can happen when trying to access
                        //            d(i), if i is not defined
                        // error  21: invalid index (for a matrix or a list)
                        // error 117: undefined list element
                        ext_ex = %f;
                        // the error that occurred in the execstr above has been recorded
                        // and must be cleared, otherwise the debugger reports it in the
                        // watchwindow (proc checkexecutionerror_bp)
                        // this was not needed before revision 13662 of the svn trunk,
                        // because the errcatch mode was (erroneously) lost during pause
                        lasterror(%t);
                    end
                end
            end
        end
    end
endfunction

