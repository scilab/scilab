c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c ...
c
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
      subroutine lsdisc (f, neq, y, t, tout, rwork,lrw,istate )
      external f
      integer neq,  lrw
      double precision y(neq), t, tout,  rwork(lrw)
c!purpose
c
c  Simulation of non linear recurrence equations of type
c  x[k+1]=f(k,xk)
c
c!summary of usage.
c a. first provide a subroutine of the form..
c               subroutine f (neq, t, y, ydot)
c               dimension y(neq), ydot(neq)
c which supplies the vector function f by loading ydot(i) with f(i).
c
c f      = name of subroutine for right-hand side vector f.
c          this name must be declared external in calling program.
c neq    = number of first order ode-s.
c y      = array of initial values, of length neq.
c t      = the initial value of the independent variable.
c tout   = first point where output is desired
c istate = 2  if lsdisc was successful, negative otherwise.
C
C # cases according to the difference between t and tout
C
c-----------------------------------------------------------------------
      integer  it,itout
      double precision tt
c
      it=int(t)
      itout=int(tout)
      ierror=0
      if ( itout.lt.it) then
         istate=-3
         return
      else if ( itout.eq.it) then
         istate=2
         return
      else
         do 10 j=it,itout-1
            tt=dble(j)
            call f (neq,tt, y, rwork)
            if(ierror.gt.0) then
               istate=-4
               return
            endif
            call dcopy(neq,rwork,1,y,1)
 10      continue
         t=tout
         istate=2
         return
      endif
      end
