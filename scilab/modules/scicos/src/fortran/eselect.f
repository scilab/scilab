      subroutine eselect(flag,nevprt,ntvec,rpar,nrpar,ipar,nipar,u,nu)
c     Scicos block simulator
c     if-then-else block
c     if event input exits from then or else clock ouputs based
c     on the sign of the unique input (if input>0 then  else )
c
c     Copyright INRIA
      include 'stack.h'
      double precision rpar(*),u(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar,nu
c
      integer iu
c
      common /dbcos/ idb
c
      if(idb.eq.1) then
c				 replaces write(6 ...) by basout bug 2598                  
c        write(6,'(''ifthel     t='',e10.3,'' flag='',i1)') t,flag
        write(buf,'(''ifthel     t='',e10.3,'' flag='',i1)') t,flag
        call basout(io,wte,buf)
      endif
c
      iu=max(min(int(u(1)),ipar(1)),1)
      if(flag.eq.3) then
            ntvec=iu
      endif
      end
