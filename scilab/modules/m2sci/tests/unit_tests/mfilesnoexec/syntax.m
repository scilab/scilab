% Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
% Copyright (C) 2009 - DIGITEO - Vincent COUVERT
% 
% This file must be used under the terms of the CeCILL.
% This source file is licensed as described in the file COPYING, which
% you should have received as part of this distribution.  The terms
% are also available at    
% http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

% Old versions of Scilab (before Scilab 4 I think) did not manage
% multiple operators such as in the following cases
% Modifications were made in m2sci_syntax.sci
a = 1++2;
b = 1--2;
c = 1+-2;
d = 1-+2;
e = 1+-+2;
f = 1-+-2;
g = 1---2;
h = 1+++2;
k = 1*+2;
l = 1/+2;