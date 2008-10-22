c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine hndlops
c     
c     operations sur les matrices de handle
c     
      include 'stack.h'
      integer op
c     
      integer dot,colon,quote
      integer less,great,equal
c     
      data dot/51/,quote/53/,colon/44/
      data less/59/,great/60/,equal/50/
c     
      op=fin
c     
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' hndlops op: '//buf(1:4))
      endif
c     
c     operations binaires et ternaires
c     --------------------------------
c     
      fun = 0
c     
c     cconc  extrac insert rconc
      goto(75  ,  95  ,  78   ,76) op
c     
c     :  +  -  * /  \  =          '
      goto(07,07,07,07,07,07,130,06,06,70) op+1-colon
c     
 06   continue
      if(op.eq.quote+dot) goto 70
      if(op.ge.less+equal) goto 130

 07   fin=-fin
      return
      
c     
c     ' or .'
 70   continue
      call hndltr
      goto 999
c     
c     concatenation [a b]
 75   continue
      call hndlrc
      goto 999
c     
c     concatenation [a;b]
 76   continue
      call hndlcc
      goto 999
c     
c     extraction a(i) and a(i,j)
c     
 78   continue
      if (rhs.eq.2) then
         call hndlext1
      elseif (rhs.eq.3) then
         call hndlext2
      else
         fin=-fin
      endif
      go to 999
c     
c     insertion
c     
 95   continue
      if (rhs.eq.3) then
         call hndlins1
      elseif (rhs.eq.4) then
         call hndlins2
      else
         fin=-fin
      endif
      goto 999

c     == <= >= ~=
 130  continue
      call hndlcmp
      goto 999
c     
      
 999  return
      end

      subroutine hndltr
c     
c     .'
c     
c     Copyright INRIA
      include 'stack.h'
c     
      integer vol
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      il1=iadr(lstk(top))
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      l1=sadr(il1+4)
      mn1=m1*n1
c     
      if(mn1 .eq. 0.or.istk(il1).eq.0) then
         return
      elseif(abs(m1).eq.1.or.abs(n1).eq.1) then
         goto 10
      else
         vol=mn1
         ll = l1+vol
         err = ll+vol - lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
         call unsfdcopy(vol,stk(l1),1,stk(ll),1)
         call mtran(stk(ll),m1,stk(l1),n1,m1,n1)
      endif
 10   istk(il1+1)=n1
      istk(il1+2)=m1
      return
      end

      subroutine hndlrc
c     
c     [a b]
c     
c     Copyright INRIA
      include 'stack.h'
c     
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      lw=lstk(top+1)+1
c     
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)
      n2=istk(il2+2)
      l2=sadr(il2+4)
      mn2=m2*n2
      top=top-1
c     
      il1=iadr(lstk(top))
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      l1=sadr(il1+4)
      mn1=m1*n1


      if(m1.lt.0.or.m2.lt.0) then
         call error(14)
         return
      endif
      if(m2.eq.0) then
         return
      elseif(m1.eq.0)then
         call unsfdcopy(lstk(top+2)-lstk(top+1),stk(lstk(top+1))
     $        ,1,stk(lstk(top)),1)
         lstk(top+1)=lstk(top)+lstk(top+2)-lstk(top+1)
         return
      elseif(m1.ne.m2) then
         call error(5)
         return
      endif
      if (istk(il1).ne.istk(il2)) then
         top=top +1
         fin=-fin
         return
      endif

c     
      call unsfdcopy(mn2,stk(l2),1,stk(l1+mn1),1)
      n=n1+n2
      istk(il1+1)=m1
      istk(il1+2)=n
      lstk(top+1)=sadr(il1+4)+m1*n
      return
      end

      subroutine hndlcc
c     
c     [a;b]
c     
c     Copyright INRIA
      include 'stack.h'
c     
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      lw=lstk(top+1)+1
c     
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)
      n2=istk(il2+2)
      l2=sadr(il2+4)
      mn2=m2*n2
      top=top-1
