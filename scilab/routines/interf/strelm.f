      subroutine strelm
c     ==================================================================
c     ==
c     
c     evaluation des fonctions elementaires sur les chaines de
c     caracteres
c     
c     ==================================================================
c     ==
c     
c     Copyright INRIA
      INCLUDE '../stack.h'
c     
      integer iadr,sadr
      integer id(nsiz)
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' strelm '//buf(1:4))
      endif
c     
c     functions/fin
c     1       2       3       4       5           6       7       8   
c     length    part   string  convstr  emptystr str2code code2str sort
c     9         10       11        12    13    14     15
c     strcat    strindex strsubst ascii  grep tokens  strsplit

c     
c      if ( fin.eq.8)  then 
c         call intssort(id)
c      else 
c         call cstrelm()
c      endif
c      return 

      goto (10,20,25,50,60,70,80,90,100,110,120,125,130, 135,140) fin
c     
c     length
c     
 10   continue
      call intlength(id)
      goto 900


c     part
 20   continue
      call intpart(id)
      goto 900
c     
c     string
c     
 25   continue
      call intstring(id)
      goto 900

c     
c     convstr
 50   continue
      call intconvstr(id)
      goto 900

c     emptystr
 60   continue
      call intemptystr(id)
      goto 900
c     
c     str2code
 70   continue
      call intstr2code(id)
      goto 900
c     
c     code2str
 80   continue
      call intcode2str(id)
      goto 900
c     
c     sort
 90   continue
      call intssort(id)
      goto 900
c     
c     strcat(str [,ins])
 100  continue
      call instrcat(id)
      goto 900
c     
c     strindex(str1,str2)
 110  continue
      call instrindex(id)
      goto 900
c     
c     strsubst(str1,str2,str3)
 120  continue 
      call intstrsubst(id)
      goto 900
c     
c     ascii
c     
 125  continue
      call intascii(id)
      goto 900
c     
c     grep(str1,str2) 
 130  continue
      call intgrep()
      goto 900
c     
c     tokens(str [,sep])
 135  continue
      call inttokens(id)
      goto 900

c     strsplit(str,v)
 140  continue
      call intstrsplit(id)
      goto 900

c     
 900  return
      end

      integer function strord(r1,l1,r2,l2)
      integer r1(*),r2(*),c1,c2
      integer blank
      data blank/40/
c
      iord=0
      if(l1.eq.0) then
         if(l2.gt.0) then
            strord=-1
            return
         else
            strord=0
            return
         endif
      else
         if(l2.eq.0) then
            strord=1
            return
         endif
      endif
      ll=min(l1,l2)
      do 10 i=1,max(l1,l2)
         if(i.le.l1) then
            c1=r1(i)
         else
            c1=blank
         endif
         if(i.le.l2) then
            c2=r2(i)
         else
            c2=blank
         endif
         if(c1.ge.0) c1=256-c1
         if(c2.ge.0) c2=256-c2
         if(c1.gt.c2) then
            strord=1
            return
         elseif(c1.lt.c2) then
            strord=-1
            return
         endif
 10   continue
      strord=0
      return
      end

      subroutine instrcat(id)
      INCLUDE '../stack.h'
c
      integer id(nsiz),vol,tops
      logical ref
c
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
c     strcat(tr [,ins])
c
      if (rhs.eq.3) then 
         call cstrelm() 
         return 
      endif

      if(rhs.ne.1.and.rhs.ne.2) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
c     
      tops=top
c
      if(rhs.eq.2) then
         il2=iadr(lstk(top))
         if(istk(il2).lt.0) il2=iadr(istk(il2+1))
         if(istk(il2).ne.10) then 
            fun=-1
            top=tops
            call funnam(ids(1,pt+1),'strcat',il2)
            return
         endif
         if(istk(il2+1)*istk(il2+2).ne.1) then
            err=2
            call error(36)
            return
         endif
         l2=il2+6
         nv2=istk(il2+5)-1
         top=top-1
      endif
c
      il1=iadr(lstk(top))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=ilr.ne.il1

      if(istk(il1).eq.1) then
         if(istk(il1+1)*istk(il1+2).eq.0) then
            istk(ilr)=10
            istk(ilr+1)=1
            istk(ilr+2)=1
            istk(ilr+3)=0
            istk(ilr+4)=1
            istk(ilr+5)=1
            lstk(top+1)=sadr(ilr+6)
            return
         endif
      endif
      if(istk(il1).ne.10) then
         fun=-1
         top=tops
         call funnam(ids(1,pt+1),'strcat',il1)
         return
      endif

      m1=istk(il1+1)
      n1=istk(il1+2)
      mn1=m1*n1
      id1=il1+4
      l1=id1+mn1+1
      vol=istk(id1+mn1)-1
