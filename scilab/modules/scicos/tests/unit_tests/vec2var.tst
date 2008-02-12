a=list("cos",[1.1,2]);
b=var2vec(a);
c=vec2var(b);
if type(c) <> 15 then pause,end
if c(1) <> 'cos' then pause,end