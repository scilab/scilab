//Copyright INRIA

files=G_make(['/tmp/ext4f.o'],'ext4f.dll');
link(files,'ext4f');

a=[1,2,3];b=[4,5,6];n=3;yes='yes';
c=call('ext4f',n,1,'i',a,2,'d',b,3,'d','out',[1,3],4,'d');
if norm(c-(sin(a)+cos(b))) > %eps then pause,end
yes='no';
c=call('ext4f',n,1,'i',a,2,'d',b,3,'d','out',[1,3],4,'d');
if norm(c-(a+b)) > %eps then pause,end
//clear yes  --> undefined variable : yes

