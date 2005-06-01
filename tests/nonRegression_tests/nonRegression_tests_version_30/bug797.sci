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

REFFILECONTENTS=["-->disp(J);";
	" ";
	"    0.001  ";
	"-->diary(0);"]

correct=%T
if or(DIAFILECONTENTS<>REFFILECONTENTS) then
	correct=%F
end

affich_result(correct,797);
