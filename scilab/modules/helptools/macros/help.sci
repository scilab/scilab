// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function help(varargin)

    [lhs,rhs]=argn(0);

    if rhs >= 1 then
        key = varargin(1);
    else
        key = "";
    end

    if (findfiles("SCI/modules/helptools/jar","*_help.jar") <> []) then

        if getscilabmode() <> "NWNI" then

            // No input argument: launch help browser
            if argn(2)==0 then
                global %helps
                helpbrowser(%helps(:,1), getlanguage());
                return
            end

            if type(key) <> 10 then
                error(999,msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"),"help",1));
            end

            // Search a function name
            key=stripblanks(key)

            if or(part(key,1)==["(",")","[","]","{","}","%","''","""",":","*","/","\",".","<",">","&","^","|","~","+","-"]) & exists(key)==0 then
                key="symbols";
            end

            // Treat "$" apart because contrarily to the previous symbols, "$" is an existing variable in Scilab
            if part(key,1)=="$" & (exists(key)==0 | length(key)==1) then
                key="symbols";
            end

            global %helps
            helpbrowser(%helps(:,1), key, getlanguage(), %f);

            // If the function name does not exists then full-text search is done (See Java code)

        else
            error(msprintf(gettext("%s: The help browser is disabled in %s mode.\n"), "help", getscilabmode()));
        end
    else
        error(msprintf(gettext("%s: help file(.jar) is not installed.\n"), "help"));
    end

endfunction
