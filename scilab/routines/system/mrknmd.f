      subroutine mrknmd()
c     Copyright INRIA
      INCLUDE '../stack.h'
      logical compil,c
      character*(6) str
c      integer iadr,sadr

      if(ddt.eq.4) then
         call cvname(ids(1,pt),buf,1)
         write(str,'(i6)') top
         call basout(io,wte,'mrknmd top = '//str//
     $        ' name = '//buf(1:24))
      endif

      if(lhs.le.0) return
      if(comp(1).eq.0) then
         infstk(top)=1
         call putid(idstk(1,top),ids(1,pt))
         pt=pt-1
      else
         c=compil(18,ids(1,pt),0,0,0)
         pt=pt-1
      endif
      return
      end
