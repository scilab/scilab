      subroutine matqr
c     Copyright INRIA
      include '../stack.h'
      character*30 fnam
 
c     fin    1  
c           qr

      goto (410),fin
 410  continue
      fnam='qr'
      call intqr(fnam)
      goto 1000

 1000 continue
      call putlhsvar
      call cvname(ids(1,pt+1),fnam,0)
      call msgs(101,0)
      return
 2000 continue
      call msgs(102,0)
      return
      end
