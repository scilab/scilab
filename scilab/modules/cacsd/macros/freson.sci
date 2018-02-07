// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2000 - 2016 - INRIA - Serge Steer
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function fr = freson(h)
    [lhs,rhs] = argn(0);

    if argn(2) < 1 then
        msg = gettext("%s: Wrong number of input argument(s): %d expected.\n")
        error(msprintf(msg, "freson", 1));
    end
    if and(typeof(h)<>["state-space","rational","zpk"]) then
        ierr = execstr("[gm,fr]=%"+overloadname(sys)+"_freson(h)","errcatch")
        if ierr<>0 then
            msg = gettext("%s: Wrong type for input argument: Linear dynamical system expected.\n")
            error(msprintf(msg, "freson", 1))
        end
        return
    end
    if size(h,"*")<>1 then
        msg = gettext("%s: Wrong size for input argument #%d: Single input, single output system expected.\n")
        error(msprintf(msg, "freson", 1))
    end

    if typeof(h)=="state-space" then
        h=ss2tf(h)
    elseif typeof(h)=="zpk" then
        h=zpk2tf(h)
    end
    dt = h.dt
    [n,d] = h(["num","den"]);
    if type(n)==1 then
        n=poly(n,varn(d),"c");
    end
    if coeff(d,0)==0 then
        msg = gettext("%s: Wrong value for input argument #%d: infinite gain at zero frequency.\n")
        error(msprintf(msg, "freson", 1))
    end

    //look for  omega such that derivative of magn. is zero
    if dt=="c" then
        //find frequencies which zeros the magnitude derivative
        hh = h*horner(h,-%s)
        r = roots(derivat(hh).num)
        // k=find(imag(r)>0&abs(real(r))<%eps*abs(r)); // http://bugzilla.scilab.org/15368
        k = find(imag(r)>0 & abs(real(r))<1.e-14*abs(r));
        fr = imag(r(k))/(2*%pi)
    else
        if dt=="d" | dt==[] then dt = 1; end
        //find frequencies which zeros the magnitude derivative
        hh = h*horner(h,1/%z)
        r = roots(derivat(hh).num);
        k = find(abs(abs(r)-1)<=sqrt(%eps)*abs(r));
        r = imag(log(r(k)));
        fr = r(r>0&r<0.999*%pi)/(2*%pi*dt)
    end
    if fr==[] then return;end

    //find frequencies that correspond to a magnitude peak and sort them:
    k = find(abs(repfreq(h,fr))-abs(repfreq(h,fr*0.999))>0)
    fr = gsort(fr(k),"g","d");
endfunction
