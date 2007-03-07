deff('[u,v,w]=myfct(x,y)','u=7+x,v=8+y,w=9+y')
//ex11c executes myfct with input variables x=5 and y=6
x=5;y=6;[u,v,w]=myfct(x,y);
[p,q,r]=ex11c(1,2,3,4,5,6);
if p<>u | q <> v | r <> w then pause,end

