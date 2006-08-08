c =========================================
c INRIA
c =========================================  
      subroutine inttokens(id)
c
c     Interface code for tokens :
c
c         words = tokens( str [, sep])
c
c     which cut the string str into words (as a vector column of words)
c     the words being sub-strings of str between separators chars.
c
c     where str must be a simple string 
c      and  sep a vector (or matrix) of characters (not string of chars)
c           sep is optionnal and if it is is omitted then sep = [ " ", TAB ]
c
c     Added by Bruno      
c        (Initially tokens was coded as a scilab macro but was very inefficient).
c
      implicit none
      INCLUDE 'stack.h'

      integer id(nsiz)

      integer lw, tops, il2, m2, n2, p2, i_sep, nb_sep, il1, l1, nv1, 
     $        ilr, nb_chars, nb_words, i, c, ipt, size, lword, iwd
      logical  in_sep

      logical is_a_sep
      external error, funnam, is_a_sep, icopy

      integer BLANK, TAB
      parameter (BLANK = 40, TAB = -40)

      integer iadr,sadr, l
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      if(rhs.lt.1 .or. rhs.gt.2) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif

      tops=top
     
      lw = iadr(lstk(top+1))
     
      if (rhs .eq. 2) then  
c        get the separators sep
         il2 = iadr(lstk(top))
         if (istk(il2).lt.0) il2 = iadr(istk(il2+1))
         if (istk(il2).ne.10) then 
            fun=-1
            top=tops
            call funnam(ids(1,pt+1),'tokens',il2)
            return
         endif
         m2 = istk(il2+1)
         n2 = istk(il2+2)
c        verify that each element of sep is a simple character
         p2 = il2 + 4
         do i = 1, m2*n2
            if (istk(p2+i)-istk(p2+i-1) .ne. 1) then
               buf='a separator must be formed by only one character'
               call error(9999)
               return
            endif
         enddo
         nb_sep = m2*n2           
c     !  there are nb_sep separators, and the first is located at istk(i_sep)
         i_sep = p2 + 1 + nb_sep  
         top = top - 1
      else
c        no separators given => uses the 2 default separators (blank and tab)
         i_sep = lw
         nb_sep = 2
         lw = lw + 2
         err=sadr(lw)-lstk(bot)
         if ( err.gt.0 ) then
            call error(17)
            return
         endif
         istk(i_sep) = BLANK
         istk(i_sep+1) = TAB
      endif

c     get the string str
      il1 = iadr(lstk(top)) 
      if(istk(il1).lt.0) il1 = iadr(istk(il1+1))
      if(istk(il1).ne.10) then 
         fun = -1
         top = tops
         call funnam(ids(1,pt+1),'tokens',il1)
         return
      endif
      if(istk(il1+1)*istk(il1+2).ne.1) then 
c     !  str must be a simple string not a vector|matrix of strings
         call error(36)
         return
      endif
      l1 = il1+6
      nv1 = istk(il1+5)-1   
c     ! nv1 is the number of chars of str
      if(nv1.eq.0) then     
c     !   str is an empty string => return a void matrix
         il1=iadr(lstk(top)) 
         istk(il1)   = 1
         istk(il1+1) = 0
         istk(il1+2) = 0
         istk(il1+3) = 0
         return
      endif

c     count the number of words (and characters)
      nb_words = 0
      nb_chars = 0
      in_sep = .true.
      do i = 0, nv1-1
         c = istk(l1+i)
         if ( is_a_sep(c, istk(i_sep), nb_sep) ) then
            in_sep = .true.
         else
            nb_chars = nb_chars + 1
            if (in_sep) then
               nb_words = nb_words + 1
               in_sep = .false.
            endif
         endif
      enddo

      if ( nb_words .eq. 0 ) then 
c     !   return a void matrix
         il1=iadr(lstk(top)) 
         istk(il1)   = 1
         istk(il1+1) = 0
         istk(il1+2) = 0
         istk(il1+3) = 0
         return
      endif

c     prepare memory for the result
      ilr = lw
      size =  5 + nb_words + nb_chars  
c     ! size of the resulting scilab object
      lw = lw + size                   
      err = sadr(lw) - lstk(bot)
      if ( err.gt.0 ) then
         call error(17)
         return
      endif

c     form the output
      istk(ilr)   = 10
      istk(ilr+1) = nb_words
      istk(ilr+2) = 1
      istk(ilr+3) = 0
      istk(ilr+4) = 1
      ipt = ilr + 5
      iwd = ipt + nb_words
      istk(iwd-1) = nb_chars + 1 
c     dernier pointeur (necessaire lorsque la chaine ne se termine pas 
C     par un separateur)
      lword = 0
      do i = 0, nv1-1
         c = istk(l1+i)
         if ( is_a_sep(c, istk(i_sep), nb_sep) ) then
            if (lword .gt. 0) then 
c     !        on vient donc de terminer un mot
               istk(ipt) = istk(ipt-1) + lword
               ipt = ipt + 1
               lword = 0
            endif
         else
            istk(iwd) = c
            iwd = iwd + 1
            lword = lword + 1
         endif
      enddo

c     copy result at top
      il1=iadr(lstk(top)) 
      call icopy(size, istk(ilr), 1, istk(il1), 1)       
      lstk(top+1)=sadr(il1 + size)

      end 


      logical function is_a_sep(c, sep, nb_sep)
c
c     utility function used by inttokens (Bruno)
c      
      implicit none
      integer c, nb_sep, sep(nb_sep)
      integer i
      is_a_sep = .false.
      do i = 1, nb_sep
         if (c .eq. sep(i)) then
            is_a_sep = .true.
            return
         endif
      enddo

      end 
c =========================================  
      