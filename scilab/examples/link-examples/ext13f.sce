//Copyright INRIA

//sharing common data
files=G_make(['/tmp/ext13f.o'],'ext13f.dll');
link(files,['ext13if','ext13of']);  //Must be linked together

a=1:10;
n=10;a=1:10;
call('ext13if',n,1,'i',a,2,'r','out',2);  //loads b with a
c=call('ext13of',n,1,'i','out',[1,10],2,'r');  //loads c with b
if norm(c-a) > %eps then pause,end




