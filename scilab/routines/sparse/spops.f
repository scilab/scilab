      subroutine spops
c     
c     operations on sparse matrices
c     
c     Copyright INRIA
      include '../stack.h'
      integer op
      common /mtlbc/ mmode
c     
      integer iadr,sadr
c     
      double precision sr,si,e1,powr,powi
      integer star,dstar,slash,bslash,dot,colon,quote
      integer less,great,equal
      integer insert,extrac
      integer top0
      logical isany
      integer tops,top4

*     the following boolean vars are defined to replace test on m*n == 0 or m*n == 1)
*     for sparse matrices the product m x n may leads to integer overflow.
*     (bruno dec 2004)
      logical a1_is_empty, a1_is_scalar, a2_is_empty, a2_is_scalar, 
     $        a3_is_empty, a3_is_scalar, a4_is_empty, a4_is_scalar
      
      data star/47/,dstar/62/,slash/48/
      data bslash/49/,dot/51/,colon/44/,quote/53/
      data less/59/,great/60/,equal/50/
      data insert/2/,extrac/3/
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      op=fin
c     
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' spops op: '//buf(1:4))
      endif
c     
      top0=top
      lw=lstk(top+1)+1
      if(op.eq.extrac) goto 70
      if(op.eq.insert) goto 80
      it2=0
      goto (04,03,02,01) rhs
      call error(39)
      return
c     
 01   il4=iadr(lstk(top))
      if(istk(il4).lt.0) il4=iadr(istk(il4+1))
      m4=istk(il4+1)
      n4=istk(il4+2)
      it4=istk(il4+3)
      if(istk(il4).eq.5) then
         nel4=istk(il4+4)
         irc4=il4+5
         l4=sadr(irc4+m4+nel4)
      elseif(istk(il4).eq.1) then
         nel4=m4*n4
         l4=sadr(il4+4)
      else
         top=top0
         fin=-fin
         return
      endif
      mn4=m4*n4   ! must not be used if arg4 is a sparse
      a4_is_empty = m4.eq.0 .or. n4.eq.0
      a4_is_scalar = (.not.a4_is_empty) .and. (m4.eq.1 .and. n4.eq.1)

      top=top-1
c     
 02   il3=iadr(lstk(top))
      if(istk(il3).lt.0) il3=iadr(istk(il3+1))
      m3=istk(il3+1)
      n3=istk(il3+2)
      it3=istk(il3+3)
      if(istk(il3).eq.5) then
         nel3=istk(il3+4)
         irc3=il3+5
         l3=sadr(irc3+m3+nel3)
      elseif(istk(il3).eq.1) then
         l3=sadr(il3+4)
         nel3=m3*n3
      else
         top=top0
         fin=-fin
         return
      endif
      mn3=m3*n3 ! must not be used if arg3 is a sparse
      a3_is_empty = m3.eq.0 .or. n3.eq.0
      a3_is_scalar = (.not.a3_is_empty) .and. (m3.eq.1 .and. n3.eq.1)

      top=top-1
c     
 03   il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)
      n2=istk(il2+2)
      it2=istk(il2+3)
      if(istk(il2).eq.5) then
         nel2=istk(il2+4)
         irc2=il2+5
         l2=sadr(irc2+m2+nel2)
      elseif(istk(il2).eq.1) then
         l2=sadr(il2+4)
         nel2=m2*n2
      else
         top=top0
         fin=-fin
         return
      endif
      mn2=m2*n2 ! must not be used if arg2 is a sparse
      a2_is_empty = m2.eq.0 .or. n2.eq.0
      a2_is_scalar = (.not.a2_is_empty) .and. (m2.eq.1 .and. n2.eq.1)

      top=top-1
c     
 04   il1=iadr(lstk(top))
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      if(istk(il1).eq.5) then
         nel1=istk(il1+4)
         irc1=il1+5
         l1=sadr(irc1+m1+nel1)
      elseif(istk(il1).eq.1) then
         l1=sadr(il1+4)
         nel1=m1*n1
      else
         top=top0
         fin=-fin
         return
      endif
      mn1=m1*n1 ! must not be used if arg1 is a sparse
      a1_is_empty = m1.eq.0 .or. n1.eq.0
      a1_is_scalar = (.not.a1_is_empty) .and. (m1.eq.1 .and. n1.eq.1)

      top=top-1
c     
c     operations binaires et ternaires
c     --------------------------------
c     
      top=top+1
      itr=max(it1,it2)
c     
      fun = 0
c
c        cconc  extrac insert rconc
      goto(65  ,  999  ,  999   ,66) op
c     
c           :  +  -  * /  \  =          '
      goto(06,07,08,10,20,25,130,05,05,60) op+1-colon
c     
 05   if(op.eq.dstar .or. op.eq.dstar+dot) goto 30  ! case dstar+dot added (bug fix 1769)
      if(op.eq.quote+dot) goto 60
      if(op.ge.3*dot+star) goto 200
      if(op.ge.2*dot+star) goto 120
      if(op.ge.less+equal) goto 130
      if(op.ge.dot+star) goto 55
      if(op.ge.less) goto 130

 06   top=top0
      fin=-fin
      return

c     
c     addition
 07   continue

      if (a1_is_empty) then
c       []+a
         if (mmode.eq.1) then
c     .     Matlab like []+a=[]
         else
c     .     []+a=a
            call icopy(5+m2+nel2,istk(il2),1,istk(il1),1)
            l1=sadr(il1+5+m2+nel2)
            call unsfdcopy(nel2*(it2+1),stk(l2),1,stk(l1),1)
            lstk(top+1)=l1+nel2*(it2+1)
            goto 999
         endif
      elseif(a2_is_empty) then
c     a+[]
         if (mmode.eq.1) then
c     .     Matlab like a+[]=[]
            istk(il1+1)=0
            istk(il1+2)=0
            istk(il1+3)=0
            lstk(top+1)=sadr(il1+4)
         else
c     .     a+[]=a
         endif
         goto 999
      endif
      if (m1 .lt. 0) then
c     eye+a
         top=top0
         fin=-fin
         return
      endif
      if (m2 .lt. 0) then
c     a+eye
         top=top0
         fin=-fin
         return
      endif

      if (a2_is_scalar .and. .not. a1_is_scalar) then
c        a+cst
         top=top0
         fin=-fin
         return
      endif
      if (a1_is_scalar .and. .not.a2_is_scalar) then
c        cst+a
         top=top0
         fin=-fin
         return
      endif
      if (m1.ne.m2 .or. n1.ne.n2) then
         call error(8)
         return
      endif

      if(istk(il1).ne.5 .or. istk(il2).ne.5) then
c     addition with a full matrix
         top=top0
         fin=-fin
         return
      endif
c     addition of 2 sparse matrices of the same size
      irc=iadr(lw)
      if(itr.eq.1) then
         nelmx=(iadr(lstk(bot))-irc-m1-10)/5
      else
         nelmx=(iadr(lstk(bot))-irc-m1-10)/3
      endif
      lc=sadr(irc+m1+nelmx)
      lw=lc+nelmx*(itr+1)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif  
      nel=nelmx
      if(itr.eq.1) then
         call wspasp(m1,n1,stk(l1),stk(l1+nel1),nel1,istk(irc1),
     $        stk(l2),stk(l2+nel2),nel2,istk(irc2),stk(lc),stk(lc+nel),
     $        nel,istk(irc),it1,it2,ierr)
      else
         call dspasp(m1,n1,stk(l1),nel1,istk(irc1),stk(l2),nel2,
     $        istk(irc2),stk(lc),nel,istk(irc),ierr)
      endif
      if(ierr.ne.0) then
         call error(17)
         return
      endif
      istk(il1+3)=itr
      istk(il1+4)=nel
      call icopy(m1+nel,istk(irc),1,istk(irc1),1)
      l1=sadr(irc1+m1+nel)
      call unsfdcopy(nel,stk(lc),1,stk(l1),1)
      if(itr.eq.1) call unsfdcopy(nel,stk(lc+nelmx),1,stk(l1+nel),1)
      lstk(top+1)=l1+nel*(itr+1)
      go to 999
c     
c     soustraction
 08   if(rhs.eq.1) then
         if(mn1.eq.0) goto 999
         call dscal(nel1*(it1+1),-1.0d+0,stk(l1),1)
         goto 999
      endif

      if (a1_is_empty) then
c        []-a
         if (mmode.eq.1) then
c     .     Matlab like []-a=[]
         else
c     .     []-a=-a
            call icopy(5+m2+nel2,istk(il2),1,istk(il1),1)
            l1=sadr(il1+5+m2+nel2)
            call unsfdcopy(nel2*(it2+1),stk(l2),1,stk(l1),1)
            call dscal(nel2*(it2+1),-1.0d0,stk(l1),1)
            lstk(top+1)=l1+nel2*(it2+1)
            goto 999
         endif
      elseif(a2_is_empty) then
c         a-[]
         if (mmode.eq.1) then
