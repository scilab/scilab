//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

prot = funcprot();
funcprot(0);

function [X,Y]=field(x,y)
    // x and y are two vectors defining a grid
    // X and Y are two matrices which gives the grid point coordinates
    //-------------------------------------------------------------
    [rx,cx]=size(x);
    [ry,cy]=size(y);
    if rx<>1, write(%io(2),"x must be a row vector");return;end;
    if ry<>1, write(%io(2),"y must be a row vector");return;end;
    X=x.*.ones(cy,1);
    Y=y'.*.ones(1,cx);
endfunction

function [z]=dup(x,n)
    // utility
    // x is a vector this function returns [x,x,x,x...] or [x;x;x;x;..]
    // depending on x
    [nr,nc]=size(x)
    if nr==1 then
        y=ones(n,1);
        z= x.*.y ;
    else
        if nc<>1 then
            error("dup : x must be a vector");
        else
            y=ones(1,n);
            z= x.*.y ;
        end
    end
endfunction


function [z] = bezier(p,t)
    //comment : Computes a  Bezier curves.
    //For a test try:
    //beziertest; bezier3dtest; nurbstest; beziersurftest; c1test;
    //Uses the following functions:
    //bezier, bezier3d, nurbs, beziersurface
    //endcomment
    //reset();
    // Evaluate sum p_i B_{i,n}(t) the easy and direct way.
    // p must be a k x n+1 matrix (n+1) points, dimension k.
    n=size(p,"c")-1;// i=nonzeros(t~=1);
    t1=(1-t); t1z= find(t1==0.0); t1(t1z)= ones(t1z);
    T=dup(t./t1,n)';
    b=[((1-t').^n),(T.*dup((n-(1:n)+1)./(1:n),size(t,"c")))];
    b=cumprod(b,"c");
    if (size(t1z,"c")>0); b(t1z,:)= dup([ 0*ones(1,n),1],size(t1z,"c")); end;
    z=p*b';
endfunction


function bezier3d(p)
    // Shows a 3D Bezier curve and its polygon
    t=linspace(0,1,300);
    s=bezier(p,t);
    dh=xget("dashes");
    xset("dashes",3)
    param3d(p(1,:),p(2,:),p(3,:),34,45)
    xset("dashes",4);
    param3d(s(1,:),s(2,:),s(3,:),34,45,"x@y@z",[0,0])
    xset("dashes",dh);
    xtitle("A 3d polygon and its Bezier curve");
    current_axe = gca();current_axe.title.font_size = 3;
endfunction


function [X,Y,Z]=beziersurface(x,y,z,n)
    // Compute a Bezier surface. Return {bx,by,bz}.
    [lhs,rhs]=argn(0);
    if rhs <= 3 ; n=20;end
    t=linspace(0,1,n);
    n=size(x,"r")-1; // i=nonzeros(t~=1);
    t1=(1-t); t1z= find(t1==0.0); t1(t1z)= ones(t1z);
    T=dup(t./t1,n)';
    b1=[((1-t').^n),(T.*dup((n-(1:n)+1)./(1:n),size(t,"c")))];
    b1=cumprod(b1,"c");
    if (size(t1z,"c")>0);
        b1(t1z,:)= dup([ 0*ones(1,n),1],size(t1z,"c"));
    end
    n=size(x,"c")-1; // i=nonzeros(t~=1);
    t1=(1-t); t1z= find(t1==0.0); t1(t1z)= ones(t1z);
    T=dup(t./t1,n)';
    b2=[((1-t').^n),(T.*dup((n-(1:n)+1)./(1:n),size(t,"c")))];
    b2=cumprod(b2,"c");
    if (size(t1z,"c")>0);
        b2(t1z,:)= dup([ 0*ones(1,n),1],size(t1z,"c"));
    end
    X=b1*x*b2';Y=b1*y*b2';Z=b1*z*b2';
endfunction

funcprot(prot);
