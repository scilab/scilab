// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function [f,d]=invr(h,flag)
    //if h is a scalar, polynomial or rational function matrix, invr
    //computes h^(-1).
    //!
    if argn(2)==1 then
        flag="C";
    end
    lhs=argn(1)
    select typeof(h)
    case "constant" then
        f=inv(h);
    case "polynomial" then //POLYNOMIAL MATRIX
        [m,n]=size(h);
        if m<>n then error(20),end
        ndeg=max(degree(h));
        if ndeg==1 then   //MATRIX PENCIL
            E=coeff(h,1);A=-coeff(h,0);
            if norm(E-eye(E),1) < 100*%eps then
                // sI -A
                [num,den]=coff(A,varn(h));f=num/den;
            else
                [Bfs,Bis,chis]=glever(E,A,varn(h));
                f=Bfs/chis - Bis;
                if lhs==2 then
                    d=lcm(f("den"));
                    f=f*d;f=f("num");
                end
            end
        else // GENERAL POLYNOMIAL MATRIX
            select flag
            case "L"
                f=eye(n,n);
                for k=1:n-1,
                    b=h*f,
                    d=-sum(diag(b))/k
                    f=b+eye(n,n)*d,
                end;
                d=sum(diag(h*f))/n,
                if degree(d)==0 then d=coeff(d),end,
                if lhs==1 then f=f/d;end
            case "C"
                [f,d]=coffg(h);
                if degree(d)==0 then d=coeff(d),end
                if lhs==1 then f=f/d;end
            else
                error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                "invr",2,"''C'',''D''"))
            end;
        end
    case "rational" then
        [m,n]=size(h(2));
        if m<>n then error(20),end
        select flag
        case "L" //    Leverrier
            f=eye(n,n);
            for k=1:n-1,
                b=h*f,
                d=0;for l=1:n,d=d+b(l,l),end,d=-d/k;
                f=b+eye(n,n)*d,
            end;
            b=h*f;d=0;for l=1:n,d=d+b(l,l),end;d=d/n,
            if lhs==1 then f=f/d;end
        case "A" //   lcm of all denominator entries
            denh=lcm(h("den"));
            Num=h*denh;Num=Num("num");
            [N,d]=coffg(Num);
            f=N*denh;
            if lhs==1 then f=f/d;end
        case "C"// default method by polynomial inverse
            [Nh,Dh]=lcmdiag(h); //h=Nh*inv(Dh); Dh diagonal;
            [N,d]=coffg(Nh);
            f=Dh*N;
            if lhs==1 then f=f/d;end
        case "Cof"// cofactors method
            [f,d]=coffg(h);
            if lhs==1 then f=f/d;end
        else
            error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
            "invr",2,"''L'',''A'',''C'',''Cof''"))
        end;
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A floating point number or polynomial or rational fraction array expected.\n"),"invr",1))
    end;
endfunction
