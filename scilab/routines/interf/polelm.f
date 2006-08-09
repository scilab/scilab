      subroutine polelm
c     ====================================================================
c     
c     evaluation des fonctions polynomiales elementaires
c     
c     ====================================================================
c     
c     Copyright INRIA
      INCLUDE '../stack.h'
      integer iadr, sadr
      integer id(nsiz)

c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      
c     
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' polelm '//buf(1:4))
      endif
c     
c     functions/fin
c     1       2       3       4       5       6       7       8
c     poly     roots  degre   coeff   eval    pdiv  simp     sum
c     
c     9       10      11      12      13      14     15        16
c     prod    diag    triu     tril    bezout sfact simp_mode  varn
c     
c     17
c     cleanp
c    
      goto (10,20,30,40,50,60,73,55,58,25,27,28,70,83,120,100,110) fin
c     
c     poly
 10   call intpoly(id)
      goto 900
c     
c     roots 
 20   call introots(id)
      goto 900
c     
c     diag
 25   call intpdiag(id)
      goto 900
c     
c     triu 
 27   call intptriu(1,id)
      goto 900
c     
c     tril
 28   call intptriu(0,id)
      goto 900
c
c     degree
 30   call intdegree(id)
      goto 900
c     
c     coeff
 40   call intcoeff(id)
      goto 900
c     
c     eval
 50   continue
      goto 900
c     
c     sum
 55   call intpsum(id)
      goto 900
c     
c     prod
 58   call intpprod(id)
      goto 900
c     
c     pdiv
 60   call intpdiv(id)
      goto 900
c     
c     bezout
 70   call intbez(id)
      goto 900
c     
c     simp
 73   call intsimp(id)
      goto 900
c     
c     sfact
c     
 83   call intsfact(id)
      goto 900
c     
c     varn
 100  call intvarn(id)
      goto 900
c     
c     cleanp  
 110  call intpclean(id)
      goto 900
c
c     simp_mode
 120  call intsimpmd(id)
      goto 900

c     
 900  return
      end




