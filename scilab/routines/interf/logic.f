      subroutine logic
c     ====================================================================
c     
c     operations sur les booleens et matrices de booleens
c     
c     ====================================================================
c     
c     Copyright INRIA
      INCLUDE '../stack.h'
c     
      integer dot,colon
      integer less,great,equal,et,ou,non
      integer insert,extrac
c     
      integer iadr,sadr,op,top0
      logical isany
c     
      data dot/51/,colon/44/
      data less/59/,great/60/,equal/50/
      data ou/57/,et/58/,non/61/
      data insert/2/,extrac/3/
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      op=fin
      top0=top
c     
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' logic op: '//buf(1:4))
      endif
c     
      lw=lstk(top+1)
      if(op.eq.extrac) goto 50
      if(op.eq.insert) goto 60
      it2=0
      goto (04,03,02,01) rhs
      call error(39)
      return
c     
 01   il4=iadr(lstk(top))
      if(istk(il4).lt.0) il4=iadr(istk(il4+1))
      m4=istk(il4+1)
      n4=istk(il4+2)
      mn4=m4*n4
      if(istk(il4).eq.4) then
         l4=il4+3
      elseif(istk(il4).le.2) then
         it4=istk(il4+3)
         l4=sadr(il4+4)
      else
         err=4
         call error(44)
         return
      endif
      top=top-1
c     
 02   il3=iadr(lstk(top))
      if(istk(il3).lt.0) il3=iadr(istk(il3+1))
      m3=istk(il3+1)
      n3=istk(il3+2)
      mn3=m3*n3
      if(istk(il3).eq.4) then
         l3=il3+3
      elseif(istk(il3).le.2) then
         it3=istk(il3+3)
         l3=sadr(il3+4)
      else
         err=3
         call error(44)
         return
      endif
      top=top-1
c     
 03   il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)
      n2=istk(il2+2)
      mn2=m2*n2
      if(istk(il2).eq.4) then
         l2=il2+3
      elseif(istk(il2).le.2) then
         it2=istk(il2+3)
         l2=sadr(il2+4)
      else
         err=2
         call error(44)
         return
      endif
      top=top-1
c     
 04   il1=iadr(lstk(top))
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      mn1=m1*n1
      if(istk(il1).eq.4) then
         l1=il1+3
      elseif(istk(il1).eq.1) then
         it1=istk(il1+3)
         l1=sadr(il1+4)
      else
         err=1
         call error(44)
         return
      endif
      top=top-1
c     
c     operations binaires et ternaires
c     --------------------------------
c     
      top=top+1
c     
      fun = 0
c     
c         cconc              rconc
      goto(45  ,  999  , 999  ,48 ) op
c     
c     
c          :  +   -  * /  \
      goto(15,15,15,15,15,15,130,06,06,40,130,130) op+1-colon
      if(op.eq.ou.or.op.eq.et) goto 20
      if(op.eq.non) goto 30
c     
      
 06   if(op.gt.3*dot) goto 15
      if(op.ge.equal+equal) goto 130
      if(op.gt.dot) goto 15
      
c     

c     operations non implantees
 15   fin=-fin
      top=top0
      go to 999
c     
c     ou/et logique
 20   m1=abs(m1)
      n1=abs(n1)
      m2=abs(m2)
      n2=abs(n2)
      if(istk(il2).ne.4.or.istk(il1).ne.4) then
         fin=-fin
         top=top0
         return
      endif
      if(mn1.eq.1.and.mn2.gt.1) then
         l1=iadr(lw)
         err=sadr(l1+mn2)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call iset(mn2,istk(il1+3),istk(l1),1)
         mn1=mn2
         m1=m2
         n1=n2
      elseif(mn2.eq.1.and.mn1.gt.1) then
         l2=iadr(lw)
         err=sadr(l2+mn2)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call iset(mn1,istk(il2+3),istk(l2),1)
         mn2=mn1
         m2=m1
         n2=n1
      else if (n1 .ne. n2.or.m1.ne.m2) then
         call error(60)
         return
      else
         l1=il1+3
         l2=il2+3
      endif
      istk(il1+1)=m1
      istk(il1+2)=n1
      if(fin.eq.et) then
         do 21 k=0,n1*m1-1
            istk(il1+3+k)=istk(l1+k)*istk(l2+k)
 21      continue  
      else
         do 22 k=0,n1*m1-1
            istk(il1+3+k)=max(istk(l1+k),istk(l2+k))
 22      continue  
      endif
      lstk(top+1)=sadr(il1+3+m1*n1)
