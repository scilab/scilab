// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 200X-2010 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//=============================================================================
function ifortCompiler = findmsifortcompiler()

  ifortCompiler = 'unknown'; // unknown

  if getos() == 'Windows' then
    if getenv('IFORT_COMPILER13','NOK') <> 'NOK' then
      ifortCompiler = 'ifort13';
      return
    end

    if getenv('IFORT_COMPILER12','NOK') <> 'NOK' then
      ifortCompiler = 'ifort12';
      return
    end

    if getenv('IFORT_COMPILER11','NOK') <> 'NOK' then
      ifortCompiler = 'ifort11';
      return
    end

    if getenv('IFORT_COMPILER10','NOK') <> 'NOK' then
      ifortCompiler = 'ifort10';
      return
    end

    if getenv('IFORT_COMPILER9','NOK') <> 'NOK' then
      ifortCompiler = 'ifort9';
      return
    end
  else // NOT Windows
    ifortCompiler = 'unknown'; // unknown
  end
endfunction
//=============================================================================


