      subroutine i_i_i
c     insertion
      include '../stack.h'
      logical ref
      integer top0
      integer is2
      external memused
      integer memused
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      top0=top
      lw=lstk(top+1)
      if(rhs.gt.4) then
         call cvname(ids(1,pt+1),'%s_i_s',0)
         fun=-1
         return
      endif
      if(rhs.eq.4) goto 100
c
c     arg3(arg1)=arg2
c
      il3=iadr(lstk(top))
      ref=istk(il3).lt.0
      if(ref) il3=iadr(istk(il3+1))
      ityp3=istk(il3)
      m3=istk(il3+1)
      n3=istk(il3+2)
      it3=istk(il3+3)
      l3=il3+4
      mn3=m3*n3
      top=top-1
c     
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      ityp2=istk(il2)
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

      if(istk(il3).eq.1.and.mn3.eq.0) then
         ityp=istk(il2)
         it3=it2
         goto 10
      endif
      if(istk(il2).eq.1.and.mn2.eq.0) then
         ityp=istk(il3)
         it2=it3
         goto 10
      endif

      if(it2.ne.it3.or.istk(il3).ne.istk(il2)) then 
         top=top0
         fin=-fin
         return
      endif
      ityp=istk(il3)

 10   if (m2.eq.0) then
c     .  arg3(arg1)=[] -->[]
         if(m1.eq.-1) then
c     .    arg3(:)=[] 
            istk(il1)=1
            istk(il1+1)=0
            istk(il1+2)=0
            istk(il1+3)=0
            lstk(top+1)=sadr(il1+4)
            goto 999
         elseif(m1.eq.0) then
c     .     arg3([])=[]  --> arg3
            call icopy(4,istk(il3),1,istk(il1),1)
            l=il1+4
            call gencopy(it3,mn3,istk(l3),1,istk(l),1)
            lstk(top+1)=sadr(l+memused(it3,mn3))
            goto 999
         else
c     .     arg3(arg1)=[] -->arg3(compl(arg1),:)
            call indxgc(il1,mn3,ilr,mi,mx,lw)
            if(err.gt.0) return
            l2=l3
            n2=n3
            m2=m3
            mn2=m2*n2
            it2=it3
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
            istk(il1)=ityp
            istk(il1+1)=m3
            istk(il1+2)=n3
            istk(il1+3)=it2
            l1=il1+4
            call gencopy(it2,mn2,istk(l2),1,istk(l1),1)
            lstk(top+1)=sadr(l1+memused(it2,mn2))
            return
         elseif(mn2.eq.1) then
            istk(il1)=ityp
            istk(il1+1)=m3
            istk(il1+2)=n3
            istk(il1+3)=it2
            l1=il1+4
            call genset(it2,mn3,istk(l2),istk(l1),1)
            lstk(top+1)=sadr(l1+memused(it2,mn3))
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
            call icopy(4,istk(il3),1,istk(il1),1)
            l=il1+4
            call gencopy(it3,mn3,istk(l3),1,istk(l),1)
            lstk(top+1)=sadr(l+memused(it3,mn3))
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

      lr=l3
      mnr=mr*nr
      if(mnr.ne.mn3) then
c     .  resulting matrix is bigger than original
         lr=iadr(lw)
         lw=sadr(lr + mnr)
         err = lw - lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
c     .  initialise result r to 0 and write arg3 in r
         call tpconv(4,it3,1,0,1,is2,1)
         call genset(it3,mnr,is2,istk(lr),1)
         if(mn3.ge.1) then
            call genmcopy(it3,istk(l3),m3,istk(lr),mr,m3,n3)
         endif
      endif
c     write arg2 in r
      call geninsert1(it3,mi,istk(ili),istk(lr),istk(l2),inc2)
c     
      if(lr.ne.l3.or..not.ref) then
         l1=il1+4
         call gencopy(it3,mnr,istk(lr),1,istk(l1),1)
         lstk(top+1)=sadr(l1+memused(it3,mnr))
         istk(il1)=ityp
         istk(il1+1)=mr
         istk(il1+2)=nr
         istk(il1+3)=it3
      else

c     la matrice a ete modifie sur place 
         k=istk(iadr(lstk(top0))+2)
         istk(il1)=-ityp
         istk(il1+1)=-1
         istk(il1+2)=k
         lstk(top+1)=lstk(top)+3
      endif
      goto 999

 100  continue
c     
c     arg4(arg1,arg2)=arg3

 01   il4=iadr(lstk(top))
      ref=istk(il4).lt.0
      if(ref) il4=iadr(istk(il4+1))
      
      m4=istk(il4+1)
      n4=istk(il4+2)
      it4=istk(il4+3)
      l4=il4+4
      mn4=m4*n4
      top=top-1
