c     ********************
c     Copyright INRIA
c     ********************
      subroutine intctree
c
c
      include 'stack.h'
      integer iadr, sadr
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      lw = lstk(top+1)
      ilw=iadr(lw)
      top=top-rhs+1
      itop=top
      ilv=iadr(lstk(top))
      lv=sadr(ilv+4)
      nb=istk(ilv+1)*istk(ilv+2)
      call entier(nb,stk(lv),istk(iadr(lv)))
      top=top+1
      iln=iadr(lstk(top))
      ln=sadr(iln+4)
      nnb=istk(iln+1)*istk(iln+2)
      call entier(nnb,stk(ln),istk(iadr(ln)))
      top=top+1
      ild=iadr(lstk(top))
      ld=sadr(ild+4)
      nnb=istk(ild+1)*istk(ild+2) 
      call entier(nnb,stk(ld),istk(iadr(ld)))
      top=top+1
      ilo=iadr(lstk(top))
      lo=sadr(ilo+4)
      nnb=istk(ilo+1)*istk(ilo+2) 
      call entier(nnb,stk(lo),istk(iadr(lo)))
      top=top+1
      ilc=iadr(lstk(top))
      lc=sadr(ilc+4)
      nnb=istk(ilc+1)*istk(ilc+2) 
      call entier(nnb,stk(lc),istk(iadr(lc)))
      ilord=ilw
      ilw=ilw+nb
      lw = sadr(ilw + nb)
      err = lw - lstk(bot )
      if (err .gt. 0) then
         call error(17 )
         return
      endif
      call sctree(nb,istk(iadr(lv)),istk(iadr(ln)),
     &     istk(iadr(ld)),istk(iadr(lo)),
     &     istk(iadr(lc)),istk(ilord),nord,iok,istk(ilw))
      top=itop
      istk(ilv+1)=nord
      istk(ilv+2)=1
      call int2db(nord,istk(ilord),1,stk(lv),1)
      lstk(top+1)=lv+nord
      top=top+1
      ilv1=iadr(lstk(top))
      istk(ilv1)=1
      istk(ilv1+1)=1  
      istk(ilv1+2)=1
      istk(ilv1+3)=0
      lv1=sadr(ilv1+4)
      stk(lv1)=dble(iok)
      lstk(top+1)=lv1+1
      return
      end
c     ********************