c
      if(rhs.eq.1) then
         istk(ilr)=10
         istk(ilr+1)=1
         istk(ilr+2)=1
         istk(ilr+3)=0
         istk(ilr+4)=1
         istk(ilr+5)=1+vol
         l=ilr+6
         call icopy(vol,istk(l1),1,istk(l),1)
         lstk(top+1)=sadr(l+vol)
      else
         lw = iadr(lstk(tops+1))
         err=sadr(lw+vol+mn1*nv2)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         ll1=l1
         ll2=lw
         do 10 i=1,mn1
            ln=istk(id1+i)-istk(id1-1+i)
            call icopy(ln,istk(ll1),1,istk(ll2),1)
            ll1=ll1+ln
            ll2=ll2+ln
            call icopy(nv2,istk(l2),1,istk(ll2),1)
            ll2=ll2+nv2
 10      continue
         vol=vol+(mn1-1)*nv2
         istk(ilr)=10
         istk(ilr+1)=1
         istk(ilr+2)=1
         istk(ilr+3)=0
         istk(ilr+4)=1
         istk(ilr+5)=1+vol
         l=ilr+6
         call icopy(vol,istk(lw),1,istk(l),1)
         lstk(top+1)=sadr(l+vol)
      endif
      end

      subroutine instrindex(id)
      INCLUDE '../stack.h'
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

      subroutine intstrsubst(id)
      INCLUDE '../stack.h'
c
      integer id(nsiz),vol,tops
      logical ref
c
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
c     strsubst(str1,str2,str3)
c
      if(rhs.ne.3) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
c
      tops=top
c     
      lw = iadr(lstk(top+1))
c     
      il3=iadr(lstk(top))
      if(istk(il3).lt.0) il3=iadr(istk(il3+1))
      if(istk(il3).ne.10) then 
         fun=-1
         top=tops
         call funnam(ids(1,pt+1),'strsubst',il3)
         return
      endif
      if(istk(il3+1)*istk(il3+2).ne.1) then
         err=3
         call error(36)
         return
      endif
      l3=il3+6
      nv3=istk(il3+5)-1
      top=top-1

      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      if(istk(il2).ne.10) then 
         fun=-1
         top=tops
         call funnam(ids(1,pt+1),'strsubst',il2)
         return
      endif
      if(istk(il2+1)*istk(il2+2).ne.1) then
         err=2
         call error(36)
         return
      endif
      l2=il2+6
      nv2=istk(il2+5)-1
      if(nv2.eq.0) then
         err=2
         call error(249)
         return
      endif
      top=top-1

c     
      il1=iadr(lstk(top))
      ilr1=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=il1.ne.ilr1
      if(istk(il1).eq.1) then
         if(istk(il1+1)*istk(il1+2).eq.0) then
            istk(ilr1)=1
            istk(ilr1+1)=0
            istk(ilr1+2)=0
            istk(ilr1+3)=0
            lstk(top+1)=sadr(ilr1+4)
            return
         endif
      endif
      if(istk(il1).ne.10) then
         fun=-1
         top=tops
         call funnam(ids(1,pt+1),'strsubst',il1)
         return
      endif

      mn1=istk(il1+1)*istk(il1+2)
      id1=il1+4
      l1=id1+mn1+1
      
c
      if(ref) then
         id1r=lw
         istk(id1r)=1
         lw=lw+mn1+1
      else
         id1r=id1
      endif
      ilr=lw
      ilr0=ilr
      err=sadr(ilr)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      vol=istk(id1+1)-istk(id1)
c
      do 40 ij=1,mn1
         mc=0
         if(vol.eq.0) goto 36
         ll0=l1
         ll1=l1-1
 10      ll1=ll1+1
         if(ll1-l1.ge.vol+1-nv2) goto 35
         if(istk(ll1).ne.istk(l2)) goto 10
c     first character matches
         k=0
 15      k=k+1
         if(k.ge.nv2) goto 30
         if(istk(ll1+k).eq.istk(l2+k)) goto 15
         goto 10
 30      continue
c     a match found
         err=sadr(ilr+ll1-ll0+nv3) -lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         mc=mc+1
         call icopy(ll1-ll0,istk(ll0),1,istk(ilr),1)
         ilr=ilr+ll1-ll0
         ll0=ll1+nv2

         call icopy(nv3,istk(l3),1,istk(ilr),1)
         ilr=ilr+nv3
         ll1=ll1+nv2-1
         goto 10
 35      continue
c     end of string reached
         err=sadr(ilr+vol-(ll0-l1))-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call icopy(vol-(ll0-l1),istk(ll0),1,istk(ilr),1)
         ilr=ilr+vol-(ll0-l1)
         l1=l1+vol
 36      vol=istk(id1+ij+1)-istk(id1+ij)
         istk(id1r+ij)=istk(id1r+ij-1)+ilr-ilr0
         ilr0=ilr
 40   continue
      id1=ilr1+4
      if(ref) then
         call icopy(4,istk(il1),1,istk(ilr1),1)
         call icopy(mn1+1,istk(id1r),1,istk(id1),1)
      endif
      call icopy(ilr-lw,istk(lw),1,istk(id1+mn1+1),1)
      lstk(top+1)=sadr(id1+mn1+1+ilr-lw)

      return
      end

      subroutine intlength(id)
      INCLUDE '../stack.h'
c
      integer id(nsiz)
      logical ref
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(lhs*rhs.ne.1) then
         call error(39)
         return
      endif

      il1=iadr(lstk(top+1-rhs))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=ilr.ne.il1
      itype=istk(il1)
      if(itype.eq.1.or.itype.eq.2.or.itype.eq.4) then
