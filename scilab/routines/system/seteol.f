      subroutine seteol
c     =====================================
c     insert eol code in compiled structure 
c     ======================================
c     Copyright INRIA
      include '../stack.h'
      logical compil,ilog
      if(err1.le.0) then
         ilog=compil(15,0,0,0,0)
         if(comp(3).eq.2) then
c     .     set profile info
            ilog=compil(25,0,0,0,0)
         endif
      endif
      return
      end
