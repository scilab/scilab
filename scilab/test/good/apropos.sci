// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function apropos(key)

if argn(2)<>1 then
  error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"), "apropos", 1));
end

global %helps
helpbrowser(%helps(:,1), key, getlanguage(), %T);
	
endfunction
