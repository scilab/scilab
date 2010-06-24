c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c     -------------------------------
c
      subroutine intmaxi(fname,id)
c     -------------------------------
c     maxi mini interface 
c     -------------------------------
      character*(*) fname
c     Interface for maxi and mini 
      INCLUDE 'stack.h'
      integer id(nsiz)
      logical checklhs,cremat
      logical getrmat,test
      logical getilist,getlistmat,checkval
      integer gettype,itype,topk,isanan
      integer sel, mtlbsel
      integer iadr
      double precision x1
c
      iadr(l)=l+l-1
c

      topk=top
      if (.not.checklhs(fname,1,2)) return
      if (rhs.le.0) then 
         buf = fname // ' rhs must be stricly positive'
         call error(999)
         return
      endif
      itype=gettype(topk)
c     ------list case 
      if(itype.eq.15) goto 200
c     ------sparse case 
      if(itype.eq.5) then
         call ref2val
         fin=fin-6
         fun=27
c        *call* spelm
         return 
      endif
      if(itype.eq.10.and.rhs.eq.2) goto 10

      if(itype.ne.1) then
c     ------call macro 
         top=topk
         il=iadr(lstk(top))
         if(istk(il).lt.0) il=iadr(istk(il+1))

         if ((fin.eq.17).or.(fin.eq.54)) then
            call funnam(ids(1,pt+1),'mini',il)
         else
            call funnam(ids(1,pt+1),'maxi',il)
         endif
         fun=-1
         return
      endif

      if(rhs.gt.1.and.itype.ne.10) goto 100
c=====maxi mini (A1)
c     ------simple case one argument which is a matrix or vector 
      sel=0
 10   if(rhs.eq.2) then 
         call  getorient(topk,sel)
         if(err.gt.0) return
         top=top-1
      endif
      if(gettype(top).ne.1) then
         top=topk
         if((fin.eq.17) .or. (fin.eq.54)) then
            call funnam(ids(1,pt+1),'mini',iadr(lstk(top-rhs+1)))
         else
            call funnam(ids(1,pt+1),'maxi',iadr(lstk(top-rhs+1)))
         endif
         fun=-1
         return
      endif
      if(.not.getrmat(fname,topk,top,m,n,lr1)) return
      if(m*n.le.0) then
         if (.not.cremat(fname,top,0,0,0,lr,lir)) return
         if (lhs.eq.2) then
            top=top+1
            if (.not.cremat(fname,top,0,0,0,lr,lir)) return
         endif
         return
      endif
      if(sel.eq.-1) sel=mtlbsel(istk(iadr(lstk(top))+1),2)

      if (sel.eq.1) then 
c     ------------max of each column of a 
         if (.not.cremat(fname,topk,0,1,n,lr,lir)) return
         if (.not.cremat(fname,topk+1,0,1,n,lkr,lkir)) return
         if ((fin.eq.17) .or. (fin.eq.54)) then
c     .    min
            do 15 j=0,n-1
               k=idmin(m,stk(lr1+m*j),1)
               stk(lr+j)=stk(lr1+m*j+k-1)
               stk(lkr+j)=k
 15         continue
         else
c     .    max
            do 16 j=0,n-1
               k=idmax(m,stk(lr1+m*j),1)
               stk(lr+j)=stk(lr1+m*j+k-1)
               stk(lkr+j)=k
 16         continue
         endif
         call copyobj(fname,topk,topk-rhs+1)
         if (lhs.eq.2) then 
            call copyobj(fname,topk+1,topk-rhs+2)
         endif
         top=topk-rhs+lhs            
c     ---------max of each row of a
      else if (sel.eq.2) then       
         if (.not.cremat(fname,topk,0,m,1,lr,lir)) return
         if (.not.cremat(fname,topk+1,0,m,1,lkr,lkir)) return
         if(fin.eq.17) then
c     .    min
            do 25 j=0,m-1
               k=idmin(n,stk(lr1+j),m)
               stk(lr+j)=stk(lr1+j+(k-1)*m)
               stk(lkr+j)=k
 25         continue
         else
c     .    max
            do 26 j=0,m-1
               k=idmax(n,stk(lr1+j),m)
               stk(lr+j)=stk(lr1+j+(k-1)*m)
               stk(lkr+j)=k
 26         continue
         endif
         call copyobj(fname,topk,topk-rhs+1)
         if (lhs.eq.2) then 
            call copyobj(fname,topk+1,topk-rhs+2)
         endif
         top=topk-rhs+lhs            
c     ----- general maxi or mini 
      else 
         if (rhs.eq.2) topk=top

         x1=stk(lr1)
         k=1
         if ((fin.eq.17) .or. (fin.eq.54)) then 
c     .     mini
            k=idmin(m*n,stk(lr1),1)
         else
c     .     maxi
            k=idmax(m*n,stk(lr1),1)
         endif
         x1=stk(lr1-1+k)
C     return the max or min 
         if (.not.cremat(fname,topk,0,1,1,l1,li1)) return
         stk(l1)=x1
