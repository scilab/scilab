function wpar=do_setup(wpar)
// set integration parameters
// Copyright INRIA
tolerances=wpar.tol;
tf=wpar.tf
atol=tolerances(1);rtol=tolerances(2);ttol=tolerances(3);
deltat=tolerances(4)
scale=tolerances(5);
solver=tolerances(6)
if length(tolerances)<7 then
  hmax=0
else
  hmax=tolerances(7)
end

while %t do
  [ok,tf,scale,atol,rtol,ttol,deltat,solver,hmax]=getvalue('Set parameters',[
      'Final integration time';
      'Realtime scaling';
      'Integrator absolute tolerance';
      'Integrator relative tolerance';
      'Tolerance on time';
      'max integration time interval'
      'solver 0(lsodar)/100(dasrt)'
      'maximum step size (0 means no limit)'],..
      list('vec',1,'vec',1,'vec',1,'vec',1,'vec',1,'vec',1,'vec',1,'vec',1),..
      [string([tf;scale;atol;rtol;ttol;deltat;solver;hmax])])
  if ~ok then break,end
  if or([tf,atol,rtol,ttol,deltat]<=0) then
    message('Parameter must  be positive')
  else
    wpar.tol=[atol;rtol;ttol;deltat;scale;solver;hmax]
    wpar.tf=tf
    break
  end
end
endfunction
