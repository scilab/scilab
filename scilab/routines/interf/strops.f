      subroutine strops
c ====================================================================
c
c   operations sur les matrices de chaines de caracteres
c
c ====================================================================
c
c     Copyright INRIA
      include '../stack.h'
c
      integer plus,quote,equal,less,great,insert,extrac
      integer top0,iadr,sadr,op,vol,volr,rhs1
      logical isany
c
      data plus/45/,quote/53/
      data equal/50/,less/59/,great/60/,insert/2/,extrac/3/
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c 
      op=fin
c
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' strops '//buf(1:4))
      endif
c
      fun=0
c
      top0=top
c
      lw=lstk(top+1)
      rhs1=rhs
      if(op.eq.extrac) goto 130
      if(op.eq.insert) goto 120
c
      if(rhs.eq.1) goto 05
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)
      n2=istk(il2+2)
      it2=istk(il2+3)
      mn2=m2*n2
      id2=il2+4
      l2r=id2+mn2+1
      l3r=lw
c
      top = top-1
   05 il1=iadr(lstk(top))
      ilrs=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      mn1 = m1*n1
      id1=il1+4
      l1r=id1+mn1+1
      vol=istk(id1+mn1)-1
c
      goto (60,120,130,65) op
      if (rhs .eq. 1.and.op.eq.quote) goto 110
      if (op .eq. plus ) go to 20
      if(op.eq.equal.or.op.eq.less+great) goto 180
c
c     operations non implantees
 10   top=top0
      rhs=rhs1
      fin=-fin
      return


c
c addition
c
 20   continue
      if(m1*n1.eq.0) then
c     .  []+b
         vol=5+mn2+istk(id2+mn2)-1
         call icopy(vol,istk(il2),1,istk(il1),1)
         lstk(top+1)=sadr(il1+vol)
         return
      elseif(m2*n2.eq.0) then
c     .  a+[]
         return
      elseif(m1.ne.m2.or.n1.ne.n2) then
         top=top0
         rhs=rhs1
         fin=-fin
         return
      endif
      if(istk(il1).ne.istk(il2)) goto 10
      err=lw+sadr(istk(id1+mn1)+istk(id2+mn2))-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c
      lr=iadr(lw)
      l=lr
      i1=1
      do 11 i=1,mn1
      la=istk(id1+i)-i1
      lb=istk(id2+i)-istk(id2+i-1)
      i1=istk(id1+i)
      istk(id1+i)=istk(id1+i-1)+la+lb
      call icopy(la,istk(l1r),1,istk(l),1)
      l1r=l1r+la
      l=l+la
      call icopy(lb,istk(l2r),1,istk(l),1)
      l2r=l2r+lb
      l=l+lb
11    continue
      call icopy(l-lr,istk(lr),1,istk(il1+5+mn1),1)
      lstk(top+1)=sadr(il1+5+mn1+l-lr)
      goto 999
c
c concatenation [a, b]
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
      if(istk(il1).ne.istk(il2)) goto 10
c
      id3=iadr(lw)
      l3r=id3+mn1+mn2+1
      vol=istk(id1+mn1)+istk(id2+mn2)-2
      err=sadr(l3r+vol)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call impcnc(istk(l1r),istk(id1),m1,istk(l2r),istk(id2),m1,
     & istk(l3r),istk(id3),m1,n1,n2,1)
c
      istk(il1)=10
      istk(il1+1)=m1
      istk(il1+2)=n1+n2
      istk(il1+3)=it1
      call icopy(mn1+mn2+vol+1,istk(id3),1,istk(il1+4),1)
      lstk(top+1)=sadr(il1+5+mn1+mn2+vol)
      goto 999
c
c     concatenation [a;b]
 65   continue
      if(n1.lt.0.or.n2.lt.0) then
            call error(14)
            return
      endif
      if(n2.eq.0) then
         return
      elseif(n1.eq.0)then
         call unsfdcopy(lstk(top+2)-lstk(top+1),stk(lstk(top+1)),1,
     &        stk(lstk(top)),1)
         lstk(top+1)=lstk(top)+lstk(top+2)-lstk(top+1)
         return
      elseif(n1.ne.n2) then
         call error(6)
         return
      endif
      if(istk(il1).ne.istk(il2)) goto 10
