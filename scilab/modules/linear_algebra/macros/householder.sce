// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// Example for householder() : Reflection of an object using Householder matrix
// ----------------------------------------------------------------------------
// (OA) = [0 0 1] is reflected in O into (OB) = [ 1 1 0.3 ]:
[n, H] = householder([0 0 1]', [ 1 1 0.3 ]');
// "n" is the unit vector orthogonal to the reflecting plane

// Emitting object (feature from shell demo):
u = linspace(0,2*%pi,40);
v = linspace(0,2*%pi,20);
Xe = (cos(u).*u)'*(1+cos(v)/2)+10;
Ye = (u/2)'*sin(v);
Ze = (sin(u).*u)'*(1+cos(v)/2);

// Reflected object:
Pe = [ Xe(:)' ; Ye(:)' ; Ze(:)'];
Pr = H*Pe;
Xr = matrix(Pr(1,:),40,-1);
Yr = matrix(Pr(2,:),40,-1);
Zr = matrix(Pr(3,:),40,-1);

// Reflecting plane containing O: n(1).x + n(2).y + n(3).z = 0
//   Sampling space:
x = linspace(min([Xe(:);Xr(:)]), max([Xe(:);Xr(:)]),20);
y = linspace(min([Ye(:);Yr(:)]), max([Ye(:);Yr(:)]),20);
[X, Y] = meshgrid(x,y);
//   Generating the mirror:
deff("z = mirror(x,y,n)","z = -n(1)/n(3)*x - n(2)/n(3)*y")
Zm = mirror(X,Y,n);

// Plotting:
clf
drawlater
f = gcf();
f.color_map = [ 0.8 0.8 0.8 ; jetcolormap(100)];
surf(Xe,Ye,Ze)
surf(X,Y,Zm)
surf(Xr,Yr,Zr)
isoview()
a = gca();
a.rotation_angles = [74 123];
a.children.color_flag = 0;
a.children.color_mode = 0;
a.children(1).foreground = color("red");
a.children(2).foreground = 1;
a.children(3).foreground = color("green");
drawnow
u = []
hm = []
