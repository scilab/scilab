      subroutine i_e
      include '../stack.h'
      external memused
      integer memused
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     

c     extraction
      lw=lstk(top+1)
c     
 78   continue
      if(rhs.gt.2) goto 85

      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)
      n2=istk(il2+2)
      it2=istk(il2+3)
      l2=il2+4
      mn2=m2*n2
      top=top-1
c     
      il1=iadr(lstk(top))
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=sadr(il1+4)
      mn1=m1*n1

c     arg2(arg1)
      if (istk(il1).eq.0) then
         call error(220)
         return
      endif

      if(mn2.eq.0) then 
c     .  arg2=[]
         il1=iadr(lstk(top))
         istk(il1)=1
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=sadr(il1+4)
         goto 999
      elseif(m2.lt.0) then
c     .  arg2=eye
         call error(14)
         return
      elseif(m1.lt.0) then
c     .  arg2(:), just reshape to column vector
         il1=iadr(lstk(top))
         istk(il1)=istk(il2)
         istk(il1+1)=mn2
         istk(il1+2)=1
         istk(il1+3)=istk(il2+3)
         l1=il1+4
         call gencopy(it2,mn2,istk(l2),1,istk(l1),1)
         lstk(top+1)=sadr(l1+memused(it2,mn2))
         goto 999
      endif
c     check and convert indices variable
      call indxg(il1,mn2,ilr,mi,mx,lw,1)
      if(err.gt.0) return
      if(mx.gt.mn2) then
         call error(21)
         return
      endif
 79   if(mi.eq.0) then
c     arg2([])
         il1=iadr(lstk(top))
         istk(il1)=1
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         l1=sadr(il1+4)
         lstk(top+1)=l1
         goto 999
      endif
c     get memory for the result
      il1=iadr(lstk(top))
      l1=il1+4
      if(ilr-1.le.l1+mi) then
         lr=iadr(lw)
         lw=sadr(lr+mi)
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
      else
         lr=l1
      endif
c     perform extraction
      call genextrac1(it2,mi,istk(ilr),istk(l2),istk(lr))
 81   continue
c     set output sizes
      if (m2.eq.1.and.n2.eq.1.and.m1.gt.0) then
         m = m1
         n = min(n1,mi)
      elseif (m2 .gt. 1.or.m1.lt.0) then
         m = mi
         n = 1
      else
         n = mi
         m = 1
      endif
c     form resulting variable
      istk(il1)=istk(il2)
      istk(il1+1)=m
      istk(il1+2)=n
      istk(il1+3)=it2
      if(lr.ne.l1) then
         call gencopy(it2,mi,istk(lr),1,istk(l1),1)
      endif
      lstk(top+1)=sadr(l1+memused(it2,mi))
      go to 999
c     
c     arg3(arg1,arg2)
 85   if(rhs.gt.3) then
         call error(36)
         return
      endif
c     
      il3=iadr(lstk(top))
      if(istk(il3).lt.0) il3=iadr(istk(il3+1))
      iv3=istk(il3)
      m3=istk(il3+1)
      n3=istk(il3+2)
      it3=istk(il3+3)
      l3=il3+4
      mn3=m3*n3
      top=top-1
c     
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)
      n2=istk(il2+2)
      it2=istk(il2+3)
      l2=sadr(il2+4)
      mn2=m2*n2
      top=top-1
c     
      il1=iadr(lstk(top))
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=sadr(il1+4)
      mn1=m1*n1

      if(mn3.eq.0) then 
c     .  arg3=[]
         il1=iadr(lstk(top))
         istk(il1)=1
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=sadr(il1+4)
         goto 999
      elseif(m3.lt.0) then
c     .arg3=eye
         call error(14)
         return
      endif
c     check and convert indices variables
      call indxg(il1,m3,ili,mi,mxi,lw,1)
      if(err.gt.0) return
      if(mxi.gt.m3) then
         call error(21)
         return
      endif
      call indxg(il2,n3,ilj,nj,mxj,lw,1)
      if(err.gt.0) return
      if(mxj.gt.n3) then
         call error(21)
         return
      endif
c
 90   mn=mi*nj
      if(mn.eq.0) then 
c     .  arg1=[] or arg2=[] 
         il1=iadr(lstk(top))
         istk(il1)=1
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=sadr(il1+4)
         goto 999
      endif
c     get memory for the result
      il1=iadr(lstk(top))
      l1=il1+4
      if(ili-1.le.l1+mi*nj) then
         lr=iadr(lw)
         lw=sadr(lr+mi*nj)
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
      else
c     .  the result may be installed at its final place
         lr=l1
      endif
c     perform extraction
      call genextrac2(it3,mi,nj,istk(ili),istk(ilj),istk(l3),
     $     m3,istk(lr))

c     form the resulting variable
      istk(il1)=iv3
      istk(il1+1)=mi
      istk(il1+2)=nj
      istk(il1+3)=it3
      if(lr.ne.l1) then
         call gencopy(it3,mn,istk(lr),1,istk(l1),1)
      endif
      lstk(top+1)=sadr(l1+memused(it3,mn))
      go to 999
c      
 999  return
      end

