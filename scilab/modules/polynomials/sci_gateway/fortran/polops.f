c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine polops
c ====================================================================
c
c     operations on polynomial matrices
c
c ====================================================================
c
      include 'stack.h'
c

      integer plus,minus,star,dstar,slash,bslash,dot,colon
      integer quote,equal,less,great,insert,extrac
c
      double precision sr,si,e1,e2,st
      integer vol,var1(4),var2(4),var3(4),var4(4),volr,rhs1,top0,op
      integer topin
      logical chkvar
      integer sadr,iadr
      integer colonid(nsiz)
c
      data plus/45/,minus/46/,star/47/,dstar/62/,slash/48/
      data bslash/49/,dot/51/,colon/44/,quote/53/
      data equal/50/,less/59/,great/60/,insert/2/,extrac/3/
      data colonid/673720364,673720360,673720360,673720360,
     $     673720360,673720360/
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      op=fin
c
      fun=0
      lw=lstk(top+1)
      topin=top

      if(op.eq.dstar) goto 80
      if(op.eq.dstar+dot) goto 70

      if(op.eq.colon) goto 200
      if(op.eq.extrac) goto 130
      if(op.eq.insert) goto 120
c

      top0=top+1-rhs
      rhs1=rhs

c
      var2(1)=0
      it2=0
      if(rhs.eq.1) goto 05
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      if(istk(il2).gt.2) goto 03
      m2=istk(il2+1)
      n2=istk(il2+2)
      it2=istk(il2+3)
      mn2=m2*n2
      if(istk(il2).ne.1) then
         id2=il2+8
         call icopy(4,istk(il2+4),1,var2,1)
         l2r=sadr(id2+mn2+1)
         vol=istk(id2+mn2)-1
         l2i=l2r+vol
         l3r=lw
         goto 03
      else
         l2r=sadr(il2+4)
         l2i=l2r+mn2
         id2=iadr(lw)
         l3r=sadr(id2+mn2+1)
         err=l3r-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call ivimp(1,mn2+1,1,istk(id2))
         l3r=l3r+1
      endif
   03 continue

      top = top-1
   05 il1=iadr(lstk(top))
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      if(istk(il1).gt.2) goto 10
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      mn1 = m1*n1
      if(istk(il1).ne.1) then
         id1=il1+8
         call icopy(4,istk(il1+4),1,var1,1)
c
         if(var2(1).eq.0) call icopy(4,var1,1,var2,1)
         if(op.ne.equal.and.op.ne.less+great) then
            if(.not.chkvar(var1,var2)) then
               fin=-fin
               top=top+1
               return
            endif
         endif
c
         l1r=sadr(id1+mn1+1)
         vol=istk(id1+mn1)-1
         l1i=l1r+vol
      else
         l1r=sadr(il1+4)
         l1i=l1r+mn1
         call icopy(4,var2,1,var1,1)
         id1=iadr(l3r)
         l3r=sadr(id1+mn1+1)
         err=l3r-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call ivimp(1,mn1+1,1,istk(id1))
         l3r=l3r+1
         lw=l3r
      endif
c
   10 it3=max(it1,it2)

c
      goto (60,120,130,65) op
      if (op .eq. quote.or.op .eq. quote+dot) goto 110
      if (rhs .eq. 1 .and. op .eq. minus) goto 101
      if (op .eq. plus .or. op .eq. minus) go to 20
      if (op .eq. star.or. op.eq.star+dot) go to 40
      if(op.eq.slash.or.op.eq.slash+dot) goto 150
      if(op.eq.bslash.or.op.eq.bslash+dot) goto 155
      if(op.eq.equal.or.op.eq.less+great) goto 160
c
c     operations non implantees
      top=top0-1+rhs
      fin=-fin
      return
c
c addition et soustraction
c
   20 continue
      vol=istk(id2+mn2)-1
      if(op.eq.minus.and.vol.gt.0) call dscal(vol*(it2+1),-1.0d+0
     $     ,stk(l2r),1)
      if(m1.eq.1.and.n1.eq.1.and.mn2.gt.1) then
c     .  p+P , p-P  
c     .  p*ones(P) is generated
         n=istk(id1+1)-istk(id1)
         istk(il1+1)=m2
         istk(il1+2)=n2
         m1=m2
         n1=n2
         mn1=m1*n1
         l=l1r
c
         id1=iadr(l3r)
         l1r=sadr(id1+mn2+1)
         vol=mn1*n
         l1i=l1r+vol
         l3r=l1i+vol*it1
         err=l3r-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         l1=l1r
         istk(id1)=1
         do 21 i=1,mn1
            istk(id1+i)=istk(id1-1+i)+n
            call unsfdcopy(n,stk(l),1,stk(l1),1)
            if(it1.eq.1) call unsfdcopy(n,stk(l+n),1,stk(l1+vol),1)
            l1=l1+n
 21      continue
      elseif(m2.eq.1.and.n2.eq.1.and.mn1.gt.1) then
c     .  P+p, P-p 
c     .  p*ones(P) is generated
         n=istk(id2+1)-istk(id2)
         m2=abs(m1)
         n2=abs(n1)
         mn2=m2*n2
         l=l2r
c
         id2=iadr(l3r)
         l2r=sadr(id2+mn1+1)
         vol=mn2*n
         l2i=l2r+vol
         l3r=l2i+vol*it2
         err=l3r-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         l2=l2r
         istk(id2)=1
         do 22 i=1,mn2
            istk(id2+i)=istk(id2-1+i)+n
            call unsfdcopy(n,stk(l),1,stk(l2),1)
            if(it2.eq.1) call unsfdcopy(n,stk(l+n),1,stk(l2+vol),1)
            l2=l2+n
 22      continue
      elseif(mn1.eq.0) then
c     .  []+P, []-P
         call icopy(9+mn2,istk(il2),1,istk(il1),1)
         l1=sadr(il1+9+mn2)
         vol=(istk(id2+mn2)-1)*(it2+1)
         call unsfdcopy(vol,stk(l2r),1,stk(l1),1)
         lstk(top+1)=l1+vol
         goto 999
      elseif(mn2.eq.0) then
c     .  P+[] , P-[]
         goto 999
      elseif(m1.lt.0) then
c     eye*p+A   
c     .  p*eye(A) is generated
         n=istk(id1+1)-istk(id1)
         istk(il1+1)=m2
         istk(il1+2)=n2
         m1=abs(m2)
         n1=abs(n2)
         mn1=m1*n1
         l=l1r

         id1=iadr(l3r)
         l1r=sadr(id1+mn2+1)
         vol=min(n1,m1)*(n-1)+mn2
         l1i=l1r+vol
         l3r=l1i+vol*it1
         err=l3r-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call dset(vol*(it1+1),0.0d+0,stk(l1r),1)
         l1=l1r
         do 23 i=1,min(n1,m1)
            call unsfdcopy(n,stk(l),1,stk(l1),1)
            if(it1.eq.1) call unsfdcopy(n,stk(l+n),1,stk(l1+vol),1)
            l1=l1+n+m1
 23      continue
         l1=id1
         istk(l1)=1
         do 25 j=1,n1
            do 24 i=1,m1
               l1=l1+1
               istk(l1)=istk(l1-1)+1
               if(i.eq.j) istk(l1)=istk(l1)+n-1
 24         continue
 25      continue
      elseif(m2.lt.0) then
c     A+eye*p 
c     .  p*eye(A) is generated
         m2=abs(m1)
         n2=abs(n1)
         l=l2r
         id=id2
         n=istk(id+1)-istk(id)
         id2=iadr(l3r)
         l2r=sadr(id2+mn1+1)
         vol=m2*(n-1)+mn1
         l2i=l2r+vol
         l3r=l2i+vol*it2
         err=l3r-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call dset(vol*(it2+1),0.0d+0,stk(l2r),1)
         l2=l2r
         do 26 i=1,min(n2,m2)
            call unsfdcopy(n,stk(l),1,stk(l2),1)
            if(it2.eq.1) call unsfdcopy(n,stk(l+n),1,stk(l2+vol),1)
            l2=l2+n+m2
 26      continue
         l2=id2
         istk(l2)=1
         do 28 j=1,n2
            do 27 i=1,m2
               l2=l2+1
               istk(l2)=istk(l2-1)+1
               if(i.eq.j) istk(l2)=istk(l2)+n-1
 27         continue
 28      continue
      elseif(m1.eq.m2.and.n1.eq.n2) then
