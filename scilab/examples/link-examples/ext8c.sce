//Copyright INRIA

//same example as # 10 with call to matptr
//param must be defined as a scilab variable

files=G_make(['/tmp/ext8c.o'],'ext8c.dll');
link(files,'ext8c','C');

param=[1,2,3]
y=call('ext8c','out',size(param),1,'d');
if norm(y-param) > %eps then pause,end
if norm(param(1)-18.0) > %eps then pause,end

