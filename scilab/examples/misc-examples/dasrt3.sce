//Copyright INRIA

files=G_make(['/tmp/res22.o'],'res22.dll');
link(files,'res22');
files=G_make(['/tmp/jac22.o'],'jac22.dll');
link(files,'jac22');
files=G_make(['/tmp/gr22.o'],'gr22.dll');
link(files,'gr22');
//
rtol=[1.d-6;1.d-6];atol=[1.d-6;1.d-4];
//
t0=0;y0=[2;0];y0d=[0;-2];t=[20:20:200];ng=1;
//
info=list([],0,[],[],[],0,0);
//Calling the routines by dasrt
//
[yy,nn]=dasrt([y0,y0d],t0,t,atol,rtol,'res22','jac22',ng,'gr22',info);
// hot restart (uncomment)
//[yy,nn,hot]=dasrt([y0,y0d],t0,t,atol,rtol,'res22','jac22',ng,'gr22',info);
//t01=nn(1);t=100:20:200;[pp,qq]=size(yy);y01=yy(2:3,qq);y0d1=yy(3:4,qq);
//[yy,nn,hot]=dasrt([y01,y0d1],t01,t,atol,rtol,'res22','jac22',ng,'gr22',info,hot);

