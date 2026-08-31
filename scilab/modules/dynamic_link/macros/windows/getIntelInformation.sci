// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2025 - Dassault Systèmes S.E. - Antoine ELIAS
//
// For more information, see the COPYING file which you should have received
// along with this program.

function FCompiler = getIntelInformation()
    if getenv("IFORT_COMPILER22", "") <> "" then
        FCompiler = fullpath(getenv("IFORT_COMPILER22", "") + "../..");
    end

    if getenv("IFORT_COMPILER23", "") <> "" then
        FCompiler = fullpath(getenv("IFORT_COMPILER23", "") + "../..");
    end

    if getenv("IFORT_COMPILER25", "") <> "" then
        FCompiler = fullpath(getenv("IFORT_COMPILER25", "") + "../..");
    end

    if getenv("IFORT_COMPILER26", "") <> "" then
        FCompiler = fullpath(getenv("IFORT_COMPILER26", "") + "../..");
    end
endfunction
