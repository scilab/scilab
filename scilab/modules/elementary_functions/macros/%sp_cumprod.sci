// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 -  INRIA - Serge Steer
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function r=%sp_cumprod(varargin)
    a = varargin(1)
    dims = size(a);

    nargs = size(varargin);
    select nargs
    case 1
        d = "*";
    case 2
        if or(varargin(2) == ["native", "double"]) then
            d = "*";
        else
            d = varargin(2);
        end
    case 3
        d = varargin(2);
    else
        error(msprintf(_("%s: Wrong number of input argument(s): %d to %d expected.\n"),"cumprod", 1, 3));
    end

    // Check second argument : d
    select type(d)
    case 1
        if size(d,'*') <> 1 then
            error(msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"),"cumprod", 2))
        end
        if int(d) <> d | d < 0 then
            error(msprintf(_("%s: Wrong value for input argument #%d: Integer >= %d expected.\n"),"cumprod", 2, 1))
        end
    case 10 
        if size(d,'*') <> 1 then
            error(msprintf(_("%s: Wrong size for input argument #%d: string expected.\n"),"cumprod",2))
        end
        if and(d<>["r","c","*","m"]) then
            error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
            "cumprod",2,"""*"",""r"",""c"",""m"""))
        end

        pos = [1,2,0,find(dims>1,1)];
        d=pos(find(d==["r","c","*","m"]))
    else
        error(msprintf(_("%s: Wrong type for input argument #%d: string or scalar expected.\n"),"cumprod",2))
    end
    
    r=sparse(dims,0,dims)
    select d
    case 0 then
        [ij,v]=spget(a)
        if or(ij(1,:)<>[1 1]) then return,end
        if dims(1)==1 then
            l=find(diff(ij(:,2))>1,1)
            if l==[] then
                r=sparse(ij,cumprod(v),dims)
            else
                r=sparse(ij(1:l,:),cumprod(v(1:l)),dims)
            end
        elseif dims(2)==1 then
            l=find(diff(ij(:,1))>1,1)
            if l==[] then
                r=sparse(ij,cumprod(v),dims)
            else
                r=sparse(ij(1:l,:),cumprod(v(1:l)),dims)
            end
        else
            r=matrix(cumprod(matrix(a,1,-1)),dims)
        end
    case 1 then
        ij=[];v=[];
        for k=1:dims(2)
            [ijk,vk]=spget(a(:,k));
            if and(ijk(1,:)==[1 1]) then
                l=find(diff(ijk(:,1))>1,1);
                if l==[] then
                    ij=[ij;[ijk(:,1) k*ones(vk)]];
                    v=[v;cumprod(vk)];
                else
                    ij=[ij;[ijk(1:l,1) k*ones(l,1)]];
                    v=[v;cumprod(vk(1:l,:))];
                end
            end
        end
        r=sparse(ij,v,dims)
    case 2 then
        ij=[];v=[]
        for k=1:dims(1)
            [ijk,vk]=spget(a(k,:))
            if and(ijk(1,:)==[1 1]) then
                l=find(diff(ijk(:,2))>1,1)
                if l==[] then
                    ij=[ij;[k*ones(vk) ijk(:,2)]];
                    v=[v;cumprod(vk)]
                else
                    ij=[ij;[k*ones(l,1),ijk(1:l,2)]];
                    v=[v;cumprod(vk(1:l,:))]
                end
            end
        end
        r=sparse(ij,v,dims)
    else
        r=a
    end

endfunction
