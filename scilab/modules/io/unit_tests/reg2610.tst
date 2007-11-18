// test bug 2610

write('foo.txt',rand(500,3)) 
fd=mopen('foo.txt');
A=mgetl(fd,300);
if size(A)<>[300,1] then pause, end

A=mgetl(fd,300);
if size(A)<>[200,1] then pause, end

A=mgetl(fd,300);
if size(A)<>[0,0] then pause, end
mclose(fd)
mdelete('foo.txt')
