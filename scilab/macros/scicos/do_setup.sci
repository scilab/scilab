function wpar=do_setup(wpar)
// set integration parameters
// Copyright INRIA
if wpar(4)==[] then wpar(4)=100000;end
if size(wpar(3), 'r') < 4 then wpar(3)=[1.d-4;1.d-6;1.d-10;wpar(4)+1;0];end
if size(wpar(3), 'r') <> 5 then wpar(3)=[wpar(3);0];end
tolerances=wpar(3);
tf=wpar(4)
atol=tolerances(1);rtol=tolerances(2);ttol=tolerances(3);deltat=tolerances(4)
scale=tolerances(5);
while %t do
  [ok,tf,scale,atol,rtol,ttol,deltat]=getvalue('Set parameters',[
      'Final integration time';
      'Realtime scaling';
      'Integrator absolute tolerance';
      'Integrator relative tolerance';
      'Tolerance on time';
      'max time step for integration'],..
      list('vec',1,'vec',1,'vec',1,'vec',1,'vec',1,'vec',1),..
      [string([tf;scale;atol;rtol;ttol;deltat])])
  if ~ok then break,end
  if or([tf,atol,rtol,ttol,deltat]<=0) then
    message('Parameter must  be positive')
  else
    wpar(3)=[atol;rtol;ttol;deltat;scale]
    wpar(4)=tf
    break
  end
end






