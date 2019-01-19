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
        funcs = struct(...
            "msvc142express", dlwIsVc142Express, ...
            "msvc142pro", dlwIsVc142Pro, ...
            "msvc141express", dlwIsVc141Express, ...
            "msvc141pro", dlwIsVc141Pro, ...
            "msvc140express", dlwIsVc14Express, ...
            "msvc140pro", dlwIsVc14Pro, ...
            "msvc120express", dlwIsVc12Express, ...
            "msvc120pro", dlwIsVc12Pro);

        try
            func = funcs(val);
            if func() then
                MSCompiler = val;
            end
        catch
        end

        if MSCompiler <> "unknown" then
            return;
        end
    end

    //Try to find fisrt the same compilator that which use to build Scilab
    if dlwIsVc141Express() then
        MSCompiler = "msvc141express";     // Microsoft Visual 2017 Express/Community
        return;
    end

    if dlwIsVc141Pro() then
        MSCompiler = "msvc141pro";       // Microsoft Visual 2017 Professional (or more)
        return;
    end

    if dlwIsVc142Express() then
        MSCompiler = "msvc142express";     // Microsoft Visual 2019 Express/Community
        return;
    end

    if dlwIsVc142Pro() then
        MSCompiler = "msvc142pro";       // Microsoft Visual 2019 Professional (or more)
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

    if dlwIsVc14Express() then
        MSCompiler = "msvc140express";     // Microsoft Visual 2015 Express
        return;
    end

    if dlwIsVc14Pro() then
        MSCompiler = "msvc140pro";       // Microsoft Visual 2015 Professional / Community (or more)
        return;
    end
endfunction
//=============================================================================
