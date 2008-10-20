// Non-regression test file for bug 797
// Pierre MARECHAL - Scilab Project
// Copyright INRIA
// 1er juin 2005

mode(-1);
clear;

J=0.001;
format("v",7);
diary(TMPDIR+"/bug797.dia");
disp(J);
diary(0);

DIAFILECONTENTS=mgetl(TMPDIR+"/bug797.dia");

REFFILECONTENTS=[" ";
	"    0.001  ";]

if and(DIAFILECONTENTS==REFFILECONTENTS) then
	affich_result(%T,797);
else
	affich_result(%F,797);
end
