// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
                error(999,msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"),"help",1));
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
