mode(-1)
// Copyright INRIA
mes=x_message(['Start pvm daemon if not already active'
           'ok=pvm_start()'
	   'if ok<>0 then disp(''pvm daemon already active''),end'],..
           ['Ok','Cancel'])
if mes==2 then return, end
ok=pvm_start()       
if ok<>0 then disp('pvm daemon already active'),end;

mes=x_message(['Start another Scilab process under PVM with the following instruction:';
           ' '
	   '[task_id,numt] = pvm_spawn(SCI+''/demos/pvm/script.sce'',1)'
	   ' '
	   'SCI+''/demos/pvm/script.sce'' file is: '
	   '======================================='
	   read(SCI+'/demos/pvm/script.sce',-1,1,'(a)')],..
           ['Ok','Cancel'])
if mes==2 then return, end   

[task_id,numt] = pvm_spawn(SCI+'/demos/pvm/script.sce',1)
if numt<0 then
  x_message(['pvm_spawn aborts to create a new process'])
  return
end
mes=x_message(['Send a simple instruction to the second Scilab process:'
	   ' '
	   'pvm_send(task_id,''disp(''''Hello, please help me to do my work'''')'',0)'  
	   ],['Ok','Cancel'])
       
if mes==2 then
  pvm_kill(task_id);
  return, 
end
       
pvm_send(task_id,'disp(''Hello, please help me to do my work'')',0)
mes=x_message(['Create a matrix A and send it to the second Scilab process:';
	   ' '
	   'A=rand(100,100);'
	   'pvm_send(task_id,A,0);pvm_send(task_id,''A=%var'',0)'],['Ok','Cancel'])
if mes==2 then
  pvm_kill(task_id);
  return, 
end
a=rand(100,100);
pvm_send(task_id,a,0);
pvm_send(task_id,'A=%var',0);
mes=x_message(['Ask second process to compute the eigenvalue of A:'
           ' ';
	   'pvm_send(task_id,''v=spec(A)'',0);'],['Ok','Cancel'])
if mes==2 then
  pvm_kill(task_id);
  return, 
end
pvm_send(task_id,'v=spec(A)',0);
mes=x_message(['Ask second process to send back the eigenvalue of A:'
           ' '
	   'pvm_send(task_id,''pvm_send(pvm_parent(),v,0)'',0);'],['Ok','Cancel'])

if mes==2 then
  pvm_kill(task_id);
  return, 
end
pvm_send(task_id,'pvm_send(pvm_parent(),v,0)',0);
mes=x_message(['Receive the computed eigenvalues:';
           ' ';
	   'v=pvm_recv(task_id,0);disp(v)'],['Ok','Cancel'])
if mes==2 then
  pvm_kill(task_id);
  return, 
end
v=pvm_recv(task_id,0);disp(v)
pvm_send(task_id,'exit',0)
x_message(['Job finished'])
pvm_halt()
//pvm_kill(task_id)
