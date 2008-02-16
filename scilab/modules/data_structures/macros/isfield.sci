// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function r=isfield(s,field)
if ~isstruct(s) then
r=0;return;end
w=getfield(1,s);
r=bool2s(or(w(3:$)==field));
endfunction
