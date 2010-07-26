// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Farid Belahcene
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function title(varargin)
// TITLE function
// Add titles on a graphics window 

if size(varargin)<1 then
  error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "title", 1));
end

varargin = list('title',varargin(1:$));
TitleLabel(varargin(:));
endfunction