c     length( )=prod(size( )) for matrices (+ polynomial and boolean)
         l=sadr(ilr+4)
         stk(l)=dble(istk(il1+1)*istk(il1+2))
         istk(ilr)=1
         istk(ilr+1)=1
         istk(ilr+2)=1
         istk(ilr+3)=0
         lstk(top+1)=l+1
      elseif(itype.ge.15.and.itype.le.17) then
c     length(list)=size(list)
         l=sadr(ilr+4)
         stk(l)=dble(istk(il1+1))
         istk(ilr)=1
         istk(ilr+1)=1
         istk(ilr+2)=1
         istk(ilr+3)=0
         lstk(top+1)=l+1
      elseif(itype.eq.10) then
         m1=istk(il1+1)
         n1=istk(il1+2)
         mn1=m1*n1
         id1=il1+4
         l1=id1+mn1+1
c
         if(ref) then
            ll=sadr(ilr+4)
         else
            ll=sadr(l1)
         endif
         err=ll+mn1-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
c
         do 11 k=1,mn1
            stk(ll-1+k)=dble(istk(id1+k)-istk(id1+k-1))
 11      continue
         l=sadr(ilr+4)
         if(.not.ref) call unsfdcopy(mn1,stk(ll),1,stk(l),1)
         istk(ilr)=1
         istk(ilr+1)=m1
         istk(ilr+2)=n1
         istk(ilr+3)=0
         lstk(top+1)=l+mn1
      else
         fun=-1
         call funnam(ids(1,pt+1),'length',il1)
         return
      endif
      return
      end

      subroutine intpart(id)
      INCLUDE '../stack.h'
c
      integer blank,id(nsiz),vol
      logical ref
      integer iadr,sadr
      data    blank/40/
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1


      if(lhs.ne.1.or.rhs.ne.2) then
         call error(39)
         return
      endif
c
      il1=iadr(lstk(top+1-rhs))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=ilr.ne.il1

      if(istk(il1).ne.10) then
         fun=-1
         call funnam(ids(1,pt+1),'part',il1)
         return
      endif
c
      m1=istk(il1+1)
      n1=istk(il1+2)
      mn1=m1*n1
      id1=il1+4
      l1=id1+mn1+1
      vol=istk(id1+mn1)-1
      lw=lstk(top+1)
c
      lr=max(iadr(lw),ilr+mn1+5)
      ilv=iadr(lstk(top))
      if(istk(ilv).lt.0) ilv=iadr(istk(ilv+1))
c
      if(istk(ilv).ne.1) then
         err=rhs
         call error(53)
         return
      endif
      if(istk(ilv+1).gt.1.and.istk(ilv+2).gt.1) then
         err=rhs
         call error(89)
         return
      endif
      if(istk(ilv+3).ne.0) then
         err=rhs
         call error(52)
         return
      endif
      nv=istk(ilv+1)*istk(ilv+2)
c
      if(nv.eq.0) then
         if(ref) then
            call icopy(4,istk(il1),1,istk(ilr),1)
            id1=ilr+4
            l1=id1+mn1+1
         endif
         call iset(mn1+1,1,istk(id1),1)
         top=top-1
         lstk(top+1)=sadr(l1)
         goto 999
      endif

      lv=sadr(ilv+4)
      do 21 k=0,nv-1
         if(int(stk(lv+k)).le.0) then
            err=2
            call error(36)
            return
         endif
 21   continue
      err=sadr(lr+mn1*nv)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call iset(mn1*nv,blank,istk(lr),1)
      lrijk=lr
      lij=l1
      do 23 ij=1,mn1
         nij=istk(id1+ij)-istk(id1+ij-1)
         do 22 k=1,nv
            mv=int(stk(lv-1+k))
            if(mv.le.nij) istk(lrijk)=istk(lij-1+mv)
            lrijk=lrijk+1
 22      continue
         lij=lij+nij
 23   continue
c     
      if(ref) then
         call icopy(4,istk(il1),1,istk(ilr),1)
         id1=ilr+4
         l1=id1+mn1+1
      endif
      istk(id1)=1
      do 24 ij=1,mn1
      istk(id1+ij)=istk(id1+ij-1)+nv
 24   continue
      top=top-1
      if(lr.ne.l1) call icopy(mn1*nv,istk(lr),1,istk(l1),1)
      lstk(top+1)=sadr(l1+istk(id1+mn1))
 999  return
      end

      subroutine intstring(id)
      INCLUDE '../stack.h'
c
      integer id(nsiz)
      integer eol,nclas
      logical ref
      integer iadr,sadr
      data eol/99/,nclas/29/
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      if(rhs.ne.1) then
         call  error(39)
         return
      endif
c
      il=iadr(lstk(top))
      ilr=il
      if(istk(il).lt.0) il=iadr(istk(il+1))
      ref=ilr.ne.il

      if(istk(il).eq.10) then
         call ref2val
         goto 999
      endif
c
      lw=iadr(lstk(top+1))
c
      if(istk(il).eq.1) then
