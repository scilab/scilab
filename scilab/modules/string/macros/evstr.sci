// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2010 - Allan CORNET
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
    select type(%str)

    case 10 then
        // matrix of character strings
        if isempty(%str) then
            %val = [];
            %ierr =  0;
            return;
        end

        // bug 7003
        if ~isdef("Nan") then
            Nan = %nan;
        end
        if ~isdef("NaN") then
            NaN = %nan;
        end

        if ~isdef("Inf") then
            Inf = %inf;
        end

        if ~isdef("INF") then
            INF = %inf;
        end

        for %l = 1:size(%str, "r")
            %t1(%l) = strcat(%str(%l, :), ",") + ";"
        end
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
        %ierr = 0;
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real or Complex matrix, Matrix of character strings or list expected.\n"), "evstr", 1));
    end
    if exists("%val", "local") == 0 then
        error(msprintf(gettext("%s: Given expression has no value.\n"), "evstr"));
    end
endfunction