c     .     Matlab like a-[]=[]
            istk(il1+1)=0
            istk(il1+2)=0
            istk(il1+3)=0
            lstk(top+1)=sadr(il1+4)
         else
c     .     a-[]=a
         endif
         goto 999
      endif
      if (m1 .lt. 0.or.m2 .lt. 0) then
c     soustraction a-eye*b
         top=top0
         fin=-fin
         return
      endif

      if (a2_is_scalar .and. .not.a1_is_scalar) then
c        a-cst
         top=top0
         fin=-fin
         return
      endif
      if (a1_is_scalar .and. .not.a2_is_scalar) then
c        cst-a
         top=top0
         fin=-fin
         return
      endif

c       check dimensions
      if (m1 .ne. m2.or.n1 .ne. n2) then
         call error(9)
         return
      endif
      if(istk(il1).ne.5.or.istk(il2).ne.5) then
c     soustraction avec une matrice non creuse
         top=top0
         fin=-fin
         return
      endif
c     soustraction de 2 matrice sparse de meme taille
      irc=iadr(lw)
      if(itr.eq.1) then
         nelmx=(iadr(lstk(bot))-irc-m1-10)/5
      else
         nelmx=(iadr(lstk(bot))-irc-m1-10)/3
      endif
      lc=sadr(irc+m1+nelmx)
      lw=lc+nelmx*(itr+1)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif  
      nel=nelmx
      if(itr.eq.1) then
         call wspssp(m1,n1,stk(l1),stk(l1+nel1),nel1,istk(irc1),
     $        stk(l2),stk(l2+nel2),nel2,istk(irc2),stk(lc),stk(lc+nel),
     $        nel,istk(irc),it1,it2,ierr)
      else
         call dspssp(m1,n1,stk(l1),nel1,istk(irc1),stk(l2),nel2,
     $        istk(irc2),stk(lc),nel,istk(irc),ierr)
      endif
      if(ierr.ne.0) then
         call error(17)
         return
      endif
      istk(il1+3)=itr
      istk(il1+4)=nel
      call icopy(m1+nel,istk(irc),1,istk(irc1),1)
      l1=sadr(irc1+m1+nel)
      call unsfdcopy(nel,stk(lc),1,stk(l1),1)
      if(itr.eq.1) call unsfdcopy(nel,stk(lc+nelmx),1,stk(l1+nel),1)
      lstk(top+1)=l1+nel*(itr+1)
      go to 999

c     multiplication
 10   continue
c$$$      if (m2*mn2 .eq. 1) go to 12
c$$$      if (mn1 .eq. 1 ) go to 13
c$$$      if (mn2 .eq. 1 ) go to 12
      if (a2_is_scalar) goto 12
      if (a1_is_scalar) goto 13
      m1=abs(m1)
      n1=abs(n1)
      m2=abs(m2)
      n2=abs(n2)
      if(a1_is_empty .or. a2_is_empty) then
         istk(il1)=1
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=sadr(il1+4)+1
         goto 999
      endif
c     matrix matrix multiplication
      if (n1 .ne. m2) then
         call error(10)
         return
      endif
      if(istk(il1).eq.1.or.istk(il2).eq.1) then
c     full x sparse or sparse x full
         lc=lw
         lw=lw+m1*n2*(itr+1)
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         if(istk(il1).eq.1) then
c     full x sparse
            if(itr.eq.1) then
               call wsmsp(m1,n1,n2,stk(l1),stk(l1+m1*n1),m1,
     $              stk(l2),stk(l2+nel2),nel2,istk(irc2),
     $              stk(lc),stk(lc+m1*n2),m1,it1,it2)
            else
               call dsmsp(m1,n1,n2,stk(l1),m1,stk(l2),nel2,
     $              istk(irc2),stk(lc),m1)
            endif
         else
c     sparse x full
            if(itr.eq.1) then
               call wspms(m1,n1,n2,stk(l1),stk(l1+nel1),nel1,istk(irc1),
     $              stk(l2),stk(l2+m2*n2),m2,stk(lc),stk(lc+m1*n2),m1,
     $              it1,it2)
            else
               call dspms(m1,n1,n2,stk(l1),nel1,istk(irc1),
     $              stk(l2),m2,stk(lc),m1)
            endif
         endif
         istk(il1)=1
         istk(il1+1)=m1
         istk(il1+2)=n2
         istk(il1+3)=itr
         l1=sadr(il1+4)
         call unsfdcopy(m1*n2*(itr+1),stk(lc),1,stk(l1),1)
         lstk(top+1)=l1+m1*n2*(itr+1)
         return   ! ici un return direct (pas de goto 999) !
      endif

*   sparse x sparse
      ib=iadr(lw)
      ic=ib+m2+1
      ixb=ic+m1+1
      lx=sadr(ixb+n2)
      lc=lx+n2*(itr+1)
      nel=(iadr(lstk(bot))-iadr(lc)-m1-10)/(1+2*(itr+1))
      irc=iadr(lc+nel*(itr+1))
      lw=sadr(irc+m1+nel)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      nelc=nel
      if(itr.eq.1) then
         call wspmsp(m1,n1,n2,stk(l1),stk(l1+nel1),nel1,istk(irc1),
     $        stk(l2),stk(l2+nel2),nel2,istk(irc2),
     $        stk(lc),stk(lc+nel),nelc,istk(irc),
     $        istk(ib),istk(ic),stk(lx),stk(lx+n2),istk(ixb),
     $        it1,it2,ierr)
      else
         call dspmsp(m1,n1,n2,stk(l1),nel1,istk(irc1),
     $        stk(l2),nel2,istk(irc2),stk(lc),nelc,istk(irc),
     $        istk(ib),istk(ic),stk(lx),istk(ixb),ierr)
      endif
      if(ierr.eq.1) then
         buf='not enough memory'
         call error(9999)
         return
      endif
      istk(il1+2)=n2
      istk(il1+3)=itr
      istk(il1+4)=nelc
      l1=sadr(il1+5+m1+nelc)
      if(lc.ge.l1) then
         call unsfdcopy(nelc,stk(lc),1,stk(l1),1)
         if(itr.eq.1) call unsfdcopy(nelc,stk(lc+nel),1,stk(l1+nelc),1)
      else
         call unsfdcopy(nelc,stk(lc),-1,stk(l1),-1)
         if(itr.eq.1) call unsfdcopy(nelc,stk(lc+nel),-1,
     $        stk(l1+nelc),-1)
      endif
      call icopy(nelc+m1,istk(irc),1,istk(il1+5),1)
      lstk(top+1)=l1+nelc*(itr+1)
      go to 999
c     
 12   continue
c     a*cst
      if(istk(il2).eq.1) then
         si = 0.0d0
         sr = stk(l2)
         if(it2.eq.1) si = stk(l2+1)
      elseif(istk(il2).eq.5) then
         if(nel2.eq.0) then
            sr = 0.0d0
            si = 0.0d0
         else
            si = 0.0d0
            sr = stk(l2)
            if(it2.eq.1) si = stk(l2+1)
         endif
      endif
      if(abs(sr)+abs(si).eq.0.0d0) then
         if(istk(il1).eq.1) then
            istk(il1+3)=0
            call dset(m1*n1,0.0d0,stk(l1),1)
            lstk(top+1)=l1+m1*n1
         else
            istk(il1+3)=0
            istk(il1+4)=0
            call iset(abs(m1),0,istk(il1+5),1)
            lstk(top+1)=sadr(il1+5+abs(m1))+1
         endif
         return
      endif
      go to 14

 13   continue
c     cst*a
      if(istk(il1).eq.1) then
         si = 0.0d0
         sr = stk(l1)
         if(it1.eq.1) si = stk(l1+1)
      elseif(istk(il1).eq.5) then
         if(nel1.eq.0) then
            sr = 0.0d0
            si = 0.0d0
         else
            si = 0.0d0
            sr = stk(l1)
            if(it1.eq.1) si = stk(l1+1)
         endif
      endif
      if(abs(sr)+abs(si).eq.0.0d0) then
         if(istk(il2).eq.1) then
            istk(il1)=1
            istk(il1+1)=m2
            istk(il1+2)=n2
            istk(il1+3)=0
            l1=sadr(il1+4)
            call dset(m2*n2,0.0d0,stk(l1),1)
            lstk(top+1)=l1+m2*n2
         else
            istk(il1)=5
            istk(il1+1)=m2
            istk(il1+2)=n2
            istk(il1+3)=0
            istk(il1+4)=0
c           call iset(iabs(m2),0,stk(il1+5),1)
c           lstk(top+1)=sadr(il1+5+abs(m2))+1
c            FD modif 
            nnnb=1+abs(m2)
            call iset(nnnb,0,istk(il1+5),1)
            lstk(top+1)=sadr(il1+5+nnnb)
         endif
         return
      endif

      if(istk(il2).eq.5) then
         call icopy(5+m2+nel2,istk(il2),1,istk(il1),1)
         l1=sadr(il1+5+m2+nel2)
         call unsfdcopy(nel2*(it2+1),stk(l2),1,stk(l1),1)
      elseif(istk(il2).eq.1) then
         call icopy(4,istk(il2),1,istk(il1),1)
         l1=sadr(il1+4)
         call unsfdcopy(mn2*(it2+1),stk(l2),1,stk(l1),1)
      endif
      m1=m2
      n1=n2
      mn1=it1
      it1=it2
      it2=mn1
      mn1=mn2
      nel1=nel2
