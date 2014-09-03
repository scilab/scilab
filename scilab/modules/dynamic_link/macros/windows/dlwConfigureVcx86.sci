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
function bOK = dlwConfigureVcx86()
  bOK = %F;
  msvc = findmsvccompiler();

  select msvc
    // Microsoft Visual 2013 Studio Professional
    case  'msvc120pro' then
        bOK = dlwSetEnvVc12(msvc, %f);

    // Microsoft Visual 2013 Express
    case  'msvc120express' then
        bOK = dlwSetEnvVc12(msvc, %f);

    // Microsoft Visual 2012 Studio Professional
    case  'msvc110pro' then
        bOK = dlwSetEnvVc11(msvc, %f);

    // Microsoft Visual 2010 Studio Professional
    case  'msvc100pro' then
        bOK = dlwSetEnvVc10(msvc, %f);

    // Microsoft Visual 2012 Express
    case  'msvc110express' then
        bOK = dlwSetEnvVc11(msvc, %f);

    // Microsoft Visual 2010 Express
    case  'msvc100express' then
        bOK = dlwSetEnvVc10(msvc, %f);

    // Microsoft Visual 2008 Studio Professional
    case  'msvc90pro' then
        bOK = dlwSetEnvVc90(msvc, %f);

    // Microsoft Visual 2008 Studio Standard
    case  'msvc90std' then
        bOK = dlwSetEnvVc90(msvc, %f);

    // Microsoft Visual 2008 Express
    case  'msvc90express' then
        bOK = dlwSetEnvVc90(msvc, %f);

    // Microsoft Visual 2005 Studio Professional
    case  'msvc80pro' then
        bOK = dlwSetEnvVc80(msvc);

    // Microsoft Visual 2005 Studio Standard
    case  'msvc80std' then
        bOK = dlwSetEnvVc80(msvc);
    
    // Microsoft Visual 2005 Express
    case  'msvc80express' then
        bOK = dlwSetEnvVc80(msvc);

    // Microsoft Visual Studio .NET 2003
    case  'msvc71' then
        bOK = dlwSetEnvVc71();

    // Microsoft Visual Studio .NET 2002
    case  'msvc70' then
        bOK = dlwSetEnvVc70();
  else
  end  
endfunction
//=============================================================================
