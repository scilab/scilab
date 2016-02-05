c     ====================================
      subroutine rkqc(y,dydx,n,x,htry,eps,yscal,hdid,hnext,derivs,rwork)
C      implicit undefined (a-z)
c ................................................................
c .   Subroutine rkqc is a fifth-order Runge-Kutta step with
c .   monitoring of local truncation error to ensure accuracy
c .   and adjust stepsize.
c .
c .   Inputs:
c .   y(1:n) = the dependent variable vector
c .   dydx(1:n) = deriv of y wrt x at starting value of indep var x
c .   htry = attempted step size
c .   eps = required accuracy
c .   yscal(1:n) = scaling vector for the error
c .
c .   Outputs:
c .   y = new value of y
c .   x = new value of x
c .   hdid = actual stepsize accomplished
c .   hnext = estimated next stepsize
c .
c .   derivs is the user-supplied subroutine that computes the
c .    right-hand side derivatives
c ................................................................
c
c     The original version has been modified to replace statically
c     allocated arrays dysav, ytemp and ysav by rwork arguments parts
c     array + blas use. Serge Steer INRIA- feb 2012
      integer n,i
      double precision fcor,one,safety,errcon
      parameter (fcor=.0666666667,one=1.0,safety=0.9,errcon=6.e-4)
      double precision x,htry,eps,hdid,hnext,pgrow,pshrnk,xsav,hh
      double precision errmax,h
      double precision y(n),dydx(n),yscal(n)
      double precision rwork(*)

      external derivs

      lysav=1
      ldysav=lysav+n
      lytemp=ldysav+n
      lwork=lytemp+n

      ierror=0
      pgrow=-0.20d0
      pshrnk=-0.25d0
      xsav=x

      call dcopy(n,y,1,rwork(lysav),1)
      call dcopy(n,dydx,1,rwork(ldysav),1)
      h=htry
1     hh=0.5*h
      call rk4(rwork(lysav),rwork(ldysav),n,xsav,hh,rwork(lytemp),
     $     derivs,rwork(lwork))
      if (ierror.gt.0) return
      x=xsav+hh
      call derivs(n,x,rwork(lytemp),dydx)
      if (ierror.gt.0) return
      call rk4(rwork(lytemp),dydx,n,x,hh,y,derivs,rwork(lwork))
      x=xsav+h
      if(x.eq.xsav) then
         ierror=1
         return
      endif
      call rk4(rwork(lysav),rwork(ldysav),n,xsav,h,rwork(lytemp),
     $     derivs,rwork(lwork))
      if (ierror.gt.0) return
      errmax=0.0d0
       do 12 i=0,n-1
        rwork(lytemp+i)=y(i+1)-rwork(lytemp+i)
        errmax=max(errmax,abs(rwork(lytemp+i)/(yscal(i+1)*eps)))
12    continue

      if(errmax.gt.one) then
        h=safety*h*(errmax**pshrnk)
        goto 1
      else
        hdid=h
        if(errmax.gt.errcon) then
          hnext=safety*h*(errmax**pgrow)
        else
          hnext=4.0d0*h
        endif
      endif
      call daxpy(n,fcor,rwork(lytemp),1,y,1)

      return
      end
c     ====================================