c     
 14   continue
      istk(il1+1)=m1
      istk(il1+2)=n1
      istk(il1+3)=itr
      lstk(top+1)=l1+nel1*(itr+1)
c     
      err=lstk(top+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c     
      goto (15,16,17),it2+2*it1
c     la matrice et le scalaire sont reel
      call dscal(nel1,sr,stk(l1),1)
      lstk(top+1)=l1+nel1
      goto 999
 15   continue
c     la matrice est reelle le scalaire est complexe
      err=l1+2*nel1-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call unsfdcopy(nel1,stk(l1),1,stk(l1+nel1),1)
      call dscal(nel1,sr,stk(l1),1)
      call dscal(nel1,si,stk(l1+nel1),1)
      lstk(top+1)=l1+2*nel1
      goto 999
 16   continue
c     la matrice est complexe, le scalaire est reel
      call dscal(nel1,sr,stk(l1),1)
      call dscal(nel1,sr,stk(l1+nel1),1)
      lstk(top+1)=l1+2*nel1
      goto 999
 17   continue
c     la matrice et le scalaire sont complexes
      call wscal(nel1,sr,si,stk(l1),stk(l1+nel1),1)
      lstk(top+1)=l1+2*nel1
      goto 999
c     
c      right division
 20   if (mn2 .ne. 1) then
c     right division by a matrix -->macro coded
         fin = -fin
         top = top0
         rhs = 2
         go to 999
      endif
c     right division by a scalar
      sr=stk(l2)
      si=0.0d+0
      if(it2.eq.1) si=stk(l2+1)
      e1=max(abs(sr),abs(si))
      if(e1.eq.0.0d+0) then
         call error(27)
         return
      endif
      sr=sr/e1
      si=si/e1
      e1=e1*(sr*sr+si*si)
      sr=sr/e1
      si=-si/e1
c     call multiplication with scalar inverse
      goto 14
c     
c     left division
 25   if ( .not.a1_is_scalar ) then
c     left division by a matrix -->macro coded
         top=top0
         fin=-fin
         return
      endif
c     left division by a scalar
      sr=stk(l1)
      si=0.0d+0
      if(it1.eq.1) si=stk(l1+1)
      e1=max(abs(sr),abs(si))
      if(e1.eq.0.0d+0) then
         call error(27)
         return
      endif
      sr=sr/e1
      si=si/e1
      e1=e1*(sr*sr+si*si)
      sr=sr/e1
      si=-si/e1
      if(istk(il2).eq.1) then
         call icopy(4,istk(il2),1,istk(il1),1)
         l1=sadr(il1+4)
         nel2=mn2
         call unsfdcopy(nel2*(it2+1),stk(l2),1,stk(l1),1)
      else
         call icopy(5+2*nel2,istk(il2),1,istk(il1),1)
         l1=sadr(il1+5+m2+nel2)
         call unsfdcopy(nel2*(it2+1),stk(l2),1,stk(l1),1)
      endif
      m1=m2
      n1=n2
      it=it1
      it1=it2
      it2=it
      mn1=mn2
      nel1=nel2
      goto 14
*
*     power operations: sp^pow or sp.^pow 
*     (modified by Bruno jan 19 2006 to fix bug 1769)
*     
*     notes : dstar corresponds to ^
*             dstar+dot corresponds to .^
*             at the beginning of this toooooooo big subroutine the code is
*             branched here in these 2 cases.
*
 30   if (mn2 .ne. 1) then
         call error(30)
         return
      endif

*     for sp^pow with a sp square and not scalar (scalar case is faster handle
*     by the element wize power op)
      if ( op.eq.dstar .and. m1.eq.n1 .and. .not.a1_is_scalar ) goto 31

*     sp^pow is nevertheless "authorized" when sp is a vector (and done
*     as an element wize power operation)
      if ( op.eq.dstar .and. m1.ne.1 .and. n1.ne.1 ) then
         err=1
         call error(20)
         return
      endif
*     so the following concerns only element wise power
*
************end of the modif for bug 1769****************
*
      err=l1+nel1*2-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      if(it2.eq.0) then
         powr=stk(l2)
         if(it1.eq.0) then
            call ddpow(nel1,stk(l1),stk(l1+nel1),1,powr,err,itr)
         else
            call wdpow(nel1,stk(l1),stk(l1+nel1),1,powr,err)
         endif
      else
         powr=stk(l2)
         powi=stk(l2+1)
         if(it1.eq.0) then
            call dwpow(nel1,stk(l1),stk(l1+nel1),1,
     &           powr,powi,err)
         else
            call wwpow(nel1,stk(l1),stk(l1+nel1),1,
     &           powr,powi,err)
         endif
      endif
      if(err.eq.1) then
         call error(30)
         return
      endif
      if(err.eq.2) then
         call error(27)
         return
      endif
      istk(il1+3)=itr
      lstk(top+1)=l1+nel1*(itr+1)
      goto 999
c     
c     elevation d'une matrice carree a une puissance
 31   continue
      top=top0
      fin=-fin
      return
c     

c     operations elements a elements
 55   continue
      i1=1
      i2=1
      op = op - dot
      if(a1_is_empty .or. a2_is_empty) then
c     [].*a     a.*[]  -->[]
         istk(il1)=1
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=sadr(il1+4)+1
         goto 999
      endif
      if(a1_is_scalar .or. a2_is_scalar) then
         goto(06,07,08,10,20,25,130,05,05,60) op+1-colon
      endif
c
c     check dimensions
      if (m1.ne.m2 .or. n1.ne.n2) then
         call error(6)
         return
      endif
      if(op.eq.slash) then
         if(istk(il2).eq.1) then
            if(it2.eq.0) then
               do 56 i=0,mn1-1
                  if(stk(l2+i).eq.0.0d0) then
                     call error(27)
                     return
                  endif
                  stk(l2+i)=1.0d0/stk(l2+i)
 56            continue
            else          
               do 57 i=0,mn2-1
                  sr=stk(l2+i)
                  si=stk(l2+mn2+i)
                  e1=abs(sr)+abs(si)
                  if(e1.eq.0.0d+0) then
                     call error(27)
                     return
                  endif
                  sr=sr/e1
                  si=si/e1
                  e1=e1*(sr*sr+si*si)
                  stk(l2+i)=sr/e1
                  stk(l2+mn2+i)=-si/e1
 57            continue
            endif
         else
            top=top0
            fin=-fin
            return
         endif
      elseif(op.eq.bslash) then
         if(istk(il1).eq.1) then
            if(it1.eq.0) then
               do 58 i=0,mn1-1
                  if(stk(l1+i).eq.0.0d0) then
                     call error(27)
                     return
                  endif
                  stk(l1+i)=1.0d0/stk(l1+i)
 58            continue
            else          
               do 59 i=0,mn1-1
                  sr=stk(l1+i)
                  si=stk(l1+mn1+i)
                  e1=abs(sr)+abs(si)
                  if(e1.eq.0.0d+0) then
                     call error(27)
                     return
                  endif
                  sr=sr/e1
                  si=si/e1
                  e1=e1*(sr*sr+si*si)
                  stk(l1+i)=sr/e1
                  stk(l1+mn1+i)=-si/e1
 59            continue
            endif
         else
            top=top0
            fin=-fin
            return
         endif
      endif
      if(nel1.eq.0.or.nel2.eq.0) then
         istk(il1)=5
         istk(il1+3)=0
         istk(il1+4)=0
         call iset(m1,0,istk(il1+5),1)
         lstk(top+1)=sadr(il1+5+m1)
         return
      endif
      if(istk(il1).eq.1) then
c     full.*sparse
         nel=nel2
         if(it1.eq.0.and.it2.eq.0) then
            call dspxs(m1,n1,stk(l2),nel2,istk(irc2),stk(l1),m1,
     $           stk(l2),nel,istk(irc2),ierr)
         else
            if(it2.eq.0) then
               err=l2+2*nel2-lstk(bot)
               if(err.gt.0) then
                  call error(17)
                  return
               endif
            endif
            call wspxs(m1,n1,stk(l2),stk(l2+nel2),nel2,istk(irc2),
     $           stk(l1),stk(l1+nel1),m1,
     $           stk(l2),stk(l2+nel2),nel,istk(irc2),ierr,it2,it1)
         endif
         istk(il1)=5
         istk(il1+3)=itr
         istk(il1+4)=nel
         l=sadr(il1+5+m1+nel)
         call icopy(m1+nel,istk(irc2),1,istk(il1+5),1)
         call unsfdcopy(nel,stk(l2),1,stk(l),1)
         if(itr.eq.1) call unsfdcopy(nel,stk(l2+nel2),1,stk(l+nel),1)
         lstk(top+1)=l+nel*(itr+1)
         return
      elseif(istk(il2).eq.1) then
c     sparse.*full
         nel=nel1
         if(it1.eq.0.and.it2.eq.0) then
            call dspxs(m1,n1,stk(l1),nel1,istk(irc1),stk(l2),m1,
     $           stk(l1),nel,istk(irc1),ierr)
         else
            lri=l1+nel1
            if(it1.eq.0) then
               lri=lw
               err=lri+nel1-lstk(bot)
               if(err.gt.0) then
                  call error(17)
                  return
               endif
            endif
            call wspxs(m1,n1,stk(l1),stk(l1+nel1),nel1,istk(irc1),
     $           stk(l2),stk(l2+nel2),m1,
     $           stk(l1),stk(lri),nel,istk(irc1),ierr,it1,it2)
         endif
         istk(il1)=5
         istk(il1+3)=itr
         istk(il1+4)=nel
         l=sadr(il1+5+m1+nel)
         call icopy(m1+nel,istk(irc1),1,istk(il1+5),1)
         call unsfdcopy(nel,stk(l1),1,stk(l),1)
         if(itr.eq.1) call unsfdcopy(nel,stk(lri),1,stk(l+nel),1)
         lstk(top+1)=l+nel*(itr+1)
         return
      endif
c     sparse.*sparse
      nel=nel1
      if(it1.eq.0.and.it2.eq.0) then
         call dspxsp(m1,n1,stk(l1),nel1,istk(irc1),
     $        stk(l2),nel2,istk(irc2),stk(l1),nel,istk(irc1),ierr)
         lrr=l1

      else
         lrr=lw
         lri=lrr+min(nel1,nel2)
         err=lri+min(nel1,nel2)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call wspxsp(m1,n1,stk(l1),stk(l1+nel1),nel1,istk(irc1),
     $        stk(l2),stk(l2+nel2),nel2,istk(irc2),
     $        stk(lrr),stk(lri),nel,istk(irc1),ierr,it1,it2)
      endif

      istk(il1+3)=itr
      istk(il1+4)=nel
      l=sadr(il1+5+m1+nel)
      call unsfdcopy(nel,stk(lrr),1,stk(l),1)
      if(itr.eq.1) call unsfdcopy(nel,stk(lri),1,stk(l+nel),1)
      lstk(top+1)=l+nel*(itr+1)
      return
c     
c     
c     transposition (modified by bruno)
 60   istk(il1+1)=n1
      istk(il1+2)=m1
      if(nel1.eq.0) then
         lw=sadr(il1+5+n1)
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call iset(n1,0,istk(il1+5),1)
         lstk(top+1)=lw
         goto 999
      endif
      iptr=iadr(lw)  ! for work array ptr of size n1
      irc2=iptr+n1 
      l2=sadr(irc2+n1+nel1)
      lw=l2+nel1*(it1+1) 
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif

      if (it1 .eq. 0) then
         inc = 0
      else
         inc = nel1
      endif

      call spt(m1, n1, nel1, it1, istk(iptr),
     $         stk(l1), stk(l1+inc), istk(irc1), istk(irc1+m1),
     $         stk(l2), stk(l2+inc), istk(irc2), istk(irc2+n1))

*     recopie en "top"
      call icopy(n1+nel1,istk(irc2),1,istk(irc1),1)
      l1=sadr(irc1+n1+nel1)
      call unsfdcopy(nel1*(it1+1),stk(l2),1,stk(l1),1)
      if(it1.eq.1 .and. op.ne.quote+dot) then
         call dscal(nel1,-1.0d0,stk(l1+nel1),1)  ! complexe conjugue si A' et it=1
      endif
      lstk(top+1)=l1+nel1*(it1+1)
      goto 999
c     
c     concatenation [a b]
 65   continue
      if(m1.lt.0.or.m2.lt.0) then
         call error(14)
         return
      endif
      if(m2.eq.0) then
         return
      elseif(m1.eq.0) then
         call icopy(5+m2+nel2,istk(il2),1,istk(il1),1)
         l1=sadr(il1+5+m2+nel2)
         call unsfdcopy(nel2*(it2+1),stk(l2),1,stk(l1),1)
         lstk(top+1)=l1+nel2*(it2+1)
         return
      elseif(m1.ne.m2) then
         call error(5)
         return
      endif
      if(istk(il1).ne.5.or.istk(il2).ne.5) then
         top=top0
         fin=-fin
         return
      endif
c
      nelr=nel1+nel2
      istk(il1+2)=n1+n2
      istk(il1+3)=itr
      istk(il1+4)=nelr
      lr=lw
      irc=iadr(lr+nelr*(itr+1))
      lw=sadr(irc+m1+nelr)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      if(itr.eq.0) then
         call dspcsp(0,m1,n1,stk(l1),nel1,istk(irc1),
     $        m2,n2,stk(l2),nel2,istk(irc2),
     $        stk(lr),nelr,istk(irc))
      else
         call wspcsp(0,m1,n1,stk(l1),stk(l1+nel1),nel1,istk(irc1),
     $        m2,n2,stk(l2),stk(l2+nel2),nel2,istk(irc2),
     $        stk(lr),stk(lr+nelr),nelr,istk(irc),it1,it2)
      endif
      call icopy(m1+nelr,istk(irc),1,istk(irc1),1)
      l1=sadr(irc1+m1+nelr)
      call unsfdcopy(nelr*(itr+1),stk(lr),1,stk(l1),1)
      lstk(top+1)=l1+nelr*(itr+1)
      return
c     
c     concatenation [a;b]
 66   continue
      if(n1.lt.0.or.n2.lt.0) then
         call error(14)
         return
      endif
      if(n2.eq.0) then
         goto 999
      elseif(n1.eq.0)then
         call icopy(5+m2+nel2,istk(il2),1,istk(il1),1)
         l1=sadr(il1+5+m2+nel2)
         call unsfdcopy(nel2*(it2+1),stk(l2),1,stk(l1),1)
         lstk(top+1)=l1+nel2*(it2+1)
         goto 999
      elseif(n1.ne.n2) then
         call error(6)
         return
      endif
      if(istk(il1).ne.5.or.istk(il2).ne.5) then
         top=top0
         fin=-fin
         return
      endif


      nelr=nel1+nel2
      istk(il1+1)=m1+m2
      istk(il1+3)=itr
      istk(il1+4)=nelr
      lr=lw
      irc=iadr(lr+nelr*(itr+1))
      lw=sadr(irc+m1+m2+nelr)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      if(itr.eq.0) then
         call dspcsp(1,m1,n1,stk(l1),nel1,istk(irc1),
     $        m2,n2,stk(l2),nel2,istk(irc2),
     $        stk(lr),nelr,istk(irc))
      else
         call wspcsp(1,m1,n1,stk(l1),stk(l1+nel1),nel1,istk(irc1),
     $        m2,n2,stk(l2),stk(l2+nel2),nel2,istk(irc2),
     $        stk(lr),stk(lr+nelr),nelr,istk(irc),it1,it2)
      endif
      call icopy(m1+m2+nelr,istk(irc),1,istk(irc1),1)
      l1=sadr(irc1+m1+m2+nelr)
      call unsfdcopy(nelr*(itr+1),stk(lr),1,stk(l1),1)
      lstk(top+1)=l1+nelr*(itr+1)
      goto 999
c     
c     extraction (modified by bruno)
c     
 70   continue
      if(rhs.lt.2) then
         call error(227)
         return
      endif
      if(rhs.gt.2) goto 75  ! goto extraction arg3(arg1,arg2)

c****** extraction arg2(arg1) *************************************
*
c     get arg2
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)                          
      n2=istk(il2+2)
      it2=istk(il2+3)
      nel2=istk(il2+4)
      irc2=il2+5
      l2=sadr(irc2+m2+nel2)
      a2_is_empty = m2.eq.0 .or. n2.eq.0
      a2_is_scalar = (.not.a2_is_empty) .and. (m2.eq.1 .and. n2.eq.1)
      mn2=m2*n2
      top=top-1