c     
 02   il3=iadr(lstk(top))
      if(istk(il3).lt.0) il3=iadr(istk(il3+1))
      m3=istk(il3+1)
      n3=istk(il3+2)
      it3=istk(il3+3)
      l3=il3+4
      mn3=m3*n3
      top=top-1
c     
 03   il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)
      n2=istk(il2+2)
      it2=istk(il2+3)
      l2=sadr(il2+4)
      mn2=m2*n2
      top=top-1
c     
 04   il1=iadr(lstk(top))
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=sadr(il1+4)
      mn1=m1*n1

      if(it3.ne.it4.or.istk(il3).ne.istk(il4)) then 
         top=top0
         fin=-fin
         return
      endif
      ityp=istk(il3)

      if (m3.eq.0) then
c     .  arg4(arg1,arg2)=[]
         if(m1.eq.-1.and.m2.eq.-1) then
c     .    arg4(:,:)=[] -->[]
            istk(il1)=1
            istk(il1+1)=0
            istk(il1+2)=0
            istk(il1+3)=0
            lstk(top+1)=sadr(il1+4)
            goto 999
         elseif(m1.eq.0.or.m2.eq.0) then
c     .     arg4([],arg2)=[],  arg4(arg1,[])=[] --> arg4
            call icopy(4,istk(il4),1,istk(il1),1)
            l=il1+4
            call gencopy(it4,mn4,istk(l4),1,istk(l),1)
            lstk(top+1)=sadr(l+memused(it4,mn4))
            goto 999
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
                  istk(il1)=1
                  istk(il1+1)=0
                  istk(il1+2)=0
                  istk(il1+3)=0
                  lstk(top+1)=sadr(il1+4)
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
                  l3=l4
                  n3=n4
                  m3=m4
                  it3=it4
                  mn3=m3*n3
c     .           call extraction
                  goto 90
               endif
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
            istk(il1)=ityp
            istk(il1+1)=m4
            istk(il1+2)=n4
            istk(il1+3)=it3
            l1=il1+4
            call gencopy(it3,mn4,istk(l3),1,istk(l1),1)
            lstk(top+1)=sadr(l1+memused(it3,mn4))
            return
         elseif(mn3.eq.1) then
c     .     set all elements of arg4 to arg3
            istk(il1)=ityp
            istk(il1+1)=m4
            istk(il1+2)=n4
            istk(il1+3)=it3
            l1=il1+4
            call genset(it3,mn4,istk(l3),istk(l1),1)
            lstk(top+1)=sadr(l1+memused(it3,mn4))
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
               call icopy(4,istk(il4),1,istk(il1),1)
               l=il1+4
               call gencopy(it4,mn4,istk(l4),1,istk(l),1)
               lstk(top+1)=sadr(l+memused(it4,mn4))
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

      if(mnr.ne.mn4) then
         lr=iadr(lw)
         lw=sadr(lr + mnr)
         err = lw - lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
c     .  set result r to 0
         call tpconv(4,it4,1,0,1,is2,1)
         call genset(it4,mnr,is2,istk(lr),1)
c     .  copy arg4 in r
         if(mn4.ge.1) then
            call genmcopy(it4,istk(l4),m4,istk(lr),mr,m4,n4)
         endif
      else
         lr=l4
      endif
c     
c     copy arg3 elements in r
      call geninsert2(it4,mj,mi,istk(ilj),istk(ili),istk(lr),mr,
     $        istk(l3),m3,inc3)
c     
      if(lr.ne.l4.or..not.ref) then
         l1=il1+4
         call gencopy(it4,mnr,istk(lr),1,istk(l1),1)
         lstk(top+1)=sadr(l1+memused(it4,mnr))
         istk(il1)=ityp
         istk(il1+1)=mr
         istk(il1+2)=nr
         istk(il1+3)=it4
      else
c     la matrice a ete modifie sur place 
         k=istk(iadr(lstk(top0))+2)
         istk(il1)=-1
         istk(il1+1)=-1
         istk(il1+2)=k
         lstk(top+1)=lstk(top)+3
      endif
      goto 999
c
c     inline extraction code 
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
      istk(il1)=ityp
      istk(il1+1)=m
      istk(il1+2)=n
      istk(il1+3)=it2
      if(lr.ne.l1) then
         call gencopy(it2,mi,istk(lr),1,istk(l1),1)
         lstk(top+1)=sadr(l1+memused(it2,mi))
      endif
      go to 999

 90   continue
c     inline extraction code
      mn=mi*nj
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
      call genextrac2(it3,mi,nj,istk(ili),istk(ilj),istk(l3),m3,
     $     istk(lr))
c     form the resulting variable
      istk(il1)=ityp
      istk(il1+1)=mi
      istk(il1+2)=nj
      istk(il1+3)=it3
      if(lr.ne.l1) then
         call gencopy(it3,mn,istk(lr),1,istk(l1),1)
      endif
      lstk(top+1)=sadr(l1+memused(it3,mn))
      go to 999


 999  return
      end
