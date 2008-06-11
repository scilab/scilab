
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine matins2
c     
c     A(i,j)=B

      include 'stack.h'
c     
      integer top0
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      top0=top
      lw=lstk(top+1)+1

      il4=iadr(lstk(top))
      if(istk(il4).lt.0) il4=iadr(istk(il4+1))
      m4=istk(il4+1)
      n4=istk(il4+2)
      it4=istk(il4+3)
      l4=sadr(il4+4)
      mn4=m4*n4
      top=top-1

      il3=iadr(lstk(top))
      if(istk(il3).lt.0) il3=iadr(istk(il3+1))
      m3=istk(il3+1)
      n3=istk(il3+2)
      it3=istk(il3+3)
      l3=sadr(il3+4)
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
      ilrs=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=sadr(il1+4)
      mn1=m1*n1

c     arg4(arg1,arg2)=arg3
      if (istk(il3)*istk(il1)*istk(il2).eq.0) then
         call error(220)
         return
      endif
      if (m3.eq.0) then
c     .  arg4(arg1,arg2)=[]
         if(m1.eq.-1.and.m2.eq.-1) then
c     .    arg4(:,:)=[] -->[]
            istk(ilrs)=1
            istk(ilrs+1)=0
            istk(ilrs+2)=0
            istk(ilrs+3)=0
            lstk(top+1)=sadr(ilrs+4)
            return
         elseif(m1.eq.0.or.m2.eq.0) then
c     .     arg4([],arg2)=[],  arg4(arg1,[])=[] --> arg4
            call icopy(4,istk(il4),1,istk(ilrs),1)
            l=sadr(ilrs+4)
            call unsfdcopy(mn4*(it4+1),stk(l4),1,stk(l),1)
            lstk(top+1)=l+mn4*(it4+1)
            return
         elseif(m2.eq.-1) then
c     .     arg4(arg1,:)=[] --> arg4(compl(arg1),:)
            call indxgc(il1,m4,ili,mi,mxi,lw)
            if(err.gt.0) return
            call indxg(il2,n4,ilj,nj,mxj,lw,1)
            if(err.gt.0) return
            l3=l4
            n3=n4
            m3=m4
            mn3=m3*n3
            it3=it4
c     .     call extraction
            goto 90
         elseif(m1.eq.-1) then
c     .     arg4(:,arg2)=[] --> arg4(:,compl(arg2))
            call indxgc(il2,n4,ilj,nj,mxj,lw)
            if(err.gt.0) return
            call indxg(il1,m4,ili,mi,mxi,lw,1)
            if(err.gt.0) return
            l3=l4
            n3=n4
            m3=m4
            mn3=m3*n3
            it3=it4
c     .     call extraction
            goto 90
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

               if(mi.eq.0) then
c     .           arg4(1:m4,1:n4)=[] 
                  istk(ilrs)=1
                  istk(ilrs+1)=0
                  istk(ilrs+2)=0
                  istk(ilrs+3)=0
                  lstk(top+1)=sadr(ilrs+4)
                  return
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
                  l3=l4
                  n3=n4
                  m3=m4
                  it3=it4
                  mn3=m3*n3
c     .           call extraction
                  goto 90
               endif
            elseif(nj.eq.n4) then
c               arg4(arg1,[])=[] --> arg4
               call icopy(4,istk(il4),1,istk(ilrs),1)
               l=sadr(ilrs+4)
               call unsfdcopy(mn4*(it4+1),stk(l4),1,stk(l),1)
               lstk(top+1)=l+mn4*(it4+1)
               return
            else
               call indxgc(il1,m4,ili,mi,mxi,lw)
               if(err.gt.0) return
               if(mi.eq.0) then
c     .           arg4(1:m4,arg2)=[] 
                  call indxg(il1,m4,ili,mi,mxi,lw,1)
                  if(err.gt.0) return
                  l3=l4
                  n3=n4
                  m3=m4
                  it3=it4
                  mn3=m3*n3
c     .           call extraction
                  goto 90
               elseif(mi.eq.m4) then
c                 arg4([],arg2)=[] --> arg4
                  call icopy(4,istk(il4),1,istk(ilrs),1)
                  l=sadr(ilrs+4)
                  call unsfdcopy(mn4*(it4+1),stk(l4),1,stk(l),1)
                  lstk(top+1)=l+mn4*(it4+1)
                  return
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
         if(mn3.eq.mn4) then
c     .     reshape arg3 according to arg4
            istk(ilrs)=1
            istk(ilrs+1)=m4
            istk(ilrs+2)=n4
            istk(ilrs+3)=it3
            l1=sadr(ilrs+4)
            call unsfdcopy((it3+1)*mn4,stk(l3),1,stk(l1),1)
            lstk(top+1)=l1+mn4*(it3+1)
            return
         elseif(mn3.eq.1) then