c     .  P1+P2 P1-P2 
      else
         if (op.eq.plus) then
            call error(8)
         else
            call error(9)
         endif
         return
      endif

c     
      id3=iadr(l3r)
      l3r=sadr(id3+mn1+1)
      vol=0
      do 31 k=1,mn1
         vol=vol+max(istk(id1+k)-istk(id1+k-1),istk(id2+k)
     $        -istk(id2+k-1))
 31   continue
      l3i=l3r+vol
      err=l3i+vol*it3-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      goto (32,33,34) it1+2*it2
      call dmpad(stk(l1r),istk(id1),m1,stk(l2r),istk(id2),m2,stk(l3r),
     & istk(id3),m1,n1)
      call dmpadj(stk(l3r),istk(id3),m1,n1)
       goto 35
   32 call wdmpad(stk(l1r),stk(l1i),istk(id1),m1,stk(l2r),istk(id2),
     & m2,stk(l3r),stk(l3i),istk(id3),m1,n1)
      call wmpadj(stk(l3r),stk(l3i),istk(id3),m1,n1)
      goto 35
   33 call wdmpad(stk(l2r),stk(l2i),istk(id2),m2,stk(l1r),istk(id1),
     & m1,stk(l3r),stk(l3i),istk(id3),m1,n1)
      call wmpadj(stk(l3r),stk(l3i),istk(id3),m1,n1)
       goto 35
   34 call wmpad(stk(l1r),stk(l1i),istk(id1),m1,stk(l2r),stk(l2i),
     & istk(id2),m2,stk(l3r),stk(l3i),istk(id3),m1,n1)
      call wmpadj(stk(l3r),stk(l3i),istk(id3),m1,n1)
   35 continue
      istk(il1)=2
      istk(il1+1)=m1
      istk(il1+2)=n1
      istk(il1+3)=it3
      call icopy(4,var1,1,istk(il1+4),1)
      vol=istk(id3+m1*n1)-1
      call icopy(mn1+1,istk(id3),1,istk(il1+8),1)
      l1r=sadr(il1+9+mn1)
      call unsfdcopy(vol,stk(l3r),1,stk(l1r),1)
      if(it3.eq.1) call unsfdcopy(vol,stk(l3i),1,stk(l1r+vol),1)
      lstk(top+1)=l1r+vol*(it3+1)
      goto 999
c
c multiplications
c
 40   if(mn1.eq.0.or.mn2.eq.0) then
         istk(il1)=1
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=sadr(il1+4)+1
         goto 999
      endif
      indef=0
      if(m1.lt.0) then
         if(mn2.eq.1) then
            indef=1
         else
            call error(14)
            return
         endif
      elseif(m2.lt.0) then
         if(mn1.eq.1) then
            indef=1
         else
            call error(14)
            return
         endif
      endif
      m1=abs(m1)
      n1=abs(n1)
      m2=abs(m2)
      n2=abs(n2)
      if(mn1.gt.1.and.mn2.gt.1 .and. op.gt.dot) then
c     .  a.*b 
         if(m1.ne.m2.or.n1.ne.n2) then
            call error(10)
            return
         endif
         m3=m1
         n3=n1
         n1=0
         vol=istk(id1+mn1)+istk(id2+mn2)-mn2-2
      elseif(mn1.eq.1) then
c     .  cst*b or cst.*b
         m1=0
         n1=m2
         m3=m2
         n3=n2
         vol=istk(id2+mn2)-1+mn2*(istk(id1+1)-2)
      elseif(mn2.eq.1) then
c     .  a*cst or a.*cst
         n2=0
         m3=m1
         n3=n1
         vol=istk(id1+mn1)-1+mn1*(istk(id2+1)-2)
      else
c     .  a*b
         if(n1.ne.m2) then
            call error(10)
            return
         endif
         m3=m1
         n3=n2
c     .  estimate result size
         vol=0
         do 46 i=1,m3
            j1=id2-m2
            do 45 j=1,n3
               j1=j1+m2
               k1=id1-m1
               mx=0
               do 44 k=1,n1
                  k1=k1+m1
                  ll1=istk(i+k1)-istk(i-1+k1)
                  ll2=istk(k+j1)-istk(k-1+j1)
                  mx=max(mx,ll1+ll2)
 44            continue
               vol=vol+mx-1
 45         continue
 46      continue
      endif

c
      id3=iadr(l3r)
      l3r=sadr(id3+m3*n3+1)
      l3i=l3r+vol
      err=l3i+it3*vol-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c
      m=max(1,m1)
      goto (51,52,53) it1+2*it2
      call dmpmu(stk(l1r),istk(id1),m,stk(l2r),istk(id2),m2,
     & stk(l3r),istk(id3),m1,n1,n2)
      call dmpadj(stk(l3r),istk(id3),m3,n3)
      goto 55
   51 call wdmpmu(stk(l1r),stk(l1i),istk(id1),m,stk(l2r),istk(id2),
     & m2,stk(l3r),stk(l3i),istk(id3),m1,n1,n2)
      call wmpadj(stk(l3r),stk(l3i),istk(id3),m3,n3)
      goto 55
   52 continue
      call dwmpmu(stk(l1r),istk(id1),m,stk(l2r),stk(l2i),istk(id2),
     & m2,stk(l3r),stk(l3i),istk(id3),m1,n1,n2)
      call wmpadj(stk(l3r),stk(l3i),istk(id3),m3,n3)
      goto 55
   53 call wmpmu(stk(l1r),stk(l1i),istk(id1),m,stk(l2r),stk(l2i),
     & istk(id2),m2,stk(l3r),stk(l3i),istk(id3),m1,n1,n2)
      call wmpadj(stk(l3r),stk(l3i),istk(id3),m3,n3)
      goto 55
c
   55 if(istk(il1).eq.1) id1=il1+8
      l1r=sadr(id1+m3*n3+1)
      call icopy(m3*n3+1,istk(id3),1,istk(id1),1)
      vol=istk(id1+m3*n3)-1
      call unsfdcopy(vol,stk(l3r),1,stk(l1r),1)
      if(it3.eq.1) call unsfdcopy(vol,stk(l3i),1,stk(l1r+vol),1)
      lstk(top+1)=l1r+vol*(it3+1)
      istk(il1)=2
      istk(il1+1)=m3
      istk(il1+2)=n3
      istk(il1+3)=it3
      call icopy(4,var1,1,istk(il1+4),1)
      
      if(indef.eq.0) goto 999
      istk(il1+1)=-1
      istk(il1+2)=-1
      goto 999
c
c concatenation [a b]
c
   60 continue
      if(m1.lt.0.or.m2.lt.0) then
            call error(14)
            return
      endif
      if(m2.eq.0) then
         return
      elseif(m1.eq.0)then
         call unsfdcopy(lstk(top+2)-lstk(top+1),stk(lstk(top+1)),1,
     &        stk(lstk(top)),1)
         lstk(top+1)=lstk(top)+lstk(top+2)-lstk(top+1)
         return
      elseif(m1.ne.m2) then
         call error(5)
         return
      endif
c
      id3=iadr(l3r)
      l3r=sadr(id3+mn1+mn2+1)
      vol=istk(id1+mn1)+istk(id2+mn2)-2
      l3i=l3r+vol
      lw=l3i+vol*it3
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      goto (61,62,63) it1+2*it2
      call dmpcnc(stk(l1r),istk(id1),m1,stk(l2r),istk(id2),m1,
     & stk(l3r),istk(id3),m1,n1,n2,1)
      goto 64
 61   call wmpcnc(stk(l1r),stk(l1i),istk(id1),m1,stk(l2r),st,
     & istk(id2),m1,stk(l3r),stk(l3i),istk(id3),m1,n1,n2,3)
      goto 64
 62   call wmpcnc(stk(l1r),st,istk(id1),m1,stk(l2r),stk(l2i),
     & istk(id2),m1,stk(l3r),stk(l3i),istk(id3),m1,n1,n2,2)
      goto 64
 63   call wmpcnc(stk(l1r),stk(l1i),istk(id1),m1,stk(l2r),stk(l2i),
     & istk(id2),m1,stk(l3r),stk(l3i),istk(id3),m1,n1,n2,1)
      goto 64
