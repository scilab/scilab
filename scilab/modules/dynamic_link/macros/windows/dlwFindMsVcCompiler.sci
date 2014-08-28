// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Allan CORNET
// Copyright (C) Scilab Enterprises - 2014 - Antoine ELIAS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

//=============================================================================
function MSCompiler = dlwFindMsVcCompiler()
    MSCompiler = "unknown"; // unknown

    // We use always last version of MS compiler


    if dlwIsVc12Express() then
      MSCompiler = 'msvc120express';     // Microsoft Visual 2013 Express
      return;
    end

    if dlwIsVc12Pro() then
      MSCompiler = 'msvc120pro';       // Microsoft Visual 2013 Professional (or more)
      return;
    end

    if dlwIsVc11Express() then
      MSCompiler = 'msvc110express';     // Microsoft Visual 2012 Express
      return;
    end

    if dlwIsVc11Pro() then
      MSCompiler = 'msvc110pro';       // Microsoft Visual 2012 Professional (or more)
      return;
    end

    if dlwIsVc10Pro() & dlwIsVc10Express() then
      MSCompiler = 'msvc100express';     // Microsoft Visual 2010 Express with SDK extension
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
