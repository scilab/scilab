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
        if find(MSCompiler == ["msvc140express";"msvc120express";"msvc110express";"msvc100express";"msvc90express"]) then
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

                if isfile(programFilesx86Path + "\Microsoft Visual Studio 11.0\VC\bin\cl.exe") then
                    vcPath64 = programFilesx86Path + "\Microsoft Visual Studio 11.0";
                    return
                end

                if isfile(programFilesx86Path + "\Microsoft Visual Studio 10.0\VC\bin\amd64\cl.exe") then
                    vcPath64 = programFilesx86Path + "\Microsoft Visual Studio 10.0";
                    return
                end

                if isfile(programFilesx86Path + "\Microsoft Visual Studio 9.0\VC\bin\amd64\cl.exe") then
                    vcPath64 = programFilesx86Path + "\Microsoft Visual Studio 9.0";
                end
            end
        else
            select MSCompiler,
                // Microsoft Visual 2015 Studio Professional
            case  "msvc140pro" then
                vcPath64 = dlwGetVc14ProPath(); //same as VS2012
                // Microsoft Visual 2013 Studio Professional
            case  "msvc120pro" then
                vcPath64 = dlwGetVc12ProPath(); //same as VS2012
                // Microsoft Visual 2012 Studio Professional
            case  "msvc110pro" then
                vcPath64 = dlwGetVc11ProPath();
                // Microsoft Visual 2010 Studio Professional
            case  "msvc100pro" then
                vcPath64 = dlwGetVc10ProPath();
                // Microsoft Visual 2008 Studio Professional
            case  "msvc90pro" then
                vcPath64 = dlwGetVc90ProPath();
                // Microsoft Visual 2008 Studio Standard
            case  "msvc90std" then
                vcPath64 = dlwGetVc90StdPath();
            else
                vcPath64 = [];
            end
        end
    end
endfunction
//=============================================================================
