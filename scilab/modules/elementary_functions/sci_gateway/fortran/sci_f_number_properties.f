c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txtc     -------------------------------
c
      subroutine intnbprop(id)

c     Interface for number_properties (an interface on dlamch) :
c    
c         number_properties("eps")    -> machine epsilon
c         number_properties("radix")  -> base
c         number_properties("digits") -> number of digits for the mantissa
c         number_properties("minexp") -> emin
c         number_properties("maxexp") -> emax
c         number_properties("huge")   -> max positive float
c         number_properties("tiny")   -> min positive normalized float 
c         number_properties("denorm") -> (boolean) true if denormalized number are used
c         number_properties("tiniest")-> min positive denormalized float 
c         
c
      implicit none

      INCLUDE 'stack.h'

      integer id(nsiz)

c     EXTERNAL FUNCTIONS
      double precision dlamch
      external         dlamch

c     EXTERNAL API FUNCTIONS
      logical  checkrhs, checklhs, getsmat, cremat, crebmat
      external checkrhs, checklhs, getsmat, cremat, crebmat

c     LOCAL VAR
      integer topk
      integer mt, nt, lstr, nlstr, lm, lr, lc, i
      integer   lmax
      parameter(lmax = 10)
      character*(lmax) inputstring
      character*17 fname
      double precision tiniest, b

c     TEXT
      fname = 'number_properties'
      topk=top
      rhs=max(0,rhs)

      if (.not.checkrhs(fname,1,1)) return
      if (.not.checklhs(fname,1,1)) return

c     1/ get the string
      if( .not. getsmat(fname,topk,top,mt,nt,1,1,lstr,nlstr)) return
c     rmq : pas de verif qu'il s'agit bien d'une matrice (1,1) ...
c     on recupere la chaine dans la variable inputstring
      lm = min(nlstr,lmax)
      call cvstr(lm,istk(lstr),inputstring,1)
c     complete (eventualy) the string with some blanks
      inputstring(lm+1:lmax) = '          '

c     2/ go on
      if     (inputstring(1:9) .eq. 'eps      ') then
         if (.not.cremat(fname,top,0,1,1,lr,lc)) return
         stk(lr) = dlamch('e')
      elseif (inputstring(1:9) .eq. 'huge     ') then
         if (.not.cremat(fname,top,0,1,1,lr,lc)) return
         stk(lr) = dlamch('o')
      elseif (inputstring(1:9) .eq. 'tiny     ') then
         if (.not.cremat(fname,top,0,1,1,lr,lc)) return
         stk(lr) = dlamch('u')
      elseif (inputstring(1:9) .eq. 'radix    ') then
         if (.not.cremat(fname,top,0,1,1,lr,lc)) return
         stk(lr) = dlamch('b')
      elseif (inputstring(1:9) .eq. 'digits   ') then
         if (.not.cremat(fname,top,0,1,1,lr,lc)) return
         stk(lr) = dlamch('n')
      elseif (inputstring(1:9) .eq. 'minexp   ') then
         if (.not.cremat(fname,top,0,1,1,lr,lc)) return
         stk(lr) = dlamch('m')
      elseif (inputstring(1:9) .eq. 'maxexp   ') then
         if (.not.cremat(fname,top,0,1,1,lr,lc)) return
         stk(lr) = dlamch('l')
      elseif (inputstring(1:9) .eq. 'denorm   ') then
         if (.not.crebmat(fname,top,1,1,lr)) return
         if (dlamch('u') / dlamch('b') .gt. 0.d0) then
            istk(lr) = 1
         else
            istk(lr) = 0
         endif
      elseif (inputstring(1:9) .eq. 'tiniest  ') then
         if (.not.cremat(fname,top,0,1,1,lr,lc)) return
         b = dlamch('b')
         tiniest = dlamch('u')
         if ( tiniest/b .ne. 0.d0 ) then
c     denormalized number are used
            do i = 1, int(dlamch('n')) - 1
               tiniest = tiniest / b
            enddo
         endif
         stk(lr) = tiniest
      else
         buf=fname//' : unknown property kind'
         call error(999)
         return
      endif
      
      end
c     -------------------------------
