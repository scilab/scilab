c =========================================
c INRIA
c =========================================        
      subroutine intgrep()
      INCLUDE 'stack.h'
c
      integer vol,vol2,tops
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
         call funnam(ids(1,pt+1),'grep',il2)
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
            return
         endif
      endif
      if(istk(il1).ne.10) then
         fun=-1
         top=tops
         call funnam(ids(1,pt+1),'grep',il1)
         return
      endif
c
      mn1=istk(il1+1)*istk(il1+2)
      id1=il1+4
      l1=id1+mn1+1
      vol=istk(id1+mn1)-1
      lw = max(lstk(tops+1),lstk(top)+mn1+2)
      ilrow=lw
      lw=ilrow+mn1
      if(lhs.eq.2) then
         ilm=lw
         lw=ilm+mn1
      endif
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c
c     loops on arg1 entries
      mc=0
      do 36 k1=1,mn1
         vol=istk(id1+k1)-istk(id1+k1-1)
         ll1=l1-1
 10      ll1=ll1+1
         if(ll1-l1.ge.vol) goto 35 
c     .  loop on arg2 strings
         do 32 k2=1,mn2
            ll2=l2+istk(il2+3+k2)-1
            vol2=istk(il2+4+k2)-istk(il2+3+k2)
c     .     check for a match on  characters  arg1(k1)(ll1) and arg2(k2)(1)
            if(istk(ll1).eq.istk(ll2))  then
c     .        they match
               i2=k2
            else
c     .        characters arg1(k1)(ll1) and arg2(k2)(1) doesnt match
               goto 32
            endif
c           first character matches check if next chars match
 12         k=0
 15         k=k+1
            if(k.ge.vol2) then
c     .        end of arg2(k2) reached a match found or ith string
               nfound=k2
               goto 30
            elseif(ll1+k.ge.l1+vol) then
c     .        not enough chars in arg1(k1)
               goto 32
            elseif(istk(ll1+k).eq.istk(ll2+k)) then
c     .        next characters matches go ahaead
               goto 15
            else
c     .        next characters doesnt match, chech for an other string in arg2
               goto 32
            endif

 30         continue
c     a match found
            mc=mc+1

            stk(ilrow+mc-1)=k1
            if(lhs.eq.2) stk(ilm+mc-1)=nfound
            goto 35
 32      continue
c     .  arg1(k1)(ll1) doesnt match with any string in arg2
         goto 10

c     end of loop skip to next entry
 35      l1=l1+vol
 36   continue
c
c     end of string str1 reached
       if(mc.eq.0) then
            istk(ilr)=1
            istk(ilr+1)=0
            istk(ilr+2)=0
            istk(ilr+3)=0
            lstk(top+1)=sadr(ilr+4)
            if(lhs.eq.2) then
               top=top+1
               ilr=iadr(lstk(top))
               istk(ilr)=1
               istk(ilr+1)=0
               istk(ilr+2)=0
               istk(ilr+3)=0
               lstk(top+1)=sadr(ilr+4)
            endif
         else
            lrow=sadr(ilr+4)
            lstk(top+1)=lrow+mc
            if(lhs.eq.2) then
               top=top+1
               il2=iadr(lstk(top))
               lm=sadr(il2+4)
               lstk(top+1)=lm+mc
            endif
            err=lstk(top+1)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            istk(ilr)=1
            istk(ilr+1)=1
            istk(ilr+2)=mc
            istk(ilr+3)=0
            inc=1
            if(lrow.gt.ilrow) inc=-1
            call unsfdcopy(mc,stk(ilrow),inc,stk(lrow),inc)
            if(lhs.eq.2) then
               istk(il2)=1
               istk(il2+1)=1
               istk(il2+2)=mc
               istk(il2+3)=0
               inc=1
               if(lm.gt.ilm) inc=-1
               call unsfdcopy(mc,stk(ilm),inc,stk(lm),inc)
            endif
         endif
      return
      end
c =========================================        