c
      id3=iadr(lw)
      l3r=id3+mn1+mn2+1
      vol=istk(id1+mn1)+istk(id2+mn2)-2
      err=sadr(l3r+vol)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call impcnc(istk(l1r),istk(id1),m1,istk(l2r),istk(id2),m2,
     & istk(l3r),istk(id3),m1,m2,n2,-1)
c
      istk(il1)=10
      istk(il1+1)=m1+m2
      istk(il1+2)=n1
      istk(il1+3)=it1
      call icopy(mn1+mn2+vol+1,istk(id3),1,istk(il1+4),1)
      lstk(top+1)=sadr(il1+5+mn1+mn2+vol)
      goto 999
c
c transposition
c
  110 continue
      id2=iadr(lw)
      l2r=id2+mn1+1
      err=sadr(l2r+vol)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call imptra(istk(l1r),istk(id1),m1,istk(l2r),istk(id2),m1,n1)
      istk(il1+1)=n1
      istk(il1+2)=m1
      call icopy(mn1+1+vol,istk(id2),1,istk(id1),1)
      goto 999
c
c insertion
c
  120 continue
      if(rhs.gt.4) then
         top=top0
         fin=-fin
         return
      endif
      if(rhs.eq.4) goto 125
c     arg3(arg1)=arg2
c     
c     get arg3
      il3=iadr(lstk(top))
      if(istk(il3).lt.0) il3=iadr(istk(il3+1))

      if(istk(il3).ne.10) then
         if(istk(il3).ne.1.or.istk(il3+1).ne.0) then
            top=top0
            fin=-fin
            return
         endif
      endif
      m3=istk(il3+1)
      n3=istk(il3+2)
      mn3=m3*n3
      id3=il3+4
      l3r=id3+mn3+1
c     get arg2
      top=top-1
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      if(istk(il2).ne.10) then
         if(istk(il2).eq.1) then
            if(istk(il2+1)*istk(il2+2).eq.0) goto 121
         endif
         top=top0
         fin=-fin
         return
      endif
 121  m2=istk(il2+1)
      n2=istk(il2+2)
      mn2=m2*n2
      id2=il2+4
      l2r=id2+mn2+1
c     get arg1
      top=top-1
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
            istk(ilrs)=10
            istk(ilrs+1)=m3
            istk(ilrs+2)=n3
            istk(ilrs+3)=0
            volr=istk(id3+mn3)-1
            call icopy(mn3+1+volr,istk(id3),1,istk(ilrs+4),1)
            lstk(top+1)=sadr(ilrs+5+mn3+volr)
            goto 999
         else
c     .     arg3(arg1)=[]
            if(istk(il1).eq.4.and.m3.eq.m1.and.n3.eq.n1) then
               if(.not.isany(il1)) then
c     .           arg3([])=[]  --> arg3
                  istk(ilrs)=10
                  istk(ilrs+1)=m3
                  istk(ilrs+2)=n3
                  istk(ilrs+3)=0
                  volr=istk(id3+mn3)-1
                  call icopy(mn3+1+volr,istk(id3),1,istk(ilrs+4),1)
                  lstk(top+1)=sadr(ilrs+5+mn3+volr)
                  goto 999
               endif
            endif
c     .     arg3(arg1)=[] --> arg3(compl(arg1))
            call indxgc(il1,mn3,ilr,mi,mx,lw)
            if(err.gt.0) return
            l2r=l3r
            n2=n3
            m2=m3
            mn2=m2*n2
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
            if(mn2.eq.1) goto 124
            call error(15)
            return
         endif
c     .  reshape arg2 according to arg3
         istk(ilrs)=10
         istk(ilrs+1)=m3
         istk(ilrs+2)=n3
         istk(ilrs+3)=0
         volr=istk(id2+mn2)-1
         call icopy(mn3+1+volr,istk(id2),1,istk(ilrs+4),1)
         lstk(top+1)=sadr(ilrs+5+mn3+volr)
         goto 999
      endif
 124  call indxg(il1,mn3,ili,mi,mxi,lw,1)
      if(err.gt.0) return
      if(mi.eq.0) then
c     .  arg3([])=arg2
         if(mn2.eq.1) then
c     .  arg3([])=c  --> arg3 
            istk(ilrs)=10
            istk(ilrs+1)=m3
            istk(ilrs+2)=n3
            istk(ilrs+3)=0
            volr=istk(id3+mn3)-1
            call icopy(mn3+1+volr,istk(id3),1,istk(ilrs+4),1)
            lstk(top+1)=sadr(ilrs+5+mn3+volr)
            goto 999
         else
            call error(15)
            return
         endif
      endif
      if(mi.ne.mn2.and.mn2.gt.1) then
         call error(15)
         return
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
      mnr=mr*nr
