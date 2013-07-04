// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2935 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2935
//
// <-- Short Description -->
// In the "Bezier surface test 2" demo, an axis is displayed, it should not.

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


function [X,Y,Z]=beziersurface (x,y,z,n)
    // Compute a Bezier surface. Return {bx,by,bz}.
    [lhs,rhs]=argn(0);
    if rhs <= 3 ; n=20;end
    t=linspace(0,1,n);
    n=size(x,'r')-1; // i=nonzeros(t~=1);
    t1=(1-t); t1z= find(t1==0.0); t1(t1z)= ones(t1z);
    T=dup(t./t1,n)';
    b1=[((1-t').^n),(T.*dup((n-(1:n)+1)./(1:n),size(t,'c')))];
    b1=cumprod(b1,'c');
    if (size(t1z,'c')>0); 
        b1(t1z,:)= dup([ 0*ones(1,n),1],size(t1z,'c'));
    end
    n=size(x,'c')-1; // i=nonzeros(t~=1);
    t1=(1-t); t1z= find(t1==0.0); t1(t1z)= ones(t1z);
    T=dup(t./t1,n)';
    b2=[((1-t').^n),(T.*dup((n-(1:n)+1)./(1:n),size(t,'c')))];
    b2=cumprod(b2,'c');
    if (size(t1z,'c')>0); 
        b2(t1z,:)= dup([ 0*ones(1,n),1],size(t1z,'c'));
    end
    X=b1*x*b2';Y=b1*y*b2';Z=b1*z*b2';
endfunction

function c1test

    // first surface'
    x1=dup(-0.5:0.25:0.5,5);
    y1=dup([0,0,0,0,1],5);
    z1=dup(2:0.25:3,5)';
    [xb1,yb1,zb1]=beziersurface(x1,y1,z1,10);

    // second surface
    x2=dup(-0.5:0.25:0.5,5);
    y2=[(ones(4,5));[0,0,0,0,0]];
    z2=-dup(-1:0.25:0,5)';
    [xb2,yb2,zb2]=beziersurface(x2,y2,z2,10);

    // a surface to link the two previous ones'
    x=zeros(5,5); y=x; z=x;
    x(1,:)=x1(1,:); x(2,:)=x(1,:)-(x1(2,:)-x1(1,:));
    x(5,:)=x2(1,:); x(4,:)=x(5,:)-(x2(2,:)-x2(1,:));
    x(3,:)=(x(4,:)+x(2,:))/2;
    y(1,:)=y1(1,:); y(2,:)=y(1,:)-(y1(2,:)-y1(1,:));
    y(5,:)=y2(1,:); y(4,:)=y(5,:)-(y2(2,:)-y2(1,:));
    y(3,:)=(y(4,:)+y(2,:))/2;
    z(1,:)=z1(1,:); z(2,:)=z(1,:)-(z1(2,:)-z1(1,:));
    z(5,:)=z2(1,:); z(4,:)=z(5,:)-(z2(2,:)-z2(1,:));
    z(3,:)=(z(4,:)+z(2,:))/2;
    A=35,T=50,L=" ",EB=[4,2,0];
    [xb,yb,zb]=beziersurface(x,y,z,10);

    //drawing
    my_handle = scf(100001);
    clf(my_handle,"reset");
    my_current_axis = gca();
    drawlater();
    subplot(2,1,1);
    title("how two bezier surfaces can be joined","fontsize",3);
    subplot(2,2,1);
    plot3d2(xb1,yb1,zb1,-1,A,T,L,EB);
    subplot(2,2,3);
    plot3d2(xb2,yb2,zb2,-1,A,T,L,EB);
    subplot(1,2,2);
    [n1,p1]=size(xb1);
    [n2,p2]=size(xb);
    plot3d2([xb1;xb;xb2],[yb1;yb;yb2],[zb1;zb;zb2],-1,A,T,L,EB);
    delete(my_current_axis);
    drawnow();

endfunction

c1test();

// there should be 3 subplots and not 4
fig = gcf();
if (size(fig.children) <> [3,1]) then pause; end

clear c1test;
clear beziersurface;
clear dup;