C     return indices of max or min ([k] for vectors  or [kl,kc] 
c     for matrices 
         if(lhs.eq.2) then 
            top=topk+1
            if(m.eq.1.or.n.eq.1) then 
               if (.not.cremat(fname,top,0,1,1,lr1,lc1)) return
               stk(lr1)=dble(k)
            else
               kc=k/m
               kl=k-kc*m
               if(kl.eq.0) then 
                  kc=kc-1
                  kl=m
               endif
               if (.not.cremat(fname,top,0,1,2,lr1,lc1)) return
               stk(lr1)=dble(kl)
               stk(lr1+1)=dble(kc+1)
            endif
         endif
      endif
      return
c=====maxi mini (A1,.....,An)
 100   continue
c     check argument and compute dimension of the result.
      do 101 i=1,rhs
         if(gettype(topk-rhs+i).ne.1) then
            top=topk
            il=iadr(lstk(top-rhs+i))
            if(istk(il).lt.0) il=iadr(istk(il+1))
            if(fin.eq.17) then
               call funnam(ids(1,pt+1),'mini',il)
            else
               call funnam(ids(1,pt+1),'maxi',il)
            endif
            fun=-1
            return
         endif
         if(.not.getrmat(fname,topk,topk-rhs+i,mi,ni,lri)) return

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
 101   continue



      if(.not.cremat(fname,topk+1,0,m,n,lv,lcw)) return
      if(.not.cremat(fname,topk+2,0,m,n,lind,lcw)) return
c     maxi mini a plusieurs argument
      call dset(m*n,1.0d0,stk(lind),1)
      test=getrmat(fname,topk,topk-rhs+1,mi,ni,lr1) 
      if(mi*ni.eq.1) then
         call dset(m*n,stk(lr1),stk(lv),1)
      else
         call unsfdcopy(m*n,stk(lr1),1,stk(lv),1)
      endif
      do 120 i=2,rhs
         test=getrmat(fname,topk,topk-rhs+i,mi,ni,lri)
         if(mi.eq.1.and.ni.eq.1) then
            inc=0
         else
            inc=1
         endif
         if ((fin.eq.17) .or. (fin.eq.54)) then 
c     mini            
            do 111 j=0,m*n-1
               if (stk(lri).lt.stk(lv+j).or.isanan(stk(lri)).eq.1) then 
                  stk(lv+j)= stk(lri) 
                  stk(lind+j)= dble(i)
               endif
               lri=lri+inc
 111         continue
         else
            do 112 j=0,m*n-1
               if (stk(lri).gt.stk(lv+j).or.isanan(stk(lri)).eq.1) then 
                  stk(lv+j)= stk(lri) 
                  stk(lind+j)= dble(i)
               endif
               lri=lri+inc
 112         continue
         endif
 120   continue
      call copyobj(fname,topk+1,topk-rhs+1)
      if (lhs.eq.2) then 
         call copyobj(fname,topk+2,topk-rhs+2)
      endif
      top=topk-rhs+lhs
      return
c
 200  continue
c=====maxi mini of list arguments 
      if(rhs.ne.1) then 
         buf = fname // ': only one argument if it is a list'
         call error(999)
         return
      endif
      if(.not.getilist(fname,topk,topk,n1,1,il1)) return
      if(n1.eq.0) then 
         buf = fname // ': empty list '
         call error(999)
         return
      endif
      if(.not.getlistmat(fname,topk,topk,1,it1,m,n,lr1,lc1)
     $     ) return
      if ( it1.ne.0) then 
         buf = fname // 'arguments must be real '
         call error(999)
         return 
      endif
      if(m*n.le.0) then
         err=1
         call error(45)
         return
      endif
      if(.not.cremat(fname,topk+1,0,m,n,lrw,lcw)) return
      if(.not.cremat(fname,topk+2,0,m,n,lrkw,lckw)) return
      call dset(m*n,1.0d0,stk(lrkw),1)           
      call unsfdcopy(m*n,stk(lr1),1,stk(lrw),1)
c     test si n1 > 1 
      if ( n1.gt.1) then 
         do 215 i=2,n1
            if(.not.getlistmat(fname,topk,topk,i,iti,mi,ni,
     $           lri,lci))           return
            if ( iti.ne.0) then 
               buf = fname // 'arguments must be real '
               call error(999)
               return 
            endif
            if(.not.checkval(fname,m,mi)) return
            if(.not.checkval(fname,n,ni)) return
            if ((fin.eq.17) .or. (fin.eq.54)) then 
c     mini            
               do 211 j=0,m*n-1
                  x1=stk(lri+j)
                  if (x1.lt.stk(lrw+j).or.isanan(x1).eq.1) then 
                     stk(lrw+j)=x1
                     stk(lrkw+j)= i
                  endif
 211           continue
            else
               do 212 j=0,m*n-1
                  x1=stk(lri+j)
                  if (x1.gt.stk(lrw+j).or.isanan(x1).eq.1) then 
                     stk(lrw+j)=x1
                     stk(lrkw+j)= i
                  endif
 212           continue
            endif
 215     continue
      endif
      call copyobj(fname,topk+1,topk)
      if (lhs.eq.2) then 
         call copyobj(fname,topk+2,topk+1)
      endif
      top=topk-rhs+lhs
c=====end of list case 
      return
      end
c     -------------------------------

