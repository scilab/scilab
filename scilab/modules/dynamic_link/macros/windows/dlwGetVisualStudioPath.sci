// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2015 - Antoine ELIAS
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
function VSPath = dlwGetVisualStudioPath()
    msvc = findmsvccompiler();

    VSPath = "";
    select msvc
        // Microsoft Visual 2019 Studio Professional
    case  "msvc142pro" then
        VSPath = dlwGetVc142ProPath();
        // Microsoft Visual 2019 Studio Express
    case  "msvc142express" then
        VSPath = dlwGetVc142ExpressPath();
        // Microsoft Visual 2017 Studio Professional
    case  "msvc141pro" then
        VSPath = dlwGetVc141ProPath();
        // Microsoft Visual 2017 Studio Express
    case  "msvc141express" then
        VSPath = dlwGetVc141ExpressPath();
        // Microsoft Visual 2015 Studio Professional
    case  "msvc140pro" then
        VSPath = dlwGetVc14ProPath();
        // Microsoft Visual 2015 Studio Express
    case  "msvc140express" then
        VSPath = dlwGetVc14ExpressPath();
        // Microsoft Visual 2013 Studio Professional
    case  "msvc120pro" then
        VSPath = dlwGetVc12ProPath();
        // Microsoft Visual 2013 Express
    case  "msvc120express" then
        VSPath = dlwGetVc12ExpressPath();
    else
    end
endfunction
//=============================================================================