c     get arg1
      il1=iadr(lstk(top))
      ilrs=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      a1_is_empty = m1.eq.0 .or. n1.eq.0
      a1_is_scalar = (.not.a1_is_empty) .and. (m1.eq.1 .and. n1.eq.1)

      if(a2_is_empty) then
c     .  arg2=[]  -> return an empty matrix []
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
      elseif(m1.lt.0) then  !  case  arg2(:) => just reshape to column vector
         if(n2.eq.1) then
c     .     already a column vector
            ilrs=iadr(lstk(top))
            call icopy(5+m2+nel2,istk(il2),1,istk(ilrs),1)
            l1=sadr(ilrs+5+m2+nel2)
            call unsfdcopy(nel2*(it2+1),stk(l2),1,stk(l1),1)
            lstk(top+1)=l1+nel2*(it2+1)
         else               !  n2 > 1
c     .     reshape to column vector via spmat (reshape is named matrix in scilab)
            ilrs=iadr(lstk(top))
            istk(ilrs)=5
            istk(ilrs+1)=mn2
            istk(ilrs+2)=1
            istk(ilrs+3)=it2
            istk(ilrs+4)=nel2
            irc1=ilrs+5
            l1=sadr(ilrs+5+m2*n2+nel2)

            ircr=iadr(lw)
            iw=ircr+m2*n2+nel2
