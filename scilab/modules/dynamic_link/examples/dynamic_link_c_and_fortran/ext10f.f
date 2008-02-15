
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      
      subroutine ext10f(neq, t, y, ydot)
c     -------------------------------------------
c     exemple with a call to readmat routine
c     -->param=[0.04,10000,3d+7];
c     -->link('ext10f.o','ext10f')
c     -->y=ode([1;0;0],0,[0.4,4],'ext10f')
c     param must be defined as a scilab variable
c     Copyright Inria
      double precision t, y, ydot, param
      logical creadmat
      dimension y(3), ydot(3), param(3)

c     If "param" does not exists return else loads param
      if(.not.creadmat('param'//char(0),m,n,param)) return
c     ***********************************

      ydot(1) = -param(1)*y(1) + param(2)*y(2)*y(3)
      ydot(3) = param(3)*y(2)*y(2)
      ydot(2) = -ydot(1) - ydot(3)
      return
      end
