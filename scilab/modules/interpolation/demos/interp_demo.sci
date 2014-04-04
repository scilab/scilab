//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// some functions for the interpolation demo

function [xf,yf,zf,qf] = nf3dq(x,y,z,q,orient)
    //
    // same than nf3d but with a supplementary val field
    //
    //   j+1 +----+     orient=1 corresponds to :
    //       |    |       (i,j) -> (i,j+1) -> (i+1,j+1) -> (i+1,j)
    //    j  +----+
    //       i   i+1    orient=2 corresponds to :
    //                    (i,j) -> (i+1,j) -> (i+1,j+1) -> (i,j+1)
    //
    if ~exists("orient","local") then, orient=2, end
    [n1,n2]=size(x)
    nx = (n1-1)*(n2-1);
    v = 1:(n1*n2-n1-1); v(n1*(1:(n2-2)))=[];
    if orient == 1 then
        ind = [v; v+n1; v+(n1+1); v+1];
    else
        ind = [v; v+1; v+(n1+1); v+n1];
    end
    xf=matrix(x(ind),4,nx)
    yf=matrix(y(ind),4,nx)
    zf=matrix(z(ind),4,nx)
    qf=matrix(q(ind),4,nx)
endfunction


function [Xm,Xp,Ym,Yp,Zm,Zp] = slice_parallelepiped(dir_, val, ebox, nx, ny, nz)
    //
    //   ebox = [xmin xmax ymin ymax zmin zmax]
    //   dir_ : string "x=", val
    //   n1 et n2 les discretisations
    //
    coef = 5.e-3;
    select dir_
    case "x=" then
        if val < ebox(1)  |  ebox(2) < val then
            error("Bad slice chosen")
        end
        dx = coef*(ebox(2)-ebox(1))
        y = linspace(ebox(3),ebox(4),ny)
        z = linspace(ebox(5),ebox(6),nz)
        [Zm,Ym] = ndgrid(z,y)
        Xm = (val-dx)*ones(Ym);
        [Yp,Zp] = ndgrid(y,z)
        Xp = (val+dx)*ones(Yp);
    case "y=" then
        if val < ebox(3)  |  ebox(4) < val then
            error("bad slice chosen")
        end
        dy = coef*(ebox(4)-ebox(3))
        x = linspace(ebox(1),ebox(2),nx)
        z = linspace(ebox(5),ebox(6),nz)
        [Xm,Zm] = ndgrid(x,z)
        Ym = (val-dy)*ones(Xm);
        [Zp,Xp] = ndgrid(z,x)
        Yp = (val+dy)*ones(Xp);
    case "z=" then
        if val < ebox(5)  |  ebox(6) < val then
            error("bad slice chosen")
        end
        dz = coef*(ebox(6)-ebox(5))
        x = linspace(ebox(1),ebox(2),nx)
        y = linspace(ebox(3),ebox(4),ny)
        [Ym,Xm] = ndgrid(y,x)
        Zm = (val-dz)*ones(Xm);
        [Xp,Yp] = ndgrid(x,y)
        Zp = (val+dz)*ones(Xp);
    else
        error("bad arg dir_")
    end // select
endfunction
