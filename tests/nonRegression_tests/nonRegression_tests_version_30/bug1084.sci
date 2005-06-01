// Non-regression test file for bug 1084
// Pierre MARECHAL - Scilab Project
// Copyright INRIA
// 1er juin 2005

rand('seed',0);
A = rand(1,3,'normal');
rand('seed',0);
B = rand(1,3,'normal');
rand('seed',0);
rand('seed',0);
C = rand(1,3,'normal');

if and(A==B) & and(B==C) & and(A==C) then
	affich_result(%T,1084);
else
	affich_result(%F,1084);
end