c
c     conversion d'une matrice de scalaires
c     -------------------------------------
         if(lhs.ne.1) then
            call error(39)
            return
         endif
         m=istk(il+1)
         n=istk(il+2)
         if(m*n.eq.0) then
            if (ref) then
               istk(ilr)=1
               istk(ilr+1)=0
               istk(ilr+2)=0
               istk(ilr+3)=0
               lstk(top+1)=sadr(ilr+4)
            endif
               
            goto 999
         endif
         it=istk(il+3)
         l=sadr(il+4)
         if(ref) then
            lw=ilr+4
         else
            lw=iadr(lstk(top+1))
         endif
         err=sadr(lw+m*n*(2*lct(7)+4))-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         lstr=lw+m*n+1
         if(it.eq.0) call cvdm(stk(l),m,m,n,lct(7),lct(6),
     1        istk(lstr),istk(lw))
         if(it.eq.1) call cvwm(stk(l),stk(l+m*n),m,m,n,lct(7),lct(6),
     1        istk(lstr),istk(lw))
         nstr=istk(lw+m*n)-1
         if(.not.ref) call icopy(m*n+1+nstr,istk(lw),1,istk(ilr+4),1)
         istk(ilr)=10
         istk(ilr+1)=m
         istk(ilr+2)=n
         istk(ilr+3)=0
         lstk(top+1)=sadr(ilr+5+m*n+nstr)
         goto 999
c
      elseif(abs(istk(il)).eq.11.or.abs(istk(il)).eq.13) then
c
c     conversion d'une macro non compilee
c     -----------------------------------

         ilm=il
c
         if(lhs.ne.3) then
            call error(41)
            return
         endif
c     form vectors of output and input variables
         il=il+1
         lw=lstk(top+1)
         do 37 i=1,2
            n=istk(il)
            il=il+1
            ilio=iadr(lw)
            istk(ilio)=10
            if(n.eq.0) istk(ilio)=1
            istk(ilio+1)=min(1,n)
            istk(ilio+2)=n
            istk(ilio+3)=0
            ilp=ilio+4
            istk(ilp)=1
            l=ilp+n
            if (n.eq.0) goto 35
            do 34 j=1,n
               call namstr(istk(il),istk(l+1),nn,1)
               l=l+nn
               istk(ilp+j)=l+1-(ilp+n)
               il=il+nsiz
 34         continue
 35         if(i.eq.1) then
               lout=lw
            else
               llin=lw
            endif
            lw=sadr(l+1)
 37      continue
c
c
         if(istk(ilm).eq.13) then
            ltxt=lw
            ilt=iadr(ltxt)
            istk(ilt)=1
            istk(ilt+2)=0
            istk(ilt+3)=0
            istk(ilt+4)=1
            ilt=ilt+4
            goto 43
         endif
         ltxt=lw
         ilt=iadr(ltxt)
         istk(ilt)=10
         istk(ilt+2)=1
         istk(ilt+3)=0
         istk(ilt+4)=1
         ilp=ilt+4
c     
c     compute number of lines of the macro
         nch=istk(il)
         nl=0
         il=il+1
         l=il-1
 39      l=l+1
         if(istk(l).eq.eol) then
            if(istk(l+1).eq.eol) goto 40
            nl=nl+1
         endif
         goto 39
c
 40      continue
         istk(ilt+1)=nl
         if(nl.eq.0) then
            istk(ilt)=1
            istk(ilt+2)=0
            ilt=ilt+3
            goto 43
         endif
         ilt=ilp+nl+1
         l=il
 41      if(istk(l).eq.eol) goto 42
         l=l+1
         goto 41
 42      if(istk(l+1).eq.eol) goto 43
         n=l-il
         ilp=ilp+1
         istk(ilp)=istk(ilp-1)+n
         call icopy(n,istk(il),1,istk(ilt),1)
         ilt=ilt+n
         il=l+1
         l=il
         goto 41
c
 43      continue
         lw=sadr(ilt+1)
c     update stack
         if(lhs.eq.3) then
            n=llin-lout
            call unsfdcopy(n,stk(lout),1,stk(lstk(top)),1)
            lstk(top+1)=lstk(top)+n+1
            top=top+1
            n=ltxt-llin
            call unsfdcopy(n,stk(llin),1,stk(lstk(top)),1)
            lstk(top+1)=lstk(top)+n+1
            top=top+1
         endif
         n=lw-ltxt
         call unsfdcopy(n,stk(ltxt),1,stk(lstk(top)),1)
         lstk(top+1)=lstk(top)+n+1
         goto 999

      elseif(istk(il).eq.14) then
c     librairies
         if(lhs.ne.1) then
            call error(39)
            return
         endif
         il0=ilr
c
         n1=istk(il+1)
         l1=il+2
         il=il+n1+2
         n=istk(il)
         il=il+nclas+2

         if(.not.ref) ilr=lw
         err=sadr(ilr+6+n1+(nlgh+1)*n)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif

         istk(ilr)=10
         istk(ilr+1)=n+1
         istk(ilr+2)=1
         istk(ilr+3)=0
         istk(ilr+4)=1
         l=ilr+6+n
         call icopy(n1,istk(l1),1,istk(l),1)
         istk(ilr+5)=1+n1
         l=l+n1
         do 49  k=1,n
           call namstr(istk(il),istk(l),nn,1)
           istk(ilr+5+k)=istk(ilr+4+k)+nn
           l=l+nn
           il=il+nsiz
 49     continue
        if(.not.ref) call icopy(l-ilr,istk(ilr),1,istk(il0),1)
        lstk(top+1)=sadr(il0+l-ilr)
        goto 999
      else
         fun=-1
         call funnam(ids(1,pt+1),'string',il)
         return
      endif
 999  return
      end

      subroutine intconvstr(id)
      INCLUDE '../stack.h'
