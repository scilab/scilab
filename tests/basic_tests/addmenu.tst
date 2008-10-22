// Copyright INRIA
//check button creation/destruction; must be executed in window mode
mode(-1)
for k=1:10
n=100
for k=1:n,addmenu('mymenu'+string(k)),end
[s,kk]=sort(rand(1,n));
for k=kk,delmenu('mymenu'+string(k)),
xbasc();plot2d() //to use memory allocator
end
end
