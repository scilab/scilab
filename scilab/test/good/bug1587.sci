// Non-regression test file for bug 1586
// Copyright INRIA
// Scilab Project - Jean-Baptiste Silvy
// Copyright INRIA 2006
// Date : 18 Juillet 2006

mode(-1) ;
clear ;

// test the modification on a figure which is not current
f1 = gcf() ;
plot3d ;

f2 = scf() ;
hc = jetcolormap(21) ;
f2.color_map = hc ;
plot3d ;

// change the colormap of the first window
jc = jetcolormap(45) ;
f.color_map = jc ;

// check that the colormaps are well copied
if abs( max( f.color_map - jc ) ) > 1e-8 then
    affich_result(%F,1587) ;
end ;

if abs( max( f2.color_map - hc ) ) > 1e-8 then
    affich_result(%F,1587) ;
end ;

affich_result(%T,1587) ;
