// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function z=narsimul(x1,x2,x3,x4,x5,x6,x7,x8)
    //function z=narsimul(a,b,d,sig,u,up,yp,ep)
    //   or
    // function z=arsimul(ar,u,up,yp,ep)
    //
    // Armax simulation  using rtitr
    //   A(z)= Id+a1*z+...+a_r*z^r;  ( r=0  => A(z)=Id)
    //   B(z)= b0+b1*z+...+b_s z^s;  ( s=-1 => B(z)=0)
    //   D(z)= Id+d1*z+...+d_t z^t;  ( t=0  => D(z)=Id)
    // z et e sont a valeurs dans dans R^n et u dans R^m
    // Auteur : J-Ph. Chancelier ENPC Cergrene
    //
    // Copyright Enpc

    [lhs,rhs]=argn(0)
    // switch to ar representation
    if type(x1)==1 then
        if rhs < 5 then
            error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"narsimul",5,8));
        end;
        ar=armac(x1,x2,x3,size(x1,"r"),size(x5,"r"),x4);
        select rhs
        case 5 then
            z=narsimul(ar,x5);
        case 6 then
            z=narsimul(ar,x5,x6);
        case 7 then
            z=narsimul(ar,x5,x6,x7);
        case 8 then
            z=narsimul(ar,x5,x6,x7,x8);
        end
    elseif typeof(x1)== "ar" then  // Here the call is always arsimul(ar,....)
        if rhs < 2|rhs>5 then
            error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"narsimul",2,5));
        end;

        a=x1("a");b=x1("b");d=x1("d");sig=x1("sig");
        u=x2;
        [mmu,Nu]=size(u);
        if mmu<>x1("nu") then
            msg = _("%s: Incompatible input arguments #%d and #%d: Number of rows of #%d are incompatible with #%d argument.\n")
            error(msprintf(msg, "narsimul", 1, 2, 2, 1));
        end;
        // dimensions
        [al,ac]=size(a);adeg=int(ac/al);
        [dl,dc]=size(d);ddeg=int(dc/dl);
        [bl,bc]=size(b);[mmu,Nu]=size(u);bdeg=int(bc/mmu);
        // quelques tests a faire : bl=al=dl,
        // <i>deg*<i>l=<i>c, pour i=a,b,d
        //
        // On genere d'abord y(k) solution de : A(z^-1)y(k)=B^(z-1)u(k)
        s=poly(0,"s");
        // Build polynomial matrix A(s)
        mata= a*((s.^[adeg-1:-1:0]).*.eye(al,al))';
        // Build polynomial matrix B(s)
        matb= b*((s.^[bdeg-1:-1:0]).*.eye(mmu,mmu))';
        //
        num=matb*s**(adeg-1)
        den=mata*s**(bdeg-1);
        // Using past values
        // yp doit etre de taille (al,(adeg-1))
        // up doit etre de taille (al,(bdeg-1))
        // ep doit etre de taille (al,(adeg-1))
        //
        if rhs <=3 then
            up=0*ones(mmu,(bdeg-1));
        else
            up=x3
            if size(up,1)<>mmu then
                msg = _("%s: Incompatible input arguments #%d and #%d: Same row dimensions expected.\n")
                error(msprintf(msg, "narsimul",2,3))
            end
            if size(up,2)<>bdeg-1,
                msg = _("%s: Incompatible input arguments #%d and #%d: Number of columns of #%d are incompatible with #%d argument.\n")
                error(msprintf(msg, "narsimul",1,3,3,1));
            end
        end
        if rhs <=4 then
            yp=0*ones(al,(adeg-1));
        else
            yp=x4
            if size(yp,1)<>al then
                msg = _("%s: Incompatible input arguments #%d and #%d: Number of rows of #%d are incompatible with #%d argument.\n")
                error(msprintf(msg, "narsimul",1,4,4,1));
            end
            if size(yp,2)<>(adeg-1)  then
                msg = _("%s: Incompatible input arguments #%d and #%d: Number of columns of #%d are incompatible with #%d argument.\n")
                error(msprintf(msg, "narsimul",1,4,4,1));
            end
        end
        if rhs <=5,
            ep=0*ones(al,(ddeg-1));
        else
            ep=x5
            if size(ep,1)<>al then
                msg = _("%s: Incompatible input arguments #%d and #%d: Number of rows of #%d are incompatible with #%d argument.\n")
                error(msprintf(msg, "narsimul", 1,5,5,1))
            end
            if size(ep,2)<>(adeg-1)  then
                msg = _("%s: Incompatible input arguments #%d and #%d: Number of columns of #%d are incompatible with #%d argument.\n")
                error(msprintf(msg, "narsimul",1,5,5,1))
            end

        end;
        //
        degnum=max(degree(den));
        yp=[0*ones(al,degnum+1-adeg),yp(:,(adeg-1):-1:1)];
        up=[0*ones(mmu,degnum+1-bdeg),up(:,(bdeg-1):-1:1)];
        y=rtitr(num,den,u,up,yp);
        // truncate the solution to only keep y_1,..y_Nu
        // (if b0=0 rtitr computes y_{Nu+1})
        y=y(:,1:Nu);
        // Generate bru such that A(z^-1)bru= D(z^-1) sig*e(t)
        // Build polynomial matrix D(s)
        matd= d*((s.^[ddeg-1:-1:0]).*.eye(al,al))';
        num=matd*s**(adeg-1)
        den=mata*s**(ddeg-1);
        degnum=max(degree(den));
        ep=[0*ones(al,degnum+1-ddeg),ep(:,(ddeg-1):-1:1)];
        // Normal noise
        br=sig*rand(al,Nu,"normal")
        bru=rtitr(num,den,br,ep,0*ones(ep));
        // z(k) = y(k) + bru(k)
        z=y+bru(:,1:Nu);
    else
        msg = _("%s: Wrong type for input argument #%d: %s data structure expected.\n")
        error(msprintf(msg, "narsimul",1,"arma"))
    end;
endfunction
