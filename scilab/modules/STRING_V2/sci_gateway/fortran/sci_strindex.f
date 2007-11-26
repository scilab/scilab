c =========================================
c INRIA
c =========================================  
      subroutine instrindex(id)
      INCLUDE 'stack.h'
c
      integer id(nsiz),vol,tops
      logical ref
c
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
c     strindex(str1,str2)
c
      tops=top

      if(rhs.ne.2) then
         call error(39)
         return
      endif
      if (lhs .gt. 2) then
         call error(41)
         return
      endif
c     
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      if(istk(il2).ne.10) then 
         fun=-1
         top=tops
         call funnam(ids(1,pt+1),'strindex',il2)
         return
      endif
      mn2=istk(il2+1)*istk(il2+2)
      l2=il2+5+mn2
      do 01 i=1,mn2
         if(istk(il2+4+i)-istk(il2+3+i).eq.0) then
            err=2
            call error(249)
            return
         endif
 01   continue
      top=top-1
c     
      il1=iadr(lstk(top))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=ilr.ne.il1
      if(istk(il1).eq.1) then
         if(istk(il1+1)*istk(il1+2).eq.0) then
            istk(ilr)=1
            istk(ilr+1)=0
            istk(ilr+2)=0
            istk(ilr+3)=0
            lstk(top+1)=sadr(ilr+4)
            if(lhs.eq.1) return
            top=top+1
            ilr=iadr(lstk(top))
            istk(ilr)=1
            istk(ilr+1)=0
            istk(ilr+2)=0
            istk(ilr+3)=0
            lstk(top+1)=sadr(ilr+4)
            return
         endif
      endif
      if(istk(il1).ne.10) then
         fun=-1
         top=tops
         call funnam(ids(1,pt+1),'strindex',il1)
         return
      endif
      if(istk(il1+1)*istk(il1+2).ne.1) then
         err=1
         call error(36)
         return
      endif
c
      mn1=1
      id1=il1+4
      l1=id1+mn1+1
      vol=istk(id1+mn1)-1
      lw = lstk(tops+1)
      ilm=iadr(lw)
      lw=sadr(ilm+mn2)
      lpos=lw
      err=lpos+10-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c
      mc=0
      if(vol.le.0) goto 36
      ll1=l1-1
 10   ll1=ll1+1
      if(ll1-l1.ge.vol) goto 35 
      nok=0
      do 11 i=1,mn2
         if(istk(ll1).eq.istk(l2+istk(il2+3+i)-1)) then
            istk(ilm+nok)=i
            nok=nok+1
         endif
 11   continue
      if(nok.eq.0) goto 10

c     first character matches
      k=0
 15   k=k+1
      nok1=0
      do 16 ii=0,nok-1
         i=istk(ilm+ii)
         if(k.ge.istk(il2+4+i)-istk(il2+3+i)) then
c     .     a match found for ith string
            nfound=i
            goto 30
         elseif(istk(ll1+k).eq.istk(l2+istk(il2+3+i)-1+k)) then
            istk(ilm+nok1)=i
            nok1=nok1+1
         endif
 16   continue
      nok=nok1
      if(nok.eq.0) goto 10
      goto 15

 30   continue
c     a match found
      mc=mc+1
      if(mod(mc,10).eq.0) then
         err=lpos+2*(mc+10)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
      endif
      stk(lpos+2*(mc-1)) = dble(ll1-l1+1)
      stk(lpos+2*(mc-1)+1)     = dble(nfound)
      goto 10
 35   continue
c     end of string str1 reached
 36   if(mc.eq.0) then
         istk(ilr)=1
         istk(ilr+1)=0
         istk(ilr+2)=0
         istk(ilr+3)=0
         lstk(top+1)=sadr(ilr+4)
         if (lhs.eq.1) return
         top=top+1
         ilr=iadr(lstk(top))
         istk(ilr)=1
         istk(ilr+1)=0
         istk(ilr+2)=0
         istk(ilr+3)=0
         lstk(top+1)=sadr(ilr+4)
      else
         if(lhs.eq.1) then
            l=sadr(ilr+4)
            call unsfdcopy(mc,stk(lpos),2,stk(l),1)
            istk(ilr)=1
            istk(ilr+1)=1
            istk(ilr+2)=mc
            istk(ilr+3)=0
            lstk(top+1)=l+mc
         else
            lw=lpos+2*mc
            ilw=iadr(lw)
            err=lw+2*sadr(4)+2*mc-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            istk(ilw)=1
            istk(ilw+1)=1
            istk(ilw+2)=mc
            istk(ilw+3)=0
            l=sadr(ilw+4)
            call dcopy(mc,stk(lpos),2,stk(l),1)
            l=l+mc
            l1=l
            ilw=iadr(l)
            istk(ilw)=1
            istk(ilw+1)=1
            istk(ilw+2)=mc
            istk(ilw+3)=0
            l=sadr(ilw+4)
            call dcopy(mc,stk(lpos+1),2,stk(l),1)
            l=l+mc
c     .     copy the variables at the top of the stack
            call unsfdcopy(l-lw,stk(lw),1,stk(lstk(top)),1)
            lstk(top+1)=lstk(top)+l1-lw
            top=top+1
            lstk(top+1)=lstk(top)+l-l1

         endif

      endif
      return
      end
c =========================================  