** correction d'un bug
            lr=sadr(iw+3*nel2)
            lw= lr + nel2*(it2+1)
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return 
            endif
            ! copie des valeurs de arg2
            call unsfdcopy(nel2*(it2+1),stk(l2),1,stk(lr),1)
            if(it2.eq.0) then
*** le bug etait du au fait que  stk(l2) (remplace par stk(lr)) est remanie 
*** (via une permutation) et donc pb si arg2 est passe par reference

*** il faudrait peut utiliser spreshape maintenant
               call dspmat(m2,n2,stk(lr),nel2,istk(irc2),m2*n2
     $              ,istk(ircr),istk(iw))  
            else
               call wspmat(m2,n2,stk(lr),stk(lr+mn2),nel2,istk(irc2)
     $              ,m2*n2,istk(ircr),istk(iw)) 
            endif
            call icopy(m2*n2+nel2,istk(ircr),1,istk(irc1),1)
            call unsfdcopy(nel2*(it2+1),stk(lr),1,stk(l1),1)
            lstk(top+1)=l1+nel2*(it2+1)
         endif
         return
      endif

*** extraction  arg2(arg1)  (suite)
      call indxg(il1,mn2,ilr,mi,mx,lw,1)  ! analysis of the index vector arg1
      if(err.gt.0) return
      if(mx.gt.mn2) then
         call error(21)
         return
      endif

 72   if(mi.eq.0) then  ! case  arg2([]) => return a void matrix (type = 1)
         ilrs=iadr(lstk(top))
         istk(ilrs)=1
         istk(ilrs+1)=0
         istk(ilrs+2)=0
         istk(ilrs+3)=0
         lstk(top+1)=sadr(ilrs+4)+1
         goto 999
      endif
c     set output sizes  !  extraction  arg2(arg1)
      if ( m2 .eq. 1 ) then  ! A is a row sparse => B also (and only in this case)
         mr = 1
         nr = mi
      else                   ! A not a row sparse => B a column sparse
         mr = mi
         nr = 1
      endif

c     get memory for the result
      lptr=iadr(lw)
      irc=lptr+m2
      lw=sadr(irc+mr)
      nelrm=(2*(lstk(bot)-lw)-1)/(3+2*it2)
      if(nelrm.le.0) then
         err=lw-lstk(bot)
         call error(17)
         return
      endif
      lr=sadr(irc+mr+nelrm)
      lw=lr+nelrm*(it2+1)
      inc2 = nel2*it2
      incr = nelrm*it2

*       subroutine spextr1(A_m, A_n, A_nel, A_mnel, A_icol, A_R, A_I,
*     $                   B_m, B_n, B_nel, B_mnel, B_icol, B_R, B_I,
*     $                   it, i, ni, nel_max, ptr, ierr)

      call spextr1(m2, n2, nel2, istk(irc2), istk(irc2+m2), stk(l2), 
     $             stk(l2+inc2),
     $             mr, nr, nelr, istk(irc), istk(irc+mr), stk(lr),
     $             stk(lr+incr), 
     $             it2, istk(ilr), mi, nelrm, istk(lptr), ierr)

c     form resulting variable
      ilrs=iadr(lstk(top))
      istk(ilrs)=5
      istk(ilrs+1)=mr
      istk(ilrs+2)=nr
      istk(ilrs+3)=it2
      istk(ilrs+4)=nelr
      call icopy(mr+nelr,istk(irc),1,istk(ilrs+5),1)
      l1=sadr(ilrs+5+mr+nelr)
      call unsfdcopy(nelr,stk(lr),1,stk(l1),1)
      if(it2.eq.1)  call unsfdcopy(nelr,stk(lr+nelrm),1,stk(l1+nelr),1)
      lstk(top+1)=l1+nelr*(it2+1)
      go to 999
c     
c     arg3(arg1,arg2)
 75   if(rhs.gt.3) then
         call error(36)
         return
      endif
c     get arg3
      il3=iadr(lstk(top))
      if(istk(il3).lt.0) il3=iadr(istk(il3+1))
      m3=istk(il3+1)
      n3=istk(il3+2)
      it3=istk(il3+3)
      nel3=istk(il3+4)
      irc3=il3+5
      l3=sadr(irc3+m3+nel3)
      mn3=m3*n3
      top=top-1
c     get arg2
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)
      top=top-1
c     get arg1
      il1=iadr(lstk(top))
      ilrs=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)

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

      call indxg(il1,m3,ili,mi,mxi,lw,11)
      if(err.gt.0) return
      if(mxi.gt.m3) then
         call error(21)
         return
      endif
      if(mi.lt.0) then
         mr=m3 ! modif bruno
      else
         mr=mi
      endif
      call indxg(il2,n3,ilj,nj,mxj,lw,11)
      if(err.gt.0) return
      if(mxj.gt.n3) then
         call error(21)
         return
      endif
      if(nj.lt.0) then
         nr=n3 ! modif bruno
      else
         nr=nj
      endif
c
 76   continue
      if(mr.eq.0 .or. nr.eq.0) then 
c     .  arg1=[] or arg2=[] 
         ilrs=iadr(lstk(top))
         istk(ilrs)=1
         istk(ilrs+1)=0
         istk(ilrs+2)=0
         istk(ilrs+3)=0
         lstk(top+1)=sadr(ilrs+4)+1
         goto 999
      endif
c     get memory for the result
      lptr=iadr(lw)    !  istk(lptr) = ptr(1)
      irc=lptr+m3+1    !  m3+1 cases pour le tableau ptr =>  istk(irc) = p(1)
      ircr = irc + nr  !  nr cases pour le tableau p de la permutation mnelr(1) = istk(ircr)
      lw=sadr(ircr+mr) !  
      nelrmax = (lstk(bot)-lw)/(1+2*(it3+1))  ! nb max possible d'elts pour la matrice resultat
      if(nelrmax.le.0) then
         err=lw-lstk(bot)
         call error(17)
         return
      endif
      lr=sadr(ircr+mr+nelrmax)
      lw=lr+nelrmax*(it3+1)

c     perform extraction
      if(it3.eq.0) then  ! les parties imaginaires (inutilisées)
         inc3 = 0        ! pointeront sur les parties réelles
         incr = 0
      else
         inc3 = nel3
         incr = nelrmax
      endif
      call spextr(m3, n3, nel3, istk(irc3), istk(irc3+m3), stk(l3),
     $            stk(l3+inc3),
     $            mr, nr, nelr, istk(ircr), istk(ircr+mr), stk(lr),
     $            stk(lr+incr),
     $            it3, istk(ili), mi, istk(ilj), nj, nelrmax, 
     $            istk(lptr), istk(irc), ierr)

*      subroutine spextr(A_m, A_n, A_nel, A_mnel, A_icol, A_R, A_I,
*     $                  B_m, B_n, B_nel, B_mnel, B_icol, B_R, B_I,
*     $                  it, i, ni, j, nj, nel_max, ptr, p, ierr)

      if(ierr .eq. -1) then  ! not enough memory
         err=1               ! valeur bidon : j'imagine qu'il faut donner 
         call error(17)      ! une idee a l'utilisateur de la mémoire manquante
         return
      endif

c     form resulting variable
      ilrs=iadr(lstk(top))
      istk(ilrs)=5
      istk(ilrs+1)=mr
      istk(ilrs+2)=nr
      istk(ilrs+3)=it3
      istk(ilrs+4)=nelr
      call icopy(mr+nelr,istk(ircr),1,istk(ilrs+5),1)
      l1=sadr(ilrs+5+mr+nelr)
      call unsfdcopy(nelr,stk(lr),1,stk(l1),1)
      if(it3.eq.1) call unsfdcopy(nelr,stk(lr+nelrmax),1,stk(l1+nelr),1)
      lstk(top+1)=l1+nelr*(it3+1)
      go to 999
c      
c     insertion
 80   continue
      if(rhs.gt.4) then
         top=top0
         fin=-fin
         return
      endif
      if(rhs.eq.4) goto 90
c     arg3(arg1)=arg2
c     get arg3      
      tops = top
      il3=iadr(lstk(top))
      if(istk(il3).lt.0) il3=iadr(istk(il3+1))
      m3=istk(il3+1)
      n3=istk(il3+2)
      it3=istk(il3+3)
      if(istk(il3).eq.5) then
         nel3=istk(il3+4)
         irc3=il3+5
         l3=sadr(irc3+m3+nel3)
      else
         top=top0
         fin=-fin
         return
      endif
      mn3=m3*n3

c     get arg2
      top=top-1
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)
      n2=istk(il2+2)
      it2=istk(il2+3)
      if(istk(il2).eq.5) then
         nel2=istk(il2+4)
         irc2=il2+5
         l2=sadr(irc2+m2+nel2)
      elseif(istk(il2).eq.1) then
         l2=sadr(il2+4)
         nel2=m2*n2
      else
         top=top0
         fin=-fin
         return
      endif
      mn2=m2*n2

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

