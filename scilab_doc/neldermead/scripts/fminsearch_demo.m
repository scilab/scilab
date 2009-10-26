% Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
% Copyright (C) 2009 - Digiteo - Michael Baudin
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

% Use tolerance on X
opt = optimset('TolX',1.e-2);
[x,fval,exitflag,output] = fminsearch(banana,[-1.2, 1] , opt );

% Use tolerance on F
opt = optimset('TolFun',1.e-10);
[x,fval,exitflag,output] = fminsearch(banana,[-1.2, 1] , opt );

% Use MaxIter
opt = optimset('MaxIter',10);
[x,fval,exitflag,output] = fminsearch(banana,[-1.2, 1] , opt );

% Use MaxFunEvals
opt = optimset('MaxFunEvals',10);
[x,fval,exitflag,output] = fminsearch(banana,[-1.2, 1] , opt );

% Set Display to 'final'
opt = optimset('Display','final');
[x,fval,exitflag,output] = fminsearch(banana,[-1.2, 1] , opt );

% Set Display to 'iter'
opt = optimset('Display','iter');
[x,fval,exitflag,output] = fminsearch(banana,[-1.2, 1] , opt );

% Set Display to 'off'
opt = optimset('Display','off');
[x,fval,exitflag,output] = fminsearch(banana,[-1.2, 1] , opt );

% Set Display to 'notify'
opt = optimset('Display','notify');
[x,fval,exitflag,output] = fminsearch(banana,[-1.2, 1] , opt );

% Set Display to 'off', when there is a maximum number of iterations reached
opt = optimset('Display','off' , 'MaxIter' , 10 );
[x,fval,exitflag,output] = fminsearch(banana,[-1.2, 1] , opt );

% Set Display to 'notify', when there is a maximum number of iterations reached
opt = optimset('Display','notify' , 'MaxIter' , 10 );
[x,fval,exitflag,output] = fminsearch(banana,[-1.2, 1] , opt );

% Set Display to 'iter', when there is a maximum number of iterations reached
opt = optimset('Display','iter' , 'MaxIter' , 10 );
[x,fval,exitflag,output] = fminsearch(banana,[-1.2, 1] , opt );

% Sample use of an output function
opt = optimset('OutputFcn', @outfun);
[x fval] = fminsearch(banana, [-1.2, 1], opt)

% Sample use of a plot function : optimplotfval
opt = optimset('PlotFcns',@optimplotfval);
[x ffinal] = fminsearch(banana,[-1.2, 1],opt)

% Sample use of a plot function : optimplotx
opt = optimset('PlotFcns',@optimplotx);
[x ffinal] = fminsearch(banana,[-1.2, 1],opt)

% Sample use of a plot function : optimplotfunccount
opt = optimset('PlotFcns',@optimplotfunccount);
[x ffinal] = fminsearch(banana,[-1.2, 1],opt)

