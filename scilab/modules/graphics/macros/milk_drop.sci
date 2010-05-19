// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [z]=milk_drop(x,y)

if argn(2)<>2 then
  error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "milk_drop", 2));
end

sq=x.^2+y.^2;
z= exp( exp(-sq).*(exp(cos(sq).^20)+8*sin(sq).^20+2*sin(2*(sq)).^8) );
endfunction