****************************************************************
*
*    code added by bruno to treat the case arg3(arg1)=arg2
*    with:
*       arg1 a sparse boolean matrix of the same dimension than arg3
*       arg2 is a full vector or a scalar with nel1 elements 
*
      if (istk(il1).eq.6 .and. m1.eq.m3 .and. n1.eq.n3 .and.
     $    istk(il2).eq.1 .and. (m2.eq.1 .or. n2.eq.1) ) then
         nel1 = istk(il1+4)
         a2_is_scalar = m2.eq.1 .and. n2.eq.1
         if (a2_is_scalar .or. m2*n2.eq.nel1) then
            ip = iadr(lstk(tops+1))
            if (.not. a2_is_scalar) then
               iq = ip + n1+1
               ilr = iq + nel1
            else
               iq = ip
               ilr = ip
            endif
            lr = sadr(ilr + 5 + m1)
            itr = max(it3,it2)
            nelmax = (2*(lstk(bot)-lr)-1)/(3+2*itr)
            if(nelmax .le. 0) then
               buf='not enough memory'
               call error(9999)
               return
            endif
            ilrc = ilr + 5 + m1
            lr = sadr(ilrc+nelmax)
            if (itr .eq. 0) then
               lri = lr
            else
               lri = lr + nelmax
            endif

            call spif1b(m3, n3, nel3, it3, istk(irc3), istk(irc3+m3), 
     $                  stk(l3), stk(l3+nel3), nel1, istk(il1+5), 
     $                  istk(il1+5+m1), it2, stk(l2), stk(l2+mn2),
     $                  a2_is_scalar, nelr, itr, istk(ilr+5), 
     $                  istk(ilrc), stk(lr), stk(lri), nelmax, istk(ip),
     $                  istk(iq), ierr)
            if(ierr.ne.0) then
               buf='not enough memory'
               call error(9999)
               return
            endif
*           form the resulting var
            istk(ilrs) = 5
            istk(ilrs+1) = m3
            istk(ilrs+2) = n3
            istk(ilrs+3) = itr
            istk(ilrs+4) = nelr
            call icopy(m3+nelr,istk(ilr+5),1,istk(ilrs+5),1)
            lrs = sadr(ilrs+5+m3+nelr)
            call unsfdcopy(nelr,stk(lr),1,stk(lrs),1)
            if (itr .eq. 1) then
               call unsfdcopy(nelr,stk(lri),1,stk(lrs+nelr),1)
            endif
            lstk(top+1) = lrs + (1+itr)*nelr
            go to 999
         endif
      endif
*
*   end of the case added by Bruno
*
****************************************************************

      if (m2.eq.0) then
c     .  arg3(arg1)=[] -->[]
         if(m1.eq.-1) then
c     .    arg3(:)=[] 
            istk(ilrs)=1
            istk(ilrs+1)=0
            istk(ilrs+2)=0
            istk(ilrs+3)=0
            lstk(top+1)=sadr(ilrs+4)+1
            goto 999
         elseif(m1.eq.0) then
c     .     arg3([])=[]  --> arg3
            call icopy(5+m3+nel3,istk(il3),1,istk(ilrs),1)
            l=sadr(ilrs+5+m3+nel3)
            call unsfdcopy(nel3*(it3+1),stk(l3),1,stk(l),1)
*            lstk(top+1)=l+mn3*(it3+1)
            lstk(top+1)=l+nel3*(it3+1)
            goto 999
         else
c     .     arg3(arg1)=[]
            if(istk(il1).eq.4.and.m3.eq.m1.and.n3.eq.n1) then
               if(.not.isany(il1)) then
c     .           arg3([])=[]  --> arg3
                  call icopy(5+m3+nel3,istk(il3),1,istk(ilrs),1)
                  l=sadr(ilrs+5+m3+nel3)
                  call unsfdcopy(nel3*(it3+1),stk(l3),1,stk(l),1)
*                  lstk(top+1)=l+mn3*(it3+1)  
                  lstk(top+1)=l+nel3*(it3+1)  
                  goto 999
               endif
            endif
c     .     arg3(arg1)=[] -->arg3(compl(arg1),:)
            if(m3.gt.1.and.n3.gt.1) then
c     .        call macro coded op to reshape and insert
               top=top0
               fin=-fin
               return
            else
               call indxgc(il1,mn3,ilr,mi,mx,lw)
               if(err.gt.0) return
               l2=l3
               n2=n3
               m2=m3
               mn2=m2*n2
               it2=it3
               nel2=nel3
               irc2=irc3
c     .     call extraction
               goto 72
            endif
         endif
      elseif(m2.lt.0.or.m3.lt.0) then
c     .  arg3=eye,arg2=eye
         call error(14)
         return
      elseif(m1.lt.0) then
c     .  arg3(:)=arg2 reshape arg2 according to arg3
*  CAUTION: bug if arg2 is a full matrix !
         if(mn2.eq.mn3) then
            if(m2.ne.m3) then
               top=top0
               fin=-fin
               return
            endif
            istk(ilrs)=5
            istk(ilrs+1)=m3
            istk(ilrs+2)=n3
            call icopy(2+m2+nel2,istk(il2+3),1,istk(ilrs+3),1)
            l1=sadr(ilrs+5+m2+nel2)
            call unsfdcopy(nel2*(it2+1),stk(l2),1,stk(l1),1)
            lstk(top+1)=l1+nel2*(it2+1)
            return
         elseif(mn2.eq.1) then
            istk(ilrs)=1
            istk(ilrs+1)=m3
            istk(ilrs+2)=n3
            istk(ilrs+3)=it2
            l1=sadr(ilrs+4)
            call dset(mn3,stk(l2),stk(l1),1)
            if(it2.eq.1) call dset(mn3,stk(l2+1),stk(l1+mn3),1)
            lstk(top+1)=l1+mn3*(it2+1)
            return
         else
            call error(15)
            return
         endif
      elseif(m3.gt.1.and.n3.gt.1) then
c     .  arg3(arg1)=arg2 with arg3 not a vector
         top=top0
         fin=-fin
         return
      endif


 81   call indxg(il1,mn3,ili,mi,mxi,lw,1)
      if(err.gt.0) return
      if(mi.eq.0) then
c     .  arg3([])=arg2
         if(mn2.eq.1) then
c     .  arg3([])=c  --> arg3 
            call icopy(5+m3+nel3,istk(il3),1,istk(ilrs),1)
            l=sadr(ilrs+5+m3+nel3)
            call unsfdcopy(nel3*(it3+1),stk(l3),1,stk(l),1)
*            lstk(top+1)=l+mn3*(it3+1)
            lstk(top+1)=l+nel3*(it3+1)
            goto 999
         else
            call error(15)
            return
         endif  
      endif
      if(mi.ne.mn2) then
         if(mn2.gt.1) then
            call error(15)
            return
         elseif(istk(il2).ne.1) then
            top=top0
            fin=-fin
            return
         endif
      endif
c     
      if (n3.gt.1.and.m3.gt.1) then  ! ce test est inutile puisque ce cas est traite par macro
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

c     set output sizes
      if (m3 .gt. 1.or.m1.lt.0) then
c     .  column vector
         m=mi
         n=-1
         mr = mi
         nr = 1
      else
c     .  row vector
         m=-1
         n=mi
         nr = mi
         mr = 1
      endif
      itr=max(it2,it3)
      lptr=iadr(lw)
      irc=lptr+mr+1
      lw=sadr(irc+mr)
      nelr=(lstk(bot)-lw-1)/(1+2*(itr+1))
      if(nelr.le.0) then
         err=lw-lstk(bot)
         call error(17)
         return
      endif
      lr=sadr(irc+mr+nelr)
      lw=lr+nelr*(itr+1)
      nel=nelr
      if(istk(il2).eq.5) then
         if(itr.eq.0) then
            call dspisp(m3,n3,stk(l3),nel3,istk(irc3),
     $           istk(ili),m,istk(ili),n,
     $           m2,n2,stk(l2),nel2,istk(irc2),
     $           mr,nr,stk(lr),nelr,istk(irc),istk(lptr),ierr)
         else
            call wspisp(m3,n3,stk(l3),stk(l3+nel3),nel3,istk(irc3),
     $           istk(ili),m,istk(ili),n,
     $           m2,n2,stk(l2),stk(l2+nel2),nel2,istk(irc2),
     $           mr,nr,stk(lr),stk(lr+nelr),nelr,istk(irc),
     $           istk(lptr),ierr,it3,it2)
         endif
      else
         if(itr.eq.0) then
            call dspis(m3,n3,stk(l3),nel3,istk(irc3),
     $           istk(ili),m,istk(ili),n,
     $           m2,n2,stk(l2),
     $           mr,nr,stk(lr),nelr,istk(irc),ierr) 
         else
            call wspis(m3,n3,stk(l3),stk(l3+nel3),nel3,istk(irc3),
     $           istk(ili),m,istk(ili),n,
     $           m2,n2,stk(l2),stk(l2+nel2),
     $           mr,nr,stk(lr),stk(lr+nelr),nelr,istk(irc),
     $           ierr,it3,it2) 
         endif
      endif
      if(ierr.ne.0) then
         buf='not enough memory'
         call error(9999)
         return
      endif
      istk(ilrs)=5
      istk(ilrs+1)=mr
      istk(ilrs+2)=nr
      istk(ilrs+3)=itr
      istk(ilrs+4)=nelr
      call icopy(mr+nelr,istk(irc),1,istk(ilrs+5),1)
      l1=sadr(ilrs+5+mr+nelr)
      call unsfdcopy(nelr,stk(lr),1,stk(l1),1)
      if(itr.eq.1) call unsfdcopy(nelr,stk(lr+nel),1,stk(l1+nelr),1)
      lstk(top+1)=l1+nelr*(itr+1)
      go to 999   ! c'est un return

 90   continue

