      subroutine logelm
c ================================== ( Inria    ) =============
c evaluation des fonctions elementaires sur les booleens
c =============================================================
c
c     Copyright INRIA
      include '../stack.h'

c
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' logelm '//buf(1:4))
      endif

c     functions/fin
c     1      2     3    4
c   find   bool2s  or  and
c
c
c
      goto (10,20,30,40) fin
 10   call intfind
      return
 20   call intsbool2s
      return
 30   call intor('or')
      return
 40   call intand('and')
      return
      end
c
