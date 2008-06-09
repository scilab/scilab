//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

mode(-1)
oldln=lines();
lines(0)
deff('[]=demoex(num)','exec(''SCI/modules/others/demos/simulation/ode/''+demolist(num,2),-1)')

demolist=[
'Simple ODE','ode1.dem';
'Chemical process (Stiff)','ode2.dem';
'Chemical process (Implicit)','ode3.dem';
'Lorenz equation','ode4.dem'
'ODE 1D vector field','odenew1_1.sce'
'Van der Pol vector field','vanderpol.sce'
'Lotka-Volterra vector field','lotka.sce']

while %t then
  num=x_choose(demolist(:,1),'Click to choose a demo')
  if num==0 then 
     lines(oldln(1))
     return
  else
     demoex(num)
  end
end