c
      integer id(nsiz)
      logical ref,lgq,cremat
      integer tops
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(rhs.gt.2.or.rhs.lt.1) then
         call error(39)
         return
      endif

c
      tops=top
c
      if(rhs.eq.2) then
c     conversion flag
         il1=iadr(lstk(top))
         if(istk(il1).lt.0) il1=iadr(istk(il1+1))
         if(istk(il1).ne.10) then
            err=2
            call error(55)
            return
         endif
         if(istk(il1+1).ne.1.or.istk(il1+2).ne.1) then
            err=2
            call error(89)
            return
         endif
         if(abs(istk(il1+6)).eq.30) then
            lgq=.true.
         elseif(abs(istk(il1+6)).ne.21) then
            err=2
            call error(36)
            return
         else
            lgq=.false.
         endif
         top=top-1
      else
         lgq=.false.
      endif
c
   51 continue
      il1=iadr(lstk(top))
      
      il=il1
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if(istk(il).ne.10) then
         if(istk(il1).eq.1) then
            if(istk(il+1)*istk(il+2).eq.0) then
c              convstr([])
               lgq=cremat('convstr', top, 0, 0, 0, lr, lc)
               return
            endif
         endif
         top=tops
         fun=-1
         call funnam(ids(1,pt+1),'convstr',il)
         return
      endif

      if(istk(il1).lt.0) then
         rhs=1
         call ref2val
      endif

      m1=istk(il1+1)
      n1=istk(il1+2)
      if (lgq) goto 53
c
c conversion en minuscule
c
      k=iadr(lstk(top+1))-1
      do 52 i=il1+5,k
         if(istk(i).lt.-35.or.istk(i).gt.-10) goto 52
         istk(i)=-istk(i)
   52 continue
      goto 999
c
c conversion en majuscule
c
   53 continue
      k0=il1+4
      k =k0+m1*n1
      do 55 i=1,m1*n1
        k1=istk(k0+1)-istk(k0)
        k0=k0+1
        do 54 l=1,k1
          k=k+1
          if(istk(k).lt.10.or.istk(k).gt.35) goto 54
          istk(k)=-istk(k)
   54   continue
   55 continue
      goto 999
c
 999  return
      end

      subroutine intemptystr(id)
      INCLUDE '../stack.h'
c
      integer id(nsiz)
      logical ref
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      if(lhs.ne.1) then 
         call error(59)
         return
      endif


      if(rhs.eq.2) then
         il=iadr(lstk(top))
         if(istk(il).lt.0) il=iadr(istk(il+1))
         if(istk(il).ne.1) then
            err=2
            call error(53)
            return
         endif
         if(istk(il+1)*istk(il+2).ne.1) then
            err=2
            call error(89)
            return
         endif
         if(istk(il+3).ne.0) then
            err=2
            call error(52)
            return
         endif
         n=int(stk(sadr(il+4)))
         top=top-1
c
         il=iadr(lstk(top))
         if(istk(il).lt.0) il=iadr(istk(il+1))
         if(istk(il).ne.1) then
            err=1
            call error(53)
            return
         endif
         if(istk(il+1)*istk(il+2).ne.1) then
            err=1
            call error(89)
            return
         endif
         if(istk(il+3).ne.0) then
            err=1
            call error(52)
            return
         endif
         m=int(stk(sadr(il+4)))
         if (m.eq.0.or.n.eq.0) then
            il=iadr(lstk(top))
            istk(il)=1
            istk(il+1)=0
            istk(il+2)=0
            istk(il+3)=0
            lstk(top+1)=sadr(il+4)
            goto 999
         endif
      elseif(rhs.eq.1) then
         il=iadr(lstk(top))
         ilr=il
         if(istk(il).lt.0) il=iadr(istk(il+1))

         if(istk(il).gt.10) then
            fun=-1
            call funnam(ids(1,pt+1),'emptystr',il)
            return
         endif
         ref=ilr.ne.il
         m=istk(il+1)
         n=istk(il+2)
         if(m*n.eq.0) then
            if(ref) call ref2val
            return
         endif
      elseif(rhs.le.0) then
         m=1
         n=1
         top=top+1
      else
         call error(42)
         return
      endif
      il=iadr(lstk(top))
      istk(il)=10
      istk(il+1)=m
      istk(il+2)=n
      istk(il+3)=0
      call iset(m*n+1,1,istk(il+4),1)
      lstk(top+1)=sadr(il+6+m*n)
c     The correct assignation should be 
c     lstk(top+1)=sadr(il+5+m*n)
      goto 999 
 999  return
      end

      subroutine intstr2code(id)
      INCLUDE '../stack.h'
c
      integer id(nsiz)
      logical ref
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1


      if (rhs .ne. 1) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
