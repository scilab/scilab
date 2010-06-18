// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//=============================================================================
if (isdef('genlib') == %f) then
  exec(SCI+'/modules/functions/scripts/buildmacros/loadgenlib.sce');
end
//=============================================================================
genlib('dynamic_linklib','SCI/modules/dynamic_link/macros',%f,%t);
//=============================================================================
if getos() == 'Windows' then
  genlib('dynamic_linkwindowslib','SCI/modules/dynamic_link/macros/windows',%f,%t);
end
//=============================================================================