c     
      il1=iadr(lstk(top))
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      l1=sadr(il1+4)
      mn1=m1*n1
c     
      if(n1.lt.0.or.n2.lt.0) then
         call error(14)
         return
      elseif(n2.eq.0) then
c     .  [a;[]]
         return
      elseif(n1.eq.0)then
c     .  [[];b]
         call unsfdcopy(lstk(top+2)-lstk(top+1),stk(lstk(top+1))
     $        ,1,stk(lstk(top)),1)
         lstk(top+1)=lstk(top)+lstk(top+2)-lstk(top+1)
         return
      elseif(n1.ne.n2) then
         call error(6)
         return
      endif

      if (istk(il1).ne.istk(il2)) then
         top=top +1
         fin=-fin
         return
      endif

      m=m1+m2
      mn=m*n1
      if(n1.eq.1) then
         call unsfdcopy(mn2,stk(l2),1,stk(l1+mn1),1)
         istk(il1+1)=m
         lstk(top+1)=l1+mn
         return
      endif
      lw1=max(lw,l1+mn)
      lw2=lw1+mn1
      err=lw2+mn2-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call unsfdcopy(mn2,stk(l2),1,stk(lw2),1)
      call unsfdcopy(mn1,stk(l1),1,stk(lw1),1)
c     
      call dmcopy(stk(lw1),m1,stk(l1),m,m1,n1)
      call dmcopy(stk(lw2),m2,stk(l1+m1),m,m2,n1)
      istk(il1+1)=m
      istk(il1+2)=n1
      lstk(top+1)=sadr(il1+4)+mn
      return
      end

      subroutine hndlext1
c     
c     B=A(i)
c     
c     Copyright INRIA
      include 'stack.h'
c     
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      lw=lstk(top+1)+1
c     
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)
      n2=istk(il2+2)
      l2=sadr(il2+4)
      mn2=m2*n2
      top=top-1
c     
      il1=iadr(lstk(top))
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      if(istk(il1).eq.10.or.istk(il1).eq.15) then
         top=top+1
         fin=-fin
         return
      endif
      m1=istk(il1+1)
      n1=istk(il1+2)
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
         return
      elseif(m2.lt.0) then
c     .  arg2=eye
         call error(14)
         return
      elseif(m1.lt.0) then
c     .  arg2(:), just reshape to column vector
         il1=iadr(lstk(top))
         istk(il1)=9
         istk(il1+1)=mn2
         istk(il1+2)=1
         l1=sadr(il1+4)
         call unsfdcopy(mn2,stk(l2),1,stk(l1),1)
         lstk(top+1)=l1+mn2
         return
      endif
c     check and convert indices variable
      call indxg(il1,mn2,ilr,mi,mx,lw,1)
      if(err.gt.0) return
      if(mx.gt.mn2) then
         call error(21)
         return
      endif
      if(mi.eq.0) then
c     arg2([])
         il1=iadr(lstk(top))
         istk(il1)=1
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         l1=sadr(il1+4)
         lstk(top+1)=l1
         return
      endif
c     get memory for the result
      il1=iadr(lstk(top))
      l1=sadr(il1+4)
      if(sadr(ilr-1).le.l1+mi) then
         lr=lw
         lw=lr+mi
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
      else
         lr=l1
      endif
c     perform extraction
      do 81 i = 0, mi-1
         ind=istk(ilr+i)-1
         stk(lr+i) = stk(l2+ind)
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
      istk(il1)=9
      istk(il1+1)=m
      istk(il1+2)=n
      if(lr.ne.l1) call unsfdcopy(mi,stk(lr),1,stk(l1),1)
      lstk(top+1)=l1+mi
      return
      end

      subroutine hndlext2
c     
c     B=A(i,j)
c     
c     Copyright INRIA
      include 'stack.h'
