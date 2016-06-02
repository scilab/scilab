// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C)  2016 - INRIA - Serge Steer
//
// This file is licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// For more information, see the COPYING file which you should have received
// along with this program.
function [z,p,C]=simplify_zp(z,p,tol)
    iszpk=typeof(z)=="zpk"
    if iszpk then //simplify_zp(sys,tol)
        if argn(2)>2 then
            error(msprintf(_("%s: Wrong number of input argument: at most %d expected.\n"),"simplify_zp",2))
        else
            if argn(2)==1 then
                tol=0;
            else
                tol=p;
            end
            sys=z;
        end
        if type(tol)<>1|~isreal(tol)|size(tol)<>1|tol<0 then
            error(msprintf(_("%s: Wrong value for input argument #%d: A scalar (>= 0) expected.\n"),"simplify_zp",2))
        end
        if argn(1)>1 then
            error(msprintf(_("%s: Wrong number of output argument: %d expected.\n"),"simplify_zp",1))
        end
    else
        if argn(2)==2 then  tol=0;end
        if type(z)<>1 then
            error(msprintf(_("Wrong type for input argument %d: Real or complex array expected.\n"),"simplify_zp",1))
        end
        if type(p)<>1 then
            error(msprintf(_("Wrong type for input argument %d: Real or complex array expected.\n"),"simplify_zp",2))
        end
        if type(tol)<>1|~isreal(tol)|size(tol)<>1|tol<0 then
            error(msprintf(_("%s: Wrong value for input argument #%d: A scalar (>= 0) expected.\n"),"simplify_zp",3))
        end
        g=1
        dt=[]
        sys=zpk(z,p,g,dt)
    end

    [ny,nu]=size(sys)
    C=cell(ny,nu);
    for k=1:nu
        for l=1:ny
            slk=sys(l,k)
            z=slk.Z{1}
            p=slk.P{1}
            //find common values
            iz=[];ip=[];
            for i=1:size(z,"*")
                for j=1:size(p,"*")
                    if and(j<>ip)&abs(z(i)-p(j))<=tol*(1+abs(z(i))) then
                        iz=[iz,i];
                        ip=[ip,j];
                        break
                    end
                end
            end
            C{l,k}=p(ip)
            //remove common values
            p(ip)=[]
            z(iz)=[]


            slk.Z={z};slk.P={p}
            sys(l,k)=slk;
        end
    end
    if ~iszpk then
        z=sys.Z{1}
        p=sys.P{1}
        C=C{1}
    else
        z=sys,p=C;
    end
endfunction
