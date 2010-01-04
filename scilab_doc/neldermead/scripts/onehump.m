% Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
% Copyright (C) 2009 - Digiteo - Michael Baudin
%
% This file must be used under the terms of the CeCILL.
% This source file is licensed as described in the file COPYING, which
% you should have received as part of this distribution.  The terms
% are also available at
% http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function f = onehump(x)
r = x(1)^2 + x(2)^2;
s = exp(-r);
f = x(1)*s+r/20;