c     
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      lw=lstk(top+1)+1

      il3=iadr(lstk(top))
      if(istk(il3).lt.0) il3=iadr(istk(il3+1))
      m3=istk(il3+1)
      n3=istk(il3+2)
      l3=sadr(il3+4)
      mn3=m3*n3
      top=top-1
c     
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)
      n2=istk(il2+2)
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
c     
c     arg3(arg1,arg2)
      if(rhs.gt.3) then
         call error(36)
         return
      endif
      if(mn3.eq.0) then 
c     .  arg3=[]
         il1=iadr(lstk(top))
         istk(il1)=1
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=sadr(il1+4)
         return
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
      mn=mi*nj
      if(mn.eq.0) then 
c     .  arg1=[] or arg2=[] 
         il1=iadr(lstk(top))
         istk(il1)=1
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=sadr(il1+4)
         return
      endif
c     get memory for the result
      il1=iadr(lstk(top))
      l1=sadr(il1+4)
      if(sadr(ili-1).le.l1+mi*nj) then
         lr=lw
         lw=lr+mi*nj
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
            l=l+1
 93      continue
 94   continue
c     form the resulting variable
      istk(il1)=9
      istk(il1+1)=mi
      istk(il1+2)=nj
      if(lr.ne.l1) call unsfdcopy(mn,stk(lr),1,stk(l1),1)
      lstk(top+1)=l1+mn
      return
      end

      subroutine hndlins1
c     
c     A(i)=B
c     
c     Copyright INRIA
      include 'stack.h'
c     
      logical isany
      integer top0
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      top0=top
      lw=lstk(top+1)+1

      il3=iadr(lstk(top))
      if(istk(il3).lt.0) il3=iadr(istk(il3+1))
      if(istk(il3).eq.1) then
         if (istk(il3+1)*istk(il3+2).eq.0) goto 01
         top=top0
         fin=-fin
         return
      endif

      if(istk(il3).ne.9) then
         top=top0
         fin=-fin
         return
      endif
 01   m3=istk(il3+1)
      n3=istk(il3+2)
      it3=istk(il3+3)
      l3=sadr(il3+4)
      mn3=m3*n3
      top=top-1
c     
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      if(istk(il2).eq.1) then
         if (istk(il2+1)*istk(il2+2).eq.0) goto 02
      endif
      if(istk(il2).ne.9) then
         top=top0
         fin=-fin
         return
      endif
      
 02   m2=istk(il2+1)
      n2=istk(il2+2)
      it2=istk(il2+3)
      l2=sadr(il2+4)
      mn2=m2*n2
      top=top-1
c     
      il1=iadr(lstk(top))
      ilrs=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      if (istk(il1).eq.10.or.istk(il1).eq.15) then
         top=top0
         fin=-fin
         return
      endif
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=sadr(il1+4)
      mn1=m1*n1


c     arg3(arg1)=arg2
c     
      if (istk(il2)*istk(il1).eq.0) then
         call error(220)
         return
      endif
      if (m2.eq.0) then
c     .  arg3(arg1)=[] -->[]
         if(m1.eq.-1) then
c     .    arg3(:)=[] 
            istk(ilrs)=1
            istk(ilrs+1)=0
            istk(ilrs+2)=0
            istk(ilrs+3)=0
            lstk(top+1)=sadr(ilrs+4)
            return
         elseif(m1.eq.0) then
c     .     arg3([])=[]  --> arg3
            call icopy(4,istk(il3),1,istk(ilrs),1)
            l=sadr(ilrs+4)
            call unsfdcopy(mn3,stk(l3),1,stk(l),1)
            lstk(top+1)=l+mn3
            return
         else
c     .     arg3(arg1)=[]
            if(istk(il1).eq.4.and.m3.eq.m1.and.n3.eq.n1) then
               if(.not.isany(il1)) then