c
 64   istk(il1)=2
      istk(il1+1)=m1
      istk(il1+2)=n1+n2
      istk(il1+3)=it3
      call icopy(mn1+mn2+1,istk(id3),1,istk(il1+8),1)
      l1r=sadr(il1+9+mn1+mn2)
      call unsfdcopy(vol*(it3+1),stk(l3r),1,stk(l1r),1)
      call icopy(4,var1,1,istk(il1+4),1)
      lstk(top+1)=l1r+vol*(it3+1)
      goto 999
c
c     concatenation [a;b]
 65   if(n1.lt.0.or.n2.lt.0) then
            call error(14)
            return
      endif
      if(n2.eq.0) then
         goto 999
      elseif(n1.eq.0)then
         call unsfdcopy(lstk(top+2)-lstk(top+1),stk(lstk(top+1)),1,
     &        stk(lstk(top)),1)
         lstk(top+1)=lstk(top)+lstk(top+2)-lstk(top+1)
         goto 999
      elseif(n1.ne.n2) then
         call error(6)
         return
      endif
      m=m1+m2
      mn=m*n1
c
      id3=iadr(l3r)
      l3r=sadr(id3+mn+1)
      vol=istk(id1+mn1)+istk(id2+mn2)-2
      l3i=l3r+vol
      lw=l3i+vol*it3
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      goto (66,67,68) it1+2*it2
      call dmpcnc(stk(l1r),istk(id1),m1,stk(l2r),istk(id2),m2,
     & stk(l3r),istk(id3),m1,m2,n2,-1)
      goto 69
 66   call wmpcnc(stk(l1r),stk(l1i),istk(id1),m1,stk(l2r),st,
     & istk(id2),m2,stk(l3r),stk(l3i),istk(id3),m1,m2,n2,-3)
      goto 69
 67   call wmpcnc(stk(l1r),st,istk(id1),m1,stk(l2r),stk(l2i),
     & istk(id2),m2,stk(l3r),stk(l3i),istk(id3),m1,m2,n2,-2)
      goto 69
 68   call wmpcnc(stk(l1r),stk(l1i),istk(id1),m1,stk(l2r),stk(l2i),
     & istk(id2),m2,stk(l3r),stk(l3i),istk(id3),m1,m2,n2,-1)
      goto 69
c
 69   istk(il1)=2
      istk(il1+1)=m
      istk(il1+2)=n1
      istk(il1+3)=it3
      call icopy(mn1+mn2+1,istk(id3),1,istk(il1+8),1)
      l1r=sadr(il1+9+mn1+mn2)
      call unsfdcopy(vol*(it3+1),stk(l3r),1,stk(l1r),1)
      call icopy(4,var1,1,istk(il1+4),1)
      lstk(top+1)=l1r+vol*(it3+1)
      goto 999
c
c puissance .^
c
   70 il2=iadr(lstk(top))
      m2=istk(il2+1)
      n2=istk(il2+2)
      top=top-1
      if(istk(il2).ne.1.or.istk(il2+3).ne.0) then
         call error(30)
         return
      endif
      mn2=istk(il2+1)*istk(il2+2)
      if (mn2.eq.0) then
         il1=iadr(lstk(top))
         istk(il1)=1
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=sadr(il1+4)+1
         return
      endif
      l2=sadr(il2+4)
c
      il1=iadr(lstk(top))
      m1=abs(istk(il1+1))
      n1=abs(istk(il1+2))
      it1=istk(il1+3)
      mn1=m1*n1
      id1=il1+8
      l1r=sadr(id1+mn1+1)
      vol=istk(id1+mn1)-1
      l1i=l1r+vol
c
      if (mn2 .gt. 1) then
         m=m2
         n=n2
         inc2=1
         if (mn1 .eq.1) then
            inc1=0
         elseif(m1.eq.m2.and.n1.eq.n2) then
            inc1=1
         else
            call error(30)
            return
         endif
      else
         inc2=0
         inc1=1
         m=m1
         n=n1
      endif
      mn=m*n
