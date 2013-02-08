c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c ...
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c
c     Examples for daskr
c     ----------------------
c
c
      subroutine pjac1(res, ires, neq, t, y, ydot, rewt, savr,
     *                   wk, h, cj, wp, iwp, ier, rpar, ipar)
      double precision t, y(*), ydot(*), rewt(*), savr(*),
     *                  wk(*), h, cj, wp(*), rpar(*)
      double precision ysave, ypsave, e(neq), del, delinv
      external res
      integer ires, neq, iwp(*), ier, ipar(*)
      ier = -1
      uround = D1MACH (4)
c     Compute the preconditioner P = dG/dy + cj*dG/dydot, LU-decompose it
c and stock it in wp and iwp
      nrow = 0
      ires = 0
      SQuround = sqrt(uround)
      do 1 i=1,neq
	 del = max (SQuround*max(abs(y(i)),abs(h*ydot(i))), 1.0D0/rewt(i))
	 del = sign(del, h*ydot(i))
	 ysave   =  y(i)
	 ypsave  =  ydot(i)
	 y(i)    = y(i) + del
	 ydot(i) = ydot(i) + cj*del
	 call res(t,y,ydot,e,ires,rpar,ipar)
      if (ires.lt.0) return
	 delinv = 1.0D0/del
	 do 2 j=1,neq
		 wp(nrow+j) = (e(j)-savr(j))*delinv
2      continue
	 nrow = nrow + neq
	 y(i)    =  ysave
	 ydot(i) =  ypsave
	 iwp(i) = i
	 iwp(i+neq) = i
1      continue
c     Do dense-matrix LU decomposition on P
      lipvt = iwp(30)
      call dgefa(wp, neq , neq, iwp(lipvt), ier)
      return
      end

      subroutine psol1(neq, t, y, ydot, savr, wk, cj, wght,
     *                   wp, iwp, b, eplin, ier, rpar, ipar)
      double precision t, y(*), ydot(*), savr(*), wk(*), cj, wght(*),
     *                  wp(*), b(*), eplin, rpar(*)
      integer neq, iwp(*), ier, ipar(*)
      integer ipiv(neq)
      nColB = 1
      info = 0
c     b = P\b
c     Call DGESV to solve the system Px = b, where P is stored in
c wp in compact row format
      call dgesv (neq, nColB, wp, neq, ipiv, b, neq, info)
      if (info.ne.0) then
          ier = -1
          return
      endif
      ier = 0
      return
      end

