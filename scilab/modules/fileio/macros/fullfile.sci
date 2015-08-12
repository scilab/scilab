// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Vincent COUVERT
// Copyright (C) DIGITEO - 2010-2012 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function f = fullfile(varargin)

    // Build a full filename from parts

    if lstsize(varargin) < 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "fullfile",1));
    end

    fs = ["/" "\"];
    f  = varargin(1);

    if ~isempty(f) then
        if type(f) <> 10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: a string expected.\n"), "fullfile", 1));
        end

        if size(f,"*") <> 1 then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: a string expected.\n"), "fullfile", 1));
        end
        f = stripblanks(f);
    end

    nbParameters =  lstsize(varargin)
    for k = 2 : nbParameters
        arg = varargin(k);
        if isempty(f) | isempty(arg)
            if ~isempty(arg) then
                if type(arg) <> 10 then
                    error(msprintf(gettext("%s: Wrong type for input argument #%d: a string expected.\n"), "fullfile", k));
                end

                if (size(arg,"*") <> 1) & (k <> nbParameters) then
                    error(msprintf(gettext("%s: Wrong size for input argument #%d: a string expected.\n"), "fullfile", k));
                end
            end
            f = f + arg;
        else

            if type(arg) <> 10 then
                error(msprintf(gettext("%s: Wrong type for input argument #%d: a string expected.\n"), "fullfile", k));
            end

            if (size(arg,"*") <> 1) & (k <> nbParameters) then
                error(msprintf(gettext("%s: Wrong size for input argument #%d: a string expected.\n"), "fullfile", k));
            end

            if or(part(f, length(f)) == fs) & or(part(arg, 1) == fs)
                f = f + stripblanks(part(arg, 2:length(arg)));
            elseif or(part(f, length(f))==fs) | or(part(arg, 1)==fs)
                f = f + stripblanks(arg);
            else
                f = f + pathconvert("/") + stripblanks(arg);
            end
        end
    end

    f = pathconvert(f, %f, %f);

endfunction
