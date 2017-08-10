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
c
c     Examples for dassl system and jacobian 
c     --------------------------------------
c
      subroutine res1(t,y,ydot,delta,ires,rpar,ipar)
      implicit double precision (a-h,o-z)
      dimension y(*), ydot(*), delta(*),rpar(*)
      neq=1
c
c     check y to make sure that it is valid input.
c     if y is less than or equal to zero, this is invalid input.
c
      if (y(1) .le. 0.0d0) then
         ires = -1
      else
c
c        call f to obtain f(t,y)
c
         call f1(neq,t,y,delta)
c
c        form f = f'-f(t,y)
c
         do 10 i = 1,neq
            delta(i) = ydot(i) - delta(i)
 10      continue
      endif
c
      return
      end
c
      subroutine f1 (neq, t, y, ydot)
      integer neq
      double precision t, y, ydot
      dimension y(*), ydot(*)
      ydot(1) = ((2.0d0*log(y(1)) + 8.0d0)/t - 5.0d0)*y(1)
      return
      end
c

      subroutine res2(t,y,ydot,delta,ires,rpar,ipar)
      implicit double precision (a-h,o-z)
      integer neq
      dimension y(*), ydot(*), delta(*)
      neq=2
c
c     call f to obtain f(t,y)
c
      call f2(neq,t,y,delta)
c
c     form f = f'-f(t,y)
c
      do 10 i = 1,neq
         delta(i) = ydot(i) - delta(i)
 10   continue
c
      return
      end
c
      subroutine f2 (neq, t, y, ydot)
      implicit double precision (a-h,o-z)
      integer neq
      double precision t, y, ydot
      dimension y(*), ydot(*)
      ydot(1) = y(2)
      ydot(2) = 100.0d0*(1.0d0 - y(1)*y(1))*y(2) - y(1)
      return
      end

      subroutine dres1(t,y,ydot,res,ires,rpar,ipar)
      implicit double precision(a-h,o-z)
      dimension y(*),ydot(*),res(*),rpar(*)
      res(1) = ydot(1) + 10.0d0*y(1)
      res(2) = y(2) + y(1) - 1.0d0
      return
      end


      subroutine dres2(t,y,ydot,res,ires,rpar,ipar)
      implicit double precision(a-h,o-z)
      dimension y(*),ydot(*),res(*),rpar(*)
      data alph1/1.0d0/, alph2/1.0d0/, ng/5/
      do 10 j = 1,ng
      do 10 i = 1,ng
        k = i + (j - 1)*ng
        d = -2.0d0*y(k)
        if (i .ne. 1) d = d + y(k-1)*alph1
        if (j .ne. 1) d = d + y(k-ng)*alph2
 10     res(k) = d - ydot(k)
      return
      end


C     Jacobian part 
C     --------------------------------------
      

      subroutine jac2 (t, y, ydot, pd, cj, rpar, ipar)
      implicit double precision (a-h,o-z)
      integer  nrowpd
      double precision t, y, pd
      parameter (nrowpd=2)
      dimension y(2), pd(nrowpd,2)
c
c first define the jacobian matrix for the right hand side
c of the ode: f' = f(t,y) , i.e. df/dy)
c
      pd(1,1) = 0.0d0
      pd(1,2) = 1.0d0
      pd(2,1) = -200.0d0*y(1)*y(2) - 1.0d0
      pd(2,2) = 100.0d0*(1.0d0 - y(1)*y(1))
c
c next update the jacobian with the right hand side to form the
c dae jacobian: d(f'-f)/dy = df'/dy - df/dy = i - df/dy
c
      pd(1,1) = cj - pd(1,1)
      pd(1,2) =    - pd(1,2)
      pd(2,1) =    - pd(2,1)
      pd(2,2) = cj - pd(2,2)
c
      return
      end


      subroutine djac1(t,y,yprime,pd,cj,rpar,ipar)
      implicit double precision(a-h,o-z)
      dimension y(*),yprime(*),pd(2,2)
      pd(1,1) = cj + 10.0d0
      pd(1,2) = 0.0d0
      pd(2,1) = 1.0d0
      pd(2,2) = 1.0d0
      return
      end

      subroutine djac2(t,y,yprime,pd,cj,rpar,ipar)
      implicit double precision(a-h,o-z)
      dimension y(*), pd(11,*), yprime(*),rpar(*)
      data alph1/1.0d0/, alph2/1.0d0/, ng/5/
      data ml/5/, mu/0/, neq/25/
      mband = ml + mu + 1
      mbandp1 = mband + 1
      mbandp2 = mband + 2
      mbandp3 = mband + 3
      mbandp4 = mband + 4
      mbandp5 = mband + 5
      do 10 j = 1,neq
        pd(mband,j) = -2.0d0 - cj
        pd(mbandp1,j) = alph1
        pd(mbandp2,j) = 0.0d0
        pd(mbandp3,j) = 0.0d0
        pd(mbandp4,j) = 0.0d0
 10     pd(mbandp5,j) = alph2
      do 20 j = 1,neq,ng
 20     pd(mbandp1,j) = 0.0d0
      return
      end