c     checking variable str (number 1)
c       
      il1 = iadr(lstk(top-rhs+1))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=ilr.ne.il1
      if (istk(il1) .ne. 10) then
         fun=-1
         call funnam(ids(1,pt+1),'str2code',il1)
         return
      endif

      if (istk(il1+1)*istk(il1+2) .ne. 1) then
         err = 1
         call error(89)
         return
      endif
      n1 = istk(il1+5)-1
      l1 = il1+6
      if(.not.ref) then
         call icopy(n1,istk(l1),1,istk(l1-2),1)
         l1=sadr(ilr+4)
         call int2db(n1,istk(il1+4),-1,stk(l1),-1)
      else
         l1=sadr(ilr+4)
         call int2db(n1,istk(il1+6),1,stk(l1),1)
      endif
      istk(ilr)=1
      istk(ilr+1)=n1
      istk(ilr+2)=1
      istk(ilr+3)=0
      lstk(top+1)=l1+n1
      goto 999
 999  return
      end

      subroutine intcode2str(id)
      INCLUDE '../stack.h'
c
      integer id(nsiz)
      logical ref
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      if (rhs .ne. 1) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
c     checking variable str (number 1)
c       
      il1 = iadr(lstk(top-rhs+1))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      if (istk(il1) .ne. 1) then
         fun=-1
         call funnam(ids(1,pt+1),'code2str2',il1)
         return
      endif
      ref=ilr.ne.il1
      if (istk(il1+3) .ne. 0) then
         err = 1
         call error(52)
         return
      endif
      n1 = istk(il1+1)*istk(il1+2)
      l1 = sadr(il1+4)
      if(.not.ref) then
         call unsfdcopy(n1,stk(l1),-1,stk(l1+2),-1)
         l1=l1+2
      endif
      istk(ilr+4)=1
      istk(ilr+5)=n1+1
      do 81 i=1,n1
         istk(ilr+5+i)=stk(l1-1+i)
 81   continue
      istk(ilr)=10
      istk(ilr+1)=1
      istk(ilr+2)=1
      istk(ilr+3)=0
      lstk(top+1)=sadr(ilr+6+n1)
      goto 999
 999  return
      end

      subroutine intssort(id)
      INCLUDE '../stack.h'
c
      integer id(nsiz),tops,vol,sel
      logical ref
      external strord
      integer strord
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      if(rhs.gt.2) then
         call error(42)
         return
      endif
      if(lhs.gt.2) then
         call error(41)
         return
      endif

      tops=top

c     select type of sort to perform
      sel=0
      if(rhs.eq.2) then
         call  getorient(top,sel)
         if(err.gt.0) return
         top=top-1
      endif


      if(sel.eq.2) then
         top=tops
         fun=-1
         call funnam(ids(1,pt+1),'sort',iadr(lstk(top+1-rhs)))
         return
      endif
      if(rhs.eq.2) rhs=rhs-1
      call ref2val
c
      il1=iadr(lstk(top))
      ilr=il1

      m=istk(il1+1)
      n=istk(il1+2)
      mn=m*n
      id1=il1+4
      l1=id1+mn+1
      vol=istk(id1+mn)-1

c
      id1r=id1
      ls=iadr(lstk(top+1))
      lsz=ls+vol


      lind=lsz+mn
      lw=lind+mn
      err=sadr(lw)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif

      call icopy(vol,istk(l1),1,istk(ls),1)

      do 91 i=0,mn-1
         istk(lsz+i)=istk(id1+i+1)-istk(id1+i)
 91   continue

c     
      if(sel.eq.0) then
c     sort(a) <=> sort(a,'*')
         call rcsort(strord,istk(lsz),istk(id1),istk(ls),mn,istk(lind))
      elseif(sel.eq.1) then
c     sort(a,'r')  <=>  sort(a,1)
         lszi=lsz
         idi=id1
         lindi=lind
         do 95 i=0,n-1
            call rcsort(strord,istk(lszi),istk(idi),istk(ls),m,
     $           istk(lindi))
            lszi=lszi+m
            idi=idi+m
            lindi=lindi+m
 95      continue   
      endif

      l2=ilr+4+mn+1
      do 93 i=0,mn-1
         call icopy(istk(lsz+i),istk(ls-1+istk(id1+i)),1,istk(l2),1)
         l2=l2+istk(lsz+i)
 93   continue
      lstk(top+1)=sadr(l2)

      istk(id1)=1
      do 94 i=0,mn-1
         istk(id1+i+1)=istk(id1+i)+istk(lsz+i)
 94   continue
      if(lhs.eq.1) goto 999
      top=top+1
      il=iadr(lstk(top))

      l=sadr(il+4)
      inc=-1
      if(sadr(lind).gt.l) inc=1
      call int2db(mn,istk(lind),inc,stk(l),inc)
      istk(il)=1
      istk(il+1)=m
      istk(il+2)=n
      istk(il+3)=0
      lstk(top+1)=l+mn

      go to 999
 999  return
      end

      subroutine intascii(id)
      include '../stack.h'
