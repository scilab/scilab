      subroutine intstr(n,istk,ni,job)
c!but
c     converti la representation decimale d'un entier en une chaine 
c     de caractere scilab
c!
c     Copyright INRIA
      parameter (nc=20)
      character*(nc) buf
      integer istk(*)
c
      write(buf,'(i20)') n
      i=0
 10   i=i+1
      if(buf(i:i).eq.' ') goto 10
      ni=nc+1-i
      call cvstr(ni,istk,buf(i:nc),0)
      end

