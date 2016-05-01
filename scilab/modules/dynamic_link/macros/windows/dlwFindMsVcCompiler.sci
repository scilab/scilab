// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Allan CORNET
// Copyright (C) Scilab Enterprises - 2014 - Antoine ELIAS
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

//=============================================================================
function MSCompiler = dlwFindMsVcCompiler()
    MSCompiler = "unknown"; // unknown

    // We use always last version of MS compiler

    val = getenv("SCILAB_PREFERED_MSVC", "");
    if val <> "" then
        funcs = list(dlwIsVc14Express,dlwIsVc12Pro,dlwIsVc14Express,dlwIsVc12Pro,dlwIsVc11Express,dlwIsVc11Pro,dlwIsVc10Express,dlwIsVc10Pro);
        compilers = [ ...
        "msvc140express";
        "msvc140pro";
        "msvc120express";
        "msvc120pro";
        "msvc110express";
        "msvc110pro";
        "msvc100express";
        "msvc100pro";];
        idx = find(val == compilers);
        if idx <> [] then
            func = funcs(idx);
            if func() then
                MSCompiler = val;
                return;
            end
        end
    end

    if dlwIsVc14Express() then
        MSCompiler = "msvc140express";     // Microsoft Visual 2015 Express
        return;
    end

    if dlwIsVc14Pro() then
        MSCompiler = "msvc140pro";       // Microsoft Visual 2015 Professional / Community (or more)
        return;
    end

    if dlwIsVc12Express() then
        MSCompiler = "msvc120express";     // Microsoft Visual 2013 Express
        return;
    end

    if dlwIsVc12Pro() then
        MSCompiler = "msvc120pro";       // Microsoft Visual 2013 Professional (or more)
        return;
    end

    if dlwIsVc11Express() then
        MSCompiler = "msvc110express";     // Microsoft Visual 2012 Express
        return;
    end

    if dlwIsVc11Pro() then
        MSCompiler = "msvc110pro";       // Microsoft Visual 2012 Professional (or more)
        return;
    end

    if dlwIsVc10Pro() & dlwIsVc10Express() then
        MSCompiler = "msvc100express";     // Microsoft Visual 2010 Express with SDK extension
        return;
    end

    if dlwIsVc10Express() then
        MSCompiler = "msvc100express";     // Microsoft Visual 2010 Express
        return;
    end

    if dlwIsVc10Pro() then
        MSCompiler = "msvc100pro";       // Microsoft Visual 2010 Professional (or more)
        return;
    end

    if dlwIsVc90Pro() then
        MSCompiler = "msvc90pro";      // Microsoft Visual 2008 Studio Professional
        return;
    end

    if dlwIsVc90Std() then
        MSCompiler = "msvc90std";      // Microsoft Visual 2008 Studio Standard
        return;
    end

    if dlwIsVc90Express() then
        MSCompiler = "msvc90express";    // Microsoft Visual 2008 Express
        return;
    end

    if dlwIsVc80Pro() then
        MSCompiler = "msvc80pro";      // Microsoft Visual 2005 Studio Professional
        return;
    end

    if dlwIsVc80Std() then
        MSCompiler = "msvc80std";      // Microsoft Visual 2005 Studio Standard
        return;
    end

    if dlwIsVc80Express() then
        MSCompiler = "msvc80express";    // Microsoft Visual 2005 Express
        return;
    end

    if dlwIsVc71() then
        MSCompiler = "msvc71";         // Microsoft Visual Studio .NET 2003
        return;
    end

    if dlwIsVc70() then
        MSCompiler = "msvc70";         // Microsoft Visual Studio .NET 2002
        return;
    end
endfunction
//=============================================================================
