// Copyright INRIA

pref='ext';
suf='f';
routines=[pref(ones(1,14))+string(1:14)+suf(ones(1,14))];
ilib_for_link(routines,'externals.o',[],"f");
exec loader.sce ;
//(very) simple example 1
a=[1,2,3];b=[4,5,6];n=3;
c=call('ext1f',n,1,'i',a,2,'d',b,3,'d','out',[1,3],4,'d');
if norm(c-(a+b)) > %eps then pause,end

//Simple example #2
a=[1,2,3];b=[4,5,6];n=3;
c=call('ext2f',n,1,'i',a,2,'d',b,3,'d','out',[1,3],4,'d');
if norm(c-(sin(a)+cos(b))) > %eps then pause,end

//Example #3
a=[1,2,3];b=[4,5,6];n=3;
c=call('ext3f','yes',1,'c',n,2,'i',a,3,'d',b,4,'d','out',[1,3],5,'d');
if norm(c-(sin(a)+cos(b)))> %eps then pause,end
c=call('ext3f','no',1,'c',n,2,'i',a,3,'d',b,4,'d','out',[1,3],5,'d');
if norm(c-(a+b)) > %eps then pause,end

//Example #4 
a=[1,2,3];b=[4,5,6];n=3;yes='yes';
c=call('ext4f',n,1,'i',a,2,'d',b,3,'d','out',[1,3],4,'d');
if norm(c-(sin(a)+cos(b))) > %eps then pause,end
yes='no';
c=call('ext4f',n,1,'i',a,2,'d',b,3,'d','out',[1,3],4,'d');
if norm(c-(a+b)) > %eps then pause,end
//clear yes  --> undefined variable : yes

//Example #5 
// reading vector a in scilab internal stack
a=[1,2,3];b=[2,3,4];
c=call('ext5f',b,1,'d','out',[1,3],2,'d');
if norm(c-(a+2*b)) > %eps then pause,end

//Example #6
//reading  vector with name='a' in scilab internal stack
a=[1,2,3];b=[2,3,4];
c=call('ext6f','a',1,'c',b,2,'d','out',[1,3],3,'d');
if norm(c-(a+2*b)) > %eps then pause,end


//Example #7
//creating vector c in scilab internal stack
clear c;
a=[1,2,3]; b=[2,3,4];
//c does not exist (c made by ext7f)
c1=call('ext7f',a,1,'d',b,2,'d','out',2);
if norm(c1-b) > %eps then pause,end
//c now exists
if norm(c-(a+2*b)) > %eps then pause,end
//d exists 
if d<>"test" then pause,end

//Example #8 
//param must be defined as a scilab variable
param=[1,2,3];
y=call('ext8f','out',size(param),1,'d');
if norm(y-param) > %eps then pause,end

//Example #9
//call ext9f argument function with dynamic link
yref=ode([1;0;0],0,[0.4,4],'ext9f');

//Example #10
//passing a parameter to ext10f routine by a list:
param=[0.04,10000,3d+7];    
y=ode([1;0;0],0,[0.4,4],list('ext10f',param));
if norm(y-yref) > 10000*%eps then pause,end

//Example #11
//Passing a parameter to argument funtion of ode
param=[0.04,10000,3d+7];
y=ode([1;0;0],0,[0.4,4],'ext11f');
//param must be defined as a scilab variable upon calling ode
if norm(y-yref) > 10000*%eps then pause,end

//Example #12 
//same example as # 10 with call to matptr
//param must be defined as a scilab variable
param=[0.04,10000,3d+7];
y=ode([1;0;0],0,[0.4,4],'ext12f');
if norm(y-yref) > 10000*%eps then pause,end

//Example #13
//sharing common data
a=1:10;
n=10;a=1:10;
call('ext13f',n,1,'i',a,2,'r','out',2);  //loads b with a
c=call('ext14f',n,1,'i','out',[1,10],2,'r');  //loads c with b
if norm(c-a) > %eps then pause,end
