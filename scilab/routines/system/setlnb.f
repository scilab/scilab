      subroutine setlnb()
c     =======================================================
c     compilation info for line counter 
c     =======================================================
c     Copyright INRIA
      include '../stack.h'
      logical ilog,compil
      if(err1.gt.0) return
      call whatln(lpt(1),lpt(2),lpt(6),nct,idebut,ifin)
      ilog=compil(16,lct(8)-nct,0,0,0)
      return
      end


