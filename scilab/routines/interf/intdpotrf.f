      subroutine intdpotrf(fname)

c     U = dpotrf(A)

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
c     
      minrhs=1
      maxrhs=1
      minlhs=1
      maxlhs=1
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return

      if(.not.getrhsvar(1,'d', M, N, lA)) return
      if(m.ne.n) then
         buf='dgetri'//': the matrix must be square'
         call error(998)
         return
      endif

      call DPOTRF( 'U', N, stk(lA), N, INFO )
c     SUBROUTINE DPOTRF( 'U', N, A, LDA, INFO )
      if(info.ne.0) then
         call errorinfo("dpotrf",info)
         return
      endif

      if( N.gt.1 ) then
         do 20 j=1,N
            do 10 i=j+1,N
               ij = i+(j-1)*N
               stk(lA+ij-1)=0.0d0    
 10         continue
 20      continue
      endif
   
      lhsvar(1)=1
      
c     
      end



