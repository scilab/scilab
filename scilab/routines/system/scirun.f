      subroutine scirun( bu1)
c!Purpose 
c     run scilab parser with initial instruction
c!Syntax
c     call scirun( bu1)
c!parameters
c     bu1 : character string which contain the initial instruction
c!
c     Copyright INRIA
      character*(*) bu1
c
      INCLUDE '../stack.h'
      integer iadr
      logical allowptr
c
      iadr(l)=l+l-1

c-------------------------------------------
c     set instruction to execute at start-up
c-------------------------------------------
      buf    = bu1
      rio=-1
      lpt(1)=1
      lpt(6)=0
      pt=0

c ------------------------
c call instructions parser
c ------------------------
c
      icall=0
      krec=99999
      include '../callinter.h'
 9999 goto 60
 200  return
c
      end
