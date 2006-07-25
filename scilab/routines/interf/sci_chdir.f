      subroutine intschdir(fname)
      character*(*) fname
      logical createvar,  putlhsvar
      logical checkrhs,checklhs,getrhsvar
      include '../stack.h'
      nbvars=0
      if(.not.checkrhs(fname,0,1)) return
      if(.not.checklhs(fname,1,1)) return
      if(rhs.eq.0) then
         if(.not.createvar(1,'i',1,1,l2)) return
         istk(l2)=0
         lhsvar(1)=1
         if(.not.putlhsvar()) return
         return 
      endif
 
      if(.not.getrhsvar(1,'c',m1,n1,lr)) return 
      call cluni0(cstk(lr:lr+m1*n1), buf,lp)
      buf(lp+1:lp+1)=char(0)
      if(.not.createvar(2,'i',1,1,l2)) return
      call scichdir(buf(1:lp+1),istk(l2))
      if(istk(l2) .gt. 0) then 
         buf = fname // ': Internal Error' 
         call error(998)
         return
      endif
      lhsvar(1)=2
      if(.not.putlhsvar()) return
      return 
      end