c     .     set all elements of arg4 to arg3
            istk(ilrs)=1
            istk(ilrs+1)=m4
            istk(ilrs+2)=n4
            istk(ilrs+3)=it3
            l1=sadr(ilrs+4)
            call dset(mn4,stk(l3),stk(l1),1)
            if(it3.eq.1) call dset(mn4,stk(l3+1),stk(l1+mn4),1)
            lstk(top+1)=l1+mn4*(it3+1)
            return
         else
            call error(15)
            return
         endif

      endif
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
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         mn4=m4*n4
         l4=lw
         lw=l4+ mn4
         call dset(mn4,0.0d0,stk(l4),1)
      endif
      call indxg(il1,m4,ili,mi,mxi,lw,1)
      if(err.gt.0) return
      call indxg(il2,n4,ilj,mj,mxj,lw,1)
      if(err.gt.0) return
      inc3=1
      if(mi.ne.m3.or.mj.ne.n3) then
c     .  sizes of arg1 or arg2 dont agree with arg3 sizes
         if(m3*n3.eq.1) then
            if(mi.eq.0.or.mj.eq.0) then
               call icopy(4,istk(il4),1,istk(ilrs),1)
               l=sadr(ilrs+4)
               call unsfdcopy(mn4*(it4+1),stk(l4),1,stk(l),1)
               lstk(top+1)=l+mn4*(it4+1)
               return
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
      if(mnr*(itr+1).ne.mn4*(it4+1) ) then
         lr=lw
         lw=lr + mnr*(itr+1)
         err = lw - lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
c     .  set result r to 0
         call dset(mnr*(itr+1),0.0d+0,stk(lr),1)
c     .  copy arg4 in r
         if(mn4.ge.1) then
            call dmcopy(stk(l4),m4,stk(lr),mr,m4,n4)
            if(it4.eq.1) then
               call dmcopy(stk(l4+mn4),m4,stk(lr+mnr),mr,m4,n4)
            endif
         endif
      else
         lr=l4
      endif
c     
c     copy arg3 elements in r
      do 115 j = 0, mj-1
         ljj =  istk(ilj+j) - 1
         do 114 i = 0, mi-1
            ll = lr+istk(ili+i)-1+ljj*mr
            ls = l3+(i+j*m3)*inc3
            stk(ll) = stk(ls)
            if(it3.eq.1) then
               stk(ll+mnr)=stk(ls+mn3)
            elseif(itr.eq.1) then
               stk(ll+mnr)=0.0d0
            endif
 114     continue
 115  continue
c     
      if(lr.ne.l4.or.init4.ne.0) then
         l1=sadr(ilrs+4)
         call unsfdcopy(mnr*(itr+1),stk(lr),1,stk(l1),1)
         istk(ilrs)=1
         istk(ilrs+1)=mr
         istk(ilrs+2)=nr
         istk(ilrs+3)=itr
         lstk(top+1)=l1+mnr*(itr+1)
      else
c     la matrice a ete modifie sur place 
         k=istk(iadr(lstk(top0))+2)
         top=top-1
         call setref(k)
      endif

      return
c     inline extraction procedure copied from matext2
 90   mn=mi*nj
      if(mn.eq.0) then 
c     .  arg1=[] or arg2=[] 
         ilrs=iadr(lstk(top))
         istk(ilrs)=1
         istk(ilrs+1)=0
         istk(ilrs+2)=0
         istk(ilrs+3)=0
         lstk(top+1)=sadr(ilrs+4)
         return
      endif
c     get memory for the result
      ilrs=iadr(lstk(top))
      l1=sadr(ilrs+4)
      if(sadr(ili-1).le.l1+(it3+1)*mi*nj) then
         lr=lw
         lw=lr+(it3+1)*mi*nj
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
      l=lr
      do 94 j = 0, nj-1
         do 93 i = 0, mi-1
            ind=istk(ili+i)-1+(istk(ilj+j)-1)*m3
            stk(l) = stk(l3+ind)
            if(it3.eq.1) stk(l+mn) = stk(l3+mn3+ind)
            l=l+1
 93      continue
 94   continue
c     form the resulting variable
      istk(ilrs)=1
      istk(ilrs+1)=mi
      istk(ilrs+2)=nj
      istk(ilrs+3)=it3
      l1=sadr(ilrs+4)
      if(lr.ne.l1) call unsfdcopy(mn*(it3+1),stk(lr),1,stk(l1),1)
      lstk(top+1)=l1+mn*(it3+1)
      return

      end

c			================================================
