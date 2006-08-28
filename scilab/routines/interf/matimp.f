      subroutine matimp
c ====================================================================
c     impl dassl dasrt : simulation  de systeme algebrico-differentiel
c ====================================================================
c     Copyright INRIA
      INCLUDE '../stack.h'
c     impl     dassl     dasrt
c     1         2          3
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' matimp '//buf(1:4))
      endif
c     
      goto(10,100,1000) fin
      return
c     impl part 
 10   call sciimpl("impl")
      return
 100  call dassli("dassl")
      return
 1000 call dasrti("dasrt")
      return
      end
      
   