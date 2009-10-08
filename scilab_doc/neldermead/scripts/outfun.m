% Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
% Copyright (C) 2009 - Digiteo - Michael Baudin
%
% This file must be used under the terms of the CeCILL.
% This source file is licensed as described in the file COPYING, which
% you should have received as part of this distribution.  The terms
% are also available at
% http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

% Use output function
function stop = outfun(x, optimValues, state)
stop = false;
hold on;
plot(x(1),x(2),'.');
drawnow

