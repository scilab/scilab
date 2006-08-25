      subroutine intzpotrf(fname)

c     U = chol(A)

      include 'stack.h'
      logical getrhsvar
      logical checklhs,checkrhs
      complex*16 ZERO
      parameter ( ZERO = (0.0d0,0.0d0) )
      character fname*(*)
      
      minrhs=1
      maxrhs=1
      minlhs=1
      maxlhs=1
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return

      if(.not.getrhsvar(1,'z', M, N, lA)) return
      if(m.ne.n) then
         err=1
         call error(20)
         return
      endif
      if (N.eq.0) then
         lhsvar(1)=1
         return
      elseif(N.eq.-1) then
         if(stk(lA).le.0.0d0) then
            call error(29)
            return
         endif
         lhsvar(1)=1
         stk(lA)=sqrt(stk(lA))
         return
      endif

      call ZPOTRF( 'U', N, zstk(lA), N, INFO )
c     SUBROUTINE ZPOTRF( 'U', N, A, LDA, INFO )
      if(info.ne.0) then
         if(info.gt.0)  call error(29)
         return
      endif

      if( N.gt.1 ) then
         do 20 j=1,N
            do 10 i=j+1,N
               ij = i+(j-1)*N
               zstk(lA+ij-1)=ZERO    
 10         continue
 20      continue
      endif
      
      lhsvar(1)=1
      
c     
      end