c
      integer id(nsiz),tops,vol,sel,eol
      logical ref
      external strord,getfastcode
      integer strord,getfastcode
      character*1 c
      integer iadr,sadr
      data eol/99/
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      if(rhs.ne.1) then
         call error(42)
         return
      endif
      if(lhs.ne.1) then
         call error(41)
         return
      endif

      il=iadr(lstk(top))
      ilr=il
      if(istk(il).lt.0) il=iadr(istk(il+1))
      ref=il.ne.ilr

      if(istk(il).eq.1) then
c     argument is a vector of ascii codes return a scilab string
         n=istk(il+1)*istk(il+2)
         l=sadr(il+4)
         if(.not.ref) then
            err=l+n-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call unsfdcopy(n,stk(l),1,stk(l+n),1) 
            l=l+n
         else
            err=sadr(ilr+6+n)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
         endif
         istk(ilr)=10
         istk(ilr+1)=1
         istk(ilr+2)=1
         istk(ilr+3)=0
         istk(ilr+4)=1
         istk(ilr+5)=1+n
         lr=ilr+6
         do 20 i=0,n-1
            c=char(int(stk(l+i)))
            istk(lr+i)=getfastcode(c)
 20      continue
         lstk(top+1)=sadr(lr+n)
      elseif(istk(il).eq.10) then
c     argument is a scilab string return a vector of ascii codes
         n=istk(il+4+istk(il+1)*istk(il+2))-1
         l=il+5+istk(il+1)*istk(il+2)
         if(.not.ref) then
            lw=l+n
            err=sadr(lw+n)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call icopy(n,istk(l),1,istk(lw),1) 
            l=lw
         else
            err=sadr(ilr+4)+n-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
         endif
         istk(ilr)=1
         istk(ilr+1)=1
         istk(ilr+2)=n
         istk(ilr+3)=0
         lr=sadr(ilr+4)
         do 30 i=0,n-1
            mc=istk(l+i)
            if(mc.gt.eol) then
               stk(lr+i)=ichar(char(mc-(eol+1)))
            elseif(mc.lt.0) then
               stk(lr+i)=ichar(alfb(abs(mc)+1))
            elseif(mc.eq.eol) then
               stk(lr+i)=10.
            else
               stk(lr+i)=ichar(alfa(mc+1))
            endif
 30      continue
         lstk(top+1)=lr+n
      else
         fun=-1
         call funnam(ids(1,pt+1),'ascii',il)
      endif
      end

      subroutine intgrep()
      INCLUDE '../stack.h'
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

      subroutine inttokens(id)
c
c     Interface code for tokens :
c
c         words = tokens( str [, sep])
c
c     which cut the string str into words (as a vector column of words)
c     the words being sub-strings of str between separators chars.
c
c     where str must be a simple string 
c      and  sep a vector (or matrix) of characters (not string of chars)
c           sep is optionnal and if it is is omitted then sep = [ " ", TAB ]
c
c     Added by Bruno      
c        (Initially tokens was coded as a scilab macro but was very inefficient).
c
      implicit none
      INCLUDE '../stack.h'

      integer id(nsiz)

      integer lw, tops, il2, m2, n2, p2, i_sep, nb_sep, il1, l1, nv1, 
     $        ilr, nb_chars, nb_words, i, c, ipt, size, lword, iwd
      logical  in_sep

      logical is_a_sep
      external error, funnam, is_a_sep, icopy

      integer BLANK, TAB
      parameter (BLANK = 40, TAB = -40)

      integer iadr,sadr, l
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      if(rhs.lt.1 .or. rhs.gt.2) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif

      tops=top
     
      lw = iadr(lstk(top+1))
     
      if (rhs .eq. 2) then  
c        get the separators sep
         il2 = iadr(lstk(top))
         if (istk(il2).lt.0) il2 = iadr(istk(il2+1))
         if (istk(il2).ne.10) then 
            fun=-1
            top=tops
            call funnam(ids(1,pt+1),'tokens',il2)
            return
         endif
         m2 = istk(il2+1)
         n2 = istk(il2+2)
c        verify that each element of sep is a simple character
         p2 = il2 + 4
         do i = 1, m2*n2
            if (istk(p2+i)-istk(p2+i-1) .ne. 1) then
               buf='a separator must be formed by only one character'
               call error(9999)
               return
            endif
         enddo
         nb_sep = m2*n2           
c     !  there are nb_sep separators, and the first is located at istk(i_sep)
         i_sep = p2 + 1 + nb_sep  
         top = top - 1
      else
c        no separators given => uses the 2 default separators (blank and tab)
         i_sep = lw
         nb_sep = 2
         lw = lw + 2
         err=sadr(lw)-lstk(bot)
         if ( err.gt.0 ) then
            call error(17)
            return
         endif
         istk(i_sep) = BLANK
         istk(i_sep+1) = TAB
      endif

c     get the string str
      il1 = iadr(lstk(top)) 
      if(istk(il1).lt.0) il1 = iadr(istk(il1+1))
      if(istk(il1).ne.10) then 
         fun = -1
         top = tops
         call funnam(ids(1,pt+1),'tokens',il1)
         return
      endif
      if(istk(il1+1)*istk(il1+2).ne.1) then 
c     !  str must be a simple string not a vector|matrix of strings
         call error(36)
         return
      endif
      l1 = il1+6
      nv1 = istk(il1+5)-1   
c     ! nv1 is the number of chars of str
      if(nv1.eq.0) then     
