// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2009-2010 - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [strs, matched_separators] = %_strsplit(varargin)

    //====== private macros ==================
    function out_str = replace_regexp_char(in_str)
        out_str = strsubst(in_str, "\", "\\");
        out_str = strsubst(out_str, "/", "\/");
        out_str = strsubst(out_str, "|", "\|");
        out_str = strsubst(out_str, ".", "\.");
        out_str = strsubst(out_str, "$", "\$");
        out_str = strsubst(out_str, "[", "\[");
        out_str = strsubst(out_str, "]", "\]");
        out_str = strsubst(out_str, "(", "\(");
        out_str = strsubst(out_str, ")", "\)");
        out_str = strsubst(out_str, "{", "\{");
        out_str = strsubst(out_str, "}", "\}");
        out_str = strsubst(out_str, "^", "\^");
        out_str = strsubst(out_str, "?", "\?");
        out_str = strsubst(out_str, "*", "\*");
        out_str = strsubst(out_str, "+", "\+");
        out_str = strsubst(out_str, "-", "\-");
    endfunction
    //========================================
    function bOK = isPattern(str)
        bOK = %f;
        lenstr = length(str);
        if lenstr > 1 then
            bOK = ((part(str, 1) == "/") & (part(str, lenstr) == "/"));
        end
    endfunction
    //========================================
    function regexp_pattern = createPattern(StringsInput)
        dims_StringsInput = size(StringsInput,"*");
        if (dims_StringsInput == 1) then
            if ~isPattern(StringsInput) then
                regexp_pattern = "/" + replace_regexp_char(StringsInput) + "/";
            else
                regexp_pattern = StringsInput;
            end
        else
            regexp_pattern = "";
            for i = 1:dims_StringsInput
                if (i == 1) then
                    regexp_pattern = replace_regexp_char(StringsInput(i));
                else
                    regexp_pattern = regexp_pattern + "|" + replace_regexp_char(StringsInput(i));
                end
            end
            regexp_pattern = "/" + regexp_pattern + "/";
        end
    endfunction
    //========================================
    matched_separators = [];
    strs = [];

    [lhs, rhs] = argn(0);

    // input types are checked in strsplit primitive
    if (rhs == 1) then
        len = length(varargin(1));
        if len == 0 then
            strs = "";
        else
            len = length(varargin(1));
            if len > 1 then
                strs = strsplit( varargin(1), 1:len - 1 );
            else
                strs = varargin(1);
            end
        end
        if (lhs == 2) then
            dims_strs = size(strs);
            matched_separators = emptystr(dims_strs(1), dims_strs(2));
        end
    else

        strsplit_limit = -1; // no limit
        if (rhs == 3) then
            strsplit_limit = varargin(3);
        end

        if varargin(2) == "" then
            [strs, matched_separators] = strsplit(varargin(1));
        else
            strsplit_pattern = createPattern(varargin(2));

            [start_regexp, end_regexp, match_regexp] =  regexp(varargin(1), strsplit_pattern);
            if (start_regexp <> []) then
                if (end_regexp($) == length(varargin(1))) then
                    end_regexp($) = end_regexp($) - 1;
                end

                len = length(varargin(1));
                if len > 1 then
                    strs = strsplit(varargin(1), end_regexp);
                    strs = strsubst(strs, strsplit_pattern, "", "r");
                    matched_separators = match_regexp;
                else
                    strs = varargin(1);
                    matched_separators = [];
                end
            else
                strs = varargin(1);
                matched_separators = [];
            end
        end

        if (strsplit_limit > 0) then
            dim_strs = size(strs,"*");
            if (strsplit_limit >= dim_strs) then
                strsplit_limit = dim_strs;
            else
                strsremain = strs(strsplit_limit + 1 : $);
                dim_strsremain = size(strsremain,"*");
                strslimited = strs(1:strsplit_limit);

                foundedremain = matched_separators(strsplit_limit + 1 : $);
                foundedremain($ + 1: dim_strsremain) = "";
                matched_separators = matched_separators(1:strsplit_limit);

                str = strsremain + foundedremain;
                strs = [strslimited ; strcat(str)];
            end
        end
    end
    //========================================

endfunction

