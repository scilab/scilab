C     This file is for backward compatility with scilab function
c     compiled with scilab-2.6
      subroutine matlu
c     Copyright INRIA
      include '../stack.h'
      character*30 fnam
      
c     1    2     3      4   5     6     7     
c     inv  det  rcond  lu      chol  rref 
      goto (110,120,130,140,150,160,170),fin
 110  continue
      fnam='inv'
      call intinv(fnam)
      goto 1000
 120  continue
      fnam='det'
      call intdet(fnam)
      goto 1000
 130  continue
      fnam='rcond'
      call intrcond(fnam)
      goto 1000
 140  continue
      fnam='lu'
      call intlu(fnam)
      goto 1000
 150  continue
      goto 1000
 160  continue
      fnam='chol'
      call intchol(fnam)
      goto 1000
 170  continue
      fnam='rref'
      call cvname(ids(1,pt+1),fnam,0)
      fun=-1
      goto 2000
c
 1000 continue
      call putlhsvar
      call cvname(ids(1,pt+1),fnam,0)
      call msgs(101,0)
      return
 2000 continue
      call msgs(102,0)
      return
      end

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