c     .           arg3([])=[]  --> arg3
                  call icopy(4,istk(il3),1,istk(ilrs),1)
                  l=sadr(ilrs+4)
                  call unsfdcopy(mn3,stk(l3),1,stk(l),1)
                  lstk(top+1)=l+mn3
                  return
               endif
            endif
c     .     arg3(arg1)=[] -->arg3(compl(arg1))
            call indxgc(il1,mn3,ilr,mi,mx,lw)
            if(err.gt.0) return
            l2=l3
            n2=n3
            m2=m3
            mn2=m2*n2
c     .     call extraction
            goto 79
         endif
      elseif(m2.lt.0.or.m3.lt.0) then
c     .  arg3=eye,arg2=eye
         call error(14)
         return
      elseif(m1.lt.0) then
c     .  arg3(:)=arg2
         if(mn2.eq.mn3) then
            istk(ilrs)=1
            istk(ilrs+1)=m3
            istk(ilrs+2)=n3
            istk(ilrs+3)=it2
            l1=sadr(ilrs+4)
            call unsfdcopy(mn2,stk(l2),1,stk(l1),1)
            lstk(top+1)=l1+mn2
            return
         elseif(mn2.eq.1) then
            istk(ilrs)=1
            istk(ilrs+1)=m3
            istk(ilrs+2)=n3
            istk(ilrs+3)=it2
            l1=sadr(ilrs+4)
            call dset(mn3,stk(l2),stk(l1),1)
            lstk(top+1)=l1+mn3
            return
         else
            call error(15)
            return
         endif
      endif
      call indxg(il1,mn3,ili,mi,mxi,lw,1)
      if(err.gt.0) return
      if(mi.eq.0) then
c     .  arg3([])=arg2
         if(mn2.eq.1) then
c     .  arg3([])=c  --> arg3
            call icopy(4,istk(il3),1,istk(ilrs),1)
            l=sadr(ilrs+4)
            call unsfdcopy(mn3,stk(l3),1,stk(l),1)
            lstk(top+1)=l+mn3
            return
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

c     commented lines for matlab compatibility
c     elseif (n3.le.1.and.m3.le.1) then
c     if(n2.le.1) then
c     mr=max(m3,mxi)
c     nr=max(n3,1)
c     else
c     nr=max(n3,mxi)
c     mr=max(m3,1)
c     endif
c     elseif (n3.le.1) then
c     .  arg3 and arg2 are  column vectors
c     mr=max(m3,mxi)
c     nr=max(n3,1)
c     elseif (m3.le.1) then
c     .  row vectors
c     nr=max(n3,mxi)
c     mr=max(m3,1)
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

      lr=l3
      mnr=mr*nr
      if(mnr.ne.mn3) then
c     .  resulting matrix is bigger than original
         lr=lw
         lw=lr + mnr
         err = lw - lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
c     .  initialise result r to 0
         call dset(mnr,0.0d+0,stk(lr),1)
c     .  write arg3 in r
         if(mn3.ge.1) then
            call dmcopy(stk(l3),m3,stk(lr),mr,m3,n3)
         endif
      endif
c     write arg2 in r
      do 98 i = 0, mi-1
         ll = lr+istk(ili+i) - 1
         ls = l2+i*inc2
         stk(ll) = stk(ls)
 98   continue
c     
      if(lr.ne.l3) then
         l1=sadr(ilrs+4)
         call unsfdcopy(mnr,stk(lr),1,stk(l1),1)
         istk(ilrs)=9
         istk(ilrs+1)=mr
         istk(ilrs+2)=nr
         lstk(top+1)=l1+mnr
      else
c     la matrice a ete modifie sur place 
         k=istk(iadr(lstk(top0))+2)
         top=top-1
         call setref(k)
      endif
      return
c     inline extraction procedure copied from hndlext1
 79   if(mi.eq.0) then
c     arg2([])
         istk(ilrs)=1
         istk(ilrs+1)=0
         istk(ilrs+2)=0
         istk(ilrs+3)=0
         l1=sadr(ilrs+4)
         lstk(top+1)=l1
         return
      endif
