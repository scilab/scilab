% Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
% Copyright (C) 2009 - DIGITEO - Vincent COUVERT
% 
 % Copyright (C) 2012 - 2016 - Scilab Enterprises
 %
 % This file is hereby licensed under the terms of the GNU GPL v2.0,
 % pursuant to article 5.3.4 of the CeCILL v.2.1.
 % This file was originally licensed under the terms of the CeCILL v2.1,
 % and continues to be available under such terms.
 % For more information, see the COPYING file which you should have received
 % along with this program.

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