      subroutine matqz
c     Copyright INRIA
      INCLUDE '../stack.h'
c     
      double precision t,eps
      logical fail,macro
      integer top0,top2,iadr,sadr,tops
      character*(nlgh+1) namef
      common/cschur/namef
      external fschur,bschur
      integer         iero
      common /ierinv/ iero

c     
c     fin    1       2     3        4
c          gschur   gspec ereduc  fstair
c     
      sadr(l)=(l/2)+1
      iadr(l)=l+l-1
c     
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' matqz '//buf(1:4))
      endif
c     
      tops=top
      eps=stk(leps)

      if(fin.eq.1) then
        call scigschur('gschur')
      elseif(fin.eq.2) then
        call scigspec('gspec')
      elseif (fin .eq.3) then
        call intereduc('ereduc')
      elseif (fin .eq. 4) then
         call intfstair('fstair')
      endif
      return
c     
      end

