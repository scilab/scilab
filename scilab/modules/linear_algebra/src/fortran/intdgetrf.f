
      subroutine intdgetrf(fname)

c     [L,U,E] = lu(A)

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision ZERO, ONE
      parameter ( ZERO = 0.0D0, ONE = 1.0D0 )
c     
      minrhs=1
      maxrhs=1
      minlhs=2
      maxlhs=3
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return

      if(.not.getrhsvar(1,'d', M, N, lA)) return
      if(M.eq.0 .or.N.eq.0) then
         lhsvar(1) = 1
         if(.not.createvar(2,'d',0,0,lL)) return
         lhsvar(2) = 2
         if(lhs.eq.3) then
            if(.not.createvar(3,'d',0,0,lE)) return
            lhsvar(3) = 3
         endif
         return
      elseif (M.eq.-1 .or.N.eq.-1) then
         lhsvar(1) = 1
         if(.not.createvar(2,'d',-1,-1,lL)) return
         stk(lL)=stk(lA)
         stk(lA)=1.0d0
         lhsvar(2) = 2
         if(lhs.eq.3) then
            if(.not.createvar(3,'d',-1,-1,lE)) return
            stk(lE)=1.0d0
            lhsvar(3) = 3
         endif
         return
      endif
      if(.not.createvar(2,'d',M,min(M,N),lL)) return
      if(.not.createvar(3,'d',min(M,N),N,lU)) return
      if(.not.createvar(4,'i',1,min(M,N),lIPIV)) return
      if(.not.createvar(5,'d',M,M,lE)) return
      if(lhs.eq.2) then
         if(.not.createvar(6,'i',1,M,lIW)) return
         if(.not.createvar(7,'d',M,min(M,N),lW)) return
      endif


      call DGETRF( M, N, stk(lA), M, istk(lIPIV), INFO )
c     SUBROUTINE DGETRF( M, N, A, LDA, IPIV, INFO )
      if(info.lt.0) then
         return
      endif
      
      do 20 j=1,min(M,N)
         do 10 i=1,M
            ij = i+(j-1)*M
            if(i.eq.j) then
               stk(lL+ij-1)=ONE
            else if(i.gt.j) then
               stk(lL+ij-1)=stk(lA+ij-1)
            else if(i.lt.j) then
               stk(lL+ij-1)=ZERO
            endif
 10      continue
 20   continue     
      do 40 j=1,N
         do 30 i=1,min(M,N)
            ij1 = i+(j-1)*min(M,N)
            ij2 = i+(j-1)*M
            if(i.gt.j) then
               stk(lU+ij1-1)=ZERO
            else if(i.le.j) then
               stk(lU+ij1-1)=stk(lA+ij2-1)
            endif
 30      continue
 40   continue
      if(lhs.eq.2) then
c     form E'*L
         do 41 i = 1, M
            istk(lIW-1+i)=i
 41      continue
         do 42 i = 1, min(M,N)
            ip = istk(lIPIV-1+i)
            if( ip.ne.i ) then
               iw=istk(lIw-1+i)
               istk(lIW-1+i)=istk(lIW-1+ip)
               istk(lIW-1+ip)=iw
            endif
 42      continue
         do 43 i= 1, M
            ip =istk(lIW-1+i)
            call dcopy(N,stk(lL-1+i),M,stk(lW-1+ip),M)
 43      continue
         lhsvar(1)=7
         lhsvar(2)=3
      else
         call DLASET( 'F', M, M, ZERO, ONE, stk(lE), M ) 
c     SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )
         call DLASWP( M, stk(lE), M, 1, min(M,N), istk(lIPIV), 1 )
c     SUBROUTINE DLASWP( N, A, LDA, K1, K2, IPIV, INCX )   
         lhsvar(1)=2
         lhsvar(2)=3
         lhsvar(3)=5
      endif   


      
c     
      return     
      end

