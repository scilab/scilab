// exec("bug569.sce")

mode (-1);
clear;

disp("check the result of the  graphic windows");

x = 0:0.1:1;
f = sin(2*%pi*x)'*cos(2*%pi*x);
nz = [0,0.1,0.2];
ierror=execstr('contour(x,x,f,nz)','errcatch');
if ierror == 0 then
	affich_result(%T,569);
else
	affich_result(%F,569);
end
