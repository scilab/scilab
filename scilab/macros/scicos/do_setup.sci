function wpar=do_setup(wpar)
// set integration parameters
// Copyright INRIA
if wpar(4)==[] then wpar(4)=100000;end
<<<<<<< do_setup.sci
if size(wpar(3), '*') < 4 then wpar(3)=[1.d-4;1.d-6;1.d-10;wpar(4)+1;0];end
if size(wpar(3), '*') < 5 then wpar(3)(5)=0;end
if size(wpar(3), '*') < 6 then wpar(3)(6)=0;end
tolerances=wpar(3);
tf=wpar(4)
atol=tolerances(1);rtol=tolerances(2);ttol=tolerances(3);
deltat=tolerances(4)
scale=tolerances(5);
solver=tolerances(6)

while %t do
  [ok,tf,scale,atol,rtol,ttol,deltat,solver]=getvalue('Set parameters',[
      'Final integration time';
      'Realtime scaling';
      'Integrator absolute tolerance';
      'Integrator relative tolerance';
      'Tolerance on time';
      'max time step for integration'
      'solver 0/100'],..
      list('vec',1,'vec',1,'vec',1,'vec',1,'vec',1,'vec',1,'vec',1),..
      [string([tf;scale;atol;rtol;ttol;deltat;solver])])
  if ~ok then break,end
  if or([tf,atol,rtol,ttol,deltat]<=0) then
    message('Parameter must  be positive')
  else
    wpar(3)=[atol;rtol;ttol;deltat;scale;solver]
    wpar(4)=tf
    break
  end
end
endfunction
