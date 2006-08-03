      subroutine matopt
c     Copyright INRIA
      INCLUDE 'stack.h'
c     
      double precision zero,df0,zng,dxmin
      double precision epsg,epsg1,epsf,dzs
      integer top2,topin,topind,topx,top3
      character*80   nomsub
      common /optim/ nomsub
      integer       nizs,nrzs,ndzs
      common /nird/ nizs,nrzs,ndzs
      external foptim,boptim,fuclid,ctonb,ctcab
      integer coin,coar,coti,cotd,cosi,cosd,nfac
c     
      character*(nlgh+1) namef,namej
      common/csolve/namef,namej
c     for semidef
      double precision abstol,reltol,nu,tv
      integer sz,upsz
c
      integer impn(nsiz)
      logical eqid, getscalar
      integer iadr, sadr
c     
      parameter (nsiz1=nsiz-1)
      data impn/672732690,nsiz1*673720360/
      data coin,coar,coti,cotd,cosi,cosd,nfac
     &     /   5906,6922,4637,3357,4636,3356, 0/
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' matopt '//buf(1:4))
      endif
c     
c     optim   quapro   semidef fsolve lsqrsolve qld
c     1        2        3        4       5      6
c     
      goto(01,400,500,600,700,800 ) fin
c     
      
c     optim
 01   call scioptim("optim")
      return
c     
c     quapro
c     
 400  call sciquapro("quapro")
      return
c     
 500  call sci_semidef("semidef")

c
c     fsolve
c     
 600  call scisolv("fsolve")
      return
c
c     lsqrsolve
c     
 700  call intlsqrsolve("lsqrsolve")
      return

c
c     qld
c     
 800  call intqld("qld")
      return


      end





