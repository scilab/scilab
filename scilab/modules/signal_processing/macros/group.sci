// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1988 - C. Bunks
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [tg,fr]=group(npts,a1i,a2i,b1i,b2i)
    //Calculate the group delay of a digital filter
    //with transfer function h(z).
    //The filter specification can be in coefficient form,
    //polynomial form, rational polynomial form, cascade
    //polynomial form, or in coefficient polynomial form.
    //  npts :Number of points desired in calculation of group delay
    //  a1i  :In coefficient, polynomial, rational polynomial, or
    //       :cascade polynomial form this variable is the transfer
    //       :function of the filter.  In coefficient polynomial
    //       :form this is a vector of coefficients (see below).
    //  a2i  :In coeff poly form this is a vector of coeffs
    //  b1i  :In coeff poly form this is a vector of coeffs
    //  b2i  :In coeff poly form this is a vector of coeffs
    //  tg   :Values of group delay evaluated on the grid fr
    //  fr   :Grid of frequency values where group delay is evaluated
    //
    //In the coefficient polynomial form the tranfer function is
    //formulated by the following expression:
    //
    //       h(z)=prod(a1i+a2i*z+z**2)/prod(b1i+b2i*z+z^2)
    //
    //!


    //get frequency grid values in [0,.5)

    fr=(0:.5/npts:.5*(npts-1)/npts);

    //get the of arguments used to called group

    [ns,ne]=argn(0);
    if and(ne <> [2 5]) then
        error(sprintf(_("%s: Wrong number of input argument(s): %d or %d expected.\n"), "group", 2, 5));
    end

    //if the number of arguments is 2 then
    //the case may be non-cascade

    hcs=1;
    if ne==2 then

        //get the type of h and the variable name

        h=a1i;
        ht=type(h);
        if and(ht <> [1 2 15 16]) then
            error(sprintf(_("%s: Wrong type for input argument #%d: A real or polynomial matrix or a rational expected.\n"), "group", 2));
        end
        if ht == 16 & or(h.dt == "c" | h.dt == []) then
            error(sprintf(_("%s: Wrong type for input argument #%d: A discrete system expected.\n"), "group", 2));
        end

        //if ht==1 then h is a vector containing filter coefficients

        if ht==1 then

            //make h a rational polynomial

            hcs=max(size(h));
            z=poly(0,"z");
            h=poly(h,"z","c");
            h=gtild(h,"d")*(1/z^(hcs-1));
        elseif ht == 2 then
            z = poly(0, varn(h));
        else  //ht==15|ht==16
            z=poly(0, varn(h(3)));
            hcs=max(size(h(2)));
        end,

        //if the rational polynomial is not in cascade form then

        if hcs==1 then
            //get the derivative of h(z)

            hzd=derivat(h);

            //get the group delay of h(z)
            tgz=-z*hzd/h;

            //evaluate tg

            rfr=exp(2*%pi*%i*fr);


            tg=real(freq(tgz(2),tgz(3),rfr));

            //done with non-cascade calculation of group delay

        end,
        //re-organize if h is in cascade form

        if hcs>1 then
            xc=[coeff(h(2)),coeff(h(3))];
            a2i=xc(1:hcs);
            a1i=xc(hcs+1:2*hcs);
            b2i=xc(3*hcs+1:4*hcs);
            b1i=xc(4*hcs+1:5*hcs);
            ne=5;
        end,
    end,

    //if implementation is in cascade form

    if ne==5 then

        //a1i,a2i,b1i,b2i are the coefficients of a
        //second order decomposition of the filter
        //(i.e. in cascade form, see Deczky)

        phi=2*%pi*fr;
        z=poly(0,"z");
        ejw=freq(z,1,exp(%i*phi));
        emjw=freq(z,1,exp(-%i*phi));

        a1=a1i'*ones(phi);
        b1=b1i'*ones(phi);
        a2=a2i'*ones(phi);
        b2=b2i'*ones(phi);
        ejw=ones(a1i)'*ejw;
        emjw=ones(a1i)'*emjw;

        aterm=(a1+2*ejw)./(a1+ejw+a2.*emjw);
        bterm=(b1+2*ejw)./(b1+ejw+b2.*emjw);

        tgi=real(bterm-aterm);
        tg=ones(a1i)*tgi;

        //done with cascade calculation of group delay
    end
endfunction
