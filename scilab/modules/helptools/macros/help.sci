// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function help(varargin)

    if findfiles("SCI/modules/helptools/jar","*_help.jar") == [] then
        error(msprintf(gettext("%s: help file(.jar) is not installed.\n"), "help"));
    end
    if getscilabmode() == "NWNI" then
        error(msprintf(gettext("%s: The help browser is disabled in %s mode.\n"), "help", getscilabmode()));
    end

    if argn(2) >= 1 then
        key = varargin(1);
        if type(key) <> 10 then
            error(msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"),"help",1));
        end
    else
        key = "";
    end

    global %helps
    // Retrieving the browser language and former page:
    filename = SCIHOME + filesep() + "configuration.xml"
    res = xmlGetValues("//Setting/Profile/HelpBrowser", ["index" "lang"], filename);
    if key == "" then
        key = res(1)
    end
    if res(2) == ""
        lang = getlanguage()
    else
        lang = res(2)
    end

    key = stripblanks(key)

    symbols = strsplit("()[]{}%''"":*/\.<>&^|~+-")';
    exceptions = ["%t" "%T" "%f" "%F" "%onprompt"]; // http://bugzilla.scilab.org/15356
    if or(part(key,1)==symbols) & exists(key)==0 & and(key~=exceptions) then
        key = "symbols";
    end

    // Treat "$" apart because contrarily to the previous symbols, "$" is an existing variable in Scilab
    if part(key,1)=="$" & (exists(key)==0 | length(key)==1) then
        key = "symbols";
    end
    // Calling the browser
    helpbrowser(%helps(:,1), key, lang, %f);
    // If the key is not a xml:id, then full-text search is done (See Java code)

endfunction
