      subroutine matsvd
c     Copyright INRIA
      include '../stack.h'
      character*30 fnam
 
c     fin     1        2        3        4        5      6
c     svd      pinv     cond     norm     rank   sva
      goto (310,320,330,340,350,360),fin
 310  continue
      fnam='svd'
      call intsvd(fnam)
      goto 1000
 320  continue
      fnam='pinv'
      call cvname(ids(1,pt+1),fnam,0)
      fun=-1
      goto 2000
 330  continue
      fnam='cond'
      call cvname(ids(1,pt+1),fnam,0)
      fun=-1
      goto 2000
 340  continue
      fnam='norm'
      call cvname(ids(1,pt+1),fnam,0)
      fun=-1
      goto 2000
 350  continue
      fnam='rank'
      call cvname(ids(1,pt+1),fnam,0)
      fun=-1
      goto 2000
 360  continue
      fnam='sva'
      call cvname(ids(1,pt+1),fnam,0)
      fun=-1
      goto 2000

 1000 continue
      call putlhsvar
      call cvname(ids(1,pt+1),fnam,0)
      call msgs(101,0)
      return
 2000 continue
      call msgs(102,0)
      return
      end
