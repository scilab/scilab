% Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
% Copyright (C) 2008-2009 - Digiteo - Michael Baudin
%
% This file must be used under the terms of the CeCILL.
% This source file is licensed as described in the file COPYING, which
% you should have received as part of this distribution.  The terms
% are also available at
% http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

% Basic use
format long
banana = @(x)100*(x(2)-x(1)^2)^2+(1-x(1))^2;
[x,fval,exitflag,output] = fminsearch(banana,[-1.2, 1])
output.message

% Now check each iteration
opt = optimset('Display','iter');
[x,fval,exitflag,output] = fminsearch(banana,[-1.2, 1] , opt );


% Check that the output functions are OK
options = optimset('OutputFcn', @outfun);
[x fval] = fminsearch(banana , [-1.2, 1] , options)

% Sample use of a plot function : optimplotfval
options = optimset('PlotFcns',@optimplotfval);
[x fval] = fminsearch(banana , [-1.2, 1] , options)