c     !   str is an empty string => return a void matrix
         il1=iadr(lstk(top)) 
         istk(il1)   = 1
         istk(il1+1) = 0
         istk(il1+2) = 0
         istk(il1+3) = 0
         return
      endif

c     count the number of words (and characters)
      nb_words = 0
      nb_chars = 0
      in_sep = .true.
      do i = 0, nv1-1
         c = istk(l1+i)
         if ( is_a_sep(c, istk(i_sep), nb_sep) ) then
            in_sep = .true.
         else
            nb_chars = nb_chars + 1
            if (in_sep) then
               nb_words = nb_words + 1
               in_sep = .false.
            endif
         endif
      enddo

      if ( nb_words .eq. 0 ) then 
c     !   return a void matrix
         il1=iadr(lstk(top)) 
         istk(il1)   = 1
         istk(il1+1) = 0
         istk(il1+2) = 0
         istk(il1+3) = 0
         return
      endif

c     prepare memory for the result
      ilr = lw
      size =  5 + nb_words + nb_chars  
c     ! size of the resulting scilab object
      lw = lw + size                   
      err = sadr(lw) - lstk(bot)
      if ( err.gt.0 ) then
         call error(17)
         return
      endif

c     form the output
      istk(ilr)   = 10
      istk(ilr+1) = nb_words
      istk(ilr+2) = 1
      istk(ilr+3) = 0
      istk(ilr+4) = 1
      ipt = ilr + 5
      iwd = ipt + nb_words
      istk(iwd-1) = nb_chars + 1 
c     dernier pointeur (necessaire lorsque la chaine ne se termine pas 
C     par un separateur)
      lword = 0
      do i = 0, nv1-1
         c = istk(l1+i)
         if ( is_a_sep(c, istk(i_sep), nb_sep) ) then
            if (lword .gt. 0) then 
c     !        on vient donc de terminer un mot
               istk(ipt) = istk(ipt-1) + lword
               ipt = ipt + 1
               lword = 0
            endif
         else
            istk(iwd) = c
            iwd = iwd + 1
            lword = lword + 1
         endif
      enddo

c     copy result at top
      il1=iadr(lstk(top)) 
      call icopy(size, istk(ilr), 1, istk(il1), 1)       
      lstk(top+1)=sadr(il1 + size)

      end 


      logical function is_a_sep(c, sep, nb_sep)
c
c     utility function used by inttokens (Bruno)
c      
      implicit none
      integer c, nb_sep, sep(nb_sep)
      integer i
      is_a_sep = .false.
      do i = 1, nb_sep
         if (c .eq. sep(i)) then
            is_a_sep = .true.
            return
         endif
      enddo

      end 
      subroutine intstrsplit(id)
      INCLUDE '../stack.h'
c
      integer id(nsiz),vol,tops
      logical ref1,ref2
c
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
c     strsplit(str,v)
c

      if(rhs.ne.2) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
c     
      tops=top
c
      lw=lstk(top+1)

      il2=iadr(lstk(top))
      ref2=istk(il2).lt.0
      if(ref2) il2=iadr(istk(il2+1))

      if(istk(il2).ne.1) then 
         fun=-1
         top=tops
         call funnam(ids(1,pt+1),'strsplit',il2)
         return
      endif
      n2=istk(il2+1)*istk(il2+2)
      l2=sadr(il2+4)
      if (stk(l2).lt.1) then
         err=2
         call error(116)
         return
      endif
      do i=1,n2-1
         if (stk(l2+i).lt.stk(l2+i-1)) then
            err=2
            call error(99)
            return
         endif
      enddo
c
      top=top-1
      il1=iadr(lstk(top))
      ref1=istk(il1).lt.0
      if(ref1) il1=iadr(istk(il1+1))

      if(istk(il1).eq.1) then
         if(istk(il1+1)*istk(il1+2).eq.0) then
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
         call funnam(ids(1,pt+1),'strsplit',il1)
         return
      endif
      if (istk(il1+1)*istk(il1+2).ne.1) then
         err=1
         call error(89)
         return
      endif
      id1=il1+4
      l1=id1+2
      vol=istk(id1+1)-1
      if (stk(l2+n2-1).ge.vol) then
         err=2
         call error(116)
         return
      endif
      if(ref1.and.ref2) then
         ilr=iadr(lstk(top))
      else
         ilr=iadr(lw)
         err = sadr(ilr+4+n2+2+vol) - lstk(bot)
         if ( err.gt.0 ) then
            call error(17)
            return
         endif
      endif
      istk(ilr)=10
      istk(ilr+1)=n2+1
      istk(ilr+2)=1
      istk(ilr+3)=0

      istk(ilr+4)=1
      do i=1,n2
         istk(ilr+4+i)=stk(l2+i-1)+1
      enddo
      istk(ilr+5+n2)=vol+1
      call icopy(vol,istk(l1),1,istk(ilr+6+n2),1)
      if(ref1.and.ref2) then
         lstk(top+1)=sadr(ilr+6+n2+vol)
      else
         il=iadr(lstk(top))
         call icopy(6+n2+vol,istk(ilr),1,istk(il),1)
         lstk(top+1)=sadr(il+6+n2+vol)
      endif
      end
