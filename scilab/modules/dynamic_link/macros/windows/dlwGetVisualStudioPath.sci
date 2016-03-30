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
        // Microsoft Visual 2015 Studio Professional
    case  "msvc140pro" then
        VSPath = dlwGetVc14ProPath();
        // Microsoft Visual 2015 Express
    case  "msvc140express" then
        VSPath = dlwGetVc14ExpressPath();
        // Microsoft Visual 2013 Studio Professional
    case  "msvc120pro" then
        VSPath = dlwGetVc12ProPath();
        // Microsoft Visual 2013 Express
    case  "msvc120express" then
        VSPath = dlwGetVc12ExpressPath();
        // Microsoft Visual 2012 Studio Professional
    case  "msvc110pro" then
        VSPath = dlwGetVc11ProPath();
        // Microsoft Visual 2012 Express
    case  "msvc110express" then
        VSPath = dlwGetVc11ExpressPath();
        // Microsoft Visual 2010 Studio Professional
    case  "msvc100pro" then
        VSPath = dlwGetVc10ProPath();
        // Microsoft Visual 2010 Express
    case  "msvc100express" then
        VSPath = dlwGetVc10ExpressPath();
        // Microsoft Visual 2008 Studio Professional
    case  "msvc90pro" then
        VSPath = dlwGetVc90ProPath();
        // Microsoft Visual 2008 Studio Standard
    case  "msvc90std" then
        VSPath = dlwGetVc90StdPath();
        // Microsoft Visual 2008 Express
    case  "msvc90express" then
        VSPath = dlwGetVc90ExpressPath();
    else
    end
endfunction
//=============================================================================
