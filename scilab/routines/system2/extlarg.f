
c     Copyright INRIA
      subroutine extlarg(l,ils,nelt,mrhs)
C     Used in dealing with externals when 
C     extra arguments are given by a list list(external,arg1,...,argn)
C      implicit undefined (a-z) 
      INCLUDE '../stack.h'
      integer l,ils,nelt,vol,mrhs,i
      nelt=nelt-1
      if(nelt.ne.0) then
         l=l+istk(ils+1)-istk(ils)
         vol=istk(ils+nelt+1)-istk(ils+1)
         if(top+1+nelt.ge.bot) then
            call error(18)
            if(err.gt.0) return 
         endif
         err=lstk(top+1)+vol-lstk(bot)
         if(err.gt.0) then
            call error(17)
            if(err.gt.0) return 
         endif
         call unsfdcopy(vol,stk(l),1,stk(lstk(top+1)),1)
         do 11 i=1,nelt
            top=top+1
            lstk(top+1)=lstk(top)+istk(ils+i+1)-istk(ils+i)
 11      continue
         mrhs=mrhs+nelt
      endif
      return 
      end