c         
      goto 999
c NOT
 30   continue
      do 31 k=0,n1*m1-1
         istk(il1+3+k)=1-istk(il1+3+k)
 31   continue  
      goto 999

c     transposition
 40   if(istk(il1).ne.4) then
         err=2
         call error(44)
         return
      endif
      if(mn1 .eq. 0) goto 999
      ll = l1+mn1+1
      err = sadr(ll+mn1) - lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
c     
      istk(il1+1)=n1
      istk(il1+2)=m1
c     
      call icopy(mn1,istk(l1),1,istk(ll),1)
      do 41 j=0,n1-1
         call icopy(m1,istk(ll+j*m1),1,istk(l1+j),n1)
 41   continue
c     
      goto 999
c     
c     concatenation
 45   if(m1.lt.0.or.m2.lt.0) then
         call error(14)
         return
      endif
      if(mn1.eq.0) then
         call icopy(mn2+3,istk(il2),1,istk(il1),1)
         lstk(top+1)=sadr(il1+4+mn2)
         goto 999
      endif
      if(mn2.eq.0)  goto 999
      if(istk(il1).ne.istk(il2)) then
         top=top0
         fin=-fin
         return
      endif
      if(m1.ne.m2) then
         call error(5)
         return
      endif
      call icopy(mn2,istk(l2),1,istk(l1+mn1),1)
      istk(il1+2)=n1+n2
      lstk(top+1)=sadr(il1+3+m1*(n1+n2))
      goto 999

c     concatenation [a;b]
 48   if(n1.lt.0.or.n2.lt.0) then
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
      else
         if(istk(il1).ne.istk(il2)) then
            top=top0
            fin=-fin
            return
         endif
         if(n1.ne.n2) then
            call error(6)
            return
         endif
      endif
      m=m1+m2
      mn=m*n1
      if(n1.eq.1) then
         call icopy(mn2,istk(l2),1,istk(l1+mn1),1)
         istk(il1+1)=m
         lstk(top+1)=sadr(l1+mn)
         goto 999
      endif
      lw1=l1+mn
      lw2=lw1+mn1
      err=sadr(lw2+mn2)-lstk(bot)
      if(err.gt.0) then
            call error(17)
            return
      endif
      call icopy(mn2,istk(l2),1,istk(lw2),1)
      call icopy(mn1,istk(l1),1,istk(lw1),1)
c
      call imcopy(istk(lw1),m1,istk(l1),m,m1,n1)
      call imcopy(istk(lw2),m2,istk(l1+m1),m,m2,n1)
      istk(il1+1)=m
      istk(il1+2)=n1
      lstk(top+1)=sadr(l1+mn)
      goto 999
c     
c     extraction
c     
 50   continue
      if(rhs.lt.2) then
         call error(227)
         return
      endif
      if(rhs.gt.2) goto 55
c     arg2(arg1)
c     get arg2
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)
      n2=istk(il2+2)
      mn2=m2*n2
      l2=il2+3
      top=top-1
c     get arg1
      il1=iadr(lstk(top))
      ilrs=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
c
      if(mn2.eq.0) then 
c     .  arg2=[]
         ilrs=iadr(lstk(top))
         istk(ilrs)=1
         istk(ilrs+1)=0
         istk(ilrs+2)=0
         istk(ilrs+3)=0
         lstk(top+1)=sadr(ilrs+4)+1
         goto 999
      elseif(m2.lt.0) then
