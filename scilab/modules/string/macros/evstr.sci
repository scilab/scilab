// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2010 - Allan CORNET
// Copyright (C) 2016, 2017 - Samuel GOUGEON
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

    [lhs, rhs] = argn(0);
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
        if tmp~=[]
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
        %t1($+1)="]";
        if lhs == 2 then
            %ierr = execstr(%t1, "errcatch");
        else
            execstr(%t1)
        end

    case 15 then
        // list
        %sexp = %str(2),
        %nstr = prod(size(%sexp));
        % = list();
        if lhs == 2 then
            for %k_ = 1:%nstr,
                [%w, %ierr] = evstr(%sexp(%k_));
                %(%k_) = %w;
                if %ierr <>0  then
                    %val = [];
                    return;
                end
            end
            [%val, %ierr] = evstr(%str(1));
        else
            for %k_ = 1:%nstr,
                %(%k_) = evstr(%sexp(%k_));
            end
            %val = evstr(%str(1))
        end

    case 1 then
        // real or complex constant matrix
        %val = %str;
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real or Complex matrix, Matrix of character strings or list expected.\n"), "evstr", 1));
    end
    if exists("%val", "local") == 0 then
        error(msprintf(gettext("%s: Given expression has no value.\n"), "evstr"));
    end
endfunction
