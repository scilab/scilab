      subroutine i_mini()
c     -------------------------------
c     min interface 
c     Author: Serge Steer, Copyright INRIA
c     -------------------------------
      INCLUDE '../stack.h'
      logical checkrhs,checklhs
      integer gettype,itype,topk,sel
      integer iadr,sadr
      external memused,uppertype,mtlbsel
      integer memused,uppertype,mtlbsel
      double precision s
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      topk=top
      if (.not.checklhs(fname,1,2)) return
      if (rhs.le.0) then 
         call error(42)
         return
      endif
      itype=gettype(topk)

      if(rhs.gt.1.and.itype.ne.10) goto 100
c

c     mini(A1)
 10   sel=0
      if(rhs.eq.2) then 
         call  getorient(topk,sel)
         if(err.gt.0) return
         top=top-1
      endif

      lw=iadr(lstk(top+1))
c
      il=iadr(lstk(top))
      if(sel.eq.-1) sel=mtlbsel(istk(il+1),2)
      m=istk(il+1)
      n=istk(il+2)
      it=istk(il+3)
      lr1=il+4

      if (sel.eq.1) then 
c     ------------min of each column of a 
         mr=1
         nr=n
         job=1
      elseif(sel.eq.2) then    
         mr=m
         nr=1
         job=2
      else
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
      call genmmin(it,job,m,n,istk(lr1),m,istk(ls),istk(lk),1) 
      call gencopy(it,mnr,istk(ls),1,istk(lr1),1)
      istk(il+1)=mr
      istk(il+2)=nr
      lstk(top+1)=sadr(lr1+memused(it,mnr))
      if(lhs.eq.2) then
         top=top+1
         ilk=iadr(lstk(top))
         l=sadr(ilk+4)
         if (job.ne.0) then
            if(ilk+4.ge.lk) then
               call int2db(mnr,istk(lk),-1,stk(l),-1)
            else
               call int2db(mnr,istk(lk),1,stk(l),1)
            endif
            istk(ilk)=1
            istk(ilk+1)=mr
            istk(ilk+2)=nr
            istk(ilk+3)=0
            lstk(top+1)=l+mnr
         else
           k=istk(lk)
           kc=k/m
           kl=k-kc*m
           if(kl.eq.0) then 
              kc=kc-1
              kl=m
           endif
           err=l+2-lstk(bot)
           if(err.gt.0) then
              call error(17)
              return
           endif
            istk(ilk)=1
            istk(ilk+1)=1
            istk(ilk+2)=2
            istk(ilk+3)=0
            lstk(top+1)=l+2
            stk(l)=kl
            stk(l+1)=kc+1
         endif
      endif
      return

 100  continue
c     min(A1,A2,...,An)
c     check argument and compute dimension of the result.
      it=1
      do 101 i=1,rhs
         itype=gettype(topk-rhs+i)
         if(itype.ne.1.and.itype.ne.8) then
            top=topk
            call funnam(ids(1,pt+1),'mini',iadr(lstk(topk-rhs+i)))
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
            if (i.eq.1) then
               it=istk(ili+3)
            else
               if(it.ne.0) it=uppertype(it,istk(ili+3))
            endif
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
         call genmin2(m*n,istk(lri),inc,iti,istk(lv),it,istk(lind),i)
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
