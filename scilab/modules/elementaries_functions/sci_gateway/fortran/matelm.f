      subroutine matelm
c ====================================================================
c
c     evaluate utility functions
c
c ====================================================================
c
c     Copyright INRIA
      INCLUDE 'stack.h'
c
      integer id(nsiz)
c     
      iadr(l)=l+l-1
c      sadr(l)=(l/2)+1

c
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' matelm '//buf(1:4))
      endif
c
c     functions/fin
c     abs  real imag conj roun int  size sum  prod diag triu tril
c      1    2    3    4    5    6    7    8    9    10   11   12
c     eye  rand ones maxi mini sort kron matr sin  cos  atan exp
c      13   14   15   16   17   18  19-21 22   23   24   25   26
c     sqrt log   ^  sign clean floor ceil expm cumsum  cumprod testmatrix
c      27   28   29  30   31     32   33   34    35      36      37
c     isreal frexp zeros tan  log1p imult  asin acos number_properties
c       38     39    40   41     42    43   44   45      46
c     nearfloat dsearch isequal spones
c       47        48      49     50
c!
c
      goto (10 ,15 ,20 ,25 ,30 ,35 ,40 ,45 ,50 ,60,
     1      61 ,62 ,70 ,72 ,71 ,90 ,91 ,105,110,110,
     2      110,130,140,150,160,170,180,190,200,210,
     3      220,37 ,39 ,173,46 ,47, 230,240,250,260,
     4      165,195,196,152,154,300,310,320,330,340 ),fin

 10   continue
      call intabs(id)
      goto 900
c     
c     real
 15   continue
      call intreal(id)
      goto 900
c     
c     imag
 20   continue
      call intimag(id)
      goto 900
c     
c     conjg
 25   continue
      call intconj(id)
      goto 900
c     
c     round
 30   continue
      call intround(id)
      goto 900
c     
c     int
 35   continue
      call intint(id)
      goto 900
c     
c     floor
 37   continue
      call intfloor(id)
      goto 900
c     
c     ceil
 39   continue
      call intceil(id)
      goto 900
c     
c     size
 40   continue
      call intsize(id)
      goto 900    

c     sum
 45   continue
      call intsum(id)
      goto 900    

c     
c     cumsum
 46   continue
      call intcumsum(id)
      go to 900
c     
c     cumprod
 47   continue
      call intcumprod(id)
      go to 900
c     
c     prod
 50   continue
      call intprod(id)
      go to 900
c     
c     diag
 60   continue
      call intdiag(id)
      go to 900

c     triu
 61   continue
      call inttriu(id)
      go to 900

c     tril
 62   continue
      call inttril(id)
      go to 900

c     eye
 70   continue
      call inteye(id)
      go to 900

c     ones
 71   continue
      call intones(id)
      go to 900

c     rand
 72   call  intrand('rand',id)
      go to 900

c     maxi
 90   continue
      call intmaxi('maxi',id)
      go to 900

c     mini
 91   continue
      call intmaxi('mini',id)
      go to 900
c     
c     sort
 105  continue
      call intsort(id)
      go to 900

c     
c     kronecker product
 110  continue
      call intkron(id)
      go to 900
c     
c     matrix
 130  continue
      call intmatrix(id)
      goto 900
c     
c     sin
c     
 140  continue
      call intsin(id)
      goto 900
c     
c     cos
c     
 150  continue
      call intcos(id)
      goto 900
c
c     asin
c
 152  continue
      call intasin(id)
      goto 900
c
c     acos
c
 154  continue
      call intacos(id)
      goto 900  
c     
c     atan
c     
 160  continue
      call intatan(id)
      goto 900
c
c     tan
c
 165  continue
      call inttan(id)
      goto 900

c     exp element wise
 170  continue
      call intexp(id)
      goto 900

c     expm matricial exponential
 173  continue
      call intexpm(id)
      goto 900

c     sqrt
 180  continue
      call intsqrt(id)
      goto 900

c     
c     log
c     
 190  continue
      call intlog(id)
      goto 900
c     
c     log1p
c     
 195  continue
      call intlog1p(id)
      goto 900
c     
c     imult
c     
 196  continue
      call intimult(id)
      goto 900

c     
c     ** non integer power of square matrices or  scalar^matrix
c     
 200  continue
      fun=-1
      il=iadr(lstk(top-rhs+1))
      if(istk(il).lt.0) il=iadr(istk(il+1))
      call funnam(ids(1,pt+1),'pow',il)
      goto 900
c     
c     sign
c     
 210  continue
      call intsign(id)
      goto 900
c     
c     clean
c     
 220  continue
      call intclean(id)
      goto 900

c     
c     testmatrix
c     
 230  continue
      call inttestmatrix(id)
      goto 900
c     
c     isreal
c     
 240  continue
      call intisreal(id)
      goto 900

 250  continue
      call intfrexp(id)
      goto 900
c
c     zeros
c

 260  continue
      call intzeros(id)
      goto 900
 
c
c     number_properties
c
 300  call  intnbprop(id)
      go to 900
c
c     number_properties
c
 310  call  intnearfl(id)
      go to 900

c
c     dsearch
c
 320  call intdsearch(id)
      goto 900
c
c     isequal
c
 330  call intisequal(id)
      goto 900
c
c     spones
c
 340  call intspones('spones',id)
      goto 900
c
 900  return
      end
