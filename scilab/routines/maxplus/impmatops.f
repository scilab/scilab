      subroutine impmatops
c     
c     operations matricielles
     
      include '../stack.h'
      include 'mpstack.h'
      integer op
c     
      double precision ddot,dlamch
      double precision cstr,csti
      integer vol,vol1,iadr,sadr
c     
      double precision sr,si,e1,st,e2,powr,powi,e1r,e1i,e2r,e2i
      double precision mpddot
      integer star,dstar,slash,bslash,dot,colon
      integer less,great,equal
      integer top0
      integer typecode, algtype, algcode
      integer itype1, itype2, itype3, itype4, gettype
      data star/47/,dstar/62/,slash/48/
      data bslash/49/,dot/51/,colon/44/
      data less/59/,great/60/,equal/50/
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c      write(6,*) 'impmatop',PCZERO,ZERO,PCTOP,TOPM 

c     
c      write(*,*)'in impmatops rhs=',rhs
c      tmp=1000.d0
c      ZERO=-exp(tmp)
      call commons(ZERO)
      TOPM=-ZERO
      PCONE=0.D0
      op=fin
      PCZERO=ZERO
      PCTOP=TOPM
      PCALGCODE=1
c
c     
c      if (ddt .eq. 4) then
c         write(buf(1:4),'(i4)') fin
c         call basout(io,wte,' impmatops op: '//buf(1:4))
c      endif
c     
      top0=top
      lw=lstk(top+1)+1
      it2=0
      goto (04,03,02,01) rhs
      call error(39)
      return
c     
 01   il4=iadr(lstk(top))
      if (istk(il4).lt.0) il4=iadr(istk(il4+1))

      m4=istk(il4+1)
      n4=istk(il4+2)
      it4=istk(il4+3)
      ia4=algcode(istk(il4))      
      l4=sadr(il4+4)
      mn4=m4*n4
      top=top-1
c     
 02   il3=iadr(lstk(top))
c      write(*,*)'istk(il3)=',istk(il3)
      if(istk(il3).lt.0) il3=iadr(istk(il3+1))
c      write(*,*)'bis/ istk(il3)=',istk(il3)
      m3=istk(il3+1)
      n3=istk(il3+2)
      it3=istk(il3+3)
      ia3=algcode(istk(il3))
      l3=sadr(il3+4)
      mn3=m3*n3
      top=top-1
c     
 03   il2=iadr(lstk(top))
c      write(*,*)'istk(il2)=',istk(il2)
      if (istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)
      n2=istk(il2+2)
      it2=istk(il2+3)
      ia2=algcode(istk(il2))
      l2=sadr(il2+4)
      mn2=m2*n2
      top=top-1
c     
 04   il1=iadr(lstk(top))
c      write(*,*)'istk(il1)=',istk(il1)
      if (istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      ia1=algcode(istk(il1))
      l1=sadr(il1+4)
      mn1=m1*n1
      top=top-1
      istkil1o=istk(il1)
      istk(il1)=257
c
c=======================================
c   aiguillages associes aux  operations 
c=======================================
c     
      top=top+1
      itr=max(it1,it2)
c     
      fun = 0
c     
c     cconc (,)   insert  extrac rconc (;)
c
      goto(75  , 395  , 378   ,78) op
c
c          :  +  -  * /  \   ==   <>
c 
      goto(50,07,08,10,20,25,130,130,
c
c       <   >   <=  =>  '  ^
c
     & 130,130,130,130,70,31,
c
c      s+mp mp+s s*mp mp*s
c   
     & 07,  07,  10,  10,
c
c      s,mp mp,s s;mp mp;s
c
     & 75,   75,  78,  78,
c
c      s/mp mp/s s\mp mp\s
c 
     & 20,  20,  25,  25,
c
c      &  s&mp mp&s
c
     & 220, 220, 220,
c
c   (s insert mp) (mp insert s)
c
     & 395,  395,
c
c      s<mp mp<s   s>mp mp>s  s<=mp mp=<s  s=>mp  mp=>s
c
     & 130, 130,   130,  130, 130,  130,   130,    130,
c
c      s=mp  mp=s  s<>mp  mp<>s
c
     & 130,  130,  130,   130, 
c
c    -(unaire)
c
     & 08) op-4
c
 06   continue
      buf='power of max-plus complexes not implemented'
      call error(999)
      if(op.eq.dstar) goto 30
      if(op.ge.3*dot+star) goto 65      
      if(op.ge.2*dot+star) goto 120
      if(op.ge.less+equal) goto 130
      if(op.ge.dot+star) goto 55
      if(op.ge.less) goto 130
c
c=================     
c     addition
c=================
c
 07   continue
c
c       []+a
c
      if (mn1.eq.0) then
         call icopy(4,istk(il2),1,istk(il1),1)
         call dcopy(mn2*(it2+1),stk(l2),1,stk(l1),1)
         lstk(top+1)=l1+mn2*(it2+1)
c         write(*,*)'_+a',stk(l1)
         goto 999
      elseif(mn2.eq.0) then
c
c       a+[]
c
         goto 999
      endif
      if (m1 .lt. 0) go to 40
      if (m2 .lt. 0) go to 41
      if (mn2.eq.1) then
c
c        a+cst
c
         call mdadd(1,mn1,stk(l2),0,stk(l1),1)
c         if(it2+2*it1.eq.1) call dcopy(mn1,stk(l2+mn2),0,
c     &                                 stk(l1+mn1),1)
c         if(it1*it2.eq.1) call mdadd(ia1,
c     &               mn1,stk(l2+mn2),0,stk(l1+mn1),1)
         lstk(top+1)=l1+mn1*(itr+1)
         istk(il1+3)=itr
         goto 999
      endif
      if (mn1.eq.1) then
c
c        cst+a
c
         cstr=stk(l1)
         csti=stk(l1+1)
         call dcopy((it2+1)*mn2,stk(l2),1,stk(l1),1)
c         if(it1.eq.1.and.it2.eq.0) call dset(mn2,0.d0,stk(l1+mn2),1)
         call mdadd(1,mn2,cstr,0,stk(l1),1)
c         if(it1.eq.1) call mdadd(ia1,
c     &              mn2,csti,0,stk(l1+mn2),1)
         lstk(top+1)=l1+mn2*(itr+1)
         istk(il1+1)=m2
         istk(il1+2)=n2
         istk(il1+3)=itr
         goto 999
      endif
c
c     a+b
c
      if (m1 .ne. m2.or.n1 .ne. n2) then
      call error(8)
      return
      endif
      call mdadd(1,mn1,stk(l2),1,stk(l1),1)
c      if(it2+2*it1.eq.1) call dcopy(mn1,stk(l2+mn1),1,stk(l1+mn1),1)
c      if(it1*it2.eq.1) call mdadd(ia1,
c     &              mn1,stk(l2+mn1),1,stk(l1+mn1),1)
      lstk(top+1)=l1+mn1*(itr+1)
      istk(il1+3)=itr
      go to 999
c
c==================
c      & ( min)
c=================
c
 220  continue
      PCZERO=TOPM
      PCTOP=ZERO
c
c       []&a
c
      if (mn1.eq.0) then
         call icopy(4,istk(il2),1,istk(il1),1)
         call dcopy(mn2*(it2+1),stk(l2),1,stk(l1),1)
         lstk(top+1)=l1+mn2*(it2+1)
         goto 999
      elseif(mn2.eq.0) then
c
c       a&[]
c
         goto 999
      endif
      if (m1 .lt. 0) go to 40
      if (m2 .lt. 0) go to 41
      if (mn2.eq.1) then
c
c        a&cst
c
         call mdadd(2,mn1,stk(l2),0,stk(l1),1)
         if(it2+2*it1.eq.1) call dcopy(mn1,stk(l2+mn2),0,
     &                                 stk(l1+mn1),1)
         if(it1*it2.eq.1) call mdadd(2,
     &               mn1,stk(l2+mn2),0,stk(l1+mn1),1)
         lstk(top+1)=l1+mn1*(itr+1)
         istk(il1+3)=itr
         goto 999
      endif
      if (mn1.eq.1) then
c
c        cst&a
c
         cstr=stk(l1)
         csti=stk(l1+1)
         call dcopy((it2+1)*mn2,stk(l2),1,stk(l1),1)
         if(it1.eq.1.and.it2.eq.0) call dset(mn2,0.d0,stk(l1+mn2),1)
         call mdadd(2,mn2,cstr,0,stk(l1),1)
         if(it1.eq.1) call mdadd(2,
     &              mn2,csti,0,stk(l1+mn2),1)
         lstk(top+1)=l1+mn2*(itr+1)
         istk(il1+1)=m2
         istk(il1+2)=n2
         istk(il1+3)=itr
         goto 999
      endif
      if (m1 .ne. m2.or.n1 .ne. n2) then
      call error(8)
      return
      endif
      call mdadd(2,mn1,stk(l2),1,stk(l1),1)
      if(it2+2*it1.eq.1) call dcopy(mn1,stk(l2+mn1),1,stk(l1+mn1),1)
      if(it1*it2.eq.1) call mdadd(2,
     &              mn1,stk(l2+mn1),1,stk(l1+mn1),1)
      lstk(top+1)=l1+mn1*(itr+1)
      istk(il1+3)=itr
      go to 999
c
c====================     
c     soustraction
c====================
c
 08   if(rhs.eq.2) goto 09
      if(mn1.le.0) goto 999
      call mpchsgn(mn1*(it1+1),PCZERO,stk(l1),1)
      goto 999
 09   continue
      if (mn1.eq.0) then
c
c        []-a
c
         call icopy(4,istk(il2),1,istk(il1),1)
         call dcopy(mn2*(it2+1),stk(l2),1,stk(l1),1)
         call mpchsgn(mn2*(it2+1),PCZERO,stk(l1),1)
         lstk(top+1)=l1+mn2*(it2+1)
         goto 999
      elseif(mn2.eq.0) then
c
c         a-[]
c
         goto 999
      endif
      if (mn2.eq.1) then
c
c        a-cst
c
            call mpddif(mn1*(it1+1),stk(l2),0,stk(l1),1,PCZERO)
            lstk(top+1)=l1+mn1*(it1+1)
            goto 999
         endif

c
      if (mn1.eq.1) then
c
c        cst-a
c
         call mpchsgn((it2+1)*mn2,PCZERO,stk(l2),1)
         goto 10
      endif
      if (m1 .lt. 0) go to 42
      if (m2 .lt. 0) go to 45
c
c       check dimensions
c
      if (m1 .ne. m2.or.n1 .ne. n2) then
      call error(9)
      return
      endif
      call mpddif(mn1*(it1+1),stk(l2),1, stk(l1),1,PCZERO)
      if(itr.eq.0) goto 999
c      if(it1.eq.0) call dscal(mn1,-1.0d+0,stk(l2+mn1),1)
c      if(it1.eq.0) call dcopy(mn1,stk(l2+mn1),1,stk(l1+mn1),1)
c      if(it1*it2.eq.1) call mpddif(mn1,stk(l2+mn1),1,
c     &  stk(l1+mn1),1,PCZERO)
      lstk(top+1)=l1+mn1*(it1+1)
      istk(il1+3)=it1r
      go to 999
c
c=====================     
c     multiplication
c====================
c
 10   if (m2*mn2 .eq. 1) go to 12
      if (mn1 .eq. 1) go to 13
      if (mn2 .eq. 1) go to 12
      if(mn1.eq.0.or.mn2.eq.0) then
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=sadr(il1+4)+1
         goto 999
      endif
      if (n1 .ne. m2) then
         call error(10)
         return
      endif
c     
      istk(il1+2)=n2
      istk(il1+3)=itr
      lstk(top+1)=l1+m1*n2*(itr+1)
c     
      mnr=m1*n2*(itr+1)
      lr=l1+mnr
      vol=lstk(top+2)-lstk(top)
      err=lr+vol-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call dcopy(vol,stk(l1),-1,stk(lr),-1)
      l2=l2+mnr
c     
      if(it1*it2.eq.1) goto 11
      call mpdmmul(stk(lr),m1,stk(l2),m2,stk(l1),m1,m1,n1,n2)
      if(it1.eq.1) call mpdmmul(stk(lr+mn1),m1,stk(l2),m2,stk(l1+m1*n2),
     1     m1,m1,n1,n2)
      if(it2.eq.1) call mpdmmul(stk(lr),m1,stk(l2+mn2),m2,stk(l1+m1*n2),
     1     m1,m1,n1,n2)
      goto 999
c
c     a et a2 sont complexes
c
 11   call mpwmmul(stk(lr),stk(lr+mn1),m1,stk(l2),
     1     stk(l2+mn2),m2,stk(l1),stk(l1+m1*n2),m1,m1,n1,n2)
c     write(*,*)'the two matrices are complex'

      go to 999
c     
 12   continue
c
c     a*cst
c
      sr = stk(l2)
      si=PCZERO
      if(it2.eq.1) si = stk(l2+1)
      go to 14
 13   continue
c
c     cst*a
c
      sr =stk(l1)
      si=PCZERO
      if(it1.eq.1) si = stk(l1+1)
      call dcopy(mn2*(it2+1),stk(l2),1,stk(l1),1)
      m1=m2
      n1=n2
      mn1=it1
      it1=it2
      it2=mn1
      mn1=mn2
c     
 14   continue
      istk(il1+1)=m1
      istk(il1+2)=n1
      istk(il1+3)=itr
      lstk(top+1)=l1+mn1*(itr+1)
c     
      err=lstk(top+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c     
      goto (15,16,17),it2+2*it1
c
c     la matrice et le scalaire sont reels
c
      call mpdscal(mn1,sr,stk(l1),1)
      goto 999
 15   continue
c
c     la matrice est reelle le scalaire est complexe
c
c      write(*,*)'la matrice est reelle le scalaire est complexe'
      call dcopy(mn1,stk(l1),1,stk(l1+mn1),1)
      call mpdscal(mn1,si,stk(l1+mn1),1)
      call mpdscal(mn1,sr,stk(l1),1)
      goto 999
 16   continue
c
c     la matrice est complexe, le scalaire est reel
c
      call mpdscal(mn1,sr,stk(l1),1)
      call mpdscal(mn1,sr,stk(l1+mn1),1)
      goto 999
 17   continue
c
c     la matrice et le scalaire sont complexe
c
      call mpwscal(mn1,sr,si,stk(l1),stk(l1+mn1),1)
      goto 999
c
c===============================     
c     division a droite
c===============================
c
 20   continue
      PCZERO=TOPM
      PCTOP=ZERO
      if (mn2 .eq. 1) go to 21
      if (n1 .ne. n2) then
         call error(11)
         return
      endif
c
c     negate B
c
      call dscal(mn2*(it2+1),-1.0d+0,stk(l2),1)
c
c     transpose B
c
      vol=mn2*(it2+1)
      ll = l2+vol
      err = ll+vol - lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
c
      istk(il2+1)=n2
      istk(il2+2)=m2
c
      call dcopy(vol,stk(l2),1,stk(ll),1)
      call mtran(stk(ll),m2,stk(l2),n2,m2,n2)
      if(it2.eq.0) goto 18
      call mtran(stk(ll+mn2),m2,stk(l2+mn2),n2,m2,n2)
      call dscal(mn2,-1.0d+0,stk(l2+mn2),1)
c
c     calculate A*B in min+ algebra
c
 18   continue
      istk(il1+2)=m2
      istk(il1+3)=itr
      lstk(top+1)=l1+m1*m2*(itr+1)
c
      mnr=m1*m2*(itr+1)
      lr=l1+mnr
      vol=lstk(top+2)-lstk(top)
      err=lr+vol-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call dcopy(vol,stk(l1),-1,stk(lr),-1)
      l2=l2+mnr
c
      if(it1*it2.eq.1) goto 19
      call mndmmul(stk(lr),m1,stk(l2),n2,stk(l1),m1,m1,n1,m2)
      if(it1.eq.1) call mndmmul(stk(lr+mn1),m1,stk(l2),n2,stk(l1+m1*m2),
     1     m1,m1,n1,m2)
      if(it2.eq.1) call mndmmul(stk(lr),m1,stk(l2+mn2),n2,stk(l1+m1*m2),
     1     m1,m1,n1,m2)
      goto 999
c
c     a et a2 sont complexes
c
 19   call mnwmmul(stk(lr),stk(lr+mn1),m1,stk(l2),
     1     stk(l2+mn2),n2,stk(l1),stk(l1+m1*m2),m1,m1,n1,m2)
      go to 999
c
 21   continue
      sr=stk(l2)
      si=PCZERO
      if(it2.eq.1) si=stk(l2+1)
 22   sr=-sr
      si=-si
      goto 14
c
c==========================     
c     division a gauche
c==========================
c
 25   continue
      PCZERO=TOPM
      PCTOP=ZERO
      if (mn1 .eq. 1) go to 26
      if (m1 .ne. m2) then
         call error(12)
         return
      endif
c
c     negate A 
c
      call dscal(mn1*(it1+1),-1.0d+0,stk(l1),1)
c
c     transpose A 
c
      vol=mn2*(it2+1)
      ll = l2+vol
      vol1=mn1*(it1+1)
      err = ll+vol1 - lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
c
      istk(il1+1)=n1
      istk(il1+2)=m1
c
      call dcopy(vol1,stk(l1),1,stk(ll),1)
      call mtran(stk(ll),m1,stk(l1),n1,m1,n1)
      if(it1.eq.0) goto 24
      call mtran(stk(ll+mn1),m1,stk(l1+mn1),n1,m1,n1)
      call dscal(mn1,-1.0d+0,stk(l1+mn1),1)
c
c     calculate A*B in min+ algebra
c
 24   continue
      istk(il1+1)=n1
      istk(il1+2)=n2
      istk(il1+3)=itr
      lstk(top+1)=l1+n1*n2*(itr+1)
c
      mnr=n1*n2*(itr+1)
      lr=l1+mnr
      vol=lstk(top+2)-lstk(top)
      err=lr+vol-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call dcopy(vol,stk(l1),-1,stk(lr),-1)
      l2=l2+mnr
c
      if(it1*it2.eq.1) goto 23
      call mndmmul(stk(lr),n1,stk(l2),m2,stk(l1),n1,n1,m1,n2)
      if(it1.eq.1) call mndmmul(stk(lr+mn1),n1,stk(l2),m2,stk(l1+n1*n2),
     1     n1,n1,m1,n2)
      if(it2.eq.1) call mndmmul(stk(lr),n1,stk(l2+mn2),m2,stk(l1+n1*n2),
     1     n1,n1,m1,n2)
      goto 999
c
c     a et a2 sont complexes
c
 23   call mnwmmul(stk(lr),stk(lr+mn1),n1,stk(l2),
     1     stk(l2+mn2),m2,stk(l1),stk(l1+n1*n2),n1,n1,m1,n2)
c
      go to 999
c============================================
c
c============================================
 26   continue
      m1=m2
      n1=n2
      sr=stk(l1)
      si=PCZERO
      if(it1.eq.1) si=stk(l1+1)
      call dcopy(mn2*(it2+1),stk(l2),1,stk(l1),1)
      m1=m2
      n1=n2
      mn1=it1
      it1=it2
      it2=mn1
      mn1=mn2
      goto 22
c         
c===================================     
c     puissance  element wise .^
c===================================
c
 30   continue
      if (mn1.eq.0) then
         return
      endif
      if (mn2.eq.0) then
         istk(il1)=257
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=sadr(il1+4)
         return
      endif
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
      err=lw+mn*2-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      itr=max(it1,it2)
      if(it2.eq.0) then
         if(it1.eq.0) then
c            write(*,*)'it2.eq.0 and it1.eq.0'
            call mpddpow1(mn,stk(l1),inc1,stk(l2),inc2,
     $           stk(lw),stk(lw+mn),1,err,itr)
         else
            buf='power of max-plus complexes not implemented'
            call error(999)
c            call wdpow1(mn,stk(l1),stk(l1+mn1),inc1,stk(l2),inc2,
c     $           stk(lw),stk(lw+mn),1,err)
         endif
      else
         if(it1.eq.0) then
            buf='power of max-plus complexes not implemented'
            call error(999)
c           call dwpow1(mn,stk(l1),inc1,stk(l2),stk(l2+mn2),inc2,
c     &           stk(lw),stk(lw+mn),1,err)
         else
            buf='power of max-plus complexes not implemented'
            call error(999)
c            call wwpow1(mn,stk(l1),stk(l1+mn1),inc1,stk(l2),stk(l2+mn2),
c     &           inc2,stk(lw),stk(lw+mn),1,err)
         endif
      endif
      if(err.eq.1) then
         call error(30)
         return
      endif
      if(err.eq.2) then
         if(ieee.eq.0) then
            call error(27)
            return
         elseif(ieee.eq.1) then
            call msgs(63)
         endif
         err=0
      endif
      istk(il1+1)=m
      istk(il1+2)=n
      istk(il1+3)=itr
      call dcopy(mn*(itr+1),stk(lw),1,stk(l1),1)
      lstk(top+1)=l1+mn*(itr+1)
      goto 999
c
c==========================================================     
c     elevation d'une matrice carree a une puissance
c==========================================================
c
 31   continue
      if(mn1.eq.0) then
         return
      endif
      if(mn2.eq.0) then
         istk(il1)=257
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=sadr(il1+4)
         return
      endif
      if(mn1.eq.1) goto 30
      if(mn2.gt.1) goto 39
      if(m1.ne.n1) then
         if(mn2.eq.1.and.(m1.eq.1.or.n1.eq.1)) goto 30
         err=1
         call error(20)
         return
      endif
      nexp = nint(stk(l2))
      if (it2 .ne. 0) go to 39
      if (stk(l2) .ne. dble(nexp)) go to 39
      if (nexp.eq.1) go to 999
      if (nexp.eq.0) then
         lw=l1+mn1*(it1+1)
         ipvt=iadr(lw+m1*(it1+1))
         err=sadr(ipvt+m1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         if (dasum(m1*n1*(it1+1),stk(l1),1).eq.PCZERO) then
            call error(30)
            return
         endif
         call dset(mn1,PCZERO,stk(l1),1)
         call dset(m1,PCONE,stk(l1),m1+1)
         istk(il1+3)=0
         lstk(top+1)=l1+mn1
         goto 999
      endif
     
      if (nexp.le.0) then
         buf='negative power of max-plus matrices not implemented'
         call error(999)
      endif
      l2=l1+mn1*(it1+1)
c     
      l3=l2+mn1*(itr+1)
      err=l3+n1*(itr+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      lstk(top+1)=l1+mn1*(itr+1)
      istk(il1+3)=itr
c     
      call dcopy(mn1*(itr+1),stk(l1),1,stk(l2),1)
      if(it1.eq.1) goto 35
c
c     la matrice est reelle
c
      do 34 kexp=2,nexp
         do 33 j=1,n1
            ls=l1+(j-1)*n1
            call dcopy(n1,stk(ls),1,stk(l3),1)
            do 32 i=1,n1
               ls=l2+(i-1)
               ll=l1+(i-1)+(j-1)*n1
               stk(ll)=mpddot(n1,stk(ls),n1,stk(l3),1)
c
c               write(*,*)'mpddot',stk(ll)
c
 32         continue
 33      continue
 34   continue
      goto 999
c     
 35   continue
c
c     la matrice est complexe
c
      buf='power of max-plus complex matrices not implemented'
      call error(999)
      goto 999
c     
c     puissance non entiere ou non positive
c
 39   fun = 6
      fin = 29
      rhs=2
      top=top+1
      go to 999
c     
c============================================================     
c     addition et soustraction d'un scalaire fois l'identite
c============================================================
c
 40   sr=stk(l1)
      si=PCZERO
      if(it1.eq.1) si=stk(l1+1)
      call dcopy(mn2*(it2+1),stk(l2),1,stk(l1),1)
      m1=m2
      n1=n2
      m2=it2
      it2=it1
      it1=m2
      mn1=mn2
      goto 46
c     
 41   sr=stk(l2)
      si=PCZERO
      if(it2.eq.1) si = stk(l2+1)
      goto 46
c
c==========================     
c     soustraction a*eye-b
c==========================
c
 42   sr=stk(l1)
      si=PCZERO
      if(it1.eq.1) si=stk(l1+1)
 43   call dcopy(mn2*(it2+1),stk(l2),1,stk(l1),1)
      call dscal(mn2*(it2+1),-1.0d+0,stk(l1),1)
      mn1=mn2
      m1=m2
      n1=n2
      m2=it2
      it2=it1
      it1=m2
      goto 46
c
c===========================     
c     soustraction a-eye*b
c===========================
c
 45   sr=-stk(l2)
      si=PCZERO
      if(it2.eq.1) si =- stk(l2+1)
c     
 46   err=l1+mn1*(itr+1) - lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      lstk(top+1)=l1+mn1*(itr+1)
      istk(il1+1)=m1
      istk(il1+2)=n1
      istk(il1+3)=itr
c     
      if(itr.eq.1.and.it1.eq.0) call dset(mn1,PCZERO,stk(l1+mn1),1)
      do 47 i = 1, max(n1,m1)
         ll = l1 + (i-1)*(m1+1)
         stk(ll) = stk(ll)+sr
         if(itr.ne.0) stk(ll+mn1) = stk(ll+mn1)+si
 47   continue
      go to 999
c     
c     boucle implicite
c
 50   e2 = stk(l2)
      st = 1.0d+0
      n = 0
      if (rhs .lt. 3) go to 51
      e2=stk(l3)
      st = stk(l2)
      if (st .eq. PCZERO) go to 53
 51   e1 = stk(l1)
c
c     check for clause
c
      if (rstk(pt-1) .eq. 801.or.rstk(pt).eq.611) go to 54
      err = l1 + max(3,int((e2-e1)/st)) - lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
      l=l1
 52   if (st*(stk(l)-e2).gt.PCZERO) then
         if (abs(stk(l)-e2).lt.abs(st*dlamch('p'))) n=n+1
         go to 53
      endif
      n = n+1
      l = l+1
      stk(l) = e1 + dble(n)*st
      go to 52
 53   continue
      istk(il1+1)=1
      if(n.eq.0) istk(il1+1)=0
      istk(il1+2)=n
      istk(il1+3)=0
      lstk(top+1)=l1+n
      go to 999
c     
c     for clause
c
 54   stk(l1) = e1
      stk(l1+1) = st
      stk(l1+2) = e2
      istk(il1+1)=-3
      istk(il1+2)=-1
      lstk(top+1)=l1+3
      go to 999
c     
c     operations elements a elements
c
 55   continue
      i1=1
      i2=1
      op = op - dot
      if(mn1.eq.0.or.mn2.eq.0) then
c
c     [].*a     a.*[]  -->[]
c
         istk(il1)=257
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=sadr(il1+4)+1
         goto 999
      endif
      if(mn1.eq.1.and.op.eq.star) goto 13
      if(mn2.eq.1.and.op.eq.star) goto 12
      if(mn1.ne.1.and.mn2.ne.1) then
c
c       check dimensions
c
      if (m1.ne.m2 .or. n1.ne.n2) then
         buf='inconsistent element-wise operation'
         call error(9999)
         return
      endif
      endif
      lstk(top+1)=l1+mn1*(itr+1)
      istk(il1+3)=itr
      if(mn2.eq.1) then
       if(op.eq.slash) then
       cstr=stk(l2)
        if(it2.eq.1) csti=stk(l2+1)
           if(it1*it2.eq.1) then
           sr=cstr
           si=csti
           e1=abs(sr)+abs(si)
            if(e1.eq.PCZERO) then
            call error(27)
            return
            endif
           sr=sr/e1
           si=si/e1
           e1=e1*(sr*sr+si*si)
           sr=sr/e1
           si=-si/e1
           call wvmul(mn1,sr,si,0,stk(l1),stk(l1+mn1),1)
           goto 999
           endif
        if(it1.eq.0.and.it2.eq.0) then
           if (cstr.eq.0.d0) then
           call error(27)
           return
           endif
           call dvmul(mn1,1.0d0/cstr,0,stk(l1),1)
           goto 999
        endif
        if(it1.eq.0.and.it2.eq.1) then
           err=l1+2*mn1-lstk(bot)
           if(err.gt.0) then
           call error(17)
           return
           endif
           lstk(top+1)=l1+2*mn1
           istk(il1+3)=it2
           sr=cstr
           si=csti
           e1=abs(sr)+abs(si)
            if(e1.eq.PCZERO) then
             call error(27)
             return
            endif
           sr=sr/e1
           si=si/e1
           e1=e1*(sr*sr+si*si)
           sr=sr/e1
           si=-si/e1
           call dset(mn1,PCZERO,stk(l1+mn1),1)
           call wvmul(mn1,sr,si,0,stk(l1),stk(l1+mn1),1)
           goto 999
        endif
        if(it1.eq.1.and.it2.eq.0) then
           if(cstr.eq.PCZERO) then
           call error(27) 
           return
           endif
           call dvmul(2*mn1,1.0d0/cstr,0,stk(l1),1)
           goto 999
        endif
       endif
       if(op.eq.bslash) then
       cstr=stk(l2)
       if(it2.eq.1) csti=stk(l2+1)
          if(it1.eq.0.and.it2.eq.0) then
          do 550 ii=1,mn1
          if(stk(l1+ii-1).eq.0.d0) then
          call error(27)
          endif
          stk(l1+ii-1)=cstr/stk(l1+ii-1)
 550      continue
          goto 999
          endif
          if(it1.eq.0.and.it2.eq.1) then
             istk(il1+3)=it2
             err=l1+2*mn1-lstk(bot)
             if(err.gt.0) then
                call error(17)
                return
             endif
             lstk(top+1)=l1+2*mn1
             do 551 i=1,mn1
                sr=stk(l1+i-1)
                e1=abs(sr)
                if(sr.eq.PCZERO) then
                   call error(27)
                   return
                endif
                stk(l1+i-1)=cstr/sr
                stk(l1+mn1+i-1)=csti/sr
 551         continue
             goto 999
          endif
          if(it1.eq.1.and.it2.eq.0) then
          do 552 i=1,mn1
          sr=stk(l1+i-1)
          si=stk(l1+mn1+i-1)
          e1=abs(sr)+abs(si)
            if(e1.eq.PCZERO) then
             call error(27)
             return
            endif
          sr=sr/e1
          si=si/e1
          e1=e1*(sr*sr+si*si)
          sr=sr/e1
          si=-si/e1
          stk(l1+i-1)=cstr*sr
          stk(l1+mn1+i-1)=cstr*si
 552      continue
          goto 999
          endif
          if(it1.eq.1.and.it2.eq.1) then
          do 553 i=1,mn1
          sr=stk(l1+i-1)
          si=stk(l1+mn1+i-1)
          e1=abs(sr)+abs(si)
            if(e1.eq.PCZERO) then
             call error(27)
             return
            endif
          sr=sr/e1
          si=si/e1
          e1=e1*(sr*sr+si*si)
          sr=sr/e1
          si=-si/e1
          stk(l1+i-1)=sr*cstr-si*csti
          stk(l1+mn1+i-1)=cstr*si+sr*csti
 553      continue
          goto 999
          endif
       endif
      endif
c              
      if(mn1.eq.1) then
       if(op.eq.slash) then
       cstr=stk(l1)
       if(it1.eq.1) csti=stk(l1+1)
           if(it1.eq.0.and.it2.eq.0) then
           istk(il1+1)=m2
           istk(il1+2)=n2
           lstk(top+1)=l1+mn2
           do 554 i=1,mn2
           sr=stk(l2+i-1)
            if(sr.eq.0.d0) then
            call error(27)
            return
            endif
           stk(l1+i-1)=cstr/sr
 554       continue
           goto 999
           endif
           if(it1.eq.0.and.it2.eq.1) then
           err=l1+2*mn2-lstk(bot)
            if(err.gt.0) then
            call error(17)
            return
            endif
           istk(il1+1)=m2
           istk(il1+2)=n2
           istk(il1+3)=it2
           lstk(top+1)=l1+2*mn2
           do 555 i=1,mn2
           sr=stk(l2+i-1)
           si=stk(l2+mn2+i-1)
           e1=abs(sr)+abs(si)
            if(e1.eq.PCZERO) then
            call error(27)
            return
            endif
           sr=sr/e1
           si=si/e1
           e1=e1*(sr*sr+si*si)
           sr=sr/e1
           si=-si/e1
           stk(l2+i-1)=sr*cstr
           stk(l2+mn2+i-1)=cstr*si
 555       continue
           call dcopy(2*mn2,stk(l2),1,stk(l1),1)
           goto 999
           endif
           if(it1.eq.1.and.it2.eq.1) then
           istk(il1+1)=m2
           istk(il1+2)=n2
           lstk(top+1)=l1+2*mn2
           do 556 i=1,mn2
           sr=stk(l2+i-1)
           si=stk(l2+mn2+i-1)
           e1=abs(sr)+abs(si)
            if(e1.eq.PCZERO) then
             call error(27)
             return
            endif
           sr=sr/e1
           si=si/e1
           e1=e1*(sr*sr+si*si)
           sr=sr/e1
           si=-si/e1
           stk(l2+i-1)=sr*cstr-si*csti
           stk(l2+mn2+i-1)=cstr*si+csti*sr
 556       continue
           call dcopy(2*mn2,stk(l2),1,stk(l1),1)
           goto 999
           endif
           if(it1.eq.1.and.it2.eq.0) then
           err=l1+2*mn2-lstk(bot)
            if(err.gt.0) then
            call error(17)
            return
            endif
           istk(il1+1)=m2
           istk(il1+2)=n2
           lstk(top+1)=l1+2*mn2
           do 557 i=1,mn2
           sr=stk(l2+i-1)
            if(sr.eq.PCZERO) then
             call error(27)
             return
            endif
           stk(l2+i-1)=cstr/sr
           stk(l2+mn2+i-1)=csti/sr
 557       continue
           call dcopy(2*mn2,stk(l2),1,stk(l1),1)
           goto 999
           endif
       endif
       if(op.eq.bslash) then
       cstr=stk(l1)
       if(it1.eq.1) csti=stk(l1+1)
           if(it1.eq.0.and.it2.eq.0) then
            if(cstr.eq.0.d0) then
            call error(17)
            return
            endif
           istk(il1+1)=m2
           istk(il1+2)=n2
           lstk(top+1)=l1+mn2
           do 558 i=1,mn2
           sr=stk(l2+i-1)
           stk(l1+i-1)=sr/cstr
 558       continue
           goto 999
           endif
           if(it1.eq.1.and.it2.eq.0) then
           err=l2+2*mn2-lstk(bot)
            if(err.gt.0) then
            call error(17)
            return
            endif
           sr=cstr
           si=csti
           e1=abs(sr)+abs(si)
            if(e1.eq.PCZERO) then
            call error(27)
            return
            endif
           sr=sr/e1
           si=si/e1
           e1=e1*(sr*sr+si*si)
           sr=sr/e1
           si=-si/e1
           cstr=sr
           csti=si
           istk(il1+1)=m2
           istk(il1+2)=n2
           lstk(top+1)=l1+2*mn2
           do 559 i=1,mn2
           sr=stk(l2+i-1)
           stk(l2+i-1)=sr*cstr
           stk(l2+mn2+i-1)=sr*csti
 559       continue
           call dcopy(2*mn2,stk(l2),1,stk(l1),1)
           goto 999
           endif
           if(it1.eq.0.and.it2.eq.1) then
            if(cstr.eq.0.d0) then
            call error(27)
            return
            endif
           istk(il1+1)=m2
           istk(il1+2)=n2
           istk(il1+3)=it2
           lstk(top+1)=l1+2*mn2
           do 560 i=1,mn2
           stk(l2+i-1)=stk(l2+i-1)/cstr
           stk(l2+mn2+i-1)=stk(l2+mn2+i-1)/cstr
 560       continue
           call dcopy(2*mn2,stk(l2),1,stk(l1),1)
           goto 999
           endif
           if(it1.eq.1.and.it2.eq.1) then
           istk(il1+1)=m2
           istk(il1+2)=n2
           istk(il1+3)=it2
           lstk(top+1)=l1+2*mn2
           sr=cstr
           si=csti
           e1=abs(sr)+abs(si)
            if(e1.eq.PCZERO) then
            call error(27)
            return
            endif
           sr=sr/e1
           si=si/e1
           e1=e1*(sr*sr+si*si)
           sr=sr/e1
           si=-si/e1
           cstr=sr
           csti=si
           do 561 i=1,mn2
           sr=stk(l2+i-1)
           si=stk(l2+mn2+i-1)
           stk(l2+i-1)=sr*cstr-si*csti
           stk(l2+mn2+i-1)=sr*csti+si*cstr
 561       continue
           call dcopy(2*mn2,stk(l2),1,stk(l1),1)
           goto 999
           endif
       endif
      endif
      if(op.ne.star) goto 60
c
c     multiplication
c
 56   if(it1*it2.eq.1) goto 58
      if(it1.eq.1) call dvmul(mn1,stk(l2),i2,stk(l1+mn1),i1)
      if(it2.eq.1) call dvmul(mn1,stk(l1),i1,stk(l2+mn2),i2)
      call dvmul(mn1,stk(l2),i2,stk(l1),i1)
      if(it2.eq.1) call dcopy(mn1,stk(l2+mn2),i2,stk(l1+mn1),i1)
      goto 999
c
c     a et a2 complexes
c
 58   call wvmul(mn1,stk(l2),stk(l2+mn2),i2,stk(l1),stk(l1+mn1),i1)
      goto 999
c
c=====================================     
c     division a droite et a gauche
c=====================================
c     
 60   lstk(top+1)=l1+mn1*(itr+1)
      istk(il1+3)=itr
      ll=l2-1
      it=it2
      if(op.eq.slash) goto 61
      ll=l1-1
      it=it1
 61   if(it.eq.1) goto 63
c
c     la matrice diviseur est reelle
c
      do 62 i=1,mn1
         if(stk(ll+i).eq.PCZERO) then
            call error(27)
            return
         endif
         stk(ll+i)=1.0d+0/stk(ll+i)
 62   continue
      if(mn1.eq.1) goto 13
      goto 56
c
c     la matrice diviseur est complexe
c
 63   do 64 i=1,mn1
         sr=stk(ll+i)
         si=stk(ll+mn1+i)
         e1=abs(sr)+abs(si)
         if(e1.eq.PCZERO) then
            call error(27)
            return
         endif
         sr=sr/e1
         si=si/e1
         e1=e1*(sr*sr+si*si)
         sr=sr/e1
         si=-si/e1
         stk(ll+i)=sr
         stk(ll+mn1+i)=si
 64   continue
      if(mn1.eq.1) goto 13
      goto 56
c
c===============     
c     kronecker
c===============
c
 65   fin = op - 3*dot - star + 19
      fun = 6
      top = top + 1
      rhs = 2
      go to 999
c     
c========================     
c     transposition
c========================
c
 70   Continue
c      Write(*,*)'Transposition'
      if(mn1 .eq. 0.or.istk(il1).eq.0) goto 999
      vol=mn1*(it1+1)
      ll = l1+vol
      err = ll+vol - lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
c     
      istk(il1+1)=n1
      istk(il1+2)=m1
c     
      call dcopy(vol,stk(l1),1,stk(ll),1)
      call mtran(stk(ll),m1,stk(l1),n1,m1,n1)
      if(it1.eq.0) goto 999
      call mtran(stk(ll+mn1),m1,stk(l1+mn1),n1,m1,n1)
      call dscal(mn1,-1.0d+0,stk(l1+mn1),1)
c     
      goto 999
c
c===============================     
c     concatenation [a b]
c===============================
c
 75   if(m1.lt.0.or.m2.lt.0) then
         call error(14)
         return
      endif
      if(m2.eq.0) then
         return
      elseif(m1.eq.0)then
         call dcopy(lstk(top+2)-lstk(top+1),stk(lstk(top+1)),1,
     &        stk(lstk(top)),1)
         lstk(top+1)=lstk(top)+lstk(top+2)-lstk(top+1)
         return
      elseif(m1.ne.m2) then
         call error(5)
         return
      endif
c     
      if(itr.eq.0) then
         call dcopy(mn2,stk(l2),1,stk(l1+mn1),1)
      else
         lw=l1+(itr+1)*(mn1+mn2)
         if(lw.gt.l2) then
            err=lw+mn2*(it2+1)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call dcopy(mn2*(it2+1),stk(l2),-1,stk(lw),-1)
            l2=lw
         endif
         if(it1.eq.1) call dcopy(mn1,stk(l1+mn1),-1,stk(l1+mn1+mn2),-1)
         call dcopy(mn2,stk(l2),1,stk(l1+mn1),1)
         if(it1.eq.0) call dset(mn1,PCZERO,stk(l1+mn1+mn2),1)
         if(it1.eq.0) call dcopy(mn2,stk(l2+mn2),1,stk(l1+2*mn1+mn2),1)
         if(it2.eq.0) call dset(mn2,PCZERO,stk(l1+2*mn1+mn2),1)
         if(it2.eq.1) call dcopy(mn2,stk(l2+mn2),1,stk(l1+2*mn1+mn2),1)
      endif
      n=n1+n2
      istk(il1+1)=m1
      istk(il1+2)=n
      istk(il1+3)=itr
      lstk(top+1)=sadr(il1+4)+m1*n*(itr+1)
      goto 999
c
c============================     
c     concatenation [a;b]
c============================
c
 78   if(n1.lt.0.or.n2.lt.0) then
         call error(14)
         return
      endif
      if(n2.eq.0) then
         goto 999
      elseif(n1.eq.0)then
         call dcopy(lstk(top+2)-lstk(top+1),stk(lstk(top+1)),1,
     &        stk(lstk(top)),1)
         lstk(top+1)=lstk(top)+lstk(top+2)-lstk(top+1)
         goto 999
      elseif(n1.ne.n2) then
         call error(6)
         return
      endif
      m=m1+m2
      mn=m*n1
      if(n1.eq.1.and.itr.eq.0) then
         call dcopy(mn2,stk(l2),1,stk(l1+mn1),1)
         istk(il1+1)=m
         istk(il1+3)=itr
         lstk(top+1)=l1+mn*(itr+1)
         goto 999
      endif
      lw1=l1+(itr+1)*mn
      lw2=lw1+mn1*(it1+1)
      err=lw2+mn2*(it2+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call dcopy(mn2*(it2+1),stk(l2),1,stk(lw2),1)
      call dcopy(mn1*(it1+1),stk(l1),1,stk(lw1),1)
c     
      if(itr.eq.1) call dset(mn,PCZERO,stk(l1+(mn1+mn2)),1)
      call dmcopy(stk(lw1),m1,stk(l1),m,m1,n1)
      if(it1.eq.1) call dmcopy(stk(lw1+mn1),m1,stk(l1+mn),m,m1,n1)
      call dmcopy(stk(lw2),m2,stk(l1+m1),m,m2,n1)
      if(it2.eq.1) call dmcopy(stk(lw2+mn2),m2,stk(l1+mn+m1),m,m2,n1)
      istk(il1+1)=m
      istk(il1+2)=n1
      istk(il1+3)=itr
      lstk(top+1)=sadr(il1+4)+mn*(itr+1)
      goto 999
c
c=========================
c     extraction
c=========================
c
 378  continue
      istk(il1)=istkil1o
      if (rhs.gt.2) goto 82
c
c     arg2(arg1)
c
      if (istk(il1).eq.0) then
         call error(220)
         return
      endif
      if(istk(il2).eq.129) then
c
c     implied polynomials vector extraction
c
         top=top+1
         call polops
         goto 999
      endif

      if (mn2.eq.0) then 
c
c     .  arg2=[]
c,
         il1=iadr(lstk(top))
         istk(il1)=257
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=sadr(il1+4)
         goto 999
      elseif (m2.lt.0) then
c
c     .  arg2=eye
c
         call error(14)
         return
      elseif (m1.lt.0) then
c
c     .  arg2(:), just reshape to column vector
c
         il1=iadr(lstk(top))
         istk(il1)=257
         istk(il1+1)=mn2
         istk(il1+2)=1
         istk(il1+3)=istk(il2+3)
         call dcopy(mn2*(it2+1),stk(l2),1,stk(l1),1)
         lstk(top+1)=sadr(il1+4)+mn2*(it2+1)
         goto 999
      endif
c
c     check and convert indices variable
c
      call indxg(il1,mn2,ilr,mi,mx,lw,1)
      if (err.gt.0) return
      if (mx.gt.mn2) then
         call error(21)
         return
      endif
 79   if (mi.eq.0) then
c
c     arg2([])
c
         il1=iadr(lstk(top))
         istk(il1)=257
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         l1=sadr(il1+4)
         lstk(top+1)=l1
         goto 999
      endif
c
c     get memory for the result
c
      il1=iadr(lstk(top))
      l1=sadr(il1+4)
      if (sadr(ilr-1).le.l1+(it2+1)*mi) then
         lr=lw
         lw=lr+(it2+1)*mi
         err=lw-lstk(bot)
         if (err.gt.0) then
            call error(17)
            return
         endif
      else
         lr=l1
      endif
c
c     perform extraction
c
      do 81 i = 0, mi-1
         ind=istk(ilr+i)-1
         stk(lr+i) = stk(l2+ind)
         if (it2.eq.1) stk(lr+mi+i) = stk(l2+mn2+ind)
 81   continue
c
c     set output sizes
c
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
c
c     form resulting variable
c
      istk(il1)=257
      istk(il1+1)=m
      istk(il1+2)=n
      istk(il1+3)=it2
      if (lr.ne.l1) call dcopy(mi*(it2+1),stk(lr),1,stk(l1),1)
      lstk(top+1)=l1+mi*(it2+1)
      go to 999
c     
c     arg3(arg1,arg2)
c
 82   if (rhs.gt.3) then
         call error(36)
         return
      endif
      if (mn3.eq.0) then
c 
c     .  arg3=[]
c
         il1=iadr(lstk(top))
         istk(il1)=257
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=sadr(il1+4)
         goto 999
      elseif (m3.lt.0) then
c
c     .arg3=eye
c
         call error(14)
         return
      endif
c
c     check and convert indices variables
c
      call indxg(il1,m3,ili,mi,mxi,lw,1)
      if (err.gt.0) return
      if (mxi.gt.m3) then
         call error(21)
         return
      endif
      call indxg(il2,n3,ilj,nj,mxj,lw,1)
      if (err.gt.0) return
      if (mxj.gt.n3) then
         call error(21)
         return
      endif
c
 90   mn=mi*nj
      if (mn.eq.0) then
c 
c     .  arg1=[] or arg2=[]
c 
         il1=iadr(lstk(top))
         istk(il1)=257
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=sadr(il1+4)
         goto 999
      endif
c
c     get memory for the result
c
      il1=iadr(lstk(top))
      l1=sadr(il1+4)
      if (sadr(ili-1).le.l1+(it3+1)*mi*nj) then
         lr=lw
         lw=lr+(it3+1)*mi*nj
         err=lw-lstk(bot)
         if (err.gt.0) then
            call error(17)
            return
         endif
      else
c
c     .  the result may be installed at its final place
c
         lr=l1
      endif
c
c     perform extraction
c
      l=lr
      do 94 j = 0, nj-1
         do 93 i = 0, mi-1
            ind=istk(ili+i)-1+(istk(ilj+j)-1)*m3
            stk(l) = stk(l3+ind)
            if (it3.eq.1) stk(l+mn) = stk(l3+mn3+ind)
            l=l+1
 93      continue
 94   continue
c
c     form the resulting variable
c
c      istk(il1)=istk(il3) la boucle 94 ecrase istk(il3) ???
      istk(il1)=257
      istk(il1+1)=mi
      istk(il1+2)=nj
      istk(il1+3)=it3
c      print *,istk(il1),mi,nj,it3
      if (lr.ne.l1) call dcopy(mn*(it3+1),stk(lr),1,stk(l1),1)
      lstk(top+1)=l1+mn*(it3+1)
      go to 999
c
c==============================
c     insertion
c==============================
c     
 395   continue
       istk(il1)=istkil1o
c       istk(il3)=257
       if(rhs.eq.4) goto 100
c
c     arg3(arg1)=arg2
c     
      if (istk(il2)*istk(il1).eq.0) then
         call error(220)
         return
      endif
      if (m2.eq.0) then
c
c     .  arg3(arg1)=[] -->[]
c
         if(m1.eq.-1) then
c
c     .    arg3(:)=[]
c 
            istk(il1)=257
            istk(il1+1)=0
            istk(il1+2)=0
            istk(il1+3)=0
            lstk(top+1)=sadr(il1+4)
            goto 999
         elseif(m1.eq.0) then
c
c     .     arg3([])=[]  --> arg3
c
            call icopy(4,istk(il3),1,istk(il1),1)
            l=sadr(il1+4)
            call dcopy(mn3*(it3+1),stk(l3),1,stk(l),1)
            lstk(top+1)=l+mn3*(it3+1)
            goto 999
         else
c
c     .     arg3(arg1)=[] -->arg3(compl(arg1),:)
c
            call indxgc(il1,mn3,ilr,mi,mx,lw)
            if(err.gt.0) return
            l2=l3
            n2=n3
            m2=m3
            mn2=m2*n2
            it2=it3
c
c     .     call extraction
c
            goto 79
         endif
      elseif(m2.lt.0.or.m3.lt.0) then
c
c     .  arg3=eye,arg2=eye
c
         call error(14)
         return
      elseif(m1.lt.0) then
c
c     .  arg3(:)=arg2
c
         if(mn2.eq.mn3) then
            istk(il1)=257
c
c            write(*,*)'arg3(:)=arg2',istk(il3)
c
            istk(il1+1)=m3
            istk(il1+2)=n3
            istk(il1+3)=it2
            l1=sadr(il1+4)
            call dcopy((it2+1)*mn2,stk(l2),1,stk(l1),1)
            lstk(top+1)=l1+mn2*(it2+1)
            return
         elseif(mn2.eq.1) then
            istk(il1)=257
c
c            write(*,*)'arg3(:)=arg2 (bis)',istk(il3)
c
            istk(il1+1)=m3
            istk(il1+2)=n3
            istk(il1+3)=it2
            l1=sadr(il1+4)
            call dset(mn3,stk(l2),stk(l1),1)
            if(it2.eq.1) call dset(mn3,stk(l2+1),stk(l1+mn3),1)
            lstk(top+1)=l1+mn3*(it2+1)
            return
         else
            call error(15)
            return
         endif
      endif
      call indxg(il1,mn3,ili,mi,mxi,lw,1)
      if(err.gt.0) return
      if(mi.eq.0) then
c
c     .  arg3([])=arg2
c
         if(mn2.eq.1) then
c
c     .  arg3([])=c  --> arg3
c
            call icopy(4,istk(il3),1,istk(il1),1)
            l=sadr(il1+4)
            call dcopy(mn3*(it3+1),stk(l3),1,stk(l),1)
            lstk(top+1)=l+mn3*(it3+1)
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
c
c     .  arg3 is not a vector
c
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
c
c     .  arg3 and arg2 are  column vectors
c
         mr=max(m3,mxi)
         nr=max(n3,1)
      elseif (m3.le.1.and.m2.le.1) then
c
c     .  row vectors
c
         nr=max(n3,mxi)
         mr=max(m3,1)
      else
c
c     .  arg3 and arg2 dimensions dont agree
c
         call error(15)
         return
      endif

      lr=l3
      mnr=mr*nr
      itr=max(it2,it3)
      if(mnr*(itr+1).ne.mn3*(it3+1) ) then
c
c     .  resulting matrix is bigger than original
c
         lr=lw
         lw=lr + mnr*(itr+1)
         err = lw - lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
c
c     .  initialise result r to 0
c
         call dset(mnr*(itr+1),PCZERO,stk(lr),1)
c
c     .  write arg3 in r
c
         if(mn3.ge.1) then
            call dmcopy(stk(l3),m3,stk(lr),mr,m3,n3)
            if(it3.eq.1) then
               call dmcopy(stk(l3+mn3),m3,stk(lr+mnr),mr,m3,n3)
            endif
         endif
      endif
c
c     write arg2 in r
c
      do 98 i = 0, mi-1
         ll = lr+istk(ili+i) - 1
         ls = l2+i*inc2
         stk(ll) = stk(ls)
         if(it2.eq.1) then
            stk(ll+mnr)=stk(ls+mn2)
         elseif(itr.eq.1) then
            stk(ll+mnr)=PCZERO
         endif
 98   continue
c     
      if(lr.ne.l3) then
         call dcopy(mnr*(itr+1),stk(lr),1,stk(l1),1)
         istk(il1)=257
         istk(il1+1)=mr
         istk(il1+2)=nr
         istk(il1+3)=itr
         l1=sadr(il1+4)
         lstk(top+1)=l1+mnr*(itr+1)
      else
c
c     la matrice a ete modifie sur place
c 
         k=istk(iadr(lstk(top0))+2)
         istk(il1)=-1
         istk(il1+1)=-1
         istk(il1+2)=k
         lstk(top+1)=lstk(top)+3
      endif
      goto 999

 100  continue
c     
c     arg4(arg1,arg2)=arg3
c
      istk(il4)=257
      if (istk(il3)*istk(il1)*istk(il2).eq.0) then
         call error(220)
         return
      endif
      if (m3.eq.0) then
c
c     .  arg4(arg1,arg2)=[]
c
         if(m1.eq.-1.and.m2.eq.-1) then
c
c     .    arg4(:,:)=[] -->[]
c
c            istk(il1)=257
            istk(il1+1)=0
            istk(il1+2)=0
            istk(il1+3)=0
            lstk(top+1)=sadr(il1+4)
            goto 999
         elseif(m1.eq.0.or.m2.eq.0) then
c
c     .     arg4([],arg2)=[],  arg4(arg1,[])=[] --> arg4
c
            call icopy(4,istk(il4),1,istk(il1),1)
            l=sadr(il1+4)
            call dcopy(mn4*(it4+1),stk(l4),1,stk(l),1)
            lstk(top+1)=l+mn4*(it4+1)
            goto 999
         elseif(m2.eq.-1) then
c
c     .     arg4(arg1,:)=[] --> arg4(compl(arg1),:)
c
            call indxgc(il1,m4,ili,mi,mxi,lw)
            if(err.gt.0) return
            call indxg(il2,n4,ilj,nj,mxj,lw,1)
            if(err.gt.0) return
            l3=l4
            n3=n4
            m3=m4
            mn3=m3*n3
            it3=it4
c
c     .     call extraction
c
            goto 90
         elseif(m1.eq.-1) then
c
c     .     arg4(:,arg2)=[] --> arg4(:,compl(arg2))
c
            call indxgc(il2,n4,ilj,nj,mxj,lw)
            if(err.gt.0) return
            call indxg(il1,m4,ili,mi,mxi,lw,1)
            if(err.gt.0) return
            l3=l4
            n3=n4
            m3=m4
            mn3=m3*n3
            it3=it4
c
c     .     call extraction
c
            goto 90
         else
c
c     .     arg4(arg1,arg2)=[]
c 
            lw1=lw
            call indxgc(il2,n4,ilj,nj,mxj,lw)
            if(err.gt.0) return
            if(nj.eq.0) then
c
c     .        arg4(arg1,1:n4)=[]
c 
               lw2=lw
               call indxgc(il1,m4,ili,mi,mxi,lw)
               if(err.gt.0) return

               if(mi.eq.0) then
c
c     .           arg4(1:m4,1:n4)=[] 
c
                  istk(il1)=257
                  istk(il1+1)=0
                  istk(il1+2)=0
                  istk(il1+3)=0
                  lstk(top+1)=sadr(il1+4)
                  goto 999
               else
c
c     .           arg4(arg1,1:n4)=[]
c 
                  lw=lw2
                  call indxg(il2,n4,ilj,nj,mxj,lw,1)
                  if(err.gt.0) return
                  l3=l4
                  n3=n4
                  m3=m4
                  it3=it4
                  mn3=m3*n3
c
c     .           call extraction
c
                  goto 90
               endif
            else
               call indxgc(il1,m4,ili,mi,mxi,lw)
               if(err.gt.0) return
               if(mi.eq.0) then
c
c     .           arg4(1:m4,arg2)=[]
c 
                  call indxg(il1,m4,ili,mi,mxi,lw,1)
                  if(err.gt.0) return
                  l3=l4
                  n3=n4
                  m3=m4
                  it3=it4
                  mn3=m3*n3
c
c     .           call extraction
c
                  goto 90
               else
                  call error(15)
                  return
               endif
            endif
         endif
      elseif(m3.lt.0.or.m4.lt.0) then
c
c     .  arg3=eye , arg4=eye
c
         call error(14)
         return
      elseif(m1.eq.-1.and.m2.eq.-1) then
c
c     .  arg4(:,:)=arg3
c
         if(mn3.eq.mn4) then
c
c     .     reshape arg3 according to arg4
c
            istk(il1)=257
            istk(il1+1)=m4
            istk(il1+2)=n4
            istk(il1+3)=it3
            l1=sadr(il1+4)
            call dcopy((it3+1)*mn4,stk(l3),1,stk(l1),1)
            lstk(top+1)=l1+mn4*(it3+1)
            return
         elseif(mn3.eq.1) then
c
c     .     set all elements of arg4 to arg3
c
            istk(il1)=257
            istk(il1+1)=m4
            istk(il1+2)=n4
            istk(il1+3)=it3
            l1=sadr(il1+4)
            call dset(mn4,stk(l3),stk(l1),1)
            if(it3.eq.1) call dset(mn4,stk(l3+1),stk(l1+mn4),1)
            lstk(top+1)=l1+mn4*(it3+1)
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
c
c     .  sizes of arg1 or arg2 dont agree with arg3 sizes
c
         if(m3*n3.eq.1) then
            if(mi.eq.0.or.mj.eq.0) then
               call icopy(4,istk(il4),1,istk(il1),1)
               l=sadr(il1+4)
               call dcopy(mn4*(it4+1),stk(l4),1,stk(l),1)
               lstk(top+1)=l+mn4*(it4+1)
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
      if(mnr*(itr+1).ne.mn4*(it4+1) ) then
         lr=lw
         lw=lr + mnr*(itr+1)
         err = lw - lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
c
c     .  set result r to 0
c
         call dset(mnr*(itr+1),PCZERO,stk(lr),1)
c
c     .  copy arg4 in r
c
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
c
      do 115 j = 0, mj-1
         ljj =  istk(ilj+j) - 1
         do 214 i = 0, mi-1
            ll = lr+istk(ili+i)-1+ljj*mr
            ls = l3+(i+j*m3)*inc3
            stk(ll) = stk(ls)
            if(it3.eq.1) then
               stk(ll+mnr)=stk(ls+mn3)
            elseif(itr.eq.1) then
               stk(ll+mnr)=PCZERO
            endif
 214     continue
 115  continue
c     
      if(lr.ne.l4) then
         call dcopy(mnr*(itr+1),stk(lr),1,stk(l1),1)
         istk(il1)=257
         istk(il1+1)=mr
         istk(il1+2)=nr
         istk(il1+3)=itr
         l1=sadr(il1+4)
         lstk(top+1)=l1+mnr*(itr+1)
      else
c
c     la matrice a ete modifie sur place 
c

         k=istk(iadr(lstk(top0))+2)
         il1=iadr(lstk(top))
         istk(il1)=-1
         istk(il1+1)=-1
         istk(il1+2)=k
         istk(il1+3)=0
         lstk(top+1)=lstk(top)+3
      endif
      goto 999
c
c==================     
c     *. /. \.
c==================
c
 120  fin=-fin
      top=top+1
      goto 999
c     
 130  continue
      if(fin.eq.61) then
         fin=-fin
         top=top+1
         goto 999
      endif
c
c===================
c     comparaisons
c===================
c
      if(mn1.eq.1.and.mn2.gt.1) then
         err=lw+mn2*(it1+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call dset(mn2,stk(l1),stk(lw),1)
         if(it1.eq.1) call dset(mn2,stk(l1+1),stk(lw+mn2),1)
         l1=lw
         mn1=mn2
         m1=m2
         n1=n2
         istk(il1+1)=m1
         istk(il1+2)=n1
      elseif(mn2.eq.1.and.mn1.gt.1) then
         err=lw+mn1*(it2+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call dset(mn1,stk(l2),stk(lw),1)
         if(it1.eq.1) call dset(mn1,stk(l2+1),stk(lw+mn1),1)
         l2=lw
         mn2=mn1
         m2=m1
         n2=n1
      elseif(mn2.eq.0.and.mn1.eq.0) then
         if(op.eq.11.or.op.eq.12) then
            istk(il1)=4
            istk(il1+1)=1
            istk(il1+2)=1
            istk(il1+3)=1
            if(op.eq.12) istk(il1+3)=0
            lstk(top+1)=sadr(il1+4)
            goto 999
         else
            call error(60)
            return
         endif

      endif
      if(n1.ne.n2.or.m1.ne.m2) then
         if(op.eq.11.or.op.eq.12) then
            istk(il1)=4
            istk(il1+1)=1
            istk(il1+2)=1
            istk(il1+3)=0
            if(op.eq.12) istk(il1+3)=1
            lstk(top+1)=sadr(il1+4)
         else
            call error(60)
            return
         endif
      else if(max(it1,it2).eq.1) then
         if(op.ne.11.and.op.ne.12) then
            call error(57)
            return
         endif
         itrue=1
         if(op.eq.12) itrue=0
         istk(il1)=4
         do 131 i=0,mn1-1
            e1r=stk(l1+i)
            e2r=stk(l2+i)
            e1i=PCZERO
            e2i=PCZERO
            if(it1.eq.1) e1i=stk(l1+mn1+i)
            if(it2.eq.1) e2i=stk(l2+mn2+i)
            if(e1r.eq.e2r.and.e1i.eq.e2i) then
               istk(il1+3+i)=itrue
            else
               istk(il1+3+i)=1-itrue
            endif
 131     continue
         lstk(top+1)=sadr(il1+3+mn1)
      else
         istk(il1)=4
         if(mn1.eq.0) then
            if(op.ne.11.and.op.ne.12) then
               call error(57)
            else
               istk(il1+1)=1
               istk(il1+2)=1
               istk(il1+3)=1
               if(op.ne.11) istk(il1+3)=0
               lstk(top+1)=sadr(il1+4)
            endif
            return
         endif
         do 132 i=0,mn1-1
            e1=stk(l1+i)
            e2=stk(l2+i)
            if ( (op.eq.11 .and. e1.eq.e2) .or.
     &           (op.eq.12 .and. e1.ne.e2) .or.
     &           (op.eq.13 .and. e1.lt.e2) .or.
     &           (op.eq.14 .and. e1.gt.e2) .or.
     &           (op.eq.15 .and. e1.le.e2) .or.
     &           (op.eq.16 .and. e1.ge.e2) ) then
               istk(il1+3+i)=1
            else
               istk(il1+3+i)=0
            endif
 132     continue
         lstk(top+1)=sadr(il1+3+mn1)
      endif
c     
      
 999  return
      end
