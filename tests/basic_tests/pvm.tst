ok=pvm_start();
[task_id,numt] = pvm_spawn(SCI+'/tests/pvm_resend.sce',1);
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
// lists
a=list();
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end
a=list([]);
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end
a=list(1,2,3);
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end
a=list(rand(10,20),rand(20,10),'s',[]);
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end
a=1/%s;
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

//a=autolib;
//pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end
a=bode;
//functions
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end
deff('x=a(y)','x=y','n')
pvm_send(task_id,a,0);a1=pvm_recv(task_id,0);if ~and(a==a1) then pause,end
pvm_send(task_id,'exit',0)
pvm_halt( );
//pvm_kill(task_id)