c     **** insertion  arg4(arg1,arg2)=arg3  *****
c     (comments added by Bruno to try to understand this stuff)

c     get arg4      
      il4=iadr(lstk(top))
      top4 = top ! pour le cas en place
      if(istk(il4).lt.0) il4=iadr(istk(il4+1))
      m4=istk(il4+1)
      n4=istk(il4+2)
      it4=istk(il4+3)
      if(istk(il4).eq.5) then  ! arg4 is a sparse matrix
         nel4=istk(il4+4)
         irc4=il4+5             ! irc4 index in istk for the arrays  mnel(m4 elts) and icol(nel4 elts)
         l4=sadr(irc4+m4+nel4)  ! l4   index in stk for the coef arrays (real and complex if any)
      else                 
         top=top0          
         fin=-fin
         return
      endif
      mn4=m4*n4

c     get arg3    ! for insertion  arg4(arg1,arg2)=arg3
      top=top-1
      il3=iadr(lstk(top))
      if(istk(il3).lt.0) il3=iadr(istk(il3+1))
      m3=istk(il3+1)
      n3=istk(il3+2)
      it3=istk(il3+3)
      if(istk(il3).eq.5) then  ! arg3 is a sparse matrix
         nel3=istk(il3+4)
         irc3=il3+5            
         l3=sadr(irc3+m3+nel3)
      elseif(istk(il3).eq.1) then  !  arg3 is a full matrix
         l3=sadr(il3+4)
         nel3=m3*n3
      else
         top=top0
         fin=-fin
         return
      endif
      mn3=m3*n3

c     get arg2    ! for insertion  arg4(arg1,arg2)=arg3
      top=top-1
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)

c     get arg1    ! for insertion  arg4(arg1,arg2)=arg3
      top=top-1
      il1=iadr(lstk(top))
      ilrs=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)

      if (m3.eq.0) then   ! So this is the operation   arg4(arg1,arg2) = [] => all rows of
                          ! of indices arg1 and all columns of indices arg2 must be deleted. 
                          ! In the following many special cases are taken into account
                          ! this is certainly not necessary.
         if(m1.eq.-1.and.m2.eq.-1) then 
            ! this is  arg4(:,:)=[] and so arg4 becomes a empty matrix arg4 <- []
            istk(ilrs)=1
            istk(ilrs+1)=0
            istk(ilrs+2)=0
            istk(ilrs+3)=0
            lstk(top+1)=sadr(ilrs+4)+1
            goto 999      ! goto the end

         elseif(m1.eq.0.or.m2.eq.0) then
            ! this is   arg4([],arg2)=[] or  arg4(arg1,[])=[] --> arg4 is not modified
            call icopy(5+m4+nel4,istk(il4),1,istk(ilrs),1)  ! ilrs index in istk of the result
            l=sadr(ilrs+5+m4+nel4)
            call unsfdcopy(nel4*(it4+1),stk(l4),1,stk(l),1)
            lstk(top+1)=l+mn4*(it4+1)
            goto 999

         elseif(m2.eq.-1) then
            ! this is   arg3(arg1,:)=[] --> arg3(compl(arg1),:)
            call indxgc(il1,m4,ili,mi,mxi,lw)
            if(err.gt.0) return
            mr=mi
            call indxg(il2,n4,ilj,nj,mxj,lw,11)
            if(err.gt.0) return
            if(nj.lt.0) then
               nr=mxj
            else
               nr=nj
            endif
            l3=l4
            n3=n4
            m3=m4
            mn3=m3*n3
            it3=it4
            irc3=irc4
            nel3=nel4
c     .     call extraction (the result is arg3(compl(arg1),:))
            goto 76

         elseif(m1.eq.-1) then
            ! this is   arg4(:,arg2)=[] --> arg4(:,compl(arg2))
            call indxgc(il2,n4,ilj,nj,mxj,lw)
            if(err.gt.0) return
            nr=nj
            call indxg(il1,m4,ili,mi,mxi,lw,11)
            if(err.gt.0) return
            if(mi.lt.0) then
               mr=mxi
            else
               mr=mi
            endif
            l3=l4
            n3=n4
            m3=m4
            mn3=m3*n3
            it3=it4
            irc3=irc4
            nel3=nel4
c     .     call extraction (the result is arg3(:,compl(arg2)))
            goto 76
         else
            ! this is   arg4(arg1,arg2)=[] 
            lw1=lw
            call indxgc(il2,n4,ilj,nj,mxj,lw)
            if(err.gt.0) return
            nr=nj
            if(nj.eq.0) then
c     .        arg4(arg1,1:n4)=[] 
               call indxgc(il1,m4,ili,mi,mxi,lw)
               lw2=lw
               if(err.gt.0) return
               mr=mi
c     .        arg2=1:n3
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
                  call indxg(il2,n4,ilj,nj,mxj,lw,11)
                  if(err.gt.0) return
                  if(nj.lt.0) then
                     nr=mxj
                  else
                     nr=nj
                  endif
                  l3=l4
                  n3=n4
                  m3=m4
                  it3=it4
                  mn3=m3*n3
                  irc3=irc4
                  nel3=nel4
c     .           call extraction
                  goto 76
               endif
            else
c               lw=lw1
               call indxgc(il1,m4,ili,mi,mxi,lw)
               if(err.gt.0) return
               if(mi.eq.0) then
c     .           arg4(1:m4,arg2)=[] 
                  call indxg(il1,m4,ili,mi,mxi,lw,11)
                  if(err.gt.0) return
                  if(mi.lt.0) then
                     mr=mxi
                  else
                     mr=mi
                  endif
                  l3=l4
                  n3=n4
                  m3=m4
                  it3=it4
                  mn3=m3*n3
                  irc3=irc4
                  nel3=nel4
c     .           call extraction
                  goto 76
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
            if(m3.ne.m4) then  ! bizarre ca on a alors A(:,:)=B qui ne change pas A
               top=top0        ! en fait on imagine plutot une erreur a indiquer
               fin=-fin
               return
