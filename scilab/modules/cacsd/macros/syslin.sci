// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [sl]=syslin(domain,a,b,c,d,x0)

    [lhs,rhs]=argn(0)
    //
    // check domain
    select type(domain)
    case 1 then  //sampled system
        if size(domain,"*")<=2 then
            tp=domain
        else
            error(msprintf(gettext("%s: Wrong size for input argument #%d: A scalar expected.\n"),"syslin",1))
        end
        z="z"
    case 10 //continuous or discrete
        if size(domain,"*")<>1 then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"),"syslin",1))
        end

        domain=part(domain,1)
        select domain
        case "c" then
            z="s"
        case "d" then
            z="z"
        else
            error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "syslin",1,"''c'',''d''"))
        end;
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String, Scalar or empty matrix expected.\n"),"syslin",1))
    end;
    //============================================================================
    if rhs==2 then //syslin(domaine,sys)

        if typeof(a)=="state-space" | typeof(a)=="rational" then
            sl=a;
            sl("dt")=domain
        else
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space or a transfer function expected.\n"),"syslin",2))
        end
        //============================================================================
    elseif rhs==3 then // syslin(domaine,num,den)
        num=a;den=b
        if type(num)>2 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Polynomial array expected.\n"),"syslin",2))
        end
        if  type(den)>2 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Polynomial array expected.\n"),"syslin",3))
        end

        if or(size(num)<>size(den)) then
            error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"syslin",2,3))
        end

        if type(num)==2 & type(den)==2 then
            if varn(num)<>varn(den) then
                error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same formal variable names expected.\n"),"syslin",2,3))
            end
        end
        if type(num)==1 then
            num=num*poly(1,z,"c")
        end
        if type(den)==1 then
            den=den*poly(1,z,"c")
        end

        sl=rlist(varn(num,z),varn(den,z),domain)
        //============================================================================
    elseif rhs>3 then // syslin(domaine,A,B,C [,D [X0]])
        if type(a)<>1 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Array of floating point numbers expected.\n"),"syslin",2))
        end
        [ma,na]=size(a);
        if ma<>na then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: Square matrix expected.\n"),"syslin",2))
        end
        if type(b)<>1 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Array of floating point numbers expected.\n"),"syslin",3))
        end
        [mb,nb]=size(b);
        if na<>mb&mb<>0 then
            error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same row dimensions expected.\n"),"syslin",2,3));
        end
        if type(c)<>1 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Array of floating point numbers expected.\n"),"syslin",4))
        end
        [mc,nc]=size(c);
        if na<>nc&nc<>0 then
            error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same column dimensions expected.\n"),"syslin",2,4));
        end
        if rhs<6 then
            x0=0*ones(na,1)
        else
            if type(x0)>1 then
                error(msprintf(gettext("%s: Wrong type for input argument #%d: Array of floating point numbers expected.\n"),"syslin",6))
            end
            [mx,nx]=size(x0);
            if mx<>na|nx<>min(na,1) then
                msg = _("%s: Incompatible input arguments #%d and #%d: number of elements of #%d must match the column dimension of #%d\n")
                error(msprintf(msg,"syslin",6,1,6,1));
            end
        end
        if rhs<5  then
            d=0*ones(mc,nb)
        else
            if type(d)>2 then
                error(msprintf(gettext("%s: Wrong type for input argument #%d: Polynomial array expected.\n"),"syslin",5))
            end
            [md,nd]=size(d);
            if c*b<>[] then
                if mc<>md then
                    error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same row dimensions expected.\n"),"syslin",2,5));
                end
                if nb<>nd then
                    error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same column dimensions expected.\n"),"syslin",3,5));
                end

            end
        end
        sl=lsslist(a,b,c,d,x0,domain)
    end

endfunction
