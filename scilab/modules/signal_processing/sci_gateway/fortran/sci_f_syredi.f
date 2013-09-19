c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c     =======================================
      subroutine scisyredi(fname)
c      
      include 'stack.h'
      character*(*) fname
      double precision adelp,adels,fact,zzi(16),zzr(16),zpi(16),zpr(16)
      integer lo,lg
      integer i
      integer iadr,sadr
      
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      
      
      il1=iadr(lstk(top))
      itv1=istk(il1)
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=sadr(il1+4)
      adels=stk(l1)
      il2=iadr(lstk(top-1))
      itv2=istk(il2)
      m2=istk(il2+1)
      n2=istk(il2+2)
      it2=istk(il2+3)
      l2=sadr(il2+4)
      adelp=stk(l2)
      il3=iadr(lstk(top-2))
      itv3=istk(il3)
      m3=istk(il3+1)
      n3=istk(il3+2)
      it3=istk(il3+3)
      l3=sadr(il3+4)
      il4=iadr(lstk(top-3))
      itv4=istk(il4)
      m4=istk(il4+1)
      n4=istk(il4+2)
      it4=istk(il4+3)
      l4=sadr(il4+4)
c     call entier(1,stk(l4),stk(l4))
      iapro=stk(l4)
      il5=iadr(lstk(top-4))
      itv5=istk(il5)
      m5=istk(il5+1)
      n5=istk(il5+2)
      it5=istk(il5+3)
      l5=sadr(il5+4)
c     call entier(1,stk(l5),stk(l5))
      ityp=stk(l5)
      lw1=lstk(top+1)
      lw2=lw1+32
      lw3=lw2+32
      lw4=lw3+32
      lw5=lw4+32
      lw6=lw5+32
      lw7=lw6+64
      lw8=lw7+64
      lw9=lw8+64
      lw10=lw9+64
      lw11=lw10+64
      lw12=lw11+64
      lw13=lw12+64
      lw14=lw13+64
      lw15=lw14+256
      lw16=lw15+256
      lw17=lw16+16
      lw18=lw17+64
      lw19=lw18+64
      err=lw19-lstk(bot)
      if(err.gt.0)then
         call error(17)
         return
      endif
      maxdeg=64
      call syredi(maxdeg,ityp,iapro,stk(l3),adelp,adels,
     *     ndeg,nb,
     *     fact,stk(lw1),stk(lw2),stk(lw3),stk(lw4),stk(lw5),
     *     stk(lw6),stk(lw7),stk(lw8),stk(lw9),
     *     ierr,stk(lw10),stk(lw11),stk(lw12),stk(lw13),
     *     stk(lw14),stk(lw15),stk(lw16),stk(lw17),stk(lw18))
c     subroutine syredi(maxdeg,ityp,iapro,om,adelp,adels,
c     *                   ndeg,nb,
c     *                   fact,b2,b1,b0,c1,c0,
c     *                   zzr,zzi,zpr,zpi,
c     *                   ierr,spr,spi,pren,pimn,
c     *                   zm,sm,rom,nzero,nze)
c
c     nb=(maxdeg+1)/2
c     dimension b2,b1,b0,c1,c0 :nb
c     dimension zzr,zzi,zpr,zpi,spr,spi :maxdeg
c     dimension pren,pimn,nzero,nze :maxdeg
c     dimension zm,sm :maxdeg x 4
      if(ierr.eq.-7) then
         buf='specs=>invalid order filter'
         call error(9999)
         return
      endif
      if(ierr.eq.-9) then
         buf='specs=>too high order filter'
         call error(9999)
         return
      endif
      if(ierr.ne.0)then
         buf='error in built in fct syredi'
         call error(9999)
         return
      endif
      top=top-4
c     fact
      ilo=iadr(lstk(top))
      istk(ilo)=1
      istk(ilo+1)=1
      istk(ilo+2)=1
      length=istk(ilo+1)*istk(ilo+2)
      istk(ilo+3)=0
      lo=sadr(ilo+4)
      stk(lo)=fact
      lstk(top+1)=lo+length

c     b2
      top=top+1
      ilo=iadr(lstk(top))
      istk(ilo)=1
      istk(ilo+1)=1
      istk(ilo+2)=nb
      length=istk(ilo+1)*istk(ilo+2)
      istk(ilo+3)=0
      lo=sadr(ilo+4)
      call unsfdcopy(length,stk(lw1),1,stk(lo),1)
      lstk(top+1)=lo+length

