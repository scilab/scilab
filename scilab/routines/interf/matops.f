      subroutine matops
c     
c     operations matricielles
c     
c     Copyright INRIA
      include '../stack.h'
      integer op
c     
      integer star,dstar,slash,bslash,dot,colon,quote
      integer less,great,equal,et,ou,non
c
      data star/47/,dstar/62/,slash/48/
      data bslash/49/,dot/51/,colon/44/,quote/53/
      data less/59/,great/60/,equal/50/
      data ou/57/,et/58/,non/61/
c     
      op=fin
c     
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' matops op: '//buf(1:4))
      endif
c     
c     operations binaires et ternaires
c     --------------------------------
c     
      fun = 0
c     
c        cconc  extrac insert rconc
      goto(75  ,  95  ,  78   ,76) op
c     
c           :  +  -  * /  \  =          '
      goto(50,07,08,10,20,25,130,06,06,70) op+1-colon
c     
 06   if(op.eq.dstar) goto 31
      if(op.eq.quote+dot) goto 71
      if(op.eq.dstar+dot) goto 30
      if(op.ge.3*dot+star) goto 65
      if(op.ge.2*dot+star) goto 120
      if(op.ge.less+equal) goto 130
      if(op.eq.dot+star) goto 51
      if(op.eq.dot+slash) goto 52
      if(op.eq.dot+bslash) goto 53


      if(op.eq.et.or.op.eq.ou.or.op.eq.non) goto 140
      if(op.ge.less) goto 130
      fin=-fin
      return
    
c     
c     addition
 07   continue
      call matadd
      go to 999
c     
c     substraction
 08   if(rhs.eq.1) then
c     .  unary minus
         call matchsgn
      else
         call matsubst
      endif
      go to 999
c     
c     multiplication
 10   continue
      call matmult
      go to 999
c     
c     division a droite
 20   continue
      call matrdiv
      go to 999
c     
c     \
 25   continue
      call matldiv
      go to 999
c     
c     .^
 30   continue
      call matxpow
      goto 999
c     
c     ^
 31   continue
      call matpow
      go to 999
c     
c     :
 50   continue
      call vecimpl
      go to 999
c     
c     .*
 51   continue
      call vecmul
      go to 999
c     
c     ./
 52   continue
      call vecrdiv
      go to 999
c     
c     .\
 53   continue
      call vecldiv
      go to 999

c     .*. ./. .\.
c     kronecker
 65   fin = op - 3*dot - star + 19
      fun = 6
      rhs = 2
      go to 999
c     
c     '
 70   continue
      call mattrc
      goto 999
c     
c     .'
 71   continue
      call mattr
      goto 999
c     
c     concatenation [a b]
 75   continue
      call matrc
      goto 999
c     
c     concatenation [a;b]
 76   continue
      call matcc
      goto 999
c     
c     extraction a(i) and a(i,j)
c     
 78   continue
      if (rhs.eq.2) then
         call matext1
      elseif (rhs.eq.3) then
         call matext2
      else
         fin=-fin
      endif
      go to 999
c      
c     insertion
c     
 95   continue
      if (rhs.eq.3) then
         call matins1
      elseif (rhs.eq.4) then
         call matins2
      else
         fin=-fin
      endif
      goto 999

c     
c     *. /. \.
 120  fin=-fin
      goto 999
c     
c     == <= >= ~=
 130  continue
      call matcmp
      goto 999
c
c     & | ~
 140  continue
      call matlog
      goto 999
      
 999  return
      end

      subroutine matadd
c     
c     matrix addition
c     
c     Copyright INRIA
      include '../stack.h'
      common /mtlbc/ mmode
c     
      double precision cstr,csti,sr,si
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
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
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=sadr(il1+4)
      mn1=m1*n1
c
      itr=max(it1,it2)
c
      if (mn1.eq.0) then
         if (mmode.eq.1) then
c     .     Matlab like []+a=[]
         else
c     .     []+a=a
            call icopy(4,istk(il2),1,istk(il1),1)
            call unsfdcopy(mn2*(it2+1),stk(l2),1,stk(l1),1)
            lstk(top+1)=l1+mn2*(it2+1)
         endif
      elseif (mn2.eq.0) then
         if (mmode.eq.1) then
c     .     Matlab like a+[]=[]
            istk(il1+1)=0
            istk(il1+2)=0
            istk(il1+3)=0
            lstk(top+1)=sadr(il1+4)
         else
c     .     a+[]=a
         endif
      elseif (m1 .lt. 0) then
c     .  eye+vector
         go to 40
      elseif (m2 .lt. 0) then
c     .  vector+eye
         go to 41
      elseif (mn2.eq.1) then
