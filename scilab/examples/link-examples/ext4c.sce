//Copyright INRIA

files=G_make(['/tmp/ext4c.o'],'ext4c.dll');
link(files,'ext4c','C');

a=[1,2,3];b=[4,5,6];n=3;YesOrNo='yes';
c=call('ext4c',n,1,'i',a,2,'d',b,3,'d','out',[1,3],4,'d');
if norm(c-(sin(a)+cos(b))) > %eps then pause,end
YesOrNo='no';
c=call('ext4c',n,1,'i',a,2,'d',b,3,'d','out',[1,3],4,'d');
if norm(c-(a+b)) > %eps then pause,end
//clear yes  --> undefined variable : yes

