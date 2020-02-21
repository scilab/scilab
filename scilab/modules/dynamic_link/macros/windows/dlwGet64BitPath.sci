// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

//=============================================================================
function vcPath64 = dlwGet64BitPath();
    vcPath64 = [];
    if win64() then
        MSCompiler = findmsvccompiler();
        if find(MSCompiler == ["msvc142express";"msvc142pro";"msvc141express";"msvc141pro"]) then
            vers = getVsWhereInformation();
            vcPath64 = vers(MSCompiler);
        elseif find(MSCompiler == ["msvc140express";"msvc120express"]) then
            programFilesx86Path = getenv("ProgramFiles(x86)", "");
            if (programFilesx86Path <> "") then
                if isfile(programFilesx86Path + "\Microsoft Visual Studio 14.0\VC\bin\cl.exe") then
                    vcPath64 = programFilesx86Path + "\Microsoft Visual Studio 14.0";
                    return
                end

                if isfile(programFilesx86Path + "\Microsoft Visual Studio 12.0\VC\bin\cl.exe") then
                    vcPath64 = programFilesx86Path + "\Microsoft Visual Studio 12.0";
                    return
                end
            end
        else
            select MSCompiler,
                // Microsoft Visual 2015 Studio Professional
            case  "msvc140pro" then
                vcPath64 = dlwGetVc14ProPath();
                // Microsoft Visual 2013 Studio Professional
            case  "msvc120pro" then
                vcPath64 = dlwGetVc12ProPath();
            else
                vcPath64 = [];
            end
        end
    end
endfunction
//=============================================================================
