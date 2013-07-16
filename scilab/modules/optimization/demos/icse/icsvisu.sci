//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

function [yobt,yob]=icsvisu(vue)
    // Visualization of the observation of the state
    // Syntax
    // [yobt,yob] = icsvisu(vue)
    // input variables :
    // vue  : vector of indexes of the componant to be visualized
    // output variables :
    // yobt : observation of the state at every instants
    // yob  : idem, at observation instant

    tob1 = [t0,tob];
    tobt = t0*ones(1,nti+ntf) + ...
    [dti*[1:nti],dtf*[(nti*dti/dtf)+1:(nti*dti/dtf)+ntf]];
    tobt = [t0,tobt];

    [ytob] = icob(dtv);
    [ytot] = icot(dtv);

    yobt = obs*[y0',ytot];
    yob  = obs*[y0',ytob];
    for i=vue
        plot(tobt,yobt(i,:)); plot(tob1,yob(i,:));
    end
endfunction