c     .   vector+const
         err=l1+mn1*(itr+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call dadd(mn1,stk(l2),0,stk(l1),1)
         if(it2+2*it1.eq.1) call unsfdcopy(mn1,stk(l2+mn2),0,
     $        stk(l1+mn1),1)
         if(it1*it2.eq.1) call dadd(mn1,stk(l2+mn2),0,stk(l1+mn1),1)
         lstk(top+1)=l1+mn1*(itr+1)
         istk(il1+3)=itr
      elseif (mn1.eq.1) then
c     .  cst+vector
         err=l1+mn2*(itr+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         cstr=stk(l1)
         csti=stk(l1+1)
         call unsfdcopy((it2+1)*mn2,stk(l2),1,stk(l1),1)
         if(it1.eq.1.and.it2.eq.0) call dset(mn2,0.d0,stk(l1+mn2),1)
         call dadd(mn2,cstr,0,stk(l1),1)
         if(it1.eq.1) call dadd(mn2,csti,0,stk(l1+mn2),1)
         lstk(top+1)=l1+mn2*(itr+1)
         istk(il1+1)=m2
         istk(il1+2)=n2
         istk(il1+3)=itr
      else
c     .  vector+vector
         if (m1 .ne. m2.or.n1 .ne. n2) then
            call error(8)
            return
         endif
         err=l1+mn1*(itr+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call dadd(mn1,stk(l2),1,stk(l1),1)
         if(it2+2*it1.eq.1) then
            call unsfdcopy(mn1,stk(l2+mn1),1,stk(l1+mn1),1)
         endif
         if(it1*it2.eq.1) then
            call dadd(mn1,stk(l2+mn1),1,stk(l1+mn1),1)
         endif
         lstk(top+1)=l1+mn1*(itr+1)
         istk(il1+3)=itr
      endif
      return
c     addition et soustraction d'un scalaire fois l'identite
 40   sr=stk(l1)
      si=0.0d+0
      if(it1.eq.1) si=stk(l1+1)
      call unsfdcopy(mn2*(it2+1),stk(l2),1,stk(l1),1)
      m1=m2
      n1=n2
      m2=it2
      it2=it1
      it1=m2
      mn1=mn2
      goto 46
c     
 41   sr=stk(l2)
      si=0.0d0
      if(it2.eq.1) si = stk(l2+1)
      goto 46
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
      if(itr.eq.1.and.it1.eq.0) call dset(mn1,0.0d+0,stk(l1+mn1),1)
      m1=abs(m1)
      n1=abs(n1)
      do 47 i = 1, min(n1,m1)
         ll = l1 + (i-1)*(m1+1)
         stk(ll) = stk(ll)+sr
         if(itr.ne.0) stk(ll+mn1) = stk(ll+mn1)+si
 47   continue
      return 
      end

      subroutine matchsgn
c     
c     matrix change sign
c     
c     Copyright INRIA
      include '../stack.h'

      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      il1=iadr(lstk(top))
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=sadr(il1+4)
      mn1=m1*n1
c
c     .  unary minus
      if(mn1.gt.0) then
         call dscal(mn1*(it1+1),-1.0d+0,stk(l1),1)
      endif
      return
      end

      subroutine matsubst
c     
c     matrix substraction
c     
c     Copyright INRIA
      include '../stack.h'
      common /mtlbc/ mmode
c     
      double precision cstr,csti,sr,si
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
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
c
      itr=max(it1,it2)
c
      if (mn1.eq.0) then
         if (mmode.eq.1) then
c     .     Matlab like []-a=[]
         else
c     .     []-a=-a
            call icopy(4,istk(il2),1,istk(il1),1)
            call unsfdcopy(mn2*(it2+1),stk(l2),1,stk(l1),1)
            call dscal(mn2*(it2+1),-1.0d0,stk(l1),1)
            lstk(top+1)=l1+mn2*(it2+1)
         endif
      elseif(mn2.eq.0) then
         if (mmode.eq.1) then
c     .     Matlab like a-[]=[]
            istk(il1+1)=0
            istk(il1+2)=0
            istk(il1+3)=0
            lstk(top+1)=sadr(il1+4)
         else
c     .     a-[]=a
         endif
      elseif (m1 .lt. 0) then
c     .  a*eye-vector
         go to 42
      elseif (m2 .lt. 0) then
c     .  vector-a*eye
         go to 45
      elseif (mn2.eq.1) then
c     .   vector-const
         call dadd(mn1,-stk(l2),0,stk(l1),1)
         if(it2+2*it1.eq.1) call unsfdcopy(mn1,-stk(l2+mn2),0,
     $        stk(l1+mn1),1)
         if(it1*it2.eq.1) call dadd(mn1,-stk(l2+mn2),0,stk(l1+mn1),1)
         lstk(top+1)=l1+mn1*(itr+1)
         istk(il1+3)=itr
      elseif (mn1.eq.1) then
c     .  cst-vector
         cstr=stk(l1)
         csti=stk(l1+1)
         call dscal((it2+1)*mn2,-1.0d0,stk(l2),1)
         call unsfdcopy((it2+1)*mn2,stk(l2),1,stk(l1),1)
         if(it1.eq.1.and.it2.eq.0) call dset(mn2,0.d0,stk(l1+mn2),1)
         call dadd(mn2,cstr,0,stk(l1),1)
         if(it1.eq.1) call dadd(mn2,csti,0,stk(l1+mn2),1)
         lstk(top+1)=l1+mn2*(itr+1)
         istk(il1+1)=m2
         istk(il1+2)=n2
         istk(il1+3)=itr
      else
c     .  vector-vector
         if (m1 .ne. m2.or.n1 .ne. n2) then
            call error(9)
            return
         endif
         call ddif(mn1,stk(l2),1,stk(l1),1)
         if(itr.eq.0) return
         if(it1.eq.0) then
            call dscal (mn1,-1.0d+0,stk(l2+mn1),1)
            call unsfdcopy(mn1,stk(l2+mn1),1,stk(l1+mn1),1)
         endif
         if(it1*it2.eq.1) call ddif(mn1,stk(l2+mn1),1,stk(l1+mn1),1)
         lstk(top+1)=l1+mn1*(itr+1)
         istk(il1+3)=itr
      endif
      return

c     a*eye-b
 42   sr=stk(l1)
      si=0.0d+0
      if(it1.eq.1) si=stk(l1+1)
 43   call unsfdcopy(mn2*(it2+1),stk(l2),1,stk(l1),1)
      call dscal(mn2*(it2+1),-1.0d+0,stk(l1),1)
      mn1=mn2
      m1=m2
      n1=n2
      m2=it2
      it2=it1
      it1=m2
      goto 46
c     
c     a-eye*b
 45   sr=-stk(l2)
      si=0.0d+0
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
      if(itr.eq.1.and.it1.eq.0) call dset(mn1,0.0d+0,stk(l1+mn1),1)
      m1=abs(m1)
      n1=abs(n1)
      do 47 i = 1, min(n1,m1)
         ll = l1 + (i-1)*(m1+1)
         stk(ll) = stk(ll)+sr
         if(itr.ne.0) stk(ll+mn1) = stk(ll+mn1)+si
 47   continue
      return
      end


      subroutine matmult
c     
c     matrix/vector multiplications
c     
c     Copyright INRIA
      include '../stack.h'
c     
      double precision sr,si
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
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
c
      itr=max(it1,it2)
c

      if(mn1.eq.0.or.mn2.eq.0) then
c     .  []*a , a*[]
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=sadr(il1+4)
      elseif (mn1 .eq. 1) then
c     .  cst*a
         sr = stk(l1)
         si=0.0d+0
         if(it1.eq.1) si = stk(l1+1)
         if (m1.lt.0) then
            if(mn2.eq.1) then
c     .     eye*cst
               istk(il1+1)=m1
               istk(il1+2)=n1
               istk(il1+3)=itr
            else
               call error(14)
               return
            endif
         else
            istk(il1+1)=m2
            istk(il1+2)=n2
            istk(il1+3)=itr
         endif
         call unsfdcopy(mn2*(it2+1),stk(l2),1,stk(l1),1)
         it21=it2+2*it1
         if(it21.eq.0) then
c     .     le scalaire et la matrice sont reel
            call dscal(mn2,sr,stk(l1),1)
         elseif(it21.eq.1) then
c     .     la matrice est complexe le scalaire est reel
            call dscal(mn2,sr,stk(l1),1)
            call dscal(mn2,sr,stk(l1+mn2),1)
         elseif(it21.eq.2) then
c     .     la matrice est reelle, le scalaire est complexe
            lstk(top+1)=l1+mn2*(itr+1)
            err=lstk(top+1)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call unsfdcopy(mn2,stk(l1),1,stk(l1+mn2),1)
            call dscal(mn2,sr,stk(l1),1)
            call dscal(mn2,si,stk(l1+mn2),1)
         elseif(it21.eq.3) then
c     .     la matrice et le scalaire sont complexes
            call wscal(mn2,sr,si,stk(l1),stk(l1+mn2),1)
         endif
         lstk(top+1)=l1+mn2*(itr+1)
      elseif (mn2 .eq. 1) then
c     .  a*cst
         if(m2.lt.0) then
            call error(14)
            return
         endif
         it21=it2+2*it1
         if(it21.eq.0) then
c     .     la matrice et le scalaire sont reel
            call dscal(mn1,stk(l2),stk(l1),1)
         elseif(it21.eq.1) then
c     .     la matrice est reelle le scalaire est complexe
            sr = stk(l2)
            si = stk(l2+1)
            lstk(top+1)=l1+mn1*(itr+1)
            err=lstk(top+1)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call unsfdcopy(mn1,stk(l1),1,stk(l1+mn1),1)
            call dscal(mn1,si,stk(l1+mn1),1)
            call dscal(mn1,sr,stk(l1),1)
            istk(il1+3)=itr
         elseif(it21.eq.2) then
c     .     la matrice est complexe, le scalaire est reel
            sr = stk(l2)
            call dscal(mn1,sr,stk(l1),1)
            call dscal(mn1,sr,stk(l1+mn1),1)
         elseif(it21.eq.3) then
            sr = stk(l2)
            si = stk(l2+1)
c     .     la matrice et le scalaire sont complexes
            call wscal(mn1,sr,si,stk(l1),stk(l1+mn1),1)
         endif
      else
c     .  matrix*matrix
         if (n1 .ne. m2) then
            call error(10)
            return
         endif
         lr=l2+mn2*(it2+1)
         err=lr+m1*n2*(itr+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         if(it1*it2.ne.1) then
*           remplacement de dmmul par dgemm (Bruno le 31/10/2001)
            call dgemm('n','n',m1,n2,n1,1.d0,stk(l1),m1,stk(l2),m2,
     $           0.d0,stk(lr),m1)
            if(it1.eq.1) call dgemm('n','n',m1,n2,n1,1.d0,stk(l1+mn1),
     $           m1,stk(l2),m2,0.d0,stk(lr+m1*n2),m1)
            if(it2.eq.1) call dgemm('n','n',m1,n2,n1,1.d0,stk(l1),m1,
     $           stk(l2+mn2),m2,0.d0,stk(lr+m1*n2),m1)      
         else
c     .     a et a2 sont complexes
            call wmmul(stk(l1),stk(l1+mn1),m1,stk(l2),stk(l2
     $           +mn2),m2,stk(lr),stk(lr+m1*n2),m1,m1,n1,n2)
         endif
         call unsfdcopy(m1*n2*(itr+1),stk(lr),1,stk(l1),1)
         istk(il1+2)=n2
         istk(il1+3)=itr
         lstk(top+1)=l1+m1*n2*(itr+1)
      endif
      return
      end


      subroutine matrdiv
c     
c     matrix/vector right division
c     
c     Copyright INRIA
      include '../stack.h'
c     
      double precision sr,si
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
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
c
      itr=max(it1,it2)
c
      if (mn1.eq.0.or.mn2.eq.0) then
c     .  a/[] or []/a
         istk(il1)=1
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=sadr(il1+4)
         return
      endif
      if (mn2 .ne. 1) then
         if(m1.lt.0) then
            call error(14)
            return
         endif
         top = top+1
         rhs = 2        
         call intslash('slash')
         if (fin.ge.0) call putlhsvar()
c         if (m2 .eq. n2) fun = 1
c         if (m2 .ne. n2) fun = 4
c         fin = -1
      else
c     .  vector / cst
         if(m2.lt.0.and.mn1.ne.1) then 
            call error(14)
            return
         endif
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
         it21=it2+2*it1
         if(it21.eq.0) then
c     .     real / real
            call ddrdiv(stk(l1),1,stk(l2),0,stk(l1),1,mn1,ierr)
         elseif(it21.eq.1) then
c     .     real / complex
            sr=stk(l2)
            si=stk(l2+1)
            call dwrdiv(stk(l1),1,sr,si,0,stk(l1),stk(l1+mn1),1,
     $           mn1,ierr)
         elseif(it21.eq.2) then 
c     .     complex / real
            call wdrdiv(stk(l1),stk(l1+mn1),1,stk(l2),0,stk(l1)
     $           ,stk(l1+mn1),1,mn1,ierr)
         elseif(it21.eq.3) then  
c     .     complex / complex
            call wwrdiv(stk(l1),stk(l1+mn1),1,stk(l2),stk(l2+1)
     $           ,0,stk(l1),stk(l1+mn1),1,mn1,ierr)
         endif
         if(ierr.ne.0) then
            if(ieee.eq.0) then
               call error(27)
               return
            elseif(ieee.eq.1) then
               call msgs(63)
            endif
         endif
      endif
      return
      end

      subroutine matldiv
c     
c     matrix/vector left division
c     
c     Copyright INRIA
      include '../stack.h'
c     
      double precision sr,si
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
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
c
      itr=max(it1,it2)

      if (mn1.eq.0.or.mn2.eq.0) then
c     .  a\[] or []\a
         istk(il1)=1
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=sadr(il1+4)
         return
      endif
      if (m1*n1 .ne. 1) then
         if(m2.lt.0) then
            call error(14)
            return
         endif
         top = top+1
         rhs = 2 
         call intbackslash('backslash')
         if (fin.ge.0) call putlhsvar()
c         if (m1 .eq. n1) fun = 1
c         if (m1 .ne. n1) fun = 4
c         fin = -2
      else
c     .  cst \ vector
         if(m1.lt.0.and.mn2.ne.1) then 
            call error(14)
            return
         endif
         istk(il1+1)=m2
         istk(il1+2)=n2
         istk(il1+3)=itr
         lstk(top+1)=l1+mn2*(itr+1)
c     
         err=lstk(top+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif

         sr=stk(l1)
         it21=it2+2*it1
         if(it21.eq.0) then
c     .     real \ real
            call ddrdiv(stk(l2),1,sr,0,stk(l1),1,mn2,ierr)
         elseif(it21.eq.1) then
c     .     real \ complex = complex/real
            call wdrdiv(stk(l2),stk(l2+mn2),1,sr,0,stk(l2)
     $           ,stk(l2+mn2),1,mn2,ierr)
            call unsfdcopy(2*mn2,stk(l2),1,stk(l1),1)
         elseif(it21.eq.2) then 
c     .     complex \ real =real / complex
            si=stk(l1+1)
            call unsfdcopy(mn2,stk(l2),1,stk(l1),1)
            call dwrdiv(stk(l1),1,sr,si,0,stk(l1),stk(l1+mn2),1
     $           ,mn2,ierr)
         elseif(it21.eq.3) then  
c     .     complex \ complex
            si=stk(l1+1)
            call unsfdcopy(2*mn2,stk(l2),1,stk(l1),1)
            call wwrdiv(stk(l1),stk(l1+mn2),1,sr,si,0,stk(l1)
     $           ,stk(l1+mn2),1,mn2,ierr)
         endif
         if(ierr.ne.0) then
            if(ieee.eq.0) then
               call error(27)
               return
            elseif(ieee.eq.1) then
               call msgs(63)
            endif
         endif
      endif
      end

      subroutine matxpow
c     
c     matrix/vector entrywize power
c     
c     Copyright INRIA
      include '../stack.h'
c     
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      lw=lstk(top+1)+1

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
c

      if (mn1.eq.0) then
         return
      endif
      if (mn2.eq.0) then
         istk(il1)=1
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
            call ddpow1(mn,stk(l1),inc1,stk(l2),inc2,
     $           stk(lw),stk(lw+mn),1,ierr,itr)
         else
            call wdpow1(mn,stk(l1),stk(l1+mn1),inc1,stk(l2),inc2,
     $           stk(lw),stk(lw+mn),1,ierr)
         endif
      else
         if(it1.eq.0) then
            call dwpow1(mn,stk(l1),inc1,stk(l2),stk(l2+mn2),inc2,
     &           stk(lw),stk(lw+mn),1,ierr)
         else
            call wwpow1(mn,stk(l1),stk(l1+mn1),inc1,stk(l2),stk(l2+mn2),
     &           inc2,stk(lw),stk(lw+mn),1,ierr)
         endif
      endif
      if(ierr.eq.1) then
         call error(30)
         return
      endif
      if(ierr.eq.2) then
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
      call unsfdcopy(mn*(itr+1),stk(lw),1,stk(l1),1)
      lstk(top+1)=l1+mn*(itr+1)
      return
      end

      subroutine matpow
c     
c     matrix/vector entrywize power
c     
c     Copyright INRIA
      include '../stack.h'
c     
      double precision ddot,dasum,sr,si
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
c
      itr=max(it1,it2)
c
      if(mn1.eq.0) then
         return
      endif
      if(mn2.eq.0) then
         istk(ilrs)=1
         istk(ilrs+1)=0
         istk(ilrs+2)=0
         istk(ilrs+3)=0
         lstk(top+1)=sadr(ilrs+4)
         return
      endif
      if(mn1.eq.1) then
c     scalar^matrix treated as scalar.^matrix (see matxpow)
         err=lw+mn2*2-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         if(it2.eq.0) then
            if(it1.eq.0) then
               call ddpow1(mn2,stk(l1),0,stk(l2),1,
     $              stk(lw),stk(lw+mn2),1,err,itr)
            else
               call wdpow1(mn2,stk(l1),stk(l1+mn1),0,stk(l2),1,
     $              stk(lw),stk(lw+mn2),1,err)
            endif
         else
            if(it1.eq.0) then
               call dwpow1(mn2,stk(l1),0,stk(l2),stk(l2+mn2),1,
     &              stk(lw),stk(lw+mn2),1,err)
            else
               call wwpow1(mn2,stk(l1),stk(l1+mn1),0,stk(l2),stk(l2
     $              +mn2),1,stk(lw),stk(lw+mn2),1,err)
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
         istk(il1+1)=m2
         istk(il1+2)=n2
         istk(il1+3)=itr
         call unsfdcopy(mn2*(itr+1),stk(lw),1,stk(l1),1)
         lstk(top+1)=l1+mn2*(itr+1)
         return
      endif
      if(mn2.gt.1) goto 39
      if(m1.ne.n1) then
         if(mn2.eq.1.and.(m1.eq.1.or.n1.eq.1)) then
c     .     vect^scalar treated as  vect.^scalar (see matxpow)
            sr=stk(l2)
            si=stk(l2+1)
            if(it1.eq.0) then
               err=l1+mn1-lstk(bot)
               if(err.gt.0) then
                  call error(17)
                  return
               endif
            endif
            if(it2.eq.0) then
               if(it1.eq.0) then
                  call ddpow1(mn1,stk(l1),1,sr,0,stk(l1),stk(l1+mn1),1,
     $                 err,itr)
               else
                  call wdpow1(mn1,stk(l1),stk(l1+mn1),1,sr,0,
     $                 stk(l1),stk(l1+mn1),1,err)
               endif
            else
               if(it1.eq.0) then
                  call dwpow1(mn1,stk(l1),1,sr,si,0,
     &                 stk(l1),stk(l1+mn1),1,err)
               else
                  call wwpow1(mn1,stk(l1),stk(l1+mn1),1,sr,si,0,
     $                 stk(l1),stk(l1+mn1),1,err)
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
            istk(il1+3)=itr
            lstk(top+1)=l1+mn1*(itr+1)
            return
         endif
         err=1
         call error(20)
         return
      endif
      nexp = nint(stk(l2))
      if (it2 .ne. 0) go to 39
      if (stk(l2) .ne. dble(nexp)) go to 39
      if (nexp.eq.1) return
      if (nexp.eq.0) then
         lw=l1+mn1*(it1+1)
         ipvt=iadr(lw+m1*(it1+1))
         err=sadr(ipvt+m1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         if (dasum(m1*n1*(it1+1),stk(l1),1).eq.0.0d0) then
            call error(30)
            return
         endif
         call dset(mn1,0.0d+0,stk(l1),1)
         call dset(m1,1.0d+0,stk(l1),m1+1)
         istk(il1+3)=0
         lstk(top+1)=l1+mn1
         return
      endif
c     
      if (nexp.le.0) then
         rhs=1
         call intinv('pow')
         call putlhsvar()
c         call matlu
         if(err.gt.0.or.err1.gt.0) return
         nexp=-nexp
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
      call unsfdcopy(mn1*(itr+1),stk(l1),1,stk(l2),1)
      if(it1.eq.1) goto 35
c     la matrice est reelle
      do 34 kexp=2,nexp
         do 33 j=1,n1
            ls=l1+(j-1)*n1
            call unsfdcopy(n1,stk(ls),1,stk(l3),1)
            do 32 i=1,n1
               ls=l2+(i-1)
               ll=l1+(i-1)+(j-1)*n1
               stk(ll)=ddot(n1,stk(ls),n1,stk(l3),1)
 32         continue
 33      continue
 34   continue
      return
c     
 35   continue
c     la matrice est complexe
      do 38 kexp=2,nexp
         do 37 j=1,n1
            ls=l1+(j-1)*n1
            call unsfdcopy(n1,stk(ls),1,stk(l3),1)
            call unsfdcopy(n1,stk(ls+mn1),1,stk(l3+n1),1)
            do 36 i=1,n1
               ls=l2+(i-1)
               ll=l1+(i-1)+(j-1)*n1
               stk(ll)=ddot(n1,stk(ls),n1,stk(l3),1)-
     $              ddot(n1,stk(ls+mn1),n1,stk(l3+n1),1)
               stk(ll+mn1)=ddot(n1,stk(ls),n1,stk(l3+n1),1)+
     $              ddot(n1,stk(ls+mn1),n1,stk(l3),1)
 36         continue
 37      continue
 38   continue
      return
c     
c     puissance non entiere ou non positive
 39   fun = 6
      fin = 29
      rhs=2
      top=top+1
      return
      end
      
      subroutine vecimpl
c     
c     implicit vector
c     
c     Copyright INRIA
      include '../stack.h'
c     
      double precision e1,e2,e3,st,e1r
      double precision dlamch
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      if(rhs.eq.2) goto 02
      il3=iadr(lstk(top))
      if(istk(il3).lt.0) il3=iadr(istk(il3+1))
      m3=istk(il3+1)
      n3=istk(il3+2)
      it3=istk(il3+3)
      l3=sadr(il3+4)
      mn3=m3*n3
      top=top-1

 02   il2=iadr(lstk(top))
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
c
      if(mn1.ne.1) then
         err=1
         call cvname(ids(1,pt+1),''':''',0)
         call error(204)
         return
      endif
      e1 = stk(l1)
c
      if(mn2.ne.1) then
         err=2
         call cvname(ids(1,pt+1),''':''',0)
         call error(204)
         return
      endif
      e2 = stk(l2)
c
      if (rhs .eq. 3) then
         if(mn3.ne.1) then
            err=3
            call cvname(ids(1,pt+1),''':''',0)
            call error(204)
            return
         endif
         e2=stk(l3)
         st = stk(l2)

      else
         st = 1.0d+0
      endif
      if (st .eq. 0.0d+0) then
         istk(il1+1)=1
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=l1
         return
      endif

c     check for clause
      if (rstk(pt-1) .eq. 801.or.rstk(pt).eq.611) go to 54
      if(rstk(pt-1).eq.611.and.rstk(pt).eq.601) then
c     : in compiled for, next line to differentiate from extraction
         if(istk(pstk(pt)).eq.16) goto  54
      endif
c
c     floating point used to avoid integer overflow
      e1r=dble(l1) + max(3.0d0,(e2-e1)/st) - dble(lstk(bot))
      if (e1r .gt. 0.0d0) then
         err=e1r
         call error(17)
         return
      endif
c
      e1r=2.0d0*max(abs(e1),abs(e2))*dlamch('p')
      n = 0
      l=l1
 52   if (st*(stk(l)-e2).gt.0.0d+0) then
         if (abs(stk(l)-e2).lt.e1r) n=n+1
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
      return
c     
c     for clause
 54   stk(l1) = e1
      stk(l1+1) = st
      stk(l1+2) = e2
      istk(il1+1)=-3
      istk(il1+2)=-1
      lstk(top+1)=l1+3
      return
      end

      subroutine vecmul
c     
c     vector multiplication .*
c     
c     Copyright INRIA
      include '../stack.h'
c     
      double precision sr,si
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
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
c
      itr=max(it1,it2)
c
      i1=1
      i2=1
      if(mn1.eq.0.or.mn2.eq.0) then
c     [].*a     a.*[]  -->[]
         istk(il1)=1
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=sadr(il1+4)
         return
      endif
      if(n1.lt.0.and.mn2.ne.1.or.n2.lt.0.and.mn1.ne.1) then
         call error(14)
         return
      endif
      if(mn1.ne.1.and.mn2.ne.1) then
c       check dimensions
         if (m1.ne.m2 .or. n1.ne.n2) then
            buf='inconsistent element-wise operation'
            call error(9999)
            return
         endif
      endif
c
      lstk(top+1)=l1+max(mn1,mn2)*(itr+1)
      err=lstk(top+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      istk(il1+3)=itr


      if(mn1.eq.1) then
c     .  cst.*a
         sr = stk(l1)
         si=0.0d+0
         if(it1.eq.1) si = stk(l1+1)
         if (m1.lt.0) then
            if(mn2.eq.1) then
c     .     eye.*cst
               istk(il1+1)=m1
               istk(il1+2)=n1
               istk(il1+3)=itr
            else
               call error(14)
               return
            endif
         else
            istk(il1+1)=m2
            istk(il1+2)=n2
            istk(il1+3)=itr
         endif
         call unsfdcopy(mn2*(it2+1),stk(l2),1,stk(l1),1)
         it21=it2+2*it1
         if(it21.eq.0) then
c     .     vector and cst are real
            call dscal(mn2,sr,stk(l1),1)
         elseif(it21.eq.1) then
c     .     complex vector, real cst
            call dscal(mn2,sr,stk(l1),1)
            call dscal(mn2,sr,stk(l1+mn2),1)
         elseif(it21.eq.2) then
c     .     real vector, complex cst
            lstk(top+1)=l1+mn2*(itr+1)
            err=lstk(top+1)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call unsfdcopy(mn2,stk(l1),1,stk(l1+mn2),1)
            call dscal(mn2,sr,stk(l1),1)
            call dscal(mn2,si,stk(l1+mn2),1)
         elseif(it21.eq.3) then
c     .     lvector and cst are complex
            call wscal(mn2,sr,si,stk(l1),stk(l1+mn2),1)
         endif
         lstk(top+1)=l1+mn2*(itr+1)
      elseif (mn2 .eq. 1) then
c     .  a.*cst
         if(m2.lt.0) then
            call error(14)
            return
         endif
         it21=it2+2*it1
         if(it21.eq.0) then
c     .     vector and cst are real
            call dscal(mn1,stk(l2),stk(l1),1)
         elseif(it21.eq.1) then
c     .     real vector, complex cst
            sr = stk(l2)
            si = stk(l2+1)
            lstk(top+1)=l1+mn1*(itr+1)
            err=lstk(top+1)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call unsfdcopy(mn1,stk(l1),1,stk(l1+mn1),1)
            call dscal(mn1,si,stk(l1+mn1),1)
            call dscal(mn1,sr,stk(l1),1)
            istk(il1+3)=itr
         elseif(it21.eq.2) then
c     .     complex vector, real cst
            sr = stk(l2)
            call dscal(mn1,sr,stk(l1),1)
            call dscal(mn1,sr,stk(l1+mn1),1)
         elseif(it21.eq.3) then
            sr = stk(l2)
            si = stk(l2+1)
c     .     vector and cst are complex
            call wscal(mn1,sr,si,stk(l1),stk(l1+mn1),1)
         endif
      else
c     .  vector.*vector
         if(it1*it2.ne.1) then
            if(it1.eq.1) call dvmul(mn1,stk(l2),i2,stk(l1
     $           +mn1),i1)
            if(it2.eq.1) call dvmul(mn1,stk(l1),i1,stk(l2
     $           +mn2),i2)
            call dvmul(mn1,stk(l2),i2,stk(l1),i1)
            if(it2.eq.1) call unsfdcopy(mn1,stk(l2+mn2),i2
     $           ,stk(l1+mn1),i1)
         else
            call wvmul(mn1,stk(l2),stk(l2+mn2),i2,stk(l1)
     $           ,stk(l1+mn1),i1)
         endif
      endif
      return
      end

      subroutine vecrdiv
c     
c     ./
c     
c     Copyright INRIA
      include '../stack.h'
c     
      double precision sr,si
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
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
c
      itr=max(it1,it2)

      i1=1
      i2=1
      if(mn1.eq.0.or.mn2.eq.0) then
c     [].*a     a.*[]  -->[]
         istk(il1)=1
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=sadr(il1+4)
         return
      endif
      if(n1.lt.0.and.mn2.ne.1.or.n2.lt.0.and.mn1.ne.1) then
         call error(14)
         return
      endif
      if(mn1.ne.1.and.mn2.ne.1) then
c       check dimensions
         if (m1.ne.m2 .or. n1.ne.n2) then
            buf='inconsistent element-wise operation'
            call error(9999)
            return
         endif
      endif
c
      lstk(top+1)=l1+max(mn1,mn2)*(itr+1)
      err=lstk(top+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      istk(il1+3)=itr

      it21=it2+2*it1
      if(mn2.eq.1) then
c     .  vector ./ cst
         if(m2.lt.0.and.mn1.ne.1) then 
            call error(14)
            return
         endif
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
         it21=it2+2*it1
         if(it21.eq.0) then
c     .     real / real
            call ddrdiv(stk(l1),1,stk(l2),0,stk(l1),1,mn1,ierr)
         elseif(it21.eq.1) then
c     .     real / complex
            sr=stk(l2)
            si=stk(l2+1)
            call dwrdiv(stk(l1),1,sr,si,0,stk(l1),stk(l1+mn1),1,
     $           mn1,ierr)
         elseif(it21.eq.2) then 
c     .     complex / real
            call wdrdiv(stk(l1),stk(l1+mn1),1,stk(l2),0,stk(l1)
     $           ,stk(l1+mn1),1,mn1,ierr)
         elseif(it21.eq.3) then  
c     .     complex / complex
            call wwrdiv(stk(l1),stk(l1+mn1),1,stk(l2),stk(l2+1)
     $           ,0,stk(l1),stk(l1+mn1),1,mn1,ierr)
         endif
      elseif(mn1.eq.1) then
c     .  cst ./ vector
         istk(il1+1)=m2
         istk(il1+2)=n2
         sr=stk(l1)
         if(it21.eq.0) then
c     .     real ./ real
            call ddrdiv(sr,0,stk(l2),1,stk(l1),1,mn2,ierr)
         elseif(it21.eq.1) then
c     .     real ./ complex
            call unsfdcopy(2*mn2,stk(l2),1, stk(l1),1)
            call dwrdiv(sr,0,stk(l1),stk(l1+mn2),1,stk(l1)
     $           ,stk(l1+mn2),1,mn2,ierr)
         elseif(it21.eq.2) then 
c     .     complex ./ real
            si=stk(l1+mn1)
            call unsfdcopy(mn2,stk(l2),1, stk(l1),1)
            call wdrdiv(sr,si,0,stk(l1),1,stk(l1),stk(l1+mn2)
     $           ,1,mn2,ierr)
         elseif(it21.eq.3) then 
c     .     complex ./ complex
            si=stk(l1+mn1)
            call unsfdcopy(2*mn2,stk(l2),1, stk(l1),1)
            call wwrdiv(sr,si,0,stk(l1),stk(l1+mn2),1,stk(l1)
     $           ,stk(l1+mn2),1,mn2,ierr)
         endif
      else
c     .  vector ./ vector
         if(it21.eq.0) then
c     .     real ./ real
            call ddrdiv(stk(l1),1,stk(l2),1,stk(l1),1,mn2
     $           ,ierr)
         elseif(it21.eq.1) then
c     .     real ./ complex
            call dwrdiv(stk(l1),1,stk(l2),stk(l2+mn1),1
     $           ,stk(l1),stk(l2),1,mn2,ierr)
            call unsfdcopy(mn2,stk(l2),1,stk(l1+mn2),1)
         elseif(it21.eq.2) then 
c     .     complex ./ real
            call wdrdiv(stk(l1),stk(l1+mn1),1,stk(l2),1
     $           ,stk(l1),stk(l1+mn2),1,mn2,ierr)
         elseif(it21.eq.3) then 
c     .     complex ./ complex
            call wwrdiv(stk(l1),stk(l1+mn1),1,stk(l2)
     $           ,stk(l2+mn2),1,stk(l1),stk(l1+mn2),1,mn2,ierr)
         endif
      endif
      if(ierr.ne.0) then
         if(ieee.eq.0) then
            call error(27)
            return
         elseif(ieee.eq.1) then
            call msgs(63)
         endif
      endif

      return
      end
      
      subroutine vecldiv
c     
c      .\
c     
c     Copyright INRIA
      include '../stack.h'
c     
      double precision sr,si
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
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
c
      itr=max(it1,it2)

      i1=1
      i2=1

      if(mn1.eq.0.or.mn2.eq.0) then
c     [].*a     a.*[]  -->[]
         istk(il1)=1
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         lstk(top+1)=sadr(il1+4)
         return
      endif
      if(n1.lt.0.and.mn2.ne.1.or.n2.lt.0.and.mn1.ne.1) then
         call error(14)
         return
      endif
      if(mn1.ne.1.and.mn2.ne.1) then
c       check dimensions
         if (m1.ne.m2 .or. n1.ne.n2) then
            buf='inconsistent element-wise operation'
            call error(9999)
            return
         endif
      endif
c
      lstk(top+1)=l1+max(mn1,mn2)*(itr+1)
      err=lstk(top+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      istk(il1+3)=itr

      it21=it2+2*it1
      if(mn1.eq.1) then
c     .  cst .\ vector
         if(m1.lt.0.and.mn2.ne.1) then 
            call error(14)
            return
         endif
         istk(il1+1)=m2
         istk(il1+2)=n2
         istk(il1+3)=itr
         lstk(top+1)=l1+mn2*(itr+1)
c     
         err=lstk(top+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif

         sr=stk(l1)
         it21=it2+2*it1
         if(it21.eq.0) then
c     .     real \ real
            call ddrdiv(stk(l2),1,sr,0,stk(l1),1,mn2,ierr)
         elseif(it21.eq.1) then
c     .     real \ complex = complex/real
            call wdrdiv(stk(l2),stk(l2+mn2),1,sr,0,stk(l2)
     $           ,stk(l2+mn2),1,mn2,ierr)
            call unsfdcopy(2*mn2,stk(l2),1,stk(l1),1)
         elseif(it21.eq.2) then 
c     .     complex \ real =real / complex
            si=stk(l1+1)
            call unsfdcopy(mn2,stk(l2),1,stk(l1),1)
            call dwrdiv(stk(l1),1,sr,si,0,stk(l1),stk(l1+mn2),1
     $           ,mn2,ierr)
         elseif(it21.eq.3) then  
c     .     complex \ complex
            si=stk(l1+1)
            call unsfdcopy(2*mn2,stk(l2),1,stk(l1),1)
            call wwrdiv(stk(l1),stk(l1+mn2),1,sr,si,0,stk(l1)
     $           ,stk(l1+mn2),1,mn2,ierr)
         endif
      elseif(mn2.eq.1) then
c     .  vector .\ cst
         sr=stk(l2)
         if(it21.eq.0) then
c     .     real .\ real
            call ddrdiv(sr,0,stk(l1),1,stk(l1),1,mn1,ierr)
         elseif(it21.eq.2) then
c     .     complex .\ real
            call dwrdiv(sr,0,stk(l1),stk(l1+mn1),1,stk(l1)
     $           ,stk(l1+mn1),1,mn1,ierr)
         elseif(it21.eq.1) then 
c     .     real .\ complex
            si=stk(l2+mn2)
            call wdrdiv(sr,si,0,stk(l1),1,stk(l1),stk(l1+mn1),1
     $           ,mn1,ierr)
         elseif(it21.eq.3) then 
c     .     complex .\ complex
            si=stk(l2+mn2)
            call wwrdiv(sr,si,0,stk(l1),stk(l1+mn1),1,stk(l1)
     $           ,stk(l1+mn1),1,mn1,ierr)
         endif
      else
c     .  vector .\ vector
         if(it21.eq.0) then
c     .     real .\ real
            call ddrdiv(stk(l2),1,stk(l1),1,stk(l1),1,mn1
     $           ,ierr)
         elseif(it21.eq.2) then
c     .     complex .\ real
            call dwrdiv(stk(l2),1,stk(l1),stk(l1+mn1),1
     $           ,stk(l1),stk(l1+mn1),1,mn1,ierr)
         elseif(it21.eq.1) then 
c     .     real .\ complex = complex /. real
            call wdrdiv(stk(l2),stk(l2+mn2),1,stk(l1),1
     $           ,stk(l1),stk(l2),1,mn1,ierr)
            call unsfdcopy(mn1,stk(l2),1,stk(l1+mn1),1)
         elseif(it21.eq.3) then 
c     .     complex .\ complex
            call wwrdiv(stk(l2),stk(l2+mn2),1,stk(l1)
     $           ,stk(l1+mn1),1,stk(l1),stk(l1+mn1),1,mn1,ierr)
         endif
      endif
      if(ierr.ne.0) then
         if(ieee.eq.0) then
            call error(27)
            return
         elseif(ieee.eq.1) then
            call msgs(63)
         endif
      endif

      return
      end

      subroutine mattrc
c     
c      '
c     
c     Copyright INRIA
      include '../stack.h'
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
      it1=istk(il1+3)
      l1=sadr(il1+4)
      mn1=m1*n1
c
      if(mn1 .eq. 0.or.istk(il1).eq.0) then
         return
      elseif(abs(m1).eq.1.or.abs(n1).eq.1) then
         if(it1.eq.1) then
            call dscal(mn1,-1.0d0,stk(l1+mn1),1)
         endif
      else
         vol=mn1*(it1+1)
         ll = l1+vol
         err = ll+vol - lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
         call unsfdcopy(vol,stk(l1),1,stk(ll),1)
         call mtran(stk(ll),m1,stk(l1),n1,m1,n1)
         if(it1.eq.1) then
            call mtran(stk(ll+mn1),m1,stk(l1+mn1),n1,m1,n1)
            call dscal(mn1,-1.0d+0,stk(l1+mn1),1)
         endif
      endif
      istk(il1+1)=n1
      istk(il1+2)=m1
      return
      end
      subroutine mattr
c     
c      .'
c     
c     Copyright INRIA
      include '../stack.h'
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
      it1=istk(il1+3)
      l1=sadr(il1+4)
      mn1=m1*n1
c
      if(mn1 .eq. 0.or.istk(il1).eq.0) then
         return
      elseif(abs(m1).eq.1.or.abs(n1).eq.1) then
         goto 10
      else
         vol=mn1*(it1+1)
         ll = l1+vol
         err = ll+vol - lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
         call unsfdcopy(vol,stk(l1),1,stk(ll),1)
         call mtran(stk(ll),m1,stk(l1),n1,m1,n1)
         if(it1.eq.1) then
            call mtran(stk(ll+mn1),m1,stk(l1+mn1),n1,m1,n1)
         endif
      endif
 10   istk(il1+1)=n1
      istk(il1+2)=m1
      return
      end

      subroutine matrc
c     
c     [a b]
c     
c     Copyright INRIA
      include '../stack.h'
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

      itr=max(it1,it2)

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
c     
      if(itr.eq.0) then
         call unsfdcopy(mn2,stk(l2),1,stk(l1+mn1),1)
      else
         lw=l1+(itr+1)*(mn1+mn2)
         if(lw.gt.l2) then
            err=lw+mn2*(it2+1)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call unsfdcopy(mn2*(it2+1),stk(l2),-1,stk(lw),-1)
            l2=lw
         endif
         if(it1.eq.1) call unsfdcopy(mn1,stk(l1+mn1),-1,stk(l1
     $        +mn1+mn2),-1)
         call unsfdcopy(mn2,stk(l2),1,stk(l1+mn1),1)
         if(it1.eq.0) then
            call dset(mn1,0.0d+0,stk(l1+mn1+mn2),1)
            call unsfdcopy(mn2,stk(l2+mn2),1,stk(l1+2*mn1+mn2),1)
         endif
         if(it2.eq.0) then
            call dset(mn2,0.0d+0,stk(l1+2*mn1+mn2),1)
         else
            call unsfdcopy(mn2,stk(l2+mn2),1,stk(l1+2*mn1+mn2),1)
         endif
      endif
      n=n1+n2
      istk(il1+1)=m1
      istk(il1+2)=n
      istk(il1+3)=itr
      lstk(top+1)=sadr(il1+4)+m1*n*(itr+1)
      return
      end

      subroutine matcc
c     
c     [a;b]
c     
c     Copyright INRIA
      include '../stack.h'
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
c
      itr=max(it1,it2)
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
      m=m1+m2
      mn=m*n1
      if(n1.eq.1.and.itr.eq.0) then
         call unsfdcopy(mn2,stk(l2),1,stk(l1+mn1),1)
         istk(il1+1)=m
         istk(il1+3)=itr
         lstk(top+1)=l1+mn*(itr+1)
         return
      endif
c      lw1=l1+(itr+1)*mn
      lw1=max(lw,l1+(itr+1)*mn)
      lw2=lw1+mn1*(it1+1)
      err=lw2+mn2*(it2+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call unsfdcopy(mn2*(it2+1),stk(l2),1,stk(lw2),1)
      call unsfdcopy(mn1*(it1+1),stk(l1),1,stk(lw1),1)
c     
      if(itr.eq.1) call dset(mn,0.0d+0,stk(l1+(mn1+mn2)),1)
      call dmcopy(stk(lw1),m1,stk(l1),m,m1,n1)
      if(it1.eq.1) call dmcopy(stk(lw1+mn1),m1,stk(l1+mn),m
     $     ,m1,n1)
      call dmcopy(stk(lw2),m2,stk(l1+m1),m,m2,n1)
      if(it2.eq.1) call dmcopy(stk(lw2+mn2),m2,stk(l1+mn+m1)
     $     ,m,m2,n1)
      istk(il1+1)=m
      istk(il1+2)=n1
      istk(il1+3)=itr
      lstk(top+1)=sadr(il1+4)+mn*(itr+1)
      return
      end

      subroutine matext1
c     
c     B=A(i)
c     
c     Copyright INRIA
      include '../stack.h'
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

c     arg2(arg1)
      if (istk(il1).eq.0) then
         call error(220)
         return
      endif
      if(istk(il2).eq.129) then
c     implied polynomials vector extraction
         top=top+1
         call polops
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
         istk(il1)=1
         istk(il1+1)=mn2
         istk(il1+2)=1
         istk(il1+3)=istk(il2+3)
         l1=sadr(il1+4)
         call unsfdcopy(mn2*(it2+1),stk(l2),1,stk(l1),1)
         lstk(top+1)=l1+mn2*(it2+1)
         return
      endif
c     check and convert indices variable
      call indxg(il1,mn2,ilr,mi,mx,lw,1)
      if(err.gt.0) return
      if(mx.gt.mn2) then
         call error(21)
         return
      endif
 79   if(mi.eq.0) then
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
      if(sadr(ilr-1).le.l1+(it2+1)*mi) then
         lr=lw
         lw=lr+(it2+1)*mi
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
         if(it2.eq.1) stk(lr+mi+i) = stk(l2+mn2+ind)
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
      istk(il1)=1
      istk(il1+1)=m
      istk(il1+2)=n
      istk(il1+3)=it2
      if(lr.ne.l1) call unsfdcopy(mi*(it2+1),stk(lr),1,stk(l1),1)
      lstk(top+1)=l1+mi*(it2+1)
      return
      end

      subroutine matext2
c     
c     B=A(i,j)
c     
c     Copyright INRIA
      include '../stack.h'
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
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=sadr(il1+4)
      mn1=m1*n1
c     
c     arg3(arg1,arg2)
 82   if(rhs.gt.3) then
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
 90   mn=mi*nj
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
      istk(il1)=1
      istk(il1+1)=mi
      istk(il1+2)=nj
      istk(il1+3)=it3
      if(lr.ne.l1) call unsfdcopy(mn*(it3+1),stk(lr),1,stk(l1),1)
      lstk(top+1)=l1+mn*(it3+1)
      return
      end

      subroutine matins1
c     
c     A(i)=B
c     
c     Copyright INRIA
      include '../stack.h'
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
            call unsfdcopy(mn3*(it3+1),stk(l3),1,stk(l),1)
            lstk(top+1)=l+mn3*(it3+1)
            return
         else
c     .     arg3(arg1)=[]
            if(istk(il1).eq.4.and.m3.eq.m1.and.n3.eq.n1) then
               if(.not.isany(il1)) then
c     .           arg3([])=[]  --> arg3
                  call icopy(4,istk(il3),1,istk(ilrs),1)
                  l=sadr(ilrs+4)
                  call unsfdcopy(mn3*(it3+1),stk(l3),1,stk(l),1)
                  lstk(top+1)=l+mn3*(it3+1)
                  return
               endif
            endif
c     .     arg3(arg1)=[] -->arg3(compl(arg1))
 97         call indxgc(il1,mn3,ilr,mi,mx,lw)
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
            istk(ilrs)=1
            istk(ilrs+1)=m3
            istk(ilrs+2)=n3
            istk(ilrs+3)=it2
            l1=sadr(ilrs+4)
            call unsfdcopy((it2+1)*mn2,stk(l2),1,stk(l1),1)
            lstk(top+1)=l1+mn2*(it2+1)
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
      endif
      call indxg(il1,mn3,ili,mi,mxi,lw,1)
      if(err.gt.0) return
      if(mi.eq.0) then
c     .  arg3([])=arg2
         if(mn2.eq.1) then
c     .  arg3([])=c  --> arg3
            call icopy(4,istk(il3),1,istk(ilrs),1)
            l=sadr(ilrs+4)
            call unsfdcopy(mn3*(it3+1),stk(l3),1,stk(l),1)
            lstk(top+1)=l+mn3*(it3+1)
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
c      elseif (n3.le.1.and.m3.le.1) then
c         if(n2.le.1) then
c            mr=max(m3,mxi)
c            nr=max(n3,1)
c         else
c            nr=max(n3,mxi)
c            mr=max(m3,1)
c         endif
c      elseif (n3.le.1) then
c     .  arg3 and arg2 are  column vectors
c         mr=max(m3,mxi)
c         nr=max(n3,1)
c      elseif (m3.le.1) then
c     .  row vectors
c         nr=max(n3,mxi)
c         mr=max(m3,1)
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
      itr=max(it2,it3)
      if(mnr*(itr+1).ne.mn3*(it3+1) ) then
c     .  resulting matrix is bigger than original
         lr=lw
         lw=lr + mnr*(itr+1)
         err = lw - lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
c     .  initialise result r to 0
         call dset(mnr*(itr+1),0.0d+0,stk(lr),1)
c     .  write arg3 in r
         if(mn3.ge.1) then
            call dmcopy(stk(l3),m3,stk(lr),mr,m3,n3)
            if(it3.eq.1) then
               call dmcopy(stk(l3+mn3),m3,stk(lr+mnr),mr,m3,n3)
            endif
         endif
      endif
c     write arg2 in r
      do 98 i = 0, mi-1
         ll = lr+istk(ili+i) - 1
         ls = l2+i*inc2
         stk(ll) = stk(ls)
         if(it2.eq.1) then
            stk(ll+mnr)=stk(ls+mn2)
         elseif(itr.eq.1) then
            stk(ll+mnr)=0.0d0
         endif
 98   continue
c     
      if(lr.ne.l3) then
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
c     inline extraction procedure copied from matext1
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
      if(sadr(ilr-1).le.l1+(it2+1)*mi) then
         lr=lw
         lw=lr+(it2+1)*mi
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
         if(it2.eq.1) stk(lr+mi+i) = stk(l2+mn2+ind)
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
      istk(ilrs)=1
      istk(ilrs+1)=m
      istk(ilrs+2)=n
      istk(ilrs+3)=it2
      if(lr.ne.l1) call unsfdcopy(mi*(it2+1),stk(lr),1,stk(l1),1)
      lstk(top+1)=l1+mi*(it2+1)
      return

      end

      subroutine matins2
c     
c     A(i,j)=B
c     
c     Copyright INRIA
      include '../stack.h'
c     
      integer top0
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      top0=top
      lw=lstk(top+1)+1

 01   il4=iadr(lstk(top))
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
      if(lr.ne.l4) then
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

      subroutine matcmp
c     
c     == <= >= <>
c     
c     Copyright INRIA
      include '../stack.h'
c     
      integer top0,op
      double precision  e1,e2,e1r,e2r,e1i,e2i
      integer less,great,equal

      integer isanan
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

      if(fin.eq.61) then
         fin=-fin
         top=top+1
         return
      endif
c     comparaisons
      if(m1.eq.-1) then
c     .  eye op b
         err=lw+mn2*(it1+1)-lstk(bot)
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
         err=lw+mn1*(it2+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call dset(mn1,0.0d0,stk(lw),1)
         call dset(min(m1,n1),stk(l2),stk(lw),m1+1)
         if(it1.eq.1) then
             call dset(mn1,0.0d0,stk(lw+mn1),1)
            call dset(min(m1,n1),stk(l2+1),stk(lw+mn1),m1+1)
         endif
         l2=lw
         mn2=mn1
         m2=m1
         n2=n1
      elseif(mn1.eq.1.and.mn2.gt.1) then
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
      endif
      if(mn2.eq.0.or.mn1.eq.0) then
         if(op.eq.equal.or.op.eq.less+great) then
            itrue=0
            if(mn2.eq.0.and.mn1.eq.0) itrue=1
            if(op.eq.less+great) itrue=1-itrue
            istk(il1)=4
            istk(il1+1)=1
            istk(il1+2)=1
            istk(il1+3)=itrue
            lstk(top+1)=sadr(il1+4)
            return
         else
            if(mn1.eq.1.or.mn2.eq.1) then
               istk(il1)=1
               istk(il1+1)=0
               istk(il1+2)=0
               istk(il1+3)=0
               lstk(top+1)=sadr(il1+4)
               return
            else
               call error(60)
               return
            endif
         endif
      endif
      if(n1.ne.n2.or.m1.ne.m2) then
         if(op.eq.equal.or.op.eq.less+great) then
            istk(il1)=4
            istk(il1+1)=1
            istk(il1+2)=1
            istk(il1+3)=0
            if(op.eq.less+great) istk(il1+3)=1
            lstk(top+1)=sadr(il1+4)
         else
            call error(60)
            return
         endif
      else if(max(it1,it2).eq.1) then
         if(op.ne.equal.and.op.ne.less+great) then
            fin=-fin
            top=top0
            return
         endif
         itrue=1
         if(op.eq.less+great) itrue=0
         istk(il1)=4
         do 131 i=0,mn1-1
            e1r=stk(l1+i)
            e2r=stk(l2+i)
            e1i=0.0d+0
            e2i=0.0d+0
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
            if(op.ne.equal.and.op.ne.less+great) then
               call error(226)
               return
            else
               istk(il1+1)=1
               istk(il1+2)=1
               istk(il1+3)=1
               if(op.ne.equal) istk(il1+3)=0
               lstk(top+1)=sadr(il1+4)
            endif
            return
         endif
         if (.true.) then 
c           add explicit nan tests when requested 
            call idcmp(stk(l1),stk(l2),mn1,istk(il1+3),op)
         else
         do 132 i=0,mn1-1
            e1=stk(l1+i)
            e2=stk(l2+i)
c     for vc++ we add an explicit test for nan 
            if(isanan(e1).eq.1.and.isanan(e2).eq.1) then 
               if (op.eq.less+great) then 
                  istk(il1+3+i)=1
               else
                  istk(il1+3+i)=0
               endif
            elseif(  (op.eq.equal   .and. e1.eq.e2) .or.
     &           (op.eq.less+great    .and. e1.ne.e2) .or.
     &           (op.eq.less          .and. e1.lt.e2) .or.
     &           (op.eq.great         .and. e1.gt.e2) .or.
     &           (op.eq.(less+equal)  .and. e1.le.e2) .or.
     &           (op.eq.(great+equal) .and. e1.ge.e2) ) then
               istk(il1+3+i)=1
            else
               istk(il1+3+i)=0
            endif
 132     continue
         endif
         lstk(top+1)=sadr(il1+3+mn1)
      endif
      return
      end

      subroutine matlog
c     
c     & | ~
c     
c     Copyright INRIA
      include '../stack.h'
c     
      integer top0,op
      double precision  e1,e2
      integer et,ou,non
      integer iadr,sadr
      data ou/57/,et/58/,non/61/

c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      top0=top
      op=fin
      lw=lstk(top+1)+1
c     
      if(rhs.eq.2) then
         il2=iadr(lstk(top))
         if(istk(il2).lt.0) il2=iadr(istk(il2+1))
         m2=istk(il2+1)
         n2=istk(il2+2)
         it2=istk(il2+3)
         l2=sadr(il2+4)
         mn2=m2*n2
         top=top-1
      endif
c     
      il1=iadr(lstk(top))
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=sadr(il1+4)
      mn1=m1*n1

      if(fin.eq.non) then
         if(mn1.eq.0) then
            istk(il1)=1
            istk(il1+1)=0
            istk(il1+2)=0
            istk(il1+3)=0
            lstk(top+1)=sadr(il1+4)
            return
         else
            istk(il1)=4
            do 10 i=0,mn1-1
               e1=stk(l1+i)
               if(e1.eq.0.0d0) then
                  istk(il1+3+i)=1
               else
                  istk(il1+3+i)=0
               endif
 10         continue
            lstk(top+1)=sadr(il1+3+mn1)
         endif
      elseif(fin.eq.et) then
         if(mn1.eq.0.or.mn2.eq.0) then
            istk(il1)=1
            istk(il1+1)=0
            istk(il1+2)=0
            istk(il1+3)=0
            lstk(top+1)=sadr(il1+4)
            return
         endif
         if(mn1.eq.1) then
            i1=0
            mn1=mn2
         else
            i1=1
         endif
         if(mn2.eq.1) then
            i2=0
            mn2=mn1
         else
            i2=1
         endif
         istk(il1)=4
         do 20 i=0,mn1-1
            e1=stk(l1+i)
            e2=stk(l2+i)
            if(e1.ne.0.0d0.and.e2.ne.0.0d0) then
               istk(il1+3+i)=1
            else
               istk(il1+3+i)=0
            endif
 20      continue
         lstk(top+1)=sadr(il1+3+mn1)
         return
      elseif(fin.eq.ou) then
         if(mn1.eq.0.or.mn2.eq.0) then
            istk(il1)=1
            istk(il1+1)=0
            istk(il1+2)=0
            istk(il1+3)=0
            lstk(top+1)=sadr(il1+4)
            return
         endif
         if(mn1.eq.1) then
            i1=0
            mn1=mn2
         else
            i1=1
         endif
         if(mn2.eq.1) then
            i2=0
            mn2=mn1
         else
            i2=1
         endif
         do 30 i=0,mn1-1
            e1=stk(l1+i)
            e2=stk(l2+i)
            if(e1.ne.0.0d0.or.e2.ne.0.0d0) then
               istk(il1+3+i)=1
            else
               istk(il1+3+i)=0
            endif
 30      continue
         istk(il1)=4
         istk(il1+1)=max(m1,m2)
         istk(il1+2)=max(n1,n2)
         lstk(top+1)=sadr(il1+3+mn1)
         return
      endif
      end
