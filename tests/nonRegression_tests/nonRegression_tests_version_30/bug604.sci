//exec("bug604.sci")
// 
g=9.81;m=2;l=1;K=10;I=12;
//M=[g*m*l,I+2*m*l^2,K/l;0,1,0;0,0,1];
 
//Le bug n'existe plus, la possibilité de faire 2m au lieu de 2*m a été supprimée 
 
if  [2*m]  == 4 & 2*m == 4 & (2*m) == 4   then

 affich_result(%T,604);

else
 affich_result(%F,604);
end;
