// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// Please be sure that current scilab is associated with .sce & .sci extensions
//
if getos() == 'Windows' then
  r = findfileassociation('.sce');
  if r == [] then pause,end
  
  r = findfileassociation('.sci');
  if r == [] then pause,end
end  