c     set result pointers
      idr=iadr(lw)
      lr=idr+mr*nr+1
      lw=sadr(lr)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call mpinsp(istk(id3),m3*n3,1,istk(ili),mi,1,1,istk(id2),m2*n2,1
     $     ,istk(idr),mnr,1,err)
      if(err.gt.0) then
         call error(15)
         return
      endif
      volr=istk(idr)
c     set result coefficients
      
      lw=sadr(lr+volr)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c     
      call impins(istk(l3r),istk(id3),m3*n3,1,istk(l2r),istk(id2),
     $     m2*n2,1,istk(lr),istk(idr),mnr,1)

c     set output variable
      ilrs=iadr(lstk(top))
      istk(ilrs)=10
      istk(ilrs+1)=mr
      istk(ilrs+2)=nr
      istk(ilrs+3)=0
      call icopy(mnr+1+volr,istk(idr),1,istk(ilrs+4),1)
      lstk(top+1)=sadr(ilrs+5+mnr+volr)
      goto 999
c
 125  continue
c     arg4(arg1,arg2)=arg3
c     get arg4
      il4=iadr(lstk(top))
      if(istk(il4).lt.0) il4=iadr(istk(il4+1))
      if(istk(il4).ne.10) then
         if(istk(il4).ne.1.or.istk(il4+1).ne.0) then
            top=top0
            fin=-fin
            return
         endif
      endif
      m4=istk(il4+1)
      n4=istk(il4+2)
      mn4=m4*n4
      id4=il4+4
      l4r=id4+mn4+1
      top=top-1
c     get arg3
      il3=iadr(lstk(top))
      if(istk(il3).lt.0) il3=iadr(istk(il3+1))
      if(istk(il3).ne.10) then
         if(istk(il3).eq.1) then
            if(istk(il3+1)*istk(il3+2).eq.0) goto 126
         endif
         top=top0
         fin=-fin
         return
      endif
 126  m3=istk(il3+1)
      n3=istk(il3+2)
      mn3=m3*n3
      id3=il3+4
      l3r=id3+mn3+1
c     get arg2
      top=top-1
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)
c     get arg1
      top=top-1
      il1=iadr(lstk(top))
      ilrs=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
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
            istk(ilrs)=10
            istk(ilrs+1)=m4
            istk(ilrs+2)=n4
            istk(ilrs+3)=0
            volr=istk(id4+mn4)-1
            call icopy(mn4+1+volr,istk(id4),1,istk(ilrs+4),1)
            lstk(top+1)=sadr(ilrs+5+mn4+volr)
            goto 999
         elseif(m2.eq.-1) then
c     .     arg4(arg1,:)=[] --> arg4(compl(arg1),:)
            call indxgc(il1,m4,ili,mi,mxi,lw)
            if(err.gt.0) return
            call indxg(il2,n4,ilj,nj,mxj,lw,1)
            if(err.gt.0) return
            l3r=l4r
            n3=n4
            m3=m4
            mn3=m3*n3
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
            n3=n4
            m3=m4
            mn3=m3*n3
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
                  mn3=m3*n3
                  id3=id4
c     .           call extraction
                  goto 133
               endif
            else
c               lw=lw1
               call indxgc(il1,m4,ili,mi,mxi,lw)
               if(err.gt.0) return
               if(mi.eq.0) then
c     .           arg4(1:m4,arg2)=[] 
                  call indxg(il1,m4,ili,mi,mxi,lw,1)
                  if(err.gt.0) return
                  l3r=l4r
                  n3=n4
                  m3=m4
                  mn3=m3*n3
                  id3=id4
c     .           call extraction
                  goto 133
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
            if(mn3.eq.1) goto 127
            call error(15)
            return
         endif
