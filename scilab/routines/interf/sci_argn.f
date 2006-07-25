      subroutine intargn
c     Copyright INRIA
      include '../stack.h'
      logical checklhs,checkrhs,cremat,getscalar
      integer nlhs,nrhs,p,choix
c 
      rhs=max(0,rhs)
      if(.not.checkrhs('argn',0,1)) return
      if(.not.checklhs('argn',1,2)) return
c
      if(rhs.eq.1)  then 
         if(.not.getscalar('argn',top,top,lr)) return

         choix=stk(lr)
         if(choix.lt.0.or.choix.gt.2) then
            err=1
            call error(36)
            return
         endif
         top=top-1
      else
         choix=0
      endif
      if(choix.ne.0.and.lhs.eq.2) then
         call error(78)
         return
      endif

c     computes then actual number of lhs ans rhs
      if(macr.eq.0) then
         nlhs=0
         nrhs=0
      else
         p=pt+1
 10      p=p-1
         if(rstk(p).ne.501.and.rstk(p).ne.502) goto 10
         nlhs=ids(2,p)
         nrhs=max(ids(1,p),0)
      endif

      if (choix.ne.2) then
c     number of lhs 
         top=top+1
         if(.not.cremat('argn',top,0,1,1,l,lc)) return
         stk(l)=nlhs
      else
         top=top+1
         if(.not.cremat('argn',top,0,1,1,l,lc)) return
         stk(l)=nrhs
      endif

      if(lhs.eq.1) return

c     number of rhs
      top=top+1
      if(.not.cremat('argn',top,0,1,1,l,lc)) return
      stk(l)=nrhs

      return
      end