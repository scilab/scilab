// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
// Copyright (C) 2011 - INRIA - Serge STEER
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [F_out, X_out, Ind_out] = pareto_filter(F_in, X_in)
    [nargout,nargin] = argn();
    if ~isdef("F_in","local") then
        error(sprintf(gettext("%s: F_in is mandatory"),"pareto_filter"));
    end
    x_defined   = (nargout>=2);
    Ind_defined = (nargout==3);

    n=size(F_in,1);
    Dominating(n)=%f;
    for i=2:n
        j=find(~Dominating(1:i-1))
        df=ones(size(j,"*"),1)*F_in(i,:)-F_in(j,:);
        kd=find(and(df<=0,2) & or(df<0,2));
        if kd<>[] then
            Dominating(j(kd))=%t
        elseif find(and(df>=0,2) & or(df>0,2),1)<>[] then
            Dominating(i) = %t
        end
    end

    Ind_out=find(~Dominating)
    F_out = F_in(Ind_out,:);

    if x_defined & (nargin==2) then
        if typeof(X_in)=="list" then
            X_out = list(X_in(Ind_out));
        else
            X_out = X_in(Ind_out,:);
        end
    elseif x_defined &(nargin~=2)
        X_out = [];
    end
endfunction
