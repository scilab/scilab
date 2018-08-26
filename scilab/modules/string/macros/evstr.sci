// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2010 - Allan CORNET
// Copyright (C) 2016, 2017, 2018 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function [%val, %ierr] = evstr(%str)

    %lhs = argn(1);
    %val = [];
    %ierr =  0;

    select type(%str)

    case 10 then
        // matrix of character strings
        if isempty(%str) then
            %val = [];
            return;
        end

        // bug 7003
        vars = ["Nan"  "NaN"  "Inf"  "INF"]
        vals = ["%nan" "%nan" "%inf" "%inf"]
        tmp = ~isdef(vars)
        if or(tmp)
            execstr(vars(tmp)+"="+vals(tmp))
        end

        // Removing comments:
        regExp = "_(?<!\:)//[^\""\'']*$_";
        %str = strsubst(%str, regExp , "", "r");
        k = grep(%str, "_^[^""'']*?//.*?(\""|\'')_", "r");
        if k~=[]
            %str(k) = strsubst(%str(k), "_(?<!\:|"")?//.*$_" , "", "r");
        end

        // Bracketing expressions: http://bugzilla.scilab.org/15308
        comm = grep(%str, "_(^|[^:])//_", "r");
        tmp = 1:size(%str,"*");
        if comm~=[]
            tmp = setdiff(tmp, comm) // indices of expressions without remaining comments
        end
        if tmp~=[]
            %str(tmp) = "[" + %str(tmp) + "]";
        end
        //
        %t1 = strcat(%str, ",", "c")+";"
        %t1(1) = "%val=[" + %t1(1);
        %t1($) = part(%t1($), 1:length(%t1($)) - 1)+";";
        %t1($+1) = "]";
        clear k tmp vars vals regExp comm %str

        %ierr = execstr(%t1, "errcatch");
        if %lhs == 1 & %ierr~=0 then
            msg = _("%s: Argument #%d: Some expression can''t be evaluated (%s).\n")
            error(msprintf(msg, "evstr", 1, strcat(lasterror())))
        end

    case 15 then
        // list
        if size(%str)~=2
            msg = _("%s: Argument #%d: %d-element list expected.\n")
            error(msprintf(msg, "evstr", 1, 2));
        end
        if type(%str(1))~=10 | type(%str(2))~=10
            msg = _("%s: Argument #%d: Both list components must be of text type.\n")
            error(msprintf(msg, "evstr", 1));
        end
        %sexp = %str(2),
        % = list();
        for %k_ = 1:prod(size(%sexp))
            [%w, %ierr] = evstr(%sexp(%k_));
            %(%k_) = %w;
            if %ierr <>0  then
                if %lhs == 2
                    %val = [];
                    return;
                else
                    msg = _("%s: Argument #1(2): The expression #%d = ""%s"" can''t be evaluated (%s).\n")
                    error(msprintf(msg, "evstr", %k_, %sexp(%k_), strcat(lasterror())))
                end
            end
        end
        if %lhs == 2
            [%val, %ierr] = evstr(%str(1));
        else
            %val = evstr(%str(1));
        end

    case 1 then
        // real or complex constant matrix
        %val = %str;
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of character strings or list expected.\n"), "evstr", 1));
    end
    if exists("%val", "local") == 0 then
        error(msprintf(gettext("%s: Given expression has no value.\n"), "evstr"));
    end
endfunction