c     .  arg2=eye
         call error(14)
         return
      elseif(m1.lt.0) then
c     .  arg2(:), just reshape to column vector
         ilrs=iadr(lstk(top))
         istk(ilrs)=4
         istk(ilrs+1)=mn2
         istk(ilrs+2)=1
         call icopy(mn2,istk(l2),1,istk(ilrs+3),1)
         lstk(top+1)=sadr(ilrs+3+mn2)
         goto 999
      endif
c     check and convert indices variable
      call indxg(il1,mn2,ilr,mi,mx,lw,1)
      if(err.gt.0) return
      if(mx.gt.mn2) then
         call error(21)
         return
      endif
 51   if(mi.eq.0) then
c     arg2([])
         ilrs=iadr(lstk(top))
         istk(ilrs)=1
         istk(ilrs+1)=0
         istk(ilrs+2)=0
         istk(ilrs+3)=0
         lstk(top+1)=sadr(ilrs+4)+1
         goto 999
      endif
c     get memory for the result
      ilrs=iadr(lstk(top))
      l1=ilrs+3
      if(ilr.le.l1+mi) then
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
      do 52 i = 0, mi-1
         ind=istk(ilr+i)-1
         istk(lr+i) = istk(l2+ind)
 52   continue
c     set output sizes
      if (m2.eq.1.and.n2.eq.1.and.m1.gt.0) then
         m = m1
         n = n1
      elseif (m2 .gt. 1.or.m1.lt.0) then
         m = mi
         n = 1
      else
         n = mi
         m = 1
      endif
c     form resulting variable
      istk(ilrs)=4
      istk(ilrs+1)=m
      istk(ilrs+2)=n
      if(lr.ne.l1) call icopy(mi,istk(lr),1,istk(l1),1)
      lstk(top+1)=sadr(l1+mi)
      go to 999
c     
c     arg3(arg1,arg2)
 55   if(rhs.gt.3) then
         call error(36)
         return
      endif
c     get arg3
      il3=iadr(lstk(top))
      if(istk(il3).lt.0) il3=iadr(istk(il3+1))
      m3=istk(il3+1)
      n3=istk(il3+2)
      mn3=m3*n3
      l3=il3+3
c     get arg2
      top=top-1
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)
      l2=il2+3
c     get arg1
      top=top-1
      il1=iadr(lstk(top))
      ilrs=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      l1=il1+3

      if(mn3.eq.0) then 
c     .  arg3=[]
         ilrs=iadr(lstk(top))
         istk(ilrs)=1
         istk(ilrs+1)=0
         istk(ilrs+2)=0
         istk(ilrs+3)=0
         lstk(top+1)=sadr(ilrs+4)+1
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
 56   mn=mi*nj
      if(mn.eq.0) then 
c     .  arg1=[] or arg2=[] 
         ilrs=iadr(lstk(top))
         istk(ilrs)=1
         istk(ilrs+1)=0
         istk(ilrs+2)=0
         istk(ilrs+3)=0
         lstk(top+1)=l1+1
         goto 999
      endif
c     get memory for the result
      ilrs=iadr(lstk(top))
      l1=ilrs+3
      if(ili.le.l1+mi*nj) then
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
      l=lr
      do 58 j = 0, nj-1
         do 57 i = 0, mi-1
            ind=istk(ili+i)-1+(istk(ilj+j)-1)*m3
            istk(l) = istk(l3+ind)
            l=l+1
 57     continue
 58   continue
c     form the resulting variable
      istk(ilrs)=4
      istk(ilrs+1)=mi
      istk(ilrs+2)=nj
      if(lr.ne.l1) call icopy(mn,istk(lr),1,istk(ilrs+3),1)
      lstk(top+1)=sadr(ilrs+3+mn)
      go to 999
c     
c     insertion
 60   continue
      if(rhs.gt.4) then
         top=top0
         fin=-fin
         return
      endif
      if(rhs.eq.4) goto 65
