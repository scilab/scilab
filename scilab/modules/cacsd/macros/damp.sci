// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - INRIA - Serge Steer
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// For more information, see the COPYING file which you should have received
// along with this program.
function [wn,z,p] = damp(R,dt1)
    //Natural frequency and damping factor for continuous systems.
    //   [Wn,Z,P] = damp(R) returns vectors Wn and Z containing the
    //   natural frequencies and damping factors of R.
    //   The variable R  must be a real or complex array of roots, a
    //   polynomial array or  a linear dynamical system

    if argn(2)<1 then
        error(msprintf(_("%s: Wrong number of input arguments: %d or %d expected.\n"),"damp",1,2))
    end
    //handling optional argument dt1
    if argn(2)==1 then
        dt1=[];
    else
        if type(dt1)==1 then
            if size(dt1,"*")<>1|dt1<0 then
                error(msprintf(_("%s: Wrong type for input argument #%d: Real non negative scalar expected.\n"),..
                "damp",2))
            end
        elseif type(dt1)==10 then
            if dt1=="c" then
                dt1=0
            elseif dt1=="d" then
                dt1=1
            else
                error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                "damp",2,"""c"", ""d"""))
            end
        else
            error(msprintf(_("%s: Wrong type for input argument #%d: Scalar or string expected.\n"),..
            "damp",2))
        end
    end

    toBeOrdered=%t;dt=[];
    select typeof(R)
    case "polynomial" then //polynomial array
        p=[];
        for k=1:size(R,"*")
            p=[p;roots(R(k),"e")];
        end
    case "rational" then
        dt=R.dt
        if dt=="c" then
            dt=0
        elseif dt=="d" then
            dt=1
        end
        p=roots(lcm(R.den))
    case "zpk" then
        dt=R.dt
        if dt=="c" then
            dt=0
        elseif dt=="d" then
            dt=1
        end
        [m,n]=size(R)
        [p,P]=findCommonValues(R.P)
        for i=1:size(P,"*")
            p=[p;P{i}]
        end
    case "state-space" then
        dt=R.dt
        if dt=="c" then
            dt=0
        elseif dt=="d" then
            dt=1
        end
        p=spec(R.A)
    case "constant" then
        p=R;
        toBeOrdered=%f
    else
        ierr=execstr("[wn,z,p]=%"+typeof(R,"overload")+"_damp(R)","errcatch")
        if ierr<>0 then
            error(msprintf(_("%s: Wrong type for input argument #%d: Array of floats, Polynomial or linear dynamical system expected.\n"),..
            "damp",1))
        end
        return

    end
    if dt==[] then
        //R does not furnish time domain
        if dt1==[] then
            //no user time domain specified, continuous time assumed
            dt=0
        else
            //user time domain specified
            dt=dt1
        end
    elseif dt1<>[] then
        warning(msprintf(_("%s: Input argument #%d ignored.\n"),"damp",2))
    end
    // Initialize
    wn=zeros(p);
    z=-ones(p);
    if dt>0 then // Discrete  time case
        ind=find(p<>1)
        s=p(ind);
        s=log(s)/dt;
    else //continuous time case
        ind=find(p<>0)
        s=p(ind);
    end
    wn(ind)=abs(s)
    z(ind)=-real(s)./abs(s)
    if toBeOrdered then
        [wn,k]=gsort(wn,"g","i");
        z=z(k);
        p=p(k)
    end
endfunction
