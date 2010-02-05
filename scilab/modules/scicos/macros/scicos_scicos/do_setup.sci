//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//

function wpar=do_setup(wpar)
// set integration parameters
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
  [ok,tf,scale,atol,rtol,ttol,deltat,solver,hmax]=scicos_getvalue('Set parameters',[
      'Final integration time';
      'Realtime scaling';
      'Integrator absolute tolerance';
      'Integrator relative tolerance';
      'Tolerance on time';
      'max integration time interval'
      'solver 0 (CVODE)/100 (IDA)'
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