c     arg3(arg1)=arg2
c     get arg3
      il3=iadr(lstk(top))
      if(istk(il3).lt.0) il3=iadr(istk(il3+1))
      if(istk(il3).ne.4) then
         if(istk(il3).eq.1.and.istk(il3+1)*istk(il3+2).eq.0) goto 61
         top=top0
         fin=-fin
         return
      endif
 61   m3=istk(il3+1)
      n3=istk(il3+2)
      mn3=m3*n3
      l3=il3+3
c     get arg2
      top=top-1
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      if(istk(il2).ne.4) then
         if(istk(il2).eq.1.and.istk(il2+1)*istk(il2+2).eq.0) goto 62
         top=top0
         fin=-fin
         return
      endif
 62   m2=istk(il2+1)
      n2=istk(il2+2)
      mn2=m2*n2
      l2=il2+3
c     get arg1
      top=top-1
      il1=iadr(lstk(top))
      ilrs=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      l1=il1+3
c     
      if (m2.eq.0) then
c     .  arg3(arg1)=[] -->[]
         if(m1.eq.-1) then
c     .    arg3(:)=[] 
            ilrs=iadr(lstk(top))
            istk(ilrs)=1
            istk(ilrs+1)=0
            istk(ilrs+2)=0
            istk(ilrs+3)=0
            lstk(top+1)=sadr(ilrs+4)+1
            goto 999
         elseif(m1.eq.0) then
c     .     arg3([])=[]  --> arg3
            ilrs=iadr(lstk(top))
            call icopy(3+mn3,istk(il3),1,istk(ilrs),1)
            lstk(top+1)=sadr(ilrs+3+mn3)
            goto 999
         else
c     .     arg3(arg1)=[] 
            if(istk(il1).eq.4.and.m3.eq.m1.and.n3.eq.n1) then
               if(.not.isany(il1)) then
c     .          arg3([])=[]  --> arg3
                  ilrs=iadr(lstk(top))
                  call icopy(3+mn3,istk(il3),1,istk(ilrs),1)
                  lstk(top+1)=sadr(ilrs+3+mn3)
                  goto 999
               endif
            endif
c     .     arg3(arg1)=[] -->arg3(compl(arg1),:)
            call indxgc(il1,mn3,ilr,mi,mx,lw)
            if(err.gt.0) return
            l2=l3
            n2=n3
            m2=m3
            mn2=m2*n2
c            it2=it3
c     .     call extraction
            goto 51
         endif
      elseif(m2.lt.0.or.m3.lt.0) then
c     .  arg3=eye,arg2=eye
         call error(14)
         return
      elseif(m1.lt.0) then
c     .  arg3(:)=arg2
         if(mn2.eq.mn3) then
            ilrs=iadr(lstk(top))
            istk(ilrs)=4
            istk(ilrs+1)=m3
            istk(ilrs+2)=n3
            call icopy(mn2,istk(l2),1,istk(ilrs+3),1)
            lstk(top+1)=sadr(ilrs+3+mn3)
            return
         elseif(mn2.eq.1) then
            ilrs=iadr(lstk(top))
            istk(ilrs)=4
            istk(ilrs+1)=m3
            istk(ilrs+2)=n3
            call iset(mn3,istk(l2),istk(ilrs+3),1)
            lstk(top+1)=sadr(ilrs+3+mn3)
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
c     .     arg3([])=c  --> arg3
            call icopy(mn3+3,istk(il3),1,istk(ilrs),1)
            lstk(top+1)=sadr(ilrs+3+mn3)
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
c     .  initialise result r to 0
         call iset(mnr,0,istk(lr),1)
c     .  write arg3 in r
         if(mn3.ge.1) then
            call imcopy(istk(l3),m3,istk(lr),mr,m3,n3)
         endif
      endif
c     write arg2 in r
      do 64 i = 0, mi-1
         ll = lr+istk(ili+i) - 1
         ls = l2+i*inc2
         istk(ll) = istk(ls)
 64   continue
