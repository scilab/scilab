
files=['intex1c.o'];
ilib_build('gw_ex1c',['ex1c','intex1c'],files,[]);

exec loader.sce;

a=1:10;b=a+1;c=ones(2,3)+2;
[x,y,z,t]=ex1c('mul',a,b,c);

 rt  = [ 2.    4.    6.    8.    10.    12.    14.    16.    18.    20.];
 rz  = [ 4.    6.    8.    10.    12.    14.    16.    18.    20.    22.];
 ry  = [ 6.    6.    6.;
        6.    6.    6.];
 rx  = [ 0.    6.     12.;
        6.    12.    18.];
        
if rt <> t then pause,end
if rz <> z then pause,end
if rx <> x then pause,end
if ry <> y then pause,end
