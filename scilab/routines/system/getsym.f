      subroutine getsym
      call getsym1(0)
      end

      subroutine getsym1(opt)
*
*     PURPOSE 
*       get the next symbol (a name, a number, an operator, ...)
*       of the current line for the parser 
*
*     INPUT
*       The "character flow" of the current line :
*       the current character is stored in the global (integer) var char1 ; 
*       the routine fortrangetch put the next char in char1 (getch also update 
*       lpt(4) which points to the following char if char1 is not eol)
*
*     OUTPUT (global vars) :
*       sym            : flag (type of the gotten symbol : num, name, ....)
*       syn(nsiz)      : in case of a name, the integer array syn (nsiz = 6) 
*                        store the "encoding of the name" : only the nlgh 
*                        (=4*nsiz=24) first letters of each name are taken 
*                        into account, then they are encoded in syn (by the 
*                        routine namstr) 
*       stk(lstk(isiz)): in case of a number, the routine getval converts it
*                        as a double float which is stored in this array cell.
*       lpt(6)         : mystere (something like "line pointers" no ?)
*       buf            : buffer to print 
*
*     REMARK
*       cette fonction modifie fin ? : a priori yes if the symbol is a number
*       as getval.f put fin=0 and the first instruction here is fin=1
*
*     COPYRIGHT INRIA 
*       (Modified by Bruno for using the new getval.f routine : now getval.f 
*       will do all the job instead of getting only "integers" : so some
*       part of getsym which worked with the old getval to get number have 
*       been removed ; also the goto 's flow chart have been replaced by 
*       some do while, if then else, ... in hoping that all pass current
*       f77 compilers (normaly it is OK))
*
*     A SUB-PART OF THE SCILAB CODED CHAR TABLE (char -> code)
*       In Scilab, chars are first converted as integers (positives and some
*       negatives) and we have :
*
*        code | 0  1 .... 9 | 10  11 ...  35 | 36  37  38  39 | 40
*        -----+-------------+----------------+----------------+------
*        char | 0  1 .... 9 |  a   b ...   z |  _   #   !   $ | blank
*        ============================================================
*        char |             |  A   B ...   Z |          ?     | tab
*        -------------------+----------------+----------------+------
*        code |             |-10 -11 ... -35 |         -38    ! -40
*
*       In fact (for the mapping code -> char), code = -1 
*       to -9 correspond also to chars 1 to 9 and code = -36,-37,-39 
*       to the char 0
*
*       So if c is a scilab coded char then :
*
*           abs(c) <= 9  => c is a digit
*           abs(c) < 40  => c is an alphanum Scilab char (which
*                           comprise _ but also # ! $ ?). Moreover
*                           Scilab names may begin with % 
*           abs(c) == 40 => c is a blank "like" (blank or tab) char  
*
      implicit none
      include '../stack.h'
      integer opt
      double precision syv
      integer namecd(nlgh), chcnt, io 
      integer blank, dot, percen, slash, comma,eol
      integer name, num, cmt

*     STATEMENTS FUNCTIONS
      integer c
      logical isDigit, isAlphaNum, isBlank
      data    blank/40/,dot/51/,percen/56/,slash/48/,comma/52/,eol/99/
      data    name/1/, num/0/, cmt/2/

 
      isDigit(c)    = abs(c) .le. 9
      isAlphaNum(c) = abs(c) .lt. blank
      isBlank(c)    = abs(c) .eq. blank

      fin=1

*     go to the first "no like blank" char
      do while ( isBlank(char1) )
         call fortrangetch
      end do

*     update some pointers (indices) onto the array lin
      lpt(2) = lpt(3)
      lpt(3) = lpt(4)

      if ( isDigit(char1) ) then 
*        -> number (beginning with a digit => 2d arg of getval : dotdet = .false.)
         sym = num
         call getval(syv, .false.)
         stk(lstk(isiz)) = syv

      elseif ( isAlphaNum(char1) .or. char1.eq.percen) then
*        -> name
         sym = name 
         chcnt = 1
         namecd(chcnt) = char1
         call fortrangetch
         do while ( isAlphaNum(char1) )
            if (chcnt.lt.nlgh) then
               chcnt = chcnt + 1
               namecd(chcnt) = char1
            endif
            call fortrangetch
         end do
*        encoding of the name
         call namstr(syn,namecd,chcnt,0)
         if (opt.eq.0.and.(lpt(4)-lpt(3).gt.chcnt)) call msgs(33,0)
      else
*        -> special character (eol, operator, part of an operator, .... 
*           but in case of a dot following by a digit it is a number)
         sym = abs(char1)
         call fortrangetch
         if (sym.eq.slash .and. abs(char1).eq.slash) then
            sym=2
            call fortrangetch
            return
         elseif (sym.eq.dot .and. isDigit(char1)) then
*           -> it is a number (beginning with a dot => 2d arg of getval : dotdet = .true.)
            sym = num
            call getval(syv, .true.)
            stk(lstk(isiz)) = syv
         endif
      endif

*     eat blanks
      do while ( isBlank(char1) )
         call fortrangetch
      end do

      if (ddt .lt. 3) return

      if (sym .eq. num) then
         write(buf(1:11),'(1x,e10.3)') syv
         call basout(io,wte,buf(1:11))
      else if (sym .eq. name) then
         call prntid(syn(1),1,wte)
      else if (sym .eq. cmt) then
         call basout(io,wte,'//')
      else if (sym .lt. csiz) then
         call basout(io,wte,alfa(sym+1))
      else
         call basout(io,wte, ' eol')
      endif

      end




