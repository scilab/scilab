      subroutine i_p
      include '../stack.h'
      logical checkrhs,checklhs,crewimat
      integer iadr,topk
c     
      iadr(l)=l+l-1
c
      topk=top+1
      if(.not.checkrhs('%i_p',1,1)) return
      if(.not.checklhs('%i_p',1,1)) return

      lineln=lct(5)
      ndgt=lct(7)

      il=iadr(lstk(top))

      if(istk(il).ne.8) then
         call error(44)
         return
      endif
      m=istk(il+1)
      n=istk(il+2)
      is=istk(il+3)
      
      li=il+4
      if (.not.crewimat("print",topk,1,m*n+2*n,lw)) return
      call genmdsp(is,istk(li),m,m,n,lineln,wte,buf,istk(lw))
      istk(il)=0
      lstk(top+1)=lstk(top)+1
      return
      end
