// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//=============================================================================
function bOK = dlwConfigureVcx64()
  bOK = %F;
  msvc = findmsvccompiler();

  select msvc,
     // Microsoft Visual 2010 Studio Professional
     case  'msvc100pro' then
     bOK = dlwSetEnvVc10(msvc, %t);

     // Microsoft Visual 2010 Express
     case  'msvc100express' then
     bOK = dlwSetEnvVc10(msvc, %t);

     // Microsoft Visual 2008 Studio Professional
     case  'msvc90pro' then
     bOK = dlwSetEnvVc90(msvc, %t);

     // Microsoft Visual 2008 Studio Standard
     case  'msvc90std' then
     bOK = dlwSetEnvVc90(msvc, %t);

    // Microsoft Visual 2008 Express
    case  'msvc90express' then
    bOK = dlwSetEnvVc90(msvc, %t);
  else
  end  
endfunction
//=============================================================================


