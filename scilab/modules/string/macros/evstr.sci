// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2010 - Allan CORNET
// Copyright (C) 2017 - Samuel GOUGEON
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
// HELP:
// * assignment forbidden
// * If a vector or matrix of strings is provided:
//   A single instruction per cell: no "," or ";" instruction separator

    [lhs, rhs] = argn(0);
    %val = [];
    %ierr =  0;

    select type(%str)
    case 10 then
        // matrix of character strings
        if isempty(%str) then
            return
        end

        // bug 7003
        vars = ["Nan"  "NaN"  "Inf"  "INF"]
        vals = ["%nan" "%nan" "%inf" "%inf"]
        tmp = ~isdef(vars)
        if tmp~=[]
            execstr(vars(tmp)+"="+vals(tmp))
        end

        // Bug 9839: support to included ascii(10) or ascii(13) characters:
        //  If the input is scalar: we replace them with ";"
        //  Otherwise:
        //    For every component including some ascii(10) or ascii(13),
        //    we replace the component with only its head up to the first ascii(10)
        //    or ascii(13) met (excluded).
        if size(%str,"*")==1
            %str = strsubst(%str, ascii(10), ";");
            %str = strsubst(%str, ascii(13), ";");
        else
            tmp = grep(%str, [ascii(10) ascii(13)]);
            if tmp~=[]
                tmp2 = strcspn(%str(tmp), ascii(10)+ascii(13));
                %str(tmp) = strncpy(%str(tmp), tmp2);
            end
        end

        // Bug 10196.b: only one component with an instruction returning nothing => [] returned
        serialized_input = %f
        if size(%str,"*")==1
            %ierr = execstr("%val=["+%str(1)+"]", "errcatch");
            if %ierr~=0
                %val = []
                return
            end
            if or(type(%val)==[11 13])
                return
            end
            try
                serialized_input = size(%val,"*")>1
            catch
                serialized_input = %f;
            end
        end

        // input strings with possible "," and ";" separators
        // or column of strings with possible "," and ending ";"
        // -----------------------------------------------------
        if serialized_input | ..
           grep(%str(1),"/^\s*\[/","r")~=[] & grep(%str($),"/\]\s*/","r")~=[]
            %t1 = strcat(%str, ",", "c") + ";"
            %t1(1) = "%val=[" + %t1(1);
            %t1($) = part(%t1($), 1:length(%t1($)) - 1)+";";
            %t1($+1)="]";
            if lhs == 2 then
                %ierr = execstr(%t1, "errcatch");
            else
                execstr(%t1)
            end

        // We assume: one %str component = only one instruction
        // ----------------------------------------------------
        else
            // Bugs 9838 & 10196.a:
            // Are considered as empty instructions
            //  - a blank or empty string
            //  - a string containing only "[ ]" and blanks
            //  - a string starting with possible spaces + "//" being a comment
            //  - a string like "  [ ]  // bla bla "
            // If at least one component is detected to be so, then
            //  * its evaluation returning nothing will be replaced with the "neutral" default,
            //    that depends on the overall result type.
            //  * So we search this type: it is the type returend by the evaluation of first
            //    non-empty component
            //  * We set the default: "" for texts, %nan for decimal and complex numbers, etc
            void = grep(%str, "/^(\s*\[\s*\]\s*|\s*|\s*\/\/.*|\s*\[\s*\]\s*\/\/.*)$/", "r")

            if void~=[]
                // The default replacement depends on the type of the output
                // It is set by the first expected valid output:
                tmp = setdiff(1:size(%str,"*"), void) // indices of valid expressions
                if tmp~=[]
                    execstr("tmp = "+%str(tmp(1))) // We get the result of the first one
                else  // none not-empty valid output is expected
                    return
                end
            else
                execstr("tmp = "+%str(1)) // result of the first component
            end
            // We test its type to set the default result according to its type:
            if or(type(tmp)==[1 2 5])
                default = "%nan"
            elseif type(tmp)==4
                default = "%F"
            elseif type(tmp)==10
                default = """"""
            elseif type(tmp)==8
                default = typeof(tmp)+"(0)"
            elseif type(tmp)==9
                default = "gdf()"
            elseif or(type(tmp)==[11 13])
                deff("voidF()","")  // Does not survive when leaving evstr(). Never mind
                default = "voidF"
            end
            // Substitution void => default
            if void~=[] then
                %str(void) = default
            end

            if or(type(tmp)==[1 2 4 8 9 10])    // matrix() supported
                // We optimize the matrix size:
                // The closer to a square matrix the faster is execstr()
                S = size(%str)
                s = size(%str,"*")
                n = ceil(sqrt(s))
                %str = [%str(:) ; repmat(default, n*n-s,1)]
                %str = matrix(%str, n, n);
            end
            %t1 = strcat(%str, ",", "c") + ";"
            %t1(1) = "%val=[" + %t1(1);
            %t1($) = part(%t1($), 1:length(%t1($)) - 1)+";";
            %t1($+1)="]";
            if lhs == 2 then
                %ierr = execstr(%t1, "errcatch");
            else
                execstr(%t1)
            end
            if or(type(tmp)==[1 2 4 8 9 10])    // Reshaping the result
                %val = matrix(%val(1:s), S)
            end
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
        msg = _("%s: Wrong type for input argument #%d: Real or Complex matrix, Matrix of character strings or list expected.\n")
        error(msprintf(msg, "evstr", 1));
    end
    if exists("%val", "local") == 0 then
        msg = _("%s: Given expression has no value.\n")
        error(msprintf(msg, "evstr"));
    end
endfunction

