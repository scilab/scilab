//clean remaining tasks

//pvm_halt();

ok=pvm_start();
[task_id,numt] = pvm_spawn(SCI+'/tests/basic_tests/pvm_resend.sce',1,'nw');
if numt<>1 then pause,end
if size(task_id,'*')<>1 then pause,end

res=pvm_config();
if type(res)<>15 then pause,end
if size(res)<>7 then pause,end
if res(1)<>1|res(2)<>1 then pause,end
if type(res(3))<>1|size(res(1),'*')<>1 then pause,end
hostid=res(3);
if type(res(4))<>10|type(res(5))<>10 then pause,end
if size(res(4),'*')<>1|size(res(5),'*')<>1 then pause,end
hostname=res(4);
if type(res(6))<>1|type(res(7))<>1 then pause,end
if size(res(6),'*')<>1|size(res(7),'*')<>1 then pause,end
rest=pvm_tasks();
if type(rest)<>15 then pause,end
if size(rest)<>7 then pause,end

if type(rest(1))<>1 then pause,end
n=size(rest(1),'*');
if size(rest(1),'*')<>n then pause,end
if rest(1)($)<>task_id then pause,end

if type(rest(2))<>1 then pause,end
if size(rest(2),'*')<>n then pause,end
if rest(2)($)<>rest(1)($-1) then pause,end

if type(rest(3))<>1 then pause,end
if size(rest(3),'*')<>n then pause,end

if type(rest(4))<>1 then pause,end
if size(rest(4),'*')<>n then pause,end

if type(rest(5))<>10 then pause,end
if size(rest(5),'*')<>n then pause,end
if rest(5)(n)<>'scilab' then pause,end
if or(rest(5)(1:n-1)<>"") then pause,end

if rest(6)<>n then pause,end
if rest(7)<>0 then pause,end

if hostid<>pvm_tidtohost(task_id) then pause,end

err=pvm_addhosts(hostname);
if err>=0 then pause,end
if pvm_error(err)<>'host already configured' then pause,end

if pvm_set_timer()<>0 then pause,end

t=pvm_get_timer();if t<0 then pause,end

inum=pvm_joingroup('test');
if inum<>0 then pause,end

tid=pvm_mytid();
if tid<>rest(2)($) then pause,end
if pvm_getinst('test',tid)<>inum then pause,end
if pvm_error(pvm_parent())<>'no parent task' then pause,end

if pvm_gettid('test',inum)<>tid  then pause,end
if pvm_gsize('test')<>1 then pause,end

if pvm_lvgroup('test')<>0 then pause,end
if pvm_error(pvm_gsize('test'))<>'no group with that name' then pause,end
inum=pvm_joingroup('test');
[buff, info] = pvm_reduce("Max", [1 2 3], 1,'test',0);

// Matrices
a=[];
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end
a=[1 2 3];
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end
a=1;
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end
a=a+%i;
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end
a=rand(7,10);
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end
a=rand(100,10)+%i*rand(100,10);
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end
a=rand(100,200);
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end
// boolean
a=rand(10,100)>0.5;
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end
a=1==1;
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end
// polynomial
a=%s;
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end
a(5,10)=%s+1+%i*%s^3;
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end
a=rand(100,200);a(1)=%s;
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end

// strings
a='';
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end
a=string(rand(5,10));
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end
a=strcat(string(rand(10,10)));
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end
a='1';
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end
a=[],a(40,30)='1';
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end

//sparse
a=sparse(1);
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end
a=sprand(100,100,0.01);
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end
a=sprand(100,99,0.01)+%i*sprand(100,99,0.01);
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end

//boolean sparse
a=a<>0;
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end
a=sparse(1==0);
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end

// lib 
a=autolib;
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end

//functions
a=bode;
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end
deff('x=a(y)','x=y','n')
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end


// lists
a=list();
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end
a=list([]);
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end
a=list(1,2,3);
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end
a=list(rand(10,20),rand(20,10),'s',[]);
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end

// tlist 

a=1/%s;
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end

// mlist 

a=hypermat([1,2,3],rand(1,6));
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a.entries==a1.entries) then pause,end

// lists of lists 
a=list(rand(2,2),list(rand(2,2),[%t,%f]));
a(4)= 10 ;
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end

function y=f(x) 
 y=sin(x)
endfunction 
a(5)=f ;
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end

// stop the slave 
pvm_send(task_id,'exit',0)

[tids, numt] = pvm_spawn_independent(SCI+'/bin/scilex',1);
if numt<>1 then pause,end
if size(task_id,'*')<>1 then pause,end

// halt pvm
pvm_halt( );

// data conversion 

a=[1:10]; 
a_sci=a(1:2:10)+%i*a(2:2:10); 
a_f77=a(1:5)+%i*a(6:10); 

// argument transmited by value 
if or(a_sci<>pvm_f772sci(a(1:5)+%i*a(6:10))) then pause,end
if or(a_f77<>pvm_sci2f77(a(1:2:10)+%i*a(2:2:10))) then pause,end

// reference transmited 
b=a_sci ; 
pvm_sci2f77(b) 
// b now has changed 
if  or(b<>a_f77)  then pause,end

b=a_f77 ; 
pvm_f772sci(b) 
// b now has changed 
if  or(b<>a_sci)  then pause,end

// more complex example 

b =list(a_f77,list(a_f77,a_f77)) ;
c =list(a_sci,list(a_sci,a_sci)) ;
pvm_f772sci(b) 
// b now has changed 
if  or(b<>c)  then pause,end

b =list(a_f77,list(a_f77,a_f77)) ;
c =list(a_sci,list(a_sci,a_sci)) ;
pvm_sci2f77(c) 
// b now has changed 
if  or(b<>c)  then pause,end

