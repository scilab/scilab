// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge STEER
// Copyright (C) 1999 - Lucien.Povy@eudil.fr (to get a good table)
// Copyright (C) 2013 - Charlotte HECQUET (new option)
// Copyright (C) 2013 - A. Khorshidi (to define a new optional output argument)
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [r,num]=routh_t(h,k,normalized)
    //r=routh_t(h,k) computes routh table of denominator of the
    //system described by transfer matrix SISO continue h with the
    //feedback by the gain k
    //If  k=poly(0,'k') we will have a polynomial matrix with dummy variable
    //k, formal expression of the Routh table.
    //r=routh_t(d) computes Routh table of h :attention ! d=denom of system

    //If a zero row appears, it means that there exist a pair of pure imaginary
    //roots (oscillating system) or symmetric real roots. In this case, the pure imaginary roots are the
    //imaginary roots of the bisquare polynomial given by the previous row. The
    //routh table can be continued replacing this row by the derivative of this
    //polynomial.
    //see http://www.jdotec.net/s3i/TD_Info/Routh/Routh.pdf for degenerated
    //cases

    //see also
    //Comments on the Routh-Hurwitz criterion, Shamash, Y.,Automatic Control, IEEE T.A.C
    //Volume 25, Issue 1, Feb 1980 Page(s): 132 - 133

    //http://controls.engin.umich.edu/wiki/index.php/RouthStability
    [lhs,rhs]=argn(0);
    h1=h(1);
    flag=1;
    if rhs==3 then
        if normalized==%t then
            flag=1;
        else
            flag=0;
        end
        rhs=2;
    end
    if rhs==2 then
        if typeof(h)<>"rational" then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: rational fraction array expected.\n"),"routh_t",1));
        end
        [n,d]=h(2:3)
        if size(n,"*")<>1 then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Single input, single output system expected.\n"),"routh_t",1))
        end
        nd=max([degree(d) degree(n)])+1;
        cod=coeff(d,0:nd-1);//coeff du denominateur
        con=coeff(n,0:nd-1);//coeff du numerateur
        cobf=cod+k*con //coeff de la boucle fermee
    else
        if type(h)>2 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Polynomial array expected.\n"),"routh_t",1));
        end
        if size(h,"*")<>1 then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: A polynomial expected.\n"),"routh_t",1))
        end

        nd=degree(h)+1;
        cobf=coeff(h,0:nd-1)
    end;
    //
    r1=cobf(nd:-2:1);
    r2=cobf(nd-1:-2:1);
    ncol=size(r1,"*");
    if size(r2,"*")<>ncol then r2=[r2,0],end
    r=[r1;r2]
    if ncol<2 then r=[],return,end;
    if rhs==2 then

        for i=3:nd,
            if flag==0 then // for a non-normalized table
                r(i,1:ncol-1)=[r(i-1,1),-r(i-2,1)]*[r(i-2,2:ncol);r(i-1,2:ncol)]
            else // for a normalized table
                if r(i-1,1)==0 then
                    if type(k)<>1 then
                        error(msprintf(gettext("%s: Wrong type for input argument #%d: A scalar expected.\n"),"routh_t",2));
                    end
                    r(i-1,1)=poly(0,"eps")
                end
                r(i,1:ncol-1)=[1.,-r(i-2,1)/r(i-1,1)]*[r(i-2,2:ncol);r(i-1,2:ncol)]
            end
        end;
    else
        for i=3:nd,
            // Special Case: Row of zeros detected:
            if and(r(i-1,:)==0) then
                naux=nd-i+2 //order of previous polynomial
                exponents=naux:-2:0
                ncoeff=size(exponents,"*")
                r(i-1,1:ncoeff)=r(i-2,1:ncoeff).*exponents //derivative of previous polynomial
            end
            // Special Case: First element of the 2nd row or upper is zero and is replaced with %eps:
            if r(i-1,1)==0 then
                if rhs==1 then
                    if typeof(r)=="rational" then
                        //scilab is not able to handle multivariable polynomials
                        r=horner(r,%eps^2);
                    end
                    r(i-1,1)=poly(0,"eps")
                else
                    r(i-1,1)=%eps^2,
                end
            end
            r(i,1:ncol-1)=[1.,-r(i-2,1)/r(i-1,1)]*[r(i-2,2:ncol);r(i-1,2:ncol)]

        end;
    end;

    if lhs==2  then
        if rhs==2 & type(k)<>1 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: A scalar expected.\n"),"routh_t",2));
        end
        nrow=size(r,"r")
        num = 0;
        c = 0;
        for i = 1:nrow
            if horner(r(i,1),%eps) >= 0 then
                if c == 1 then
                    num= num+1;
                    c = 0;
                end
            elseif c == 0 then
                num= num+1;
                c = 1;
            end;
        end;
    end

endfunction