c     .  reshape arg3 according to arg4
         istk(ilrs)=10
         istk(ilrs+1)=m4
         istk(ilrs+2)=n4
         istk(ilrs+3)=0
         volr=istk(id3+mn3)-1
         call icopy(mn3+volr,istk(id3),1,istk(ilrs+4),1)
         lstk(top+1)=sadr(ilrs+5+mn3+volr)
         goto 999
      endif

 127  call indxg(il1,m4,ili,mi,mxi,lw,1)
      if(err.gt.0) return
      call indxg(il2,n4,ilj,mj,mxj,lw,1)
      if(err.gt.0) return
      if(mi.ne.m3.or.mj.ne.n3) then
         if(m3*n3.eq.1) then
            if(mi.eq.0.or.mj.eq.0) then
               istk(ilrs)=10
               istk(ilrs+1)=m4
               istk(ilrs+2)=n4
               istk(ilrs+3)=0
               volr=istk(id4+mn4)-1
               call icopy(mn4+volr,istk(id4),1,istk(ilrs+4),1)
               lstk(top+1)=sadr(ilrs+5+mn4+volr)
               goto 999
            endif
         else
c     .  sizes of arg1 or arg2 dont agree with arg3 sizes
            call error(15)
            return
         endif
         if(mi.eq.0.or.mj.eq.0) then
            call error(15)
            return
         endif
      endif
      mr=max(m4,mxi)
      nr=max(n4,mxj)
c
      mnr=mr*nr
c     set result pointers
      idr=iadr(lw)
      lr=idr+mr*nr+1
      lw=sadr(lr)
      err=lw-lstk(bot)
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
      lw=sadr(lr+volr)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c     
      call impins(istk(l4r),istk(id4),m4,n4,istk(l3r),istk(id3),m3,n3
     $     ,istk(lr),istk(idr),mr,nr)
c     set output variable
      ilrs=iadr(lstk(top))
      istk(ilrs)=10
      istk(ilrs+1)=mr
      istk(ilrs+2)=nr
      istk(ilrs+3)=0
      call icopy(mnr+1+volr,istk(idr),1,istk(ilrs+4),1)
      lstk(top+1)=sadr(ilrs+5+mnr+volr)
      goto 999
c
c extraction
c
  130 continue
      if(rhs.lt.2) then
         call error(227)
         return
      endif
      if(rhs.gt.2) goto 132
c     arg2(arg1)
c     get arg2
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)
      n2=istk(il2+2)
      mn2=m2*n2
      id2=il2+4
      l2r=id2+mn2+1
c     get arg1
      top=top-1
      il1=iadr(lstk(top))
      ilrs=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)

      if(mn2.eq.0) then 
c     .  arg2=[]
         ilrs=iadr(lstk(top))
         istk(ilrs)=1
         istk(ilrs+1)=0
         istk(ilrs+2)=0
         istk(ilrs+3)=0
         l1=sadr(ilrs+4)
         lstk(top+1)=l1+1
         goto 999
      elseif(m2.lt.0) then
c     .  arg2=eye
         call error(14)
         return
      elseif(m1.lt.0) then
c     .  arg2(:), just reshape to column vector
         ilrs=iadr(lstk(top))
         istk(ilrs)=10
         istk(ilrs+1)=mn2
         istk(ilrs+2)=1
         istk(ilrs+3)=istk(il2+3)
         volr=istk(id2+mn2)-1
         call icopy(mn2+1+volr,istk(id2),1,istk(ilrs+4),1)
         lstk(top+1)=sadr(ilrs+5+mn2+volr)
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
         ilrs=iadr(lstk(top))
         istk(ilrs)=1
         istk(ilrs+1)=0
         istk(ilrs+2)=0
         istk(ilrs+3)=0
         l1=sadr(ilrs+4)
         lstk(top+1)=l1+1
         goto 999
      endif
c     get memory for the result
      idr=iadr(lw)
      lr=idr+mi+1
      lw=sadr(lr)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c     set result pointers
      if (m2 .gt. 1.or.m1.lt.0) then
         call impext(istk(l2r),istk(id2),m2,n2,istk(ili),mi,1,1,istk(lr)
     $     ,istk(idr),0,err)
      else
         call impext(istk(l2r),istk(id2),m2,n2,1,1,istk(ili),mi,istk(lr)
     $     ,istk(idr),0,err)
      endif
      if(err.gt.0) then
         call error(21)
         return
      endif

c     set result coefficients 
      volr=istk(idr+mi)-1
      lw=sadr(lr+volr)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      if (m2.eq.1.and.n2.eq.1.and.m1.gt.0) then
         call impext(istk(l2r),istk(id2),m2,n2,istk(ili),mi,1,1,istk(lr)
     $     ,istk(idr),1,err)
         m = m1
         n = min(n1,mi)
      elseif (m2 .gt. 1.or.m1.lt.0) then
         call impext(istk(l2r),istk(id2),m2,n2,istk(ili),mi,1,1,istk(lr)
     $     ,istk(idr),1,err)
         m = mi
         n = 1
      else
         call impext(istk(l2r),istk(id2),m2,n2,1,1,istk(ili),mi,istk(lr)
     $     ,istk(idr),1,err)
         n = mi
         m = 1
      endif

