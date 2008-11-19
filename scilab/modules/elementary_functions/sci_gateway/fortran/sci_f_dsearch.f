c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txtc     -------------------------------
c
      subroutine intdsearch(id)
*
*     interface for dsearch (Bruno le 10/12/2001)
*
*       [ind , occ, info] = dsearch(X, val [, ch])
*        
*       X and val must be real vectors (says of length m for X and n for val ), 
*       if ch is not present then ch = 'c'  (dsearch on "intervals")
*       ch must be 'd' or 'c'
*
*       ind is a vector with the same format than X
*       occ is a vector with the same format than val (but with n-1
*           components in the case ch='c')
*       info is a scalar
*
      implicit none

      INCLUDE 'stack.h'

      integer id(nsiz)

c     EXTERNAL SUBROUTINES
      external  dsearchc, dsearchd

c     EXTERNAL API FUNCTIONS
      logical  checkrhs, checklhs, getsmat, getrvect, cremat, getrmat
      external checkrhs, checklhs, getsmat, getrvect, cremat, getrmat

c     LOCAL VAR
      integer topk, topl
      integer mX, nX, lX, mval, nval, lval, mch, nch, lch, nlch
      integer lind, mocc, nocc, locc, linfo, lc, j
      character*1 ch
      character*9 fname

c     STATEMENT FUNC
      integer l, iadr
      iadr(l)=l+l-1

c     TEXT
      fname = 'dsearch'
      topk=top
      rhs=max(0,rhs)

      if (.not.checkrhs(fname,2,3)) return
      if (.not.checklhs(fname,1,3)) return

*     get ch
      if (rhs .eq. 3) then
         if( .not. getsmat(fname,topk,top,mch,nch,1,1,lch,nlch)) return
         top = top - 1
         call cvstr(1,istk(lch),ch,1)
      else
         ch = 'c'
      endif
      if (ch.ne.'c' .and. ch.ne.'d') then
         buf=fname//' : unknown char specifier (must be ''c'' or ''d'')'
         call error(999)
         return
      endif

c     get val 
      if( .not. getrvect(fname, topk, top, mval, nval, lval) ) return
      if (ch.eq.'d') then
         if (mval*nval.lt.1) then
            buf=fname//' : argument 2 must not be an empty vector'
            call error(999)
            return
         endif
         mocc = mval
         nocc = nval
      else    ! case ch='c'
         if (mval*nval.lt.2) then
            buf=fname//' : in the interval case, argument 2 must be'
     $               //' a vector with length > 1'
            call error(999)
            return
         endif
         if (mval .eq. 1) then 
            mocc = 1
            nocc = nval - 1
         else
            mocc = mval - 1
            nocc = nval
         endif
      endif
*     verif that val is in strict increasing order
      do j = 1, mval*nval-1
         if (.not. stk(lval+j-1) .lt. stk(lval+j)) then  ! cette forme permet de detecter les nans
            buf=fname//' : the array val (arg 2) is not well ordered'
            call error(999)
            return
         endif
      enddo
      top = top - 1
      
c     get X
      if( .not. getrmat(fname, topk, top, mX, nX, lX) ) return

c     reserve space for ind
      if (.not.cremat(fname, topk+1, 0, mX, nX, lind, lc)) return

c     reserve space for occ
      if (.not.cremat(fname, topk+2, 0, mocc, nocc, locc, lc)) return

c     reserve space for info
      if (.not.cremat(fname, topk+3, 0, 1, 1, linfo, lc)) return

      if (mX.eq.0.or.nX.eq.0) then
         stk(linfo)=0
         call dset(mocc*nocc,0.0D0,stk(locc),1)
      else

c     go on for the computation
         if ( ch .eq. 'c') then
            call dsearchc(stk(lX), mX*nX, stk(lval), mval*nval-1,
     $           stk(lind), stk(locc), stk(linfo))
         else 
            call dsearchd(stk(lX), mX*nX, stk(lval), mval*nval, stk(lind
     $           ),stk(locc), stk(linfo))
         endif

c     int2db ... (normalement ca doit passer avec -1 sans copie
C     supplementaire)
         call int2db(mX*nX,     istk(iadr(lind)), -1, stk(lind), -1) 
         call int2db(mocc*nocc, istk(iadr(locc)), -1, stk(locc), -1) 
         call int2db(1,     istk(iadr(linfo)),-1, stk(linfo),-1) 
      endif
*     copie en "haut" 
      topl = topk - rhs
      if(lhs .ge. 1) then
         call copyobj(fname,topk+1,topl+1)
      endif
      if(lhs .ge. 2) then
         call copyobj(fname,topk+2,topl+2)
      endif
      if(lhs .ge. 3) then
         call copyobj(fname,topk+3,topl+3)
      endif
      top=topl+lhs
      return 
      end
c     -------------------------------
