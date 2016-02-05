// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 4339 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4339
//
// <-- Short Description -->
// listvarinfile does not manage graphic handles

clear;
warning("off")
//figure
hF = figure();

//axes + plot3d
plot3d();
hPlot3d = gca();

//axes + compound + polyline + legend
scf();
plot2d();
hPlot2d = gca();

//Fac3d
scf();
t = [0 : 0.3 : 2*%pi]';
z = sin(t) * cos(t');
[xx, yy, zz] = genfac3d(t, t, z);
plot3d([xx xx],[yy yy],list([zz zz+4],[4*ones(1,400) 5*ones(1,400)]))
hFac3d = gce();
clear t z xx yy zz;
//Rectangle
scf();
plot2d(0,0,-1,"010"," ",[-2,-2,2,2]);
xrect(-1,1,2,2);
hRectangle = gce();

//Arc
xarc(-1.5,1.5,3,3,0,360*64)
hArc = gce();

//Champ
champ(-5:5,-5:5,rand(11,11),rand(11,11))
hChamp = gce();

//Segs
x=2*%pi*(0:7)/8;
xv=[2*sin(x);9*sin(x)];
yv=[2*cos(x);9*cos(x)];
xsegs(xv,yv,1:8)
hSeg = gce();
clear x xv yv;

//Grayplot
scf();
m=5;n=5;
M=round(32*rand(m,n));
grayplot(1:m,1:n,M)
hGrayplot = gce();
clear m n M;

//Matplot
Matplot((1:xget("lastpattern")))
hMatplot = gce();

//Fec
scf();
x = [0 1 0 -1];
y = [0 0 1  1];
T = [1 1 2 3 1;
2 3 4 1 1];
z = [0 1 0 -1];
fec(x, y, T, z, strf="040", mesh=%t)
hFec = gce();
clear x y z T

//Text
xstring(0.5,0.5,"Text demo",0,0)
hText = gce();

//Axis
scf();
plot2d(1:10,1:10,1,"020")
// horizontal axis
drawaxis(x=2:7,y=4,dir="u",tics="v");
hAxis = gce();

//uimenu
hMenu = uimenu(hF,"label", "windows");

//uicontrol
hUiControl = uicontrol(hF,"style","listbox", "position", [10 10 150 160]);

save(TMPDIR + "/bug_4339.bin", "hUiControl", "hMenu", "hAxis", "hText", "hFec", "hMatplot", "hGrayplot", "hSeg", "hChamp", "hArc", "hRectangle", "hFac3d", "hPlot2d", "hPlot3d", "hF");

[varList, varSize] = listvarinfile(TMPDIR + "/bug_4339.bin");
varListRef = ["hUiControl"; "hMenu"; "hAxis"; "hText"; "hFec"; "hMatplot"; "hGrayplot"; "hSeg"; "hChamp"; "hArc"; "hRectangle"; "hFac3d"; "hPlot2d"; "hPlot3d"; "hF"];
assert_checkequal(gsort(varList), gsort(varListRef));

