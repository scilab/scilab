c     -------------------------------------------------------
c     The two function provided here can be used
c     with the scilab call 
c     impl([1;0;0],[-0.04;0.04;0],0,0.4,'resid','aplusp')
c     dgbyby is added if one want to use the jacobian.
c     
c     Example of external function for impl 
c     You can add in this file 
C     other function for impl don't forget to add their names 
c     in Flist
c     -------------------------------------------------------

c     Copyright INRIA
      subroutine resid(neq, t, y, s, r, ires)
c     ----------------------------------------
c     this subroutine must calculate vector r=g(t,y)-a(t,y)*s
c     inputs : t    time
c          y    y at time t
c          s    dy/dt at time t
c          ny   size of y
c          ires can be ignored
      double precision r, s, t, y
      dimension y(3), s(3), r(3)
      r(1) = -.040d+0*y(1) + 1.0d+4*y(2)*y(3) - s(1)
      r(2) =  .040d+0*y(1) - 1.0d+4*y(2)*y(3) - 3.0d+7*y(2)*y(2) - s(2)
      r(3) = y(1) + y(2) + y(3) - 1.0d+0
      return
      end

      subroutine aplusp(neq, t, y, ml, mu, p, nrowp)
c     ----------------------------------------------
c     example of routine called by impl
c     this routine makes p=p+a, where a=a(t,y) is a ny x ny matrix
      double precision p, t, y
      dimension y(3), p(nrowp,3)
      p(1,1) = p(1,1) + 1.0d+0
      p(2,2) = p(2,2) + 1.0d+0
      return
      end

      subroutine dgbydy(neq, t, y, s, ml, mu, p, nrowp)
c     ----------------------------------------------
c     Jacobian function for impl 

      double precision s, t, p, y
      dimension y(3), s(3), p(nrowp,3)
      p(1,1) = -.040d+0
      p(1,2) = 1.0d+4*y(3)
      p(1,3) = 1.0d+4*y(2)
      p(2,1) = .040d+0
      p(2,2) = -1.0d+4*y(3) - 6.0d+7*y(2)
      p(2,3) = -1.0d+4*y(2)
      p(3,1) = 1.0d+0
      p(3,2) = 1.0d+0
      p(3,3) = 1.0d+0
      return
      end

