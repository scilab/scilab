function wpar=do_setup(wpar)
// set integration parameters
// Copyright INRIA
tolerances=wpar.tol;
tf=wpar.tf
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
      'solver 0(lsodar)/100(dasrt)'],..
      list('vec',1,'vec',1,'vec',1,'vec',1,'vec',1,'vec',1,'vec',1),..
      [string([tf;scale;atol;rtol;ttol;deltat;solver])])
  if ~ok then break,end
  if or([tf,atol,rtol,ttol,deltat]<=0) then
    message('Parameter must  be positive')
  else
    wpar.tol=[atol;rtol;ttol;deltat;scale;solver]
    wpar.tf=tf
    break
  end
end
endfunction
