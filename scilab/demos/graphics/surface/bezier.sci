function gammatest (N)
    // Bezier curve approximating a circle
    // Copyright INRIA
    [lhs,rhs] =argn(0);
    if rhs < 1 ;N=[10,20,50,100];end
    x=linspace(0,1,100);
    plot2d(cos(2*%pi*x)',sin(2*%pi*x)',1,"151"," ",[-2,-2,2,2]);
    xtitle('Bezier curve approximating a circle');
    current_axe = gca();
    current_axe.title.font_size = 3;
    icol=2;
    for n=N ; 
        t=sqrt(linspace(0,1,n));
        p=[cos(2*%pi*t);sin(2*%pi*t)];
        y=bezier(p,x);
        plot2d(y(1,:)',y(2,:)',icol,"000");
        icol=icol+1;
    end;
endfunction


function beziertest
    // a random polygon and a bezier curve 
    // Copyright INRIA
    plot2d(-0.2,-0.2,0,"011"," ",[-0.2,-0.2,1.2,1.2]);
    xtitle('Bezier Test : random polygon and bezier curve');
    current_axe = gca();current_axe.title.font_size = 3;
    rand('uniform');
    p=rand(2,5);
    t=linspace(0,1,300);
    s=bezier(p,t);
    plot2d(p(1,:)',p(2,:)',1,"000");
    plot2d(s(1,:)',s(2,:)',2,"000");
endfunction


function bezier3dtest ()
    // Show a Beziercurve of dimension 3
    // Copyright INRIA
    p=[-1,-1,-1;0,-1,-1;1,0,0;1,1,0;0,1,1;-1,1,0]';
    bezier3d(p);
endfunction


function beziersurftest
    // Show a Bezier surface
    // Copyright INRIA
    x=linspace(-%pi,%pi,5)
    [x,y]=field(x,x);
    z= 3*sin(x).*cos(y)
    [xb,yb,zb]=beziersurface(x,y,z);
    subplot(2,1,1);
    drawlater();
    plot3d3(x,y,z);
    xtitle('A first surface');
    current_axe = gca();current_axe.title.font_size = 3;
    subplot(2,1,2);
    plot3d2(xb,yb,zb,-1,35,45," ",[4,2,3]);
    xtitle('The bezier interpolated surface (n=10)');
    current_axe = gca();current_axe.title.font_size = 3;
    drawnow();
endfunction

function c1test
    // Show how two bezier surfaces can be joined.
    // Copyright INRIA
 
    // first surface
    x1=dup(-0.5:0.25:0.5,5);
    y1=dup([0,0,0,0,1],5);
    z1=dup(2:0.25:3,5)';
    [xb1,yb1,zb1]=beziersurface(x1,y1,z1,10);
 
    // second surface 
    x2=dup(-0.5:0.25:0.5,5);
    y2=[(ones(4,5));[0,0,0,0,0]];
    z2=-dup(-1:0.25:0,5)';
    [xb2,yb2,zb2]=beziersurface(x2,y2,z2,10);
 
    // a surface to link the two previous ones 
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
    A=35,T=50,L=" ",EB=[4,2,0]
    [xb,yb,zb]=beziersurface(x,y,z,10);
 
    //drawing
    drawlater();
    subplot(2,1,1);
    xtitle('how two bezier surfaces can be joined');
    current_axe = gca();current_axe.title.font_size = 3;
    subplot(2,2,1);
    plot3d2(xb1,yb1,zb1,-1,A,T,L,EB); 
    subplot(2,2,3);
    plot3d2(xb2,yb2,zb2,-1,A,T,L,EB);
    subplot(1,2,2);
    [n1,p1]=size(xb1);
    [n2,p2]=size(xb);
    plot3d2([xb1;xb;xb2],[yb1;yb;yb2],[zb1;zb;zb2],-1,A,T,L,EB);
    drawnow();
endfunction
