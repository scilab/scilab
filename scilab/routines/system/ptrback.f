
c     Copyright INRIA
      subroutine mvptr(topk,il)
C     just for dealing with lists 
      include '../stack.h'
      integer topk ,il
      common / ptkeep / lwk
      lwk= lstk(topk)
      lstk(topk) = il 
      return 
      end 

      subroutine ptrback(topk)
      integer topk 
      include '../stack.h'
      common / ptkeep / lwk
      lstk(topk)=lwk
      return 
      end 
