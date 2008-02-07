
files=['intex6c.o','crestrc.o'];
ilib_build('gw_ex6c',['ex6c','intex6c'],files,[]);

exec loader.sce;

// all together 

[y1,y2,y3]=ex6c();
if y1<>"Scilab is ..." then pause,end
if norm(y2-matrix((1:5*3),3,5)) > %eps then pause,end
if norm(y3-matrix((1:5*3),3,5)) > %eps then pause,end