c
      idr=iadr(lw)
      lr=sadr(idr+mn+1)
      err=lr-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      istk(idr)=1
      do 71 i=0,mn-1
         i1=i*inc1
         i2=i*inc2
         nexp=int(stk(l2+i2))
         if(dble(nexp).ne.stk(l2+i2)) then
            call error(30)
            return
         elseif(nexp.lt.0) then
            fin=-fin
            top=top+1
            return
         endif
         istk(idr+i+1)=istk(idr+i)+
     $        (istk(id1+i1+1)-istk(id1+i1)-1)*nexp+1
 71   continue

      nr=istk(idr+mn)-1
      err=lr+nr*(it1+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif

      do 73 i=1,mn
         i1=(i-1)*inc1
         i2=(i-1)*inc2
         nexp=int(stk(l2+i2))
         l1=l1r+istk(id1+i1)-1
         l=lr+istk(idr+i-1)-1

         ni=istk(id1+i1+1)-istk(id1+i1)
         if(nexp.gt.0) then
            call unsfdcopy(ni,stk(l1),1,stk(l),1)
            if(it1.eq.1) call unsfdcopy(ni,stk(l1+vol),1,stk(l+nr),1)
         else
            stk(l)=1.0d0
            if(it1.eq.1) stk(l+nr)=0.0d0
         endif
         mi=ni-1
         if(nexp.gt.1) then
            do 72 ne=2,nexp
               if(it1.eq.0) then
                  call dpmul1(stk(l),mi,stk(l1),ni-1,stk(l))
               else
                  call wpmul1(stk(l),stk(l+nr),mi,stk(l1),
     &                 stk(l1+vol),ni-1,stk(l),stk(l+nr))
               endif
               mi=mi+ni-1
 72         continue
         endif
 73   continue
      istk(il1+1)=m
      istk(il1+2)=n
      istk(il1+3)=it1
      call icopy(mn+1,istk(idr),1,istk(id1),1)
      l1=sadr(id1+mn+1)
      call unsfdcopy(nr*(it1+1),stk(lr),1,stk(l1),1)
      lstk(top+1)=l1+nr*(it1+1)
      goto 999

c puissance de matrice
   80 continue
      il2=iadr(lstk(top))
      mn2=istk(il2+1)*istk(il2+2)
      il1=iadr(lstk(top-1))
      m1=abs(istk(il1+1))
      n1=abs(istk(il1+2))
      it1=istk(il1+3)
      mn1=m1*n1

      if(mn1.eq.1) goto 70
      if(mn2.gt.1) then
         fin=-fin
         return
      endif
      if(m1.ne.n1) then
         if(mn2.eq.1.and.(m1.eq.1.or.n1.eq.1)) goto 70
         err=1
         call error(20)
         return
      endif
      fin=-fin
      return
c
  101 vol=istk(id1+mn1)-1
c multiplication par -1
      call dscal(vol*(it1+1),-1.0d+0,stk(l1r),1)
      goto 999
c
c transposition
  110 continue
      vol=istk(id1+mn1)-1
      if(abs(m1).eq.1.or.abs(n1).eq.1) then
         if(it1.eq.1.and.op.ne.quote+dot) then
            call dscal(vol,-1.0d0,stk(l1i),1)
         endif
         istk(il1+1)=n1
         istk(il1+2)=m1
         goto 999
      endif

      id2=iadr(lstk(top+1))
      l2r=sadr(id2+mn1+1)
      l2i=l2r+vol
      err=l2r+vol*(it1+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      if(it1.eq.1) goto 111
      call dmptra(stk(l1r),istk(id1),m1,stk(l2r),istk(id2),m1,n1)
      goto 112
  111 call wmptra(stk(l1r),stk(l1i),istk(id1),m1,stk(l2r),stk(l2i),
     & istk(id2),m1,n1)
  112 istk(il1+1)=n1
      istk(il1+2)=m1
      call icopy(mn1+1,istk(id2),1,istk(id1),1)
      call unsfdcopy(vol*(it1+1),stk(l2r),1,stk(l1r),1)
      if (it1.eq.1.and.op.ne.quote+dot) then
         call dscal(vol,-1.0d0,stk(l1r+vol),1)
      endif
      goto 999
c
c
c     insertion
c
  120 continue
      if(rhs.gt.4) then
         top=topin
         fin=-fin
         return
      endif
      if(rhs.eq.4) goto 124
c     arg3(arg1)=arg2
c     
c     get arg3
      var3(1)=0
      il3=iadr(lstk(top))
      if(istk(il3).lt.0) il3=iadr(istk(il3+1))
      m3=istk(il3+1)
      n3=istk(il3+2)
      it3=istk(il3+3)
      mn3=m3*n3
      if(istk(il3).eq.2) then
         id3=il3+8
         call icopy(4,istk(il3+4),1,var3,1)
         l3r=sadr(id3+mn3+1)
         vol=istk(id3+mn3)-1
         l3i=l3r+vol
      elseif(istk(il3).eq.1) then
         l3r=sadr(il3+4)
         l3i=l3r+mn3
         id3=iadr(lw)
         lw=sadr(id3+mn3+1)
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call ivimp(1,mn3+1,1,istk(id3))
      endif
c     get arg2
      top=top-1
      var2(1)=0
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      if(istk(il2).eq.0) then
         call error(220)
         return
      endif
      m2=istk(il2+1)
      n2=istk(il2+2)
      it2=istk(il2+3)
      mn2=m2*n2
      if(istk(il2).eq.2) then
         id2=il2+8
         call icopy(4,istk(il2+4),1,var2,1)
         l2r=sadr(id2+mn2+1)
         vol=istk(id2+mn2)-1
         l2i=l2r+vol
      else
         l2r=sadr(il2+4)
         l2i=l2r+mn2
         id2=iadr(lw)
         lw=sadr(id2+mn2+1)
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call ivimp(1,mn2+1,1,istk(id2))
      endif
      if(var2(1).eq.0) call icopy(4,var3,1,var2,1)
      if(.not.chkvar(var2,var3)) then
         fin=-fin
         top=top+1
         call error(278)
         return
      endif
c     get arg1
      top=top-1
      il1=iadr(lstk(top))
      ilrs=il1
      if(il1.lt.0) il1=iadr(istk(il1))
      if(istk(il1).eq.0) then
         call error(220)
         return
      endif
      if (istk(il1).eq.10.or.istk(il1).eq.15) then
         top=topin
         fin=-fin
         return
      endif
      m1=istk(il1+1)
      n1=istk(il1+2)
c
      if (m2.eq.0) then
c     .  arg3(arg1)=[] 
         if(m1.eq.-1) then
c     .    arg3(:)=[] -->[]
            istk(ilrs)=1
            istk(ilrs+1)=0
            istk(ilrs+2)=0
            istk(ilrs+3)=0
            lstk(top+1)=sadr(ilrs+4)+1
            goto 999
         elseif(m1.eq.0) then
c     .     arg3([])=[]  --> arg3
            volr=istk(id3+mn3)-1
            istk(ilrs)=2
            istk(ilrs+1)=m3
            istk(ilrs+2)=n3
            istk(ilrs+3)=it3
            call icopy(4,var2,1,istk(ilrs+4),1)
            call icopy(mn3+1,istk(id3),1,istk(ilrs+8),1)
            l1=sadr(ilrs+9+mn3)
            call unsfdcopy(volr*(it3+1),stk(l3r),1,stk(l1),1)
            lstk(top+1)=l1+volr*(it3+1)
            goto 999
         else
c     .     arg3(arg1)=[] --> arg3(compl(arg1))
            call indxgc(il1,mn3,ilr,mi,mx,lw)
            if(err.gt.0) return
            l2r=l3r
            l2i=l3i
            n2=n3
            m2=m3
            mn2=m2*n2
            it2=it3
            id2=id3
            ili=ilr
c     .     call extraction
            goto 131
         endif
      elseif(m2.lt.0.or.m3.lt.0) then
c     .  arg3=eye,arg2=eye
         call error(14)
         return
      elseif(m1.lt.0) then
c     .  arg3(:)=arg2
         if(mn2.ne.mn3) then
            if(mn2.eq.1) goto 121
            call error(15)
            return
         endif
c     .  reshape arg2 according to arg3
         volr=lstk(top+2)-lstk(top+1)
c     .  copy arg2
         call unsfdcopy(volr,stk(lstk(top+1)),1,stk(lstk(top)),1)

c     .  change dimensions
         istk(ilrs+1)=m3
         istk(ilrs+2)=n3
         lstk(top+1)=lstk(top)+volr
         goto 999
      endif
 121  call indxg(il1,mn3,ili,mi,mxi,lw,1)
      if(err.gt.0) return
      if(mi.eq.0) then
c     .  arg3([])=arg2 
         if(mn2.eq.1) then
c     .  arg3([])=c  --> arg3
            volr=istk(id3+mn3)-1
            istk(ilrs)=2
            istk(ilrs+1)=m3
            istk(ilrs+2)=n3
            istk(ilrs+3)=it3
            call icopy(4,var2,1,istk(ilrs+4),1)
            call icopy(mn3+1,istk(id3),1,istk(ilrs+8),1)
            l1=sadr(ilrs+9+mn3)
            call unsfdcopy(volr*(it3+1),stk(l3r),1,stk(l1),1)
            lstk(top+1)=l1+volr*(it3+1)
            goto 999
         else
            call error(15)
            return
         endif  
      endif
      inc2=1
      if(mi.ne.mn2) then
         if(mn2.eq.1) then
            inc2=0
         else
            call error(15)
            return
         endif
      endif
c
      if (n3.gt.1.and.m3.gt.1) then
c     .  arg3 is not a vector
         if(n2.gt.1.and.m2.gt.1) then
            call error(15)
            return
         endif
         if(mxi.gt.m3*n3) then
            call error(21)
            return
         endif
         mr=m3
         nr=n3
      elseif (n3.le.1.and.n2.le.1) then
c     .  arg3 and arg2 are  column vectors
         mr=max(m3,mxi)
         nr=max(n3,1)

      elseif (m3.le.1.and.m2.le.1) then
c     .  row vectors
         nr=max(n3,mxi)
         mr=max(m3,1)
      else
c     .  arg3 and arg2 dimensions dont agree
         call error(15)
         return
      endif
c
      itr=max(it2,it3)
      mnr=mr*nr
c     set result pointers
      idr=iadr(lw)
      lr=sadr(idr+mr*nr+1)
      lw=lr
      err=lr-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call mpinsp(istk(id3),m3*n3,1,istk(ili),mi,1,1,istk(id2),m2*n2,1
     $     ,istk(idr),mr*nr,1,err)
      if(err.gt.0) then
         call error(15)
         return
      endif
      volr=istk(idr)
c     set result coefficients
      li=lr+volr
      lw=li+volr*itr
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c     
      if(it2.eq.0) then
         if(it3.eq.0) then
            call dmpins(stk(l3r),istk(id3),m3*n3,1,stk(l2r),istk(id2),
     $           m2*n2,1,stk(lr),istk(idr),mr*nr,1)
         else
            l2i=lw
            lw=l2i+istk(id2+mn2)
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call dset(istk(id2+mn2)-1,0.0d+0,stk(l2i),1)
            call wmpins(stk(l3r),stk(l3i),istk(id3),m3*n3,1,
     $           stk(l2r),stk(l2i),istk(id2),m2*n2,1,stk(lr),
     $           stk(lr+volr),istk(idr),mr*nr,1)
         endif
      else
         if(it3.eq.0) then
            l3i=lw
            lw=l3i+istk(id3+mn3)
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call dset(istk(id3+mn3)-1,0.0d+0,stk(l3i),1)
            call wmpins(stk(l3r),stk(l3i),istk(id3),m3*n3,1,
     $           stk(l2r),stk(l2i),istk(id2),m2*n2,1,stk(lr),
     $           stk(lr+volr),istk(idr),mr*nr,1)
         else
            call wmpins(stk(l3r),stk(l3i),istk(id3),m3*n3,1,
     $           stk(l2r),stk(l2i),istk(id2),m2*n2,1,stk(lr),
     $           stk(lr+volr),istk(idr),mr*nr,1)
         endif
      endif
c     set output variable
      il1=iadr(lstk(top))
      istk(il1)=2
      istk(il1+1)=mr
      istk(il1+2)=nr
      istk(il1+3)=itr
      call icopy(4,var2,1,istk(il1+4),1)
      call icopy(mr*nr+1,istk(idr),1,istk(il1+8),1)
      l1=sadr(il1+mr*nr+9)
      call unsfdcopy(volr*(itr+1),stk(lr),1,stk(l1),1)
      lstk(top+1)=l1+volr*(itr+1)
      goto 999
c
 124  continue
c     arg4(arg1,arg2)=arg3
c     get arg4
      var4(1)=0
      il4=iadr(lstk(top))
      if(istk(il4).lt.0) il4=iadr(istk(il4+1))
      m4=istk(il4+1)
      n4=istk(il4+2)
      it4=istk(il4+3)
      mn4=m4*n4
      if(istk(il4).eq.2) then
         id4=il4+8
         call icopy(4,istk(il4+4),1,var4,1)
         l4r=sadr(id4+mn4+1)
         vol=istk(id4+mn4)-1
         l4i=l4r+vol
      else
         l4r=sadr(il4+4)
         l4i=l4r+mn4
         id4=iadr(lw)
         lw=sadr(id4+mn4+1)
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call ivimp(1,mn4+1,1,istk(id4))
      endif
      top=top-1
c     get arg3
      var3(1)=0
      il3=iadr(lstk(top))
      if(istk(il3).lt.0) il3=iadr(istk(il3+1))
      if(istk(il3).eq.0) then
         call error(220)
         return
      endif
      m3=istk(il3+1)
      n3=istk(il3+2)
      it3=istk(il3+3)
      mn3=m3*n3
      if(istk(il3).eq.2) then
         id3=il3+8
         call icopy(4,istk(il3+4),1,var3,1)
         l3r=sadr(id3+mn3+1)
         vol=istk(id3+mn3)-1
         l3i=l3r+vol
      else
         l3r=sadr(il3+4)
         l3i=l3r+mn3
         id3=iadr(lw)
         lw=sadr(id3+mn3+1)
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call ivimp(1,mn3+1,1,istk(id3))
      endif
      if(var3(1).eq.0) call icopy(4,var4,1,var3,1)
      if(.not.chkvar(var3,var4)) then
         fin=-fin
         top=top+2
      endif
c     get arg2
      top=top-1
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      if(istk(il2).eq.0) then
         call error(220)
         return
      endif
      m2=istk(il2+1)
c     get arg1
      top=top-1
      il1=iadr(lstk(top))
      ilrs=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      if(istk(il1).eq.0) then
         call error(220)
         return
      endif
      m1=istk(il1+1)

      if (m3.eq.0) then
c     .  arg4(arg1,arg2)=[]
         if(m1.eq.-1.and.m2.eq.-1) then
c     .    arg4(:,:)=[] -->[]
            istk(ilrs)=1
            istk(ilrs+1)=0
            istk(ilrs+2)=0
            istk(ilrs+3)=0
            lstk(top+1)=sadr(ilrs+4)+1
            goto 999
         elseif(m1.eq.0.or.m2.eq.0) then
c     .     arg4([],arg2)=[],  arg4(arg1,[])=[] --> arg4
            volr=istk(id4+mn4)-1
            istk(ilrs)=2
            istk(ilrs+1)=m4
            istk(ilrs+2)=n4
            istk(ilrs+3)=it4
            call icopy(4,var3,1,istk(ilrs+4),1)
            call icopy(mn4+1,istk(id4),1,istk(ilrs+8),1)
            l1=sadr(ilrs+9+mn4)
            call unsfdcopy(volr*(it4+1),stk(l4r),1,stk(l1),1)
            lstk(top+1)=l1+volr*(it4+1)
            goto 999
         elseif(m2.eq.-1) then
c     .     arg4(arg1,:)=[] --> arg3(compl(arg1),:)
            call indxgc(il1,m4,ili,mi,mxi,lw)
            if(err.gt.0) return
            call indxg(il2,n4,ilj,nj,mxj,lw,1)
            if(err.gt.0) return
            l3r=l4r
            l3i=l4i
            n3=n4
            m3=m4
            mn3=m3*n3
            it3=it4
            id3=id4
c     .     call extraction
            goto 133
         elseif(m1.eq.-1) then
c     .     arg4(:,arg2)=[] --> arg4(:,compl(arg2))
            call indxgc(il2,n4,ilj,nj,mxj,lw)
            if(err.gt.0) return
            call indxg(il1,m4,ili,mi,mxi,lw,1)
            if(err.gt.0) return
            l3r=l4r
            l3i=l4i
            n3=n4
            m3=m4
            mn3=m3*n3
            it3=it4
            id3=id4
c     .     call extraction
            goto 133
         else
c     .     arg4(arg1,arg2)=[] 
            lw1=lw
            call indxgc(il2,n4,ilj,nj,mxj,lw)
            if(err.gt.0) return
            if(nj.eq.0) then
c     .        arg4(arg1,1:n4)=[] 
               call indxgc(il1,m4,ili,mi,mxi,lw)
               lw2=lw
               if(err.gt.0) return
c     .        arg2=1:n4
               if(mi.eq.0) then
c     .           arg4(1:m4,1:n4)=[] 
                  istk(ilrs)=1
                  istk(ilrs+1)=0
                  istk(ilrs+2)=0
                  istk(ilrs+3)=0
                  lstk(top+1)=sadr(ilrs+4)+1
                  goto 999
               else
c     .           arg4(arg1,1:n4)=[] 
c     .           replace arg2 by ":"
                  il2=iadr(lw2)
                  istk(il2)=1
                  istk(il2+1)=-1
                  istk(il2+2)=-1
                  istk(il2+3)=0
c     .
                  lw=lw2+2
                  call indxg(il2,n4,ilj,nj,mxj,lw,1)
                  if(err.gt.0) return
                  l3r=l4r
                  n3=n4
                  m3=m4
                  it3=it4
                  mn3=m3*n3
                  id3=id4
c     .     call extraction
                  goto 133
               endif
            elseif(nj.eq.n4) then
c              arg4(arg1,[])=[] --> arg4
               volr=istk(id4+mn4)-1
               istk(ilrs)=2
               istk(ilrs+1)=m4
               istk(ilrs+2)=n4
               istk(ilrs+3)=it4
               call icopy(4,var3,1,istk(ilrs+4),1)
               call icopy(mn4+1,istk(id4),1,istk(ilrs+8),1)
               l1=sadr(ilrs+9+mn4)
               call unsfdcopy(volr*(it4+1),stk(l4r),1,stk(l1),1)
               lstk(top+1)=l1+volr*(it4+1)
               goto 999
            else
c               lw=lw1
               call indxgc(il1,m4,ili,mi,mxi,lw)
               if(err.gt.0) return
               if(mi.eq.0) then
                  call indxg(il1,m4,ili,mi,mxi,lw,1)
                  if(err.gt.0) return
                  l3r=l4r
                  n3=n4
                  m3=m4
                  it3=it4
                  mn3=m3*n3
                  id3=id4
c     .     call extraction
                  goto 133
               elseif(mi.eq.m4) then
c     arg4([],arg2)=[] --> arg4
                  volr=istk(id4+mn4)-1
                  istk(ilrs)=2
                  istk(ilrs+1)=m4
                  istk(ilrs+2)=n4
                  istk(ilrs+3)=it4
                  call icopy(4,var3,1,istk(ilrs+4),1)
                  call icopy(mn4+1,istk(id4),1,istk(ilrs+8),1)
                  l1=sadr(ilrs+9+mn4)
                  call unsfdcopy(volr*(it4+1),stk(l4r),1,stk(l1),1)
                  lstk(top+1)=l1+volr*(it4+1)
                  goto 999
               else
                  call error(15)
                  return
               endif
            endif
         endif
      elseif(m3.lt.0.or.m4.lt.0) then
c     .  arg3=eye , arg4=eye
         call error(14)
         return
      elseif(m1.eq.-1.and.m2.eq.-1) then
c     .  arg4(:,:)=arg3
         if(mn3.ne.mn4) then
            if(mn3.eq.1) goto 125
            call error(15)
            return
         endif
c     .  reshape arg3 according to arg4
         volr=istk(id3+mn3)-1
         istk(ilrs)=2
         istk(ilrs+1)=m4
         istk(ilrs+2)=n4
         istk(ilrs+3)=it3
         call icopy(4,var3,1,istk(ilrs+4),1)
         call icopy(mn3+1,istk(id3),1,istk(ilrs+8),1)
         l1=sadr(ilrs+9+mn3)
         call unsfdcopy(volr*(it3+1),stk(l3r),1,stk(l1),1)
         lstk(top+1)=l1+volr*(it3+1)
         goto 999
      endif

 125  continue
      init4=0
      if(m1.eq.-1.and.m4.eq.0) then
c     .  arg4(:,i)=arg3
         m3=m3*n3
         n3=1
         n4=1
         m4=m3
         init4=1
         
      elseif(m2.eq.-1.and.m4.eq.0) then
c     .  arg4(i,:)=arg3
         n3=m3*n3
         m3=1
         m4=1
         n4=n3
         init4=1
      endif
      if(init4.eq.1) then
      
         mn4=m4*n4
         l4r=lw
         l4i=l4r+mn4
         id4=iadr(l4i)
         lw=sadr(id4+mn4+1)
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call dset(mn4,0.0d0,stk(l4r),1)
         call ivimp(1,mn4+1,1,istk(id4))
      endif
      call indxg(il1,m4,ili,mi,mxi,lw,1)
      if(err.gt.0) return
      call indxg(il2,n4,ilj,mj,mxj,lw,1)
      if(err.gt.0) return
      if(mi.ne.m3.or.mj.ne.n3) then
c     .  sizes of arg1 or arg2 dont agree with arg3 sizes
         inc3=1
         if(mn3.eq.1) then
            if(mi.eq.0.or.mj.eq.0) then 
               volr=istk(id4+mn4)-1
               istk(ilrs)=2
               istk(ilrs+1)=m4
               istk(ilrs+2)=n4
               istk(ilrs+3)=it4
               call icopy(4,var3,1,istk(ilrs+4),1)
               call icopy(mn4+1,istk(id4),1,istk(ilrs+8),1)
               l1=sadr(ilrs+9+mn4)
               call unsfdcopy(volr*(it4+1),stk(l4r),1,stk(l1),1)
               lstk(top+1)=l1+volr*(it4+1)
               goto 999
            endif
            inc3=0
         else
            call error(15)
            return
         endif
      else
         if(mi.eq.0.or.mj.eq.0) then
            call error(15)
            return
         endif
      endif
      mr=max(m4,mxi)
      nr=max(n4,mxj)
c
      mnr=mr*nr
      itr=max(it4,it3)
c     set result pointers
      idr=iadr(lw)
      lr=sadr(idr+mr*nr+1)
      lw=lr
      err=lr-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call mpinsp(istk(id4),m4,n4,istk(ili),mi,istk(ilj),mj,istk(id3)
     $     ,m3,n3,istk(idr),mr,nr,err)
      if(err.gt.0) then
         call error(15)
         return
      endif
      volr=istk(idr)
c     set result coefficients
      li=lr+volr
      lw=li+volr*itr
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c     
      if(it3.eq.0) then
         if(it4.eq.0) then
            call dmpins(stk(l4r),istk(id4),m4,n4,stk(l3r),istk(id3),
     $           m3,n3,stk(lr),istk(idr),mr,nr)
         else
            l3i=lw
            lw=l3i+istk(id3+mn3)
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call dset(istk(id3+mn3),0.0d+0,stk(l3i),1)
            call wmpins(stk(l4r),stk(l4i),istk(id4),m4,n4,
     $           stk(l3r),stk(l3i),istk(id3),m3,n3,stk(lr),
     $           stk(lr+volr),istk(idr),mr,nr)
         endif
      else
         if(it4.eq.0) then
            l4i=lw
            lw=l4i+istk(id4+mn4)
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call dset(istk(id4+mn4),0.0d+0,stk(l4i),1)
            call wmpins(stk(l4r),stk(l4i),istk(id4),m4,n4,
     $           stk(l3r),stk(l3i),istk(id3),m3,n3,stk(lr),
     $           stk(lr+volr),istk(idr),mr,nr)
         else
            call wmpins(stk(l4r),stk(l4i),istk(id4),m4,n4,
     $           stk(l3r),stk(l3i),istk(id3),m3,n3,stk(lr),
     $           stk(lr+volr),istk(idr),mr,nr)
         endif
      endif
c     set output variable
      il1=iadr(lstk(top))
      istk(il1)=2
      istk(il1+1)=mr
      istk(il1+2)=nr
      istk(il1+3)=itr
      call icopy(4,var3,1,istk(il1+4),1)
      call icopy(mr*nr+1,istk(idr),1,istk(il1+8),1)
      l1=sadr(il1+mr*nr+9)
      call unsfdcopy(volr*(itr+1),stk(lr),1,stk(l1),1)
      lstk(top+1)=l1+volr*(itr+1)
      goto 999

      
c     
c extraction
  130 continue
      if(rhs.lt.2) then
         call error(227)
         return
      endif
      if(rhs.eq.3) goto 132
      if(rhs.gt.3) then
         fin=-fin
         return
      endif
c     arg2(arg1)
c     get arg2
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)
      n2=istk(il2+2)
      it2=istk(il2+3)
      mn2=m2*n2
      id2=il2+8
      call icopy(4,istk(il2+4),1,var2,1)
      l2r=sadr(id2+mn2+1)
      vol=istk(id2+mn2)-1
      l2i=l2r+vol
c     get arg1
      top=top-1
      il1=iadr(lstk(top))
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      if(istk(il1).eq.0) then
         call error(220)
         return
      endif
      m1=istk(il1+1)
      n1=istk(il1+2)

      if(mn2.eq.0) then 
c     .  arg2=[]
         il1=iadr(lstk(top))
         istk(il1)=1
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         l1=sadr(il1+4)
         lstk(top+1)=l1+1
         goto 999
      elseif(m2.lt.0) then
c     .  arg2=eye
         call error(14)
         return
      elseif(m1.lt.0) then
c     .  arg2(:), just reshape to column vector
         volr=istk(id2+mn2)-1
         il1=iadr(lstk(top))
         istk(il1)=2
         istk(il1+1)=mn2
         istk(il1+2)=1
         istk(il1+3)=istk(il2+3)
         call icopy(4,var2,1,istk(il1+4),1)
         call icopy(mn2+1,istk(id2),1,istk(il1+8),1)
         l1=sadr(il1+9+mn2)
         call unsfdcopy(volr*(it2+1),stk(l2r),1,stk(l1),1)
         lstk(top+1)=l1+volr*(it2+1)
         goto 999
      endif
c     check and convert indices variable
      call indxg(il1,mn2,ili,mi,mx,lw,1)
      if(err.gt.0) return
      if(mx.gt.mn2) then
         call error(21)
         return
      endif
 131  if(mi.eq.0) then
c     arg2([])
         il1=iadr(lstk(top))
         istk(il1)=1
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         l1=sadr(il1+4)
         lstk(top+1)=l1+1
         goto 999
      endif
c     get memory for the result
      idr=iadr(lw)
      lr=sadr(idr+mi+1)
      lw=lr
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c     set result pointers
      if (m2 .gt. 1.or.m1.lt.0) then
         call dmpext(stk(l2r),istk(id2),m2,n2,istk(ili),mi,1,1,stk(lr)
     $        ,istk(idr),0,err)
      else
         call dmpext(stk(l2r),istk(id2),m2,n2,1,1,istk(ili),mi,stk(lr)
     $        ,istk(idr),0,err)
      endif
      if(err.gt.0) then
         call error(21)
         return
      endif
c     set result coefficients 
      volr=istk(idr+mi)-1
      lw=lr+volr*(it2+1)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      if (m2.eq.1.and.n2.eq.1.and.m1.gt.0) then
         m = m1
         n = min(n1,mi)
         call dmpext(stk(l2r),istk(id2),m2,n2,istk(ili),mi,1,1,stk(lr)
     $        ,istk(idr),1,err)
         if(it2.eq.1) call dmpext(stk(l2i),istk(id2),m2,n2,istk(ili),mi
     $        ,1,1,stk(lr+volr),istk(idr),1,err)
      elseif (m2 .gt. 1.or.m1.lt.0) then
         call dmpext(stk(l2r),istk(id2),m2,n2,istk(ili),mi,1,1,stk(lr)
     $        ,istk(idr),1,err)
         if(it2.eq.1) call dmpext(stk(l2i),istk(id2),m2,n2,istk(ili),mi
     $        ,1,1,stk(lr+volr),istk(idr),1,err)
         m = mi
         n = 1
      else
         call dmpext(stk(l2r),istk(id2),m2,n2,1,1,istk(ili),mi,stk(lr)
     $        ,istk(idr),1,err)
         if(it2.eq.1) call dmpext(stk(l2i),istk(id2),m2,n2,1,1,istk(ili)
     $        ,mi,stk(lr+volr),istk(idr),1,err)
         n = mi
         m = 1
      endif
c     form resulting variable
      il1=iadr(lstk(top))
      istk(il1)=2
      istk(il1+1)=m
      istk(il1+2)=n
      istk(il1+3)=it2
      call icopy(4,var2,1,istk(il1+4),1)
      call icopy(mi+1,istk(idr),1,istk(il1+8),1)
      l1=sadr(il1+9+mi)
      call unsfdcopy(volr*(it2+1),stk(lr),1,stk(l1),1)
      lstk(top+1)=l1+volr*(it2+1)
      go to 999
c
 132  continue
c     arg3(arg1,arg2)
c     get arg3
      il3=iadr(lstk(top))
      if(istk(il3).lt.0) il3=iadr(istk(il3+1))
      m3=istk(il3+1)
      n3=istk(il3+2)
      it3=istk(il3+3)
      mn3=m3*n3
      id3=il3+8
      call icopy(4,istk(il3+4),1,var3,1)
      l3r=sadr(id3+mn3+1)
      vol=istk(id3+mn3)-1
      l3i=l3r+vol
c     get arg2
      top=top-1
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      if(istk(il2).eq.0) then
         call error(220)
         return
      endif
      m2=istk(il2+1)
c     get arg1
      top=top-1
      il1=iadr(lstk(top))
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      if(istk(il1).eq.0) then
         call error(220)
         return
      endif
      m1=istk(il1+1)
c
      if(mn3.eq.0) then 
c     .  arg3=[]
         il1=iadr(lstk(top))
         istk(il1)=1
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         l1=sadr(il1+4)
         lstk(top+1)=l1+1
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
c     perform extraction
 133  mnr=mi*nj
      if(mnr.eq.0) then 
c     .  arg1=[] or arg2=[] 
         il1=iadr(lstk(top))
         istk(il1)=1
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         l1=sadr(il1+4)
         lstk(top+1)=l1+1
         goto 999
      endif
      idr=iadr(lw)
      lr=sadr(idr+mnr+1)
      lw=lr
      err=lr-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c     set result pointers
      call dmpext(stk(l3r),istk(id3),m3,n3,istk(ili),mi,istk(ilj),nj
     $     ,stk(lr),istk(idr),0,err)
      if(err.gt.0) then
         call error(21)
         return
      endif
c     set result coefficients 
      volr=istk(idr+mnr)-1
      lw=lr+volr*(it3+1)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call dmpext(stk(l3r),istk(id3),m3,n3,istk(ili),mi,istk(ilj),nj
     $     ,stk(lr),istk(idr),1,err)
      if(it3.eq.1) call dmpext(stk(l3i),istk(id3),m3,n3,istk(ili),mi
     $     ,istk(ilj),nj,stk(lr+volr),istk(idr),1,err)
c
      il1=iadr(lstk(top))
      istk(il1)=2
      istk(il1+1)=mi
      istk(il1+2)=nj
      istk(il1+3)=it3
      call icopy(4,var3,1,istk(il1+4),1)
      call icopy(mnr+1,istk(idr),1,istk(il1+8),1)
      l1=sadr(il1+9+mnr)
      call unsfdcopy(volr*(it3+1),stk(lr),1,stk(l1),1)
      lstk(top+1)=l1+volr*(it3+1)
      goto 999
c
c     divisions 
c
c     division a droite
  150 continue
      if(mn1.eq.0.or.mn2.eq.0) then
         istk(il1)=1
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=sadr(il1+4)+1
         goto 999
      endif
      if(op.eq.slash) then
         if(mn2.ne.1.and.mn1.ne.1.and.n2.ne.n1) then
            call error(11)
            return
         endif
         if(istk(il2).ne.1) then
c     .     divisor is a polynomial
            fin=-op
            top=top+1
            goto 999
         elseif(mn2.gt.1) then
c     .     divisor is not a scalar
            fin=-op
            top=top+1
            goto 999
         else
c     .     scalar divisor
            mn=mn2
            l=l2r
            il=il2
            it=it2
            goto 157
         endif
       elseif(op.eq.dot+slash) then
          if(mn2.ne.1.and.mn1.ne.1.and.(m1.ne.m2.or.n1.ne.n2)) then 
             call error(11)
             return
          endif
          if(istk(il2).ne.1) then
c     .     divisor is a polynomial
             fin=-op
             top=top+1
             goto 999
          else
c     .     divisor is a vector of scalar
             mn=mn2
             l=l2r
             il=il2
             it=it2
             goto 157
          endif
       endif
c
c
c     division a gauche
  155 continue
      if(mn1.eq.0.or.mn2.eq.0) then
         istk(il1)=1
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=sadr(il1+4)+1
         goto 999
      endif
      if(op.eq.bslash) then
         if(mn2.ne.1.and.mn1.ne.1.and.m2.ne.m1) then
            call error(12)
            return
         endif
         if(istk(il1).ne.1) then
c     .     divisor is a polynomial
            fin=-op
            top=top+1
            goto 999
         elseif(mn1.gt.1) then
c     .     divisor is not a scalar
            fin=-op
            top=top+1
            goto 999
         else
c     .     scalar divisor
            mn=mn1
            l=l1r
            il=il1
            it=it1
            goto 157
         endif
       elseif(op.eq.dot+bslash) then
         if(mn2.ne.1.and.mn1.ne.1.and.(m1.ne.m2.or.n1.ne.n2)) then
            call error(12)
            return
         endif
         if(istk(il1).ne.1) then
c     .     divisor is a polynomial
            fin=-op
            top=top+1
            goto 999
         else
c     .     divisor is a vector of scalar
            mn=mn1
            l=l1r
            il=il1
            it=it1
            goto 157
         endif
      endif
      go to 999

c     in-line inversion procedure
  157 continue
c     divisor is a vector of scalars
      do 158 i=1,mn
         sr=stk(l-1+i)
         si=0.0d+0
         if(it.eq.1) si=stk(l+mn-1+i)
         e1=max(abs(sr),abs(si))
         if(e1.eq.0.d0) then
            call error(27)
            return
         endif
         sr=sr/e1
         si=si/e1
         e1=e1*(sr*sr+si*si)
         stk(l-1+i)=sr/e1
         if(it.eq.1) stk(l+mn-1+i)=-si/e1
 158  continue
c     . goto multiplication alg
      goto 40

c
c     comparaisons
 160  continue
      itrue=1
      if(op.eq.less+great) itrue=0
c     comparaison des types
      if(istk(il1).gt.2.or.istk(il2).gt.2) then
         istk(il1)=4
         istk(il1+1)=1
         istk(il1+2)=1
         istk(il1+3)=1-itrue
         lstk(top+1)=sadr(il1+4)
         return
      endif
c     des nom de variable
      do 161 i=1,4
         if(var1(i).ne.var2(i)) then
            istk(il1)=4
            istk(il1+1)=1
            istk(il1+2)=1
            istk(il1+3)=1-itrue
            lstk(top+1)=sadr(il1+4)
            return
         endif
 161  continue
c     des dimensions
      if(m1.eq.-1) then
c     .  eye op b
         nn1=istk(id1+1)-1
         il1w=iadr(lw)
         lw=sadr(il1w+m2*n2)
         err=lw+2-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         stk(lw)=0.0d0
         stk(lw+1)=0.0d0
         i2=id2
         n2=abs(n2)
         m2=abs(m2)
         do 166 i=1,n2
            do 165 j=1,m2
               if (i.eq.j) then
                  if(nn1.ne.istk(i2+1)-istk(i2) ) goto 164
                  nl=nn1
                  l1=l1r-1
               else
                  if(1.ne.istk(i2+1)-istk(i2) ) goto 164
                  l1=lw-1
                  nl=1
               endif
               l2=l2r+istk(i2)-2
               do 162 ii=1,nl
                  if(stk(l1+ii).ne.stk(l2+ii)) goto 164
 162           continue
               if(max(it1,it2).eq.1) then
                  e1=0.0d+0
                  e2=0.0d+0
                  do 163 ii=1,nl
                     if(it1.eq.1) e1=stk(l1+nl+ii)
                     if(it2.eq.1) e2=stk(l2i+istk(i2)-2+ii)
                     if(e1.ne.e2) goto 164
 163              continue
               endif
               i2=i2+1
               istk(il1w-1+(j-1)*m2+i)=itrue
               goto 165
 164           continue
               i2=i2+1
               istk(il1w-1+(j-1)*m2+i)=1-itrue
 165           continue
 166        continue
            istk(il1)=4
            istk(il1+1)=m2
            istk(il1+2)=n2
            call icopy(m2*n2,istk(il1w),1,istk(il1+3),1)
            lstk(top+1)=sadr(il1+3+m2*n2)
            goto 999
      elseif(m2.eq.-1) then
c     .   b op eye
         nn2=istk(id2+1)-1
         il1w=iadr(lw)
         lw=sadr(il1w+m1*n1)
         err=lw+2-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         stk(lw)=0.0d0
         stk(lw+1)=0.0d0
         i1=id1
         do 176 i=1,n1
            do 175 j=1,m1
               if (i.eq.j) then
                  if(nn2.ne.istk(i1+1)-istk(i1) ) goto 174
                  nl=nn2
                  l2=l2r-1
               else
                  if(1.ne.istk(i1+1)-istk(i1) ) goto 174
                  l2=lw-1
                  nl=1
               endif
               l1=l1r+istk(i1)-2
               do 172 ii=1,nl
                  if(stk(l2+ii).ne.stk(l1+ii)) goto 174
 172           continue
               if(max(it1,it2).eq.1) then
                  e1=0.0d+0
                  e2=0.0d+0
                  do 173 ii=1,nl
                     if(it1.eq.1) e1=stk(l2+nl+ii)
                     if(it2.eq.1) e2=stk(l1i+istk(i1)-2+ii)
                     if(e1.ne.e2) goto 174
 173              continue
               endif
               i1=i1+1
               istk(il1w-1+(j-1)*m1+i)=itrue
               goto 175
 174           continue
               i2=i2+1
               istk(il1w-1+(j-1)*m1+i)=1-itrue
 175           continue
 176        continue
            istk(il1)=4
            istk(il1+1)=m1
            istk(il1+2)=n1
            call icopy(m1*n1,istk(il1w),1,istk(il1+3),1)
            lstk(top+1)=sadr(il1+3+m1*n1)
            goto 999

      elseif(mn1.eq.1.and.mn2.gt.1) then
         nn1=istk(id1+1)-1
         err=lw+nn1*(it1+1)+2-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call unsfdcopy(nn1*(it1+1),stk(l1r),1,stk(lw),1)
         l1r=lw
         l1i=l1r+nn1
         id1=iadr(l1r+nn1*(it1+1))
         istk(id1)=1
         istk(id1+1)=nn1+1
         inc1=0
         inc2=1
         mn1=mn2
         m1=m2
         n1=n2
         istk(il1+1)=m1
         istk(il1+2)=n1
      else if(mn2.eq.1.and.mn1.gt.1) then
         nn2=istk(id2+1)-1
         err=lw+nn2*(it2+1)+2-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call unsfdcopy(nn2*(it2+1),stk(l2r),1,stk(lw),1)
         l2r=lw
         l2i=l2r+nn2
         id2=iadr(l2r+nn2*(it2+1))
         istk(id2)=1
         istk(id2+1)=nn2+1
         inc1=1
         inc2=0
         mn2=mn1
         m2=m1
         n2=n1
      else if(n1.ne.n2.or.m1.ne.m2) then
         istk(il1)=4
         istk(il1+1)=1
         istk(il1+2)=1
         istk(il1+3)=1-itrue
         lstk(top+1)=sadr(il1+4)
         return
      else
         inc1=1
         inc2=1
      endif
c     des valeurs
      i1=id1-inc1
      i2=id2-inc2
      l1r=l1r-1
      l2r=l2r-1
      l1i=l1i-1
      l2i=l2i-1
      do 185 i=0,mn1-1
         i1=i1+inc1
         i2=i2+inc2
         if(istk(i1+1)-istk(i1).ne.istk(i2+1)-istk(i2) ) goto 184
         nl=istk(i1+1)-istk(i1)-1
         do 182 ii=0,nl
            if(stk(l1r+istk(i1)+ii).ne.stk(l2r+istk(i2)+ii)) goto 184
 182     continue
         istk(il1+3+i)=itrue
         if(max(it1,it2).eq.0) goto 185
         e1=0.0d+0
         e2=0.0d+0
         do 183 ii=0,nl
            if(it1.eq.1) e1=stk(l1i+istk(i1)+ii)
            if(it2.eq.1) e2=stk(l2i+istk(i2)+ii)
            if(e1.ne.e2) goto 184
 183     continue
         istk(il1+3+i)=itrue
         goto 185
 184     istk(il1+3+i)=1-itrue
 185  continue
      istk(il1)=4
      istk(il1+1)=m1
      istk(il1+2)=n1
      lstk(top+1)=sadr(il1+3+mn1)
      goto 999
c
 200  continue
c      a [:b]:c
      var1(1)=0
      if(rhs.eq.3) then
         il3=iadr(lstk(top))
         if(istk(il3).lt.0) il3=iadr(istk(il3+1))
         if(istk(il3+1).ne.1.or.istk(il3+2).ne.1) then
            err=3
            call putid(ids(1,pt+1),colonid)
            call error(204)
            return
         endif
         if(istk(il3+3).ne.0) then
            err=3
            call error(52)
            return
         endif
         if(istk(il3).eq.1) then
            n3=1
            l3=sadr(il3+4)
         else
            n3=istk(il3+9)-1
            l3=sadr(il3+10)
            call icopy(4,istk(il3+4),1,var1,1)
         endif
         top=top-1
      endif
      if (rhs.ge.2) then
         il2=iadr(lstk(top))
         if(istk(il2).lt.0) il2=iadr(istk(il2+1))
         if(istk(il2+1).ne.1.or.istk(il2+2).ne.1) then
            err=2
            call putid(ids(1,pt+1),colonid)
            call error(204)
            return
         endif
         if(istk(il2+3).ne.0) then
            err=2
            call error(52)
            return
         endif
         if(istk(il2).eq.1) then
            n2=1
            l2=sadr(il2+4)
         else
            n2=istk(il2+9)-1
            l2=sadr(il2+10)
            if (var1(1).ne.0) then
               if(var1(1).ne.istk(il2+4).or.var1(2).ne.istk(il2+5).or
     $              .var1(3).ne.istk(il2+6).or.var1(4).ne.istk(il2+7))
     $              then
                  call error(278)
                  return
               endif
            else
               call icopy(4,istk(il2+4),1,var1,1)
            endif
         endif
         top=top-1
      endif
      il1=iadr(lstk(top))
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      if(istk(il1+1).ne.1.or.istk(il1+2).ne.1) then
            err=1
            call putid(ids(1,pt+1),colonid)
            call error(204)
            return
         endif
         if(istk(il1+3).ne.0) then
            err=2
            call error(52)
            return
         endif
      if(istk(il1).eq.1) then
         n1=1
         l1=sadr(il1+4)
      else
         n1=istk(il1+9)-1
         l1=sadr(il1+10)
         if (var1(1).ne.0) then
            if(var1(1).ne.istk(il1+4).or.var1(2).ne.istk(il1+5).or.
     $           var1(3).ne.istk(il1+6).or.var1(4).ne.istk(il1+7)) then
               call error(278)
               return
            endif
         else
            call icopy(4,istk(il1+4),1,var1,1)
         endif
      endif
      if (rhs.eq.2) then
         n3=n2
         l3=l2
         n2=1
         l2=lw
         stk(l2)=1
         lw=lw+1
      endif
      call unsfdcopy(n1,stk(l1),1,stk(lw),1)
      call unsfdcopy(n2,stk(l2),1,stk(lw+n1),1)
      call unsfdcopy(n3,stk(l3),1,stk(lw+n1+n2),1)
      il1=iadr(lstk(top))
      istk(il1)=129
      istk(il1+1)=1
      istk(il1+2)=3
      istk(il1+3)=0
      call icopy(4,var1,1,istk(il1+4),1)
      istk(il1+8)=1
      istk(il1+9)=1+n1
      istk(il1+10)=istk(il1+9)+n2
      istk(il1+11)=istk(il1+10)+n3
      l=sadr(il1+12)
      call unsfdcopy(n1+n2+n3,stk(lw),1,stk(l),1)
      lstk(top+1)=l+n1+n2+n3
c
  999 return
      end
