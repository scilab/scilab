// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C)  2016 - INRIA - Serge Steer
//
// This file is licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// For more information, see the COPYING file which you should have received
// along with this program.
function s=zpk(varargin)
    if size(varargin)==1 then //zpk(sys)
        sys=varargin(1)
        dt=sys.dt
        select typeof(sys)
        case "state-space"
            [ny,nu]=size(sys);
            z=cell(ny,nu);
            p=cell(ny,nu)
            k=zeros(ny,nu);
            for i=1:ny
                for j=1:nu
                    [z{i,j},p{i,j},k(i,j)]=ss2zp(minss(sys(i,j)))
                end
            end
        case "rational"
            [ny,nu]=size(sys);
            z=cell(ny,nu);
            p=cell(ny,nu);
            k=zeros(ny,nu);
            for i=1:ny
                for j=1:nu
                    [z{i,j},p{i,j},k(i,j)]=tf2zp(sys(i,j))
                end
            end
        case "zpk"
            s=sys
            return
        else
            error(msprintf(_("Wrong type for argument %d: A system in state space or transfer matrix form expected.\n"),"zpk",1))
        end
    elseif size(varargin)==4 then//zpk(z,p,k,dt)
        z=varargin(1);
        p=varargin(2);
        k=varargin(3);
        dt=varargin(4);
        if and(type(dt)<>[1 10])|and(size(dt,"*")<>[0 1]) then
            error(msprintf(_("%s: Wrong type for input argument #%d: A scalar or a string expected.\n"),"zpk",4))
        end
        if typeof(z)=="ce" then
            if typeof(p)<>"ce" then
                error(msprintf(_("%s: Wrong type for input argument  #%d: Cell expected.\n"),"zpk",2))
            end
            if typeof(k)<>"constant"|~isreal(k) then
                error(msprintf(_("%s: Wrong type for input argument #%d: A real vector expected.\n"),"zpk",3))
            end

            if or(size(z)<>size(p)) then
                error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"zpk",1,2))
            end
            if or(size(z)<>size(k)) then
                error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"zpk",1,3))
            end
            [ny,nu]=size(z);
            for iy=1:ny
                for iu=1:nu
                    if type(z{iy,iu})<>1 then
                        error(msprintf(_("%s: Wrong type for input argument  #%d: Cell elements must be numerical arrays.\n"),"zpk",1))
                    end
                    if type(p{iy,iu})<>1 then
                        error(msprintf(_("%s: Wrong type for input argument  #%d: Cell elements must be numerical arrays.\n"),"zpk",2))
                    end
                end
            end

        elseif typeof(z)=="constant" then
            if and(size(z)>1) then
                error(msprintf(_("%s: Wrong type for input argument #%d: A vector expected.\n"),"zpk",1))
            end
            if and(size(p)>1) then
                error(msprintf(_("%s: Wrong type for input argument #%d: A vector expected.\n"),"zpk",2))
            end
            if size(k,"*")<>1|~isreal(k) then
                error(msprintf(_("%s: Wrong type for input argument #%d: Real scalar expected.\n"),"zpk",3))
            end
            ny=1;nu=1;
            z={z};p={p};
        else
            error(msprintf(_("%s: Wrong type for input argument #%d: A vector or a cell of vectors expected.\n"),"zpk",1))
        end
    else
        error(msprintf(_("%s: Wrong number of input argument(s): %d or %d expected."),"zpk",1,4))
    end
    //order poles ans zeros
    for iy=1:ny
        for iu=1:nu
            if k(iy,iu)<>0 then
                zz=z{iy,iu}(:);
                [s,l]=gsort([real(zz),imag(zz)],"lr","d");
                z{iy,iu}=zz(l);

                pp=p{iy,iu}(:);
                [s,l]=gsort([real(pp),imag(pp)],"lr","d");
                p{iy,iu}=pp(l);
            else
                z{iy,iu}=[];
                p{iy,iu}=[];
            end
        end
    end
    s=mlist(["zpk","Z","P","K","dt"],z,p,k,dt)
endfunction
