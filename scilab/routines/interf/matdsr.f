      subroutine matdsr
C
C ====================================================================
C
C     evaluate functions involving eigenvalues and eigenvectors
C
C ====================================================================
C
c     Copyright INRIA
      include '../stack.h'
C
      if (ddt .eq. 4) then
        write (buf(1:4),'(i4)') fin
        call basout(io,wte,' matdsr '//buf(1:4))
      endif
C
C     functions/fin
C        1       2       3       4       5       6      
C     0 hess    schur  spec  bdiag            balanc
C
      if(fin.eq.4) then
         call intbdiagr('bdiag')
         call putlhsvar
      elseif(fin.eq.1) then
c     for compatibility
         call inthess('hess')
         call putlhsvar
      elseif(fin.eq.2) then
c     for compatibility
         call intschur('schur')
         call putlhsvar
      elseif(fin.eq.3) then
c     for compatibility
         call inteig('spec')
         call putlhsvar
      elseif (fin .eq. 6) then
c     for compatibility
         call intbalanc('balanc')
         call putlhsvar
      endif
      return
      end

      subroutine intbdiagr(fname)

c     [Ab [,X [,bs]]]=bdiag(A [,rmax]) 

      include '../stack.h'
      logical getrhsvar,createvar,createcvar,getrhscvar
      logical checklhs,checkrhs,fail
      integer vfinite,iadr

      character fname*(*)
      double precision ZERO,rmax,t
      parameter ( ZERO =0.0D0 )
c
      iadr(l)=l+l-1

      minrhs=1
      maxrhs=2
      minlhs=1
      maxlhs=3
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return 

      if(.not.getrhscvar(1,'d',it, M, N, lA,lAi)) return

      if(M.ne.N) then
         err=1
         call error(20)
         return
      endif

      if(N.eq.0) then
         if(.not.createvar(2,'d', 0, 0, lX)) return
         if(.not.createvar(3,'d', 0, 0, lBs)) return
         lhsvar(1) = 1
         lhsvar(2) = 2
         lhsvar(3) = 3
         return
      endif

      if(vfinite((it+1)*M*N,stk(lA)).eq.0) then
         err=1
         call error(264)
         return
      endif 
      
      if (rhs.eq.2) then
         if(.not.getrhsvar(2,'d', n1, m1, lrmax)) return
         if ( m1*n1 .ne.1 ) then
            err=2 
            call error(204)
            return 
         endif
         rmax=stk(lrmax)
      else
         rmax = 1.0d+0
         lj = lA - 1
         do 03 j = 1,n
            t = 0.0d+0
            do 04 i = 1,n
               t = t + abs(stk(lj+i))
 04         continue
            if (t .gt. rmax) rmax = t
            lj = lj + n
 03      continue
      endif

      if(.not.createcvar(2,'d',it, N, N, lXr,lXi)) return
      if(.not.createvar(3,'d', 1, 2*N, lE)) return
      if(.not.createvar(4,'i', 1, N, lIB)) return
      if(.not.createvar(5,'d', 1, N, lW)) return
      if (it.eq.0) then
c     subroutine bdiag(lda,n,a,epsshr,rmax,er,ei,bs,x,xi,scale,job,fail)
         call bdiag(N,N,stk(lA),0.d0,rmax,stk(lE),stk(lE+N),istk(lIB),
     &        stk(lXr),stk(lXi),stk(lW),0,fail)
      else
         call wbdiag(n,n,stk(lA),stk(lA+N*N),rmax,stk(lE),stk(lE+N),
     &        istk(lIB),stk(lXr),stk(lXi),t,t,stk(lW),0,fail)
      endif
C     
      if (fail) then
         call msgs(2,0)
         call error(24)
         return
      endif

      if(lhs.eq.3) then
         nbloc = 0
         do 10 k = 1,n
            if (istk(lib+k-1) .ge. 0) nbloc = nbloc + 1
 10      continue
         if(.not.createvar(6,'d', nbloc,1, lBs)) return
         i=0
         do 11 k = 1,n 
            if (istk(lIB+k-1) .ge. 0) then 
               stk(lBs+i) = istk(lIB+k-1) 
               i = i+1
            endif
 11      continue
      endif

      lhsvar(1)=1
      lhsvar(2)=2
      lhsvar(3)=6
c     
      end