c     get memory for the result
      l1=sadr(ilrs+4)
      if(sadr(ilr-1).le.l1+mi) then
         lr=lw
         lw=lr+mi
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
      else
         lr=l1
      endif
c     perform extraction
      do 81 i = 0, mi-1
         ind=istk(ilr+i)-1
         stk(lr+i) = stk(l2+ind)
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
      istk(ilrs)=9
      istk(ilrs+1)=m
      istk(ilrs+2)=n
      if(lr.ne.l1) call unsfdcopy(mi,stk(lr),1,stk(l1),1)
      lstk(top+1)=l1+mi
      return

      end

      subroutine hndlins2
c     
c     A(i,j)=B
c     
c     Copyright INRIA
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
      if(istk(il4).ne.9) then
         top=top0
         fin=-fin
         return
      endif
      m4=istk(il4+1)
      n4=istk(il4+2)
      it4=istk(il4+3)
      l4=sadr(il4+4)
      mn4=m4*n4
      top=top-1

      il3=iadr(lstk(top))
      if(istk(il3).lt.0) il3=iadr(istk(il3+1))
      if(istk(il3).eq.1) then
         if (istk(il3+1)*istk(il3+2).eq.0) goto 02
      endif
      if(istk(il3).ne.9) then
         top=top0
         fin=-fin
         return
      endif
 02   m3=istk(il3+1)
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
            call unsfdcopy(mn4,stk(l4),1,stk(l),1)
            lstk(top+1)=l+mn4
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
                  mn3=m3*n3
c     .           call extraction
                  goto 90
               endif
            elseif(nj.eq.n4) then
c               arg4(arg1,[])=[] --> arg4
               call icopy(4,istk(il4),1,istk(ilrs),1)
               l=sadr(ilrs+4)
               call unsfdcopy(mn4,stk(l4),1,stk(l),1)
               lstk(top+1)=l+mn4
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
                  call unsfdcopy(mn4,stk(l4),1,stk(l),1)
                  lstk(top+1)=l+mn4
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
            istk(ilrs)=9
            istk(ilrs+1)=m4
            istk(ilrs+2)=n4
            l1=sadr(ilrs+4)
            call unsfdcopy(mn4,stk(l3),1,stk(l1),1)
            lstk(top+1)=l1+mn4
            return
         elseif(mn3.eq.1) then
c     .     set all elements of arg4 to arg3
            istk(ilrs)=9
            istk(ilrs+1)=m4
            istk(ilrs+2)=n4
            istk(ilrs+3)=it3
            l1=sadr(ilrs+4)
            call dset(mn4,stk(l3),stk(l1),1)
            lstk(top+1)=l1+mn4
            return
         else
            call error(15)
            return
         endif
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
               call unsfdcopy(mn4,stk(l4),1,stk(l),1)
               lstk(top+1)=l+mn4
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
      if(mnr.ne.mn4) then
         lr=lw
         lw=lr + mnr
         err = lw - lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
c     .  set result r to 0
         call dset(mnr,0.0d+0,stk(lr),1)
c     .  copy arg4 in r
         if(mn4.ge.1) then
            call dmcopy(stk(l4),m4,stk(lr),mr,m4,n4)
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
 114     continue
 115  continue
c     
      if(lr.ne.l4) then
         l1=sadr(ilrs+4)
         call unsfdcopy(mnr,stk(lr),1,stk(l1),1)
         istk(ilrs)=9
         istk(ilrs+1)=mr
         istk(ilrs+2)=nr
         istk(ilrs+3)=0
         lstk(top+1)=l1+mnr
      else
c     la matrice a ete modifie sur place 
         k=istk(iadr(lstk(top0))+2)
         top=top-1
         call setref(k)
      endif

      return
