      subroutine i_maxi()
c     -------------------------------
c     maxi mini interface 
c     -------------------------------
      character*(2) type
c     Interface for maxi and mini 
      INCLUDE '../stack.h'
      logical checkrhs,checklhs,getsmat
      integer gettype,itype,topk
      integer iadr,sadr
      external memused,uppertype
      integer memused,uppertype
      double precision s
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      type='g'//char(0)

      topk=top
      if (.not.checklhs(fname,1,2)) return
      if (rhs.le.0) then 
         call error(42)
         return
      endif
      itype=gettype(topk)

      if(rhs.gt.1.and.itype.ne.10) goto 100
c

c     maxi(A1)
 10   if(rhs.eq.2) then 
         if(.not.getsmat(fname,topk,top,m2,n2,1,1,lr2,nlr2))return
         if (nlr2.ne.1) then
            buf='max : second argument must be "c" or "r"'
            call error(999)         
            return
         endif
         call cvstr(nlr2,istk(lr2),type,1)
         top=top-1
      endif
      lw=iadr(lstk(top+1))
c
      il=iadr(lstk(top))
      m=istk(il+1)
      n=istk(il+2)
      it=istk(il+3)
      lr1=il+4

      if ( type(1:1).eq.'r') then 
c     ------------max of each column of a 
         mr=1
         nr=n
         job=1
      elseif( type(1:1).eq.'c') then    
         mr=m
         nr=1
         job=2
      elseif( type(1:1).eq.'g') then    
         mr=1
         nr=1
         job=0
      endif

      ls=lw
      mnr=mr*nr
      lk=lw+memused(it,mnr)
      err=sadr(lk+mnr+4)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call genmmax(it,job,m,n,istk(lr1),m,istk(ls),istk(lk),1) 
      call gencopy(it,mnr,istk(ls),1,istk(lr1),1)
      istk(il+1)=mr
      istk(il+2)=nr
      lstk(top+1)=sadr(lr1+memused(it,mnr))
      if(lhs.eq.2) then
         top=top+1
         ilk=iadr(lstk(top))

         l=sadr(ilk+4)
         if(ilk+4.gt.lk) then
            call int2db(mnr,istk(lk),-1,stk(l),-1)
         else
            call int2db(mnr,istk(lk),1,stk(l),1)
         endif
         istk(ilk)=1
         istk(ilk+1)=mr
         istk(ilk+2)=nr
         istk(ilk+3)=0
         lstk(top+1)=l+mnr
      endif
      return

 100  continue
c     max(A1,A2,...,An)
c     check argument and compute dimension of the result.
      it=1
      do 101 i=1,rhs
         itype=gettype(topk-rhs+i)
         if(itype.ne.1.and.itype.ne.8) then
            top=topk
            call funnam(ids(1,pt+1),'maxi',iadr(lstk(topk-rhs+i)))
            fun=-1
            return
         endif
         if (itype.eq.1) then
            ili=iadr(lstk(topk-rhs+i))
            mi=istk(ili+1)
            ni=istk(ili+2)
            it=0
         else
            ili=iadr(lstk(topk-rhs+i))
            mi=istk(ili+1)
            ni=istk(ili+2)
            if(it.ne.0) it=uppertype(it,istk(ili+3))
         endif


         if(mi*ni.le.0) then
            err=i
            call error(45)
            return
         endif
         if(i.eq.1) then
            m=mi
            n=ni
         else
            if(mi.ne.1.or.ni.ne.1) then
               if(mi.ne.m.or.ni.ne.n) then
                  if(m*n.ne.1) then
                     err=i
                     call error(42)
                     return
                  else
                     m=mi
                     n=ni
                  endif
               endif
            endif
         endif
 101  continue
c     
      lw=lstk(top+1)
c     attention lv doit pointer sur une frontiere de double precision
      llv=lw
      lv=iadr(llv)
      lind=lv+memused(it,m*n)
      lw=sadr(lind+m*n)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call iset(m*n,1,istk(lind),1)
      i=1
      ili=iadr(lstk(topk-rhs+i))
      mi=istk(ili+1)
      ni=istk(ili+2)
      iti=istk(ili+3)
      if(istk(ili).eq.1) then
         lr1=sadr(ili+4)
         if(mi*ni.eq.1) then
            call dset(m*n,stk(lr1),stk(llv),1)
         else
            call unsfdcopy(m*n,stk(lr1),1,stk(llv),1)
         endif
         lr1=ili+4
      else
         lr1=ili+4
         if(mi*ni.eq.1) then
            call tpconv(iti,it,1,istk(lr1),1,s,1)
            call genset(it,m*n,s,istk(lv),1)
         else
            call tpconv(iti,it,m*n,istk(lr1),1,istk(lv),1)
         endif
      endif
      do 120 i=2,rhs
         ili=iadr(lstk(topk-rhs+i))
         mi=istk(ili+1)
         ni=istk(ili+2)
         iti=istk(ili+3)
         if(istk(ili).eq.1) iti=0
         lri=ili+4 
         inc=1
         if(mi*ni.eq.1) inc=0
         call genmax2(m*n,istk(lri),inc,iti,istk(lv),it,istk(lind),i)
 120  continue
      top=topk-rhs+1
      il=iadr(lstk(top))
      if(it.eq.0) then
         l=sadr(il+4)
         if(l.le.llv) then
            call unsfdcopy(m*n,stk(llv),1,stk(l),1)
         else
            call unsfdcopy(m*n,stk(llv),-1,stk(l),-1)
         endif
         istk(il)=1
         istk(il+1)=m
         istk(il+2)=n
         istk(il+3)=0
         lstk(top+1)=l+m*n
      else
         l=il+4
         if(l.le.lv) then
            call gencopy(it,m*n,istk(lv),1,istk(l),1)
         else
            call gencopy(it,m*n,istk(lv),-1,istk(l),-1)
         endif
         istk(il)=8
         istk(il+1)=m
         istk(il+2)=n
         istk(il+3)=it
         lstk(top+1)=sadr(l+memused(it,m*n))
      endif
      if (lhs.eq.2) then 
         top=top+1
         il=iadr(lstk(top))
         l=sadr(il+4)
         call int2db(m*n,istk(lind),1,stk(l),1)
         istk(il)=1
         istk(il+1)=m
         istk(il+2)=n
         istk(il+3)=0
         lstk(top+1)=l+m*n
      endif
      return
      end