c     
      ilrs=iadr(lstk(top))
      if(lr.ne.l3) then
         call icopy(mnr,istk(lr),1,istk(ilrs+3),1)
         istk(ilrs)=4
         istk(ilrs+1)=mr
         istk(ilrs+2)=nr
         lstk(top+1)=sadr(ilrs+3+mnr)
      else
c     la matrice a ete modifie sur place 
         k=istk(iadr(lstk(top0))+2)
         top=top-1
         call setref(k)
      endif
      goto 999

 65   continue 
c     
c     arg4(arg1,arg2)=arg3
c     get arg4
      il4=iadr(lstk(top))
      if(istk(il4).lt.0) il4=iadr(istk(il4+1))
      if(istk(il4).ne.4) then
         if(istk(il4).eq.1.and.istk(il4+1)*istk(il4+2).eq.0) goto 66
         top=top0
         fin=-fin
         return
      endif
 66   m4=istk(il4+1)
      n4=istk(il4+2)
      mn4=m4*n4
      l4=il4+3
c     get arg3
      top=top-1
      il3=iadr(lstk(top))
      if(istk(il3).lt.0) il3=iadr(istk(il3+1))
      if(istk(il3).ne.4) then
         if(istk(il3).eq.1.and.istk(il3+1)*istk(il3+2).eq.0) goto 67
         top=top0
         fin=-fin
         return
      endif
 67   m3=istk(il3+1)
      n3=istk(il3+2)
      mn3=m3*n3
      l3=il3+3
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
c
      l1=il1+3
      if (m3.eq.0) then
c     .  arg4(arg1,arg2)=[]
         if(m1.eq.-1.and.m2.eq.-1) then
c     .    arg4(:,:)=[] -->[]
            ilrs=iadr(lstk(top))
            istk(ilrs)=1
            istk(ilrs+1)=0
            istk(ilrs+2)=0
            istk(ilrs+3)=0
            lstk(top+1)=sadr(ilrs+4)+1
            goto 999
         elseif(m1.eq.0.or.m2.eq.0) then
c     .     arg4([],arg2)=[],  arg4(arg1,[])=[] --> arg4
            ilrs=iadr(lstk(top))
            call icopy(3+mn4,istk(il4),1,istk(ilrs),1)
            lstk(top+1)=sadr(ilrs+3+mn4)
            goto 999
         elseif(m2.eq.-1) then
c     .     arg3(arg1,:)=[] --> arg3(compl(arg1),:)
            call indxgc(il1,m4,ili,mi,mxi,lw)
            if(err.gt.0) return
            call indxg(il2,n4,ilj,nj,mxj,lw,1)
            if(err.gt.0) return
            l3=l4
            n3=n4
            m3=m4
            mn3=m3*n3
c     .     call extraction
            goto 56
         elseif(m1.eq.-1) then
c     .     arg3(:,arg2)=[] --> arg3(:,compl(arg2))
            call indxgc(il2,n4,ilj,nj,mxj,lw)
            if(err.gt.0) return
            call indxg(il1,m4,ili,mi,mxi,lw,1)
            if(err.gt.0) return
            l3=l4
            n3=n4
            m3=m4
            mn3=m3*n3
c     .     call extraction
            goto 56
         else
c     .     arg4(arg1,arg2)=[] --> arg4(:,compl(arg2))
            lw1=lw
            call indxgc(il2,n4,ilj,nj,mxj,lw)
            if(err.gt.0) return
            if(nj.eq.0) then
c     .        arg4(arg1,1:n4)=[] 
               call indxgc(il1,m4,ili,mi,mxi,lw)
               lw2=lw
               if(err.gt.0) return
c     .        arg2=1:n3
               if(mi.eq.0) then
c     .           arg4(1:m4,1:n4)=[] 
                  ilrs=iadr(lstk(top))
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
                  l3=l4
                  n3=n4
                  m3=m4
                  mn3=m3*n3
