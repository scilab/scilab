// all together 

[y1,y2,y3]=ex6c();
if y1<>"Scilab is ..." then pause,end
if norm(y2-matrix((1:5*3),3,5)) > %eps then pause,end
if norm(y3-matrix((1:5*3),3,5)) > %eps then pause,end