c     inline extraction procedure copied from hndlext2
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
      if(sadr(ili-1).le.l1+mi*nj) then
         lr=lw
         lw=lr+mi*nj
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
            l=l+1
 93      continue
 94   continue
c     form the resulting variable
      istk(ilrs)=9
      istk(ilrs+1)=mi
      istk(ilrs+2)=nj
      istk(ilrs+3)=0
      l1=sadr(ilrs+4)
      if(lr.ne.l1) call unsfdcopy(mn,stk(lr),1,stk(l1),1)
      lstk(top+1)=l1+mn
      return

      end

      subroutine hndlcmp
c     
c     ==  <>
c     
c     Copyright INRIA
      include 'stack.h'
c     
      integer top0,op
      integer less,great,equal,cmp,comparehandles
      integer iadr,sadr
      data less/59/,great/60/,equal/50/
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      top0=top
      op=fin
      lw=lstk(top+1)+1
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

      if(op.ne.equal.and.op.ne.less+great) then
         fin=-fin
         top=top+1
         return
      endif
c     comparaisons
      if(m1.eq.-1) then
c     .  eye op b
         err=lw+mn2-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         if(m2.eq.-1) then
            m2=1
            n2=1
         elseif(mn2.gt.0) then
            call dset(mn2,0.0d0,stk(lw),1)
            call dset(min(m2,n2),stk(l1),stk(lw),m2+1)
            if(it1.eq.1) then
               call dset(mn2,0.0d0,stk(lw+mn2),1)
               call dset(min(m2,n2),stk(l1+1),stk(lw+mn2),m2+1)
            endif
            l1=lw
         endif
         m1=m2
         n1=n2
         mn1=mn2
         istk(il1+1)=m1
         istk(il1+2)=n1
      elseif(m2.eq.-1) then
         err=lw+mn1-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call dset(mn1,0.0d0,stk(lw),1)
         call dset(min(m1,n1),stk(l2),stk(lw),m1+1)
         l2=lw
         mn2=mn1
         m2=m1
         n2=n1
      elseif(mn1.eq.1.and.mn2.gt.1) then
         err=lw+mn2-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call dset(mn2,stk(l1),stk(lw),1)
         l1=lw
         mn1=mn2
         m1=m2
         n1=n2
         istk(il1+1)=m1
         istk(il1+2)=n1
      elseif(mn2.eq.1.and.mn1.gt.1) then
         err=lw+mn1-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call dset(mn1,stk(l2),stk(lw),1)
         l2=lw
         mn2=mn1
         m2=m1
         n2=n1
      endif
      if(mn2.eq.0.or.mn1.eq.0) then
         itrue=0
         if(mn2.eq.0.and.mn1.eq.0) itrue=1
         if(op.eq.less+great) itrue=1-itrue
         istk(il1)=4
         istk(il1+1)=1
         istk(il1+2)=1
         istk(il1+3)=itrue
         lstk(top+1)=sadr(il1+4)
         return
      endif
      if(n1.ne.n2.or.m1.ne.m2) then
         istk(il1)=4
         istk(il1+1)=1
         istk(il1+2)=1
         istk(il1+3)=0
         if(op.eq.less+great) istk(il1+3)=1
         lstk(top+1)=sadr(il1+4)
      else
         istk(il1)=4
         if(mn1.eq.0) then
            istk(il1+1)=1
            istk(il1+2)=1
            istk(il1+3)=1
            if(op.ne.equal) istk(il1+3)=0
            lstk(top+1)=sadr(il1+4)
            return
         endif
         
         do 132 i=0,mn1-1
            cmp=comparehandles(stk(l1+i),stk(l2+i))
            if((op.eq.equal.and.cmp.eq.1).or.
     $           (op.eq.less+great.and. cmp.eq.0)) then
               istk(il1+3+i)=1
            else
               istk(il1+3+i)=0
            endif
 132     continue
      endif
      lstk(top+1)=sadr(il1+3+mn1)
      return
      end