c     form resulting variable
      ilrs=iadr(lstk(top))
      istk(ilrs)=10
      istk(ilrs+1)=m
      istk(ilrs+2)=n
      istk(ilrs+3)=0
      volr=istk(idr+mi)-1
      call icopy(mi+1+volr,istk(idr),1,istk(ilrs+4),1)
      lstk(top+1)=sadr(ilrs+5+m*n+volr)
      go to 999
 132  continue
c     arg3(arg1,arg2)
      if(rhs.gt.3) then
         call error(36)
         return
      endif
c     get arg3
      il3=iadr(lstk(top))
      if(istk(il3).lt.0) il3=iadr(istk(il3+1))
      m3=istk(il3+1)
      n3=istk(il3+2)
      mn3=m3*n3
      id3=il3+4
      l3r=id3+mn3+1
c     get arg2
      top=top-1
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)
c     get arg1
      top=top-1
      il1=iadr(lstk(top))
      ilrs=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      l1=sadr(il1+4)
c
      if(mn3.eq.0) then 
c     .  arg3=[]
         ilrs=iadr(lstk(top))
         istk(ilrs)=1
         istk(ilrs+1)=0
         istk(ilrs+2)=0
         istk(ilrs+3)=0
         l1=sadr(ilrs+4)
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
         ilrs=iadr(lstk(top))
         istk(ilrs)=1
         istk(ilrs+1)=0
         istk(ilrs+2)=0
         istk(ilrs+3)=0
         lstk(top+1)=sadr(ilrs+4)+1
         goto 999
      endif
      idr=iadr(lw)
      lr=idr+mnr+1
      lw=sadr(lr)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c     set result pointers
      call impext(istk(l3r),istk(id3),m3,n3,istk(ili),mi,istk(ilj),nj
     $     ,istk(lr),istk(idr),0,err)
      if(err.gt.0) then
         call error(21)
         return
      endif
c     set result coefficients 
      volr=istk(idr+mnr)-1
      lw=sadr(lr+volr)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call impext(istk(l3r),istk(id3),m3,n3,istk(ili),mi,istk(ilj),nj
     $     ,istk(lr),istk(idr),1,err)
c
      ilrs=iadr(lstk(top))
      istk(ilrs)=10
      istk(ilrs+1)=mi
      istk(ilrs+2)=nj
      istk(ilrs+3)=0
      call icopy(mnr+1+volr,istk(idr),1,istk(ilrs+4),1)
      lstk(top+1)=sadr(ilrs+5+mnr+volr)
      goto 999

c
c     comparaisons
 180  continue
      itrue=1
      if(op.eq.less+great) itrue=0
c     comparaison des types
      if(istk(il1).ne.istk(il2)) then
         istk(il1)=4
         istk(il1+1)=1
         istk(il1+2)=1
         istk(il1+3)=1-itrue
         lstk(top+1)=sadr(il1+4)
         return
      endif
 181  continue
c     des dimensions
      if(mn1.eq.1.and.mn2.gt.1) then
         nn1=istk(il1+5)-1
         l1r=iadr(lw)
         err=sadr(l1r+nn1+2)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call icopy(nn1,istk(il1+6),1,istk(l1r),1)
         id1=l1r+nn1
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
         nn2=istk(il2+5)-1
         l2r=iadr(lw)
         err=sadr(l2r+nn2+2)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call icopy(nn2,istk(il2+6),1,istk(l2r),1)
         id2=l2r+nn2
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
         l1=il1+5+mn1
         l2=il2+5+mn2
      endif
c     des valeurs
      i1=id1-inc1
      i2=id2-inc2
      l1r=l1r-1
      l2r=l2r-1
      do 185 i=0,mn1-1
         i1=i1+inc1
         i2=i2+inc2
         if(istk(i1+1)-istk(i1).ne.istk(i2+1)-istk(i2) ) goto 184
         nl=istk(i1+1)-istk(i1)-1
         do 182 ii=0,nl
            if(istk(l1r+istk(i1)+ii).ne.istk(l2r+istk(i2)+ii)) goto 184
 182     continue
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

  999 return
      end
