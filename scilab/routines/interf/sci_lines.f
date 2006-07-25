  
      subroutine intlines()
c     Copyright INRIA
      include '../stack.h'
      logical checklhs,checkrhs,getscalar,cremat
      integer topk
      integer iadr
c 
      iadr(l)=l+l-1
c
      rhs=max(0,rhs)
      topk=top
c
      if(.not.checkrhs('lines',0,2)) return
      if(.not.checklhs('lines',1,1)) return
c

      if(rhs.eq.0) then
c     .  get lines properties
         top=top+1
         if(.not.cremat('lines',top,0,1,2,l,lc)) return
         stk(l)=lct(5)
         stk(l+1)=lct(2)
      else
c     .  set lines properties
         if(rhs.eq.2) then
c     .     number of columns
            if(.not.getscalar('lines',topk,top,l)) return
            lct(5)=max(10,int(stk(l)))
            top=top-1
         endif
c     .  number of lines
         if(.not.getscalar('lines',topk,top,l)) return
         lct(2) = max(0,int(stk(l)))

c     .  return a null matrix
         il=iadr(lstk(top))
         istk(il)=0
         lstk(top+1)=lstk(top)+1
      endif
      return
      end
