// Non-regression test file for bug 1198
// Pierre MARECHAL - Scilab Project
// Copyright INRIA
// 1er juin 2005

mode(-1)
clear

G = syslin('c', [1 0 ; 0 1], [ 1; 1], [1 1]);
minss(G)("A")
B=lasterror()

if B=='invalid index' then
	affich_result(%T,1198);
else
	affich_result(%F,1198);
end
