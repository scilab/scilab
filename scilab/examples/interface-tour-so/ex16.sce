w=int16([3,5,7]);
w1=ex16c(w);
wref=int16([33,44,55]);
w2=ex16c();
if norm(double(w1-wref)) > %eps then pause,end
if norm(double(w2-wref)) > %eps then pause,end