c     b1
      top=top+1
      ilo=iadr(lstk(top))
      istk(ilo)=1
      istk(ilo+1)=1
      istk(ilo+2)=nb
      length=istk(ilo+1)*istk(ilo+2)
      istk(ilo+3)=0
      lo=sadr(ilo+4)
      call unsfdcopy(length,stk(lw2),1,stk(lo),1)
      lstk(top+1)=lo+length

c     b0
      top=top+1
      ilo=iadr(lstk(top))
      istk(ilo)=1
      istk(ilo+1)=1
      istk(ilo+2)=nb
      length=istk(ilo+1)*istk(ilo+2)
      istk(ilo+3)=0
      lo=sadr(ilo+4)
      call unsfdcopy(length,stk(lw3),1,stk(lo),1)
      lstk(top+1)=lo+length
c     c1
      top=top+1
      ilo=iadr(lstk(top))
      istk(ilo)=1
      istk(ilo+1)=1
      istk(ilo+2)=nb
      length=istk(ilo+1)*istk(ilo+2)
      istk(ilo+3)=0
      lo=sadr(ilo+4)
      call unsfdcopy(length,stk(lw4),1,stk(lo),1)
      lstk(top+1)=lo+length
c     c0
      top=top+1
      ilo=iadr(lstk(top))
      istk(ilo)=1
      istk(ilo+1)=1
      istk(ilo+2)=nb
      length=istk(ilo+1)*istk(ilo+2)
      istk(ilo+3)=0
      lo=sadr(ilo+4)
      call unsfdcopy(length,stk(lw5),1,stk(lo),1)
      lstk(top+1)=lo+length

c     zeros
      j=0
      i=0
 1001 if(j.ge.ndeg) goto 1002
      if(stk(lw7+i).eq.0.0d0)then
         j=j+1
         zzr(j)=stk(lw6+i)
         zzi(j)=0.0d0
      else
         j=j+1
         zzr(j)=stk(lw6+i)
         zzi(j)=stk(lw7+i)
         j=j+1
         zzr(j)=stk(lw6+i)
         zzi(j)=-stk(lw7+i)
      endif
      i=i+1
      goto 1001
c     poles

c      if(j.lt.ndeg)then
c         do 1003 i=j+1,ndeg
c            zzr(i)=0.d0
c            zzi(i)=0.d0
c 1003    continue
c      endif

 1002 top=top+1
      ilo=iadr(lstk(top))
      istk(ilo)=1
      istk(ilo+1)=1
      istk(ilo+2)=ndeg
      length=istk(ilo+1)*istk(ilo+2)
      istk(ilo+3)=1
      lo=sadr(ilo+4)
      call unsfdcopy(length,zzr(1),1,stk(lo),1)
      call unsfdcopy(length,zzi(1),1,stk(lo+length),1)
      lstk(top+1)=lo+2*length

c     poles
      k=0
      i=0
 1004 if(k.ge.ndeg) goto 1005
      if(stk(lw9+i).eq.0.0d0)then
         k=k+1
         zpr(k)=stk(lw8+i)
         zpi(k)=0.0d0
      else
         k=k+1
         zpr(k)=stk(lw8+i)
         zpi(k)=stk(lw9+i)
         k=k+1
         zpr(k)=stk(lw8+i)
         zpi(k)=-stk(lw9+i)
      endif
      i=i+1
      goto 1004

c      if(k.lt.ndeg)then
c         do 1005 i=k+1,ndeg
c            zpr(i)=0.0d0
c            zpi(i)=0.0d0
c 1005    continue
c      endif

 1005 top=top+1
      ilo=iadr(lstk(top))
      istk(ilo)=1
      istk(ilo+1)=1
      istk(ilo+2)=ndeg
      length=istk(ilo+1)*istk(ilo+2)
      istk(ilo+3)=1
      lo=sadr(ilo+4)
      call unsfdcopy(length,zpr(1),1,stk(lo),1)
      call unsfdcopy(length,zpi(1),1,stk(lo+length),1)
      lstk(top+1)=lo+2*length
      return
      end
c     =======================================
      