c     .           call extraction
                  goto 56
               endif
            else
c               lw=lw1
               call indxgc(il1,m4,ili,mi,mxi,lw)
               if(err.gt.0) return
               if(mi.eq.0) then
c     .           arg4(1:m4,arg2)=[] 
                  call indxg(il1,m4,ili,mi,mxi,lw,1)
                  if(err.gt.0) return
                  l3=l4
                  n3=n4
                  m3=m4
                  mn3=m3*n3
c     .           call extraction
                  goto 56
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
            ilrs=iadr(lstk(top))
            istk(ilrs)=4
            istk(ilrs+1)=m4
            istk(ilrs+2)=n4
            call icopy(mn4,istk(l3),1,istk(ilrs+3),1)
            lstk(top+1)=sadr(ilrs+3+mn4)
            return
         elseif(mn3.eq.1) then
            ilrs=iadr(lstk(top))
            istk(ilrs)=4
            istk(ilrs+1)=m4
            istk(ilrs+2)=n4
            call iset(mn4,istk(l3),istk(ilrs+3),1)
            lstk(top+1)=sadr(ilrs+3+mn4)
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
      if(mi.eq.0.or.mj.eq.0) then
         call error(15)
         return
      endif
      inc3=1
      if(mi.ne.m3.or.mj.ne.n3) then
c     .  sizes of arg1 or arg2 dont agree with arg3 sizes
         if(m3*n3.eq.1) then
            inc3=0
         else
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
         call iset(mnr,0,istk(lr),1)
c     .  copy arg4 in r
         if(mn4.ge.1) then
            call imcopy(istk(l4),m4,istk(lr),mr,m4,n4)
         endif
      else
         lr=l4
      endif
c     
c     copy arg3 elements in r
      do 69 j = 0, mj-1
         ljj =  istk(ilj+j) - 1
         do 68 i = 0, mi-1
            ll = lr+istk(ili+i)-1+ljj*mr
            ls = l3+(i+j*m3)*inc3
            istk(ll) = istk(ls)
 68     continue
 69   continue
c     
      ilrs=iadr(lstk(top))
      if(lr.ne.l4) then
         call icopy(mnr,istk(lr),1,istk(ilrs+3),1)
         istk(ilrs)=4
         istk(ilrs+1)=mr
         istk(ilrs+2)=nr
         lstk(top+1)=sadr(ilrs+3+mnr)
      else
c     la matrice a ete modifie sur place 
         k=istk(iadr(lstk(top0))+2)
         top=top-1
         call setref(k)
      endif
      goto 999

c     comparaisons
c     
 130  continue
      itrue=1
      if(op.eq.less+great) itrue=0
      if(op.ne.equal.and.op.ne.less+great) then
         call error(43)
         return
      endif
      if(istk(il1).ne.istk(il2)) then
         istk(il1)=4
         istk(il1+1)=1
         istk(il1+2)=1
         istk(il1+3)=1-itrue
         lstk(top+1)=sadr(il1+4)
         return
      else if(mn1.eq.1.and.mn2.gt.1) then
         l1=iadr(lw)
         err=sadr(l1+mn2)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call iset(mn2,istk(il1+3),istk(l1),1)
         mn1=mn2
         m1=m2
         n1=n2
         istk(il1+1)=m1
         istk(il1+2)=n1
      else if(mn2.eq.1.and.mn1.gt.1) then
         l2=iadr(lw)
         err=sadr(l2+mn1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call iset(mn1,istk(il2+3),istk(l2),1)
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
         l1=il1+3
         l2=il2+3
      endif
      do 132 i=0,mn1-1
         if(istk(l1+i).eq.istk(l2+i)) then
            istk(il1+3+i)=itrue
         else
            istk(il1+3+i)=1-itrue
         endif
 132  continue
      istk(il1)=4
      istk(il1+1)=m1
      istk(il1+2)=n1
      lstk(top+1)=sadr(il1+3+mn1)
      goto 999
c     
 999  return
      end