c     .  reshape arg3 according to arg4  : ici on a donc des matrices de tailles identiques
c             il ne semble pas y avoir de distinctions entre sparse ou pleine pour arg3 (B)
c             ca peut expliquer le plantage ...  OUI
c             remede : cependant ce cas peut etre pris en compte par les routines generiques
c                      plutot que de le traiter dans l'interface. De plus ce n'est pas si
c                      court comme code vu qu'il faut tester les coefs de B. L'interface
c                      ne doit s'occuper que des cas triviaux et des cas d'erreurs
*
*   A signaler a Serge.
*
            elseif (istk(il3) .eq. 5) then ! ajout bruno (si arg3 est pleine le code suivant n'est pas bon)
               istk(ilrs)=5
               istk(ilrs+1)=m4
               istk(ilrs+2)=n4
               call icopy(2+m3+nel3,istk(il3+3),1,istk(ilrs+3),1)
               l1=sadr(ilrs+5+m3+nel3)
               call unsfdcopy(nel3*(it3+1),stk(l3),1,stk(l1),1)
               lstk(top+1)=l1+nel3*(it3+1)
               return
            endif
         elseif(mn3.eq.1) then   ! arg4(:,:)=arg3 avec arg3 un scalaire 
            istk(ilrs)=1         ! on change de type (matrice pleine)
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

      ! insertion  arg4(arg1,arg2)=arg3 ... suite 
      call indxg(il1,m4,ili,mi,mxi,lw,11)
      if(err.gt.0) return
      if(mi.lt.0) then
         mr1=mxi   ! car indice implicite :
      else
         mr1=mi
      endif
      call indxg(il2,n4,ilj,mj,mxj,lw,11)
      if(err.gt.0) return
      if(mj.lt.0) then
         nr1=mxj   ! car indice implicite :
      else
         nr1=mj
      endif
      if(mr1.ne.m3.or.nr1.ne.n3) then
c     .  sizes of arg1 or arg2 dont agree with arg3 sizes
         if(m3*n3.eq.1) then
            if(mr1.eq.0.or.nr1.eq.0) then 
               call icopy(5+m4+nel4,istk(il4),1,istk(ilrs),1)
               l=sadr(ilrs+5+m4+nel4)
               call unsfdcopy(nel4*(it4+1),stk(l4),1,stk(l),1)
               lstk(top+1)=l+mn4*(it4+1)
               goto 999
            endif
            if(istk(il3).ne.1) then
               top=top0
               fin=-fin
               return
            endif
         else
            call error(15)
            return
         endif
      else
         if(mr1.eq.0.or.nr1.eq.0) then  ! est-ce possible ?
            call error(15)
            return
         endif
      endif
      mr=max(m4,mxi)
      nr=max(n4,mxj)
c      ! insertion  arg4(arg1,arg2)=arg3 ... suite 

      ! try if we can do insertion in place
      if (      (istk(il3).eq.1) .and. (it4.ge.it3) 
     $    .and. (mi.gt.0).and.(mj.gt.0).and.(mi.le.m4).and.(mj.le.n4)
     $    .and. (mi*mj.lt.nel4/4) ) then 
*
         lws = lw               ! sauvegarde
         lptr=iadr(lw)          ! for ptr (size m4)
         lka = lptr + m4        ! for ka  (size mi*mj)
         lw = sadr(lka+mi*mj)
         err=lw-lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
         call spifp(m4, n4, nel4, istk(irc4), istk(irc4+m4), stk(l4),
     $              stk(l4+it4*nel4), it4, istk(ili), mi, istk(ilj), mj,
     $              istk(lptr), istk(lka), it3, stk(l3), 
     $              stk(l3+mi*mj*it3), iflag)
         if (iflag .eq. 1) then ! yes insertion in place is OK (and also done by spifp)
            k=istk(iadr(lstk(top4))+2)
            top = top - 1
            call setref(k)
            goto 999
         else
            lw = lws 
         endif
      endif

      itr=max(it4,it3)
      if(istk(il3).eq.5) then  ! insertion sparse(ind_i, ind_j) = sparse matrix
         lptr=iadr(lw)    
         irc=lptr+mr+1
         lw=sadr(irc+mr)
         nelr=(lstk(bot)-lw)/(1+2*(itr+1))
         if(nelr.le.0) then
            err=lw-lstk(bot)
            call error(17)
            return
         endif
         lr=sadr(irc+mr+nelr)
         lw=lr+nelr*(itr+1)
         nel=nelr

         if(itr.eq.0) then
            call dspisp(m4,n4,stk(l4),nel4,istk(irc4),
     $           istk(ili),mi,istk(ilj),mj,
     $           m3,n3,stk(l3),nel3,istk(irc3),
     $           mr,nr,stk(lr),nelr,istk(irc),istk(lptr),ierr)
         else
            call wspisp(m4,n4,stk(l4),stk(l4+nel4),nel4,istk(irc4),
     $           istk(ili),mi,istk(ilj),mj,
     $           m3,n3,stk(l3),stk(l3+nel3),nel3,istk(irc3),
     $           mr,nr,stk(lr),stk(lr+nelr),nelr,istk(irc),
     $           istk(lptr),ierr,it4,it3)
         endif

      else  ! insertion sparse(ind_i, ind_j) = full matrix

         ipi = iadr(lw)   ! indice pour pi
         ipj = ipi + max(0,mi)  ! indice pour pj
         irc = ipj + max(0,mj)  ! indice pour C_mnel
         lw = sadr(irc + mr)    ! indice de stk à partir duquel il faut caser C_icol, C_R et C_I
         nelmax = 2*(lstk(bot)-lw)/(1+2*(itr+1))
         if(nelmax .le. 0) then
            err = lw-lstk(bot)
            call error(17)
            return
         endif
         lr = sadr(irc + mr + nelmax)
         lw = lr + nelmax*(itr + 1)

c     arg4(arg1,arg2)=arg3   A(i,j) = B
*      subroutine spif(A_m, A_n, A_nel, A_it, A_mnel, A_icol, A_R, A_I,
*     $                B_m, B_n, B_it, B_R, B_I,
*     $                C_m, C_n, C_nel, C_it, C_mnel, C_icol, C_R, C_I,
*     $                i, pi, ni, j, pj, nj, nelmax, ierr)
*
         call spif(m4,n4,nel4,it4,istk(irc4),istk(irc4+m4),
     $             stk(l4),stk(l4+it4*nel4),
     $             m3,n3,it3,stk(l3),stk(l3+it3*m3*n3),
     $             mr,nr,nelr,itr,istk(irc), istk(irc+mr), 
     $             stk(lr), stk(lr+itr*nelmax),
     $             istk(ili), istk(ipi), mi, istk(ilj), istk(ipj), mj, 
     $             nelmax, ierr) 
         nel = nelmax

      endif

      if(ierr.ne.0) then     ! methode a utiliser lorsque l'on peut difficilement
         buf='not enough memory'   ! evaluer la memoire manquante
         call error(9999)
         return
      endif
      istk(ilrs)=5
      istk(ilrs+1)=mr
      istk(ilrs+2)=nr
      istk(ilrs+3)=itr
      istk(ilrs+4)=nelr
      call icopy(mr+nelr,istk(irc),1,istk(ilrs+5),1)
      l1=sadr(ilrs+5+mr+nelr)
      call unsfdcopy(nelr,stk(lr),1,stk(l1),1)
      if(itr.eq.1) call unsfdcopy(nelr,stk(lr+nel),1,stk(l1+nelr),1)
      lstk(top+1)=l1+nelr*(itr+1)
      go to 999
c     
c     *. /. \.
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

c     comparaisons
      if(max(it1,it2).eq.1) then
         if(op.ne.equal.and.op.ne.less+great) then
            fin=-fin
            top=top0
            return
         endif
      endif
      if (a1_is_empty .and. a2_is_empty) then
         if(op.eq.equal.or.op.eq.less+great) then
            istk(il1)=4
            istk(il1+1)=1
            istk(il1+2)=1
            istk(il1+3)=1
            if(op.eq.less+great) istk(il1+3)=0
            lstk(top+1)=sadr(il1+4)
            goto 999
         else
            call error(60)
            return
         endif
      endif
      if( (a1_is_empty .or. a2_is_empty).or.
     &    (.not.a1_is_scalar .and. .not. a2_is_scalar)) then
         if(n1.ne.n2.or.m1.ne.m2) then
            if(op.eq.equal.or.op.eq.less+great) then
               istk(il1)=4
               istk(il1+1)=1
               istk(il1+2)=1
               istk(il1+3)=0
               if(op.eq.less+great) istk(il1+3)=1
               lstk(top+1)=sadr(il1+4)
               return
            else
               call error(60)
               return
            endif
         endif
      endif
c
      mr=m1
      nr=n1
      if( a1_is_scalar ) then
         mr=m2
         nr=n2
      endif
      irc=iadr(lw)
      nelmx=(iadr(lstk(bot))-irc-mr-10)
      lw=sadr(irc+mr+nelmx)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif  
      nel=nelmx
      if(istk(il1).eq.1) then
         if(itr.eq.1) then
            call wsosp(op,m1,n1,stk(l1),stk(l1+nel1),
     $           m2,n2,stk(l2),stk(l2+nel2),nel2,istk(irc2),
     $           nel,istk(irc),ierr,it1,it2)
         else
            call dsosp(op,m1,n1,stk(l1),m2,n2,stk(l2),nel2,
     $           istk(irc2),nel,istk(irc),ierr)
         endif
      elseif(istk(il2).eq.1) then
         if(itr.eq.1) then
            call wspos(op,m1,n1,stk(l1),stk(l1+nel1),nel1,istk(irc1),
     $           m2,n2,stk(l2),stk(l2+nel2),
     $           nel,istk(irc),ierr,it1,it2)
         else
            call dspos(op,m1,n1,stk(l1),nel1,istk(irc1),
     $           m2,n2,stk(l2),nel,istk(irc),ierr)
         endif
      else
         if(itr.eq.1) then
            call wsposp(op,m1,n1,stk(l1),stk(l1+nel1),nel1,
     $           istk(irc1),m2,n2,stk(l2),stk(l2+nel2),nel2,istk(irc2),
     $           nel,istk(irc),ierr,it1,it2)
         else
            call dsposp(op,m1,n1,stk(l1),nel1,istk(irc1),
     $           m2,n2,stk(l2),nel2,istk(irc2),
     $           nel,istk(irc),ierr)
         endif
      endif
      if(ierr.ne.0) then
         buf='not enough memory'
         call error(9999)
         return
      endif
      istk(il1)=6
      istk(il1+1)=mr
      istk(il1+2)=nr

      istk(il1+3)=0
      istk(il1+4)=nel
      irc1=il1+5
      call icopy(mr+nel,istk(irc),1,istk(irc1),1)
      l1=sadr(irc1+mr+nel)
      lstk(top+1)=l1
      go to 999
c     
c     kronecker
 200   continue
      top=top0
      fin=-fin
      return

      
c     
      
 999  return
      end
