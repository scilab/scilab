      subroutine sigbas(n)
c ====================================================================
c     dealing with signals inside Scilab 
c ====================================================================
c
c traitement de l'erreur fortran ou du break
c
c     Copyright INRIA
      integer  n
      include '../stack.h'
      logical iflag,interruptible
      common /basbrk/ iflag,interruptible
      integer  ilk,k,l,lunit,nc,mode(2)
c     
      iadr(l)=l+l-1
c      sadr(l)=(l/2)+1
c
      if(ddt.eq.4) then
         write(buf(1:5),'(i5)') n
         call basout(io,wte,'signal :'//buf(1:5))
      endif
c
      mode(2)=0
      if ( n.eq.2 ) then
         iflag=.true.
      elseif (n.eq.11 ) then
         call error(68)
         goto 10
      elseif (n.eq.8 ) then
         call msgstxt('Floating point exception !')
      else
         iflag=.false.
      endif
      goto 99
c
c     erreur fatale : sauvegarde de la pile avant le stop
c     -------------
c
 10   continue
c
c     ouverture du fichier
c
      err  =0
      lunit=0
      call inffic( 5, buf, nc)
      nc = max ( 1 , nc )
      call inffic( 5, buf, nc)
      mode(1)=103
      call clunit( lunit, buf(1:nc), mode)
      if ( err.gt.0 ) call error(err)
      if ( err.gt.0 ) goto 90
c
      call error(68)
      err=0
c
c sauvegarde
c
 30   k = isiz-6
      if (k .lt. bot) k = isiz
      call savlod(lunit,0,-1,0)
      if(err.gt.0) goto 33
 32   continue
      l=k
      ilk=iadr(lstk(k))
      if(istk(ilk).lt.0) l=istk(ilk+1)
      call savlod(lunit,idstk(1,k),0,l)
      k = k-1
      if(k.ge.bot) goto 32
c 
 33   mode(1)=103
      call clunit( -lunit, buf, mode)
 90   stop
c
c     fin
c     ---
c
 99   continue
      end

