      subroutine getval(s,dotdet)
*
*     PURPOSE
*        get a number : on output s must be a double float
*                       which must be very close from the decimal
*                       number represented by the pattern of char
*
*        this subroutine is called by getsym when this last one
*        have detected the beginning of a lexical token which corresponds 
*        to a positive number (an integer or a float). There are two 
*        cases (whom this routine is informed by the logical
*        dotdet (as "dot detected")) :
*
*          1/ the token begins with a digit d (in [0-9]) : in this case
*             dotdet = .false.
*
*          2/ the token begins with a point following by a digit .d : in
*             this case  dotdet = .true.
*
*        On entry, the global var char1 contains the first digit of the number
*        Getting the next "char" is done by a call to getch (which put the
*        next char is the global var char1). 
*     
*     MOTIVATION
*        Written by Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr> so as
*        to replace the old string -> number method used by Scilab
*        which was not accurate enough (even in some easy cases we can got
*        the near float s (and the float gotten may be 3 or 4 floats after
*        or before the optimal one) : in fact contrarily to the old method 
*        this subroutine computes actually the float s only when the string 
*        pattern is such that only one (hoped correctly) rounded float 
*        operation will be done in computing s, all others operations being 
*        exact (and so we are sure to get the nearer floating point number). 
*        In the others cases we call an "intrinsic" function of Fortran 
*        (as strtod in C) to do the job (what fortran called an internal file). 
*        The overhead comes from the fact that at this level the "scilab characters" 
*        are actually integers (a first convertion string -> integer is already 
*        done) so a "reconversion" to a string is necessary.
*
*     A BRIEF EXPLANATION  
*        On an example, suppose that the "string" pattern is 1234.56789012345e+23  :
*
*        1/ the mantissa is red and the digits are recorded inside the array
*           digit = [1 2 3 4 5 6 7 8 9 0 1 2 3 4 5]  (only the first ndgmax
*           digits of the mantissa are red)
*           -> the dot is detected and a correction of -11 will be
*              bring in the exponent
*        2/ the exponent is computed directly in integer arithmetic, then the
*           correction is brought to got the final exponent : 23-11 = 12 
*
*        All that to say that the string number to convert is equal to
*           123456789012345 * 10^12 and in general  x = integer 10^expo
*
*        So if integer <= 2^53 (all integer n such that |n| <= 2^53 belong
*        in the ieee754 double float number) then we must compute this integer
*        (from the digit array) exactly in double precision (if all intermediary 
*        computed quantities are integers <= 2^53 which is the case). 
*        A simple way to impose this condition is the following :
*            2^53 = 9007199254740992 > 8 10^15 > 10^15
*        So that if our integer has 15 digits (or 16 digits with d1 <= 8) then
*        it is OK.
*
*        For the exponent : 10^0, 10^1, 10^2, ...., 10^22 are all exactly representable
*        in double ieee 754 (10^22 = 5^22 * 2^22  and 5^22=2384185791015625 < 2^53
*        but 5^23 > 2^53 so that 10^23 is not a float point number)
* 
*        Conclusion : 
*          (i) if our integer have less than 15 digits and if |expo|<= 22
*              then only one non exact operation (a multiplication or a division
*              depending the sign of expo) will be done (eventually) and we
*              got the near float ;
*         (ii) one other trivial case are also considered (see explanation at 
*              the end of this file).
*        (iii) If not we form a string as  123456789012345.d+12 and we call 
*              a fortran intrinsic routine to do the job (via internal file)           
*
*     A LAST REMARK : this routine doesn't change the syntax of tokens considered
*        as numbers in Scilab, in particular 1.d- or 1.d+ are still valid (the exponent
*        is taken as 0)

      implicit none

*     PARAMETER
      double precision s
      logical dotdet

*     GLOBAL SCILAB VAR (in fact only char1 is used)
      include '../stack.h'

*     LOCAL VAR
      logical detdot
      integer    ndgmax
      parameter (ndgmax=25)
      integer    digit(ndgmax)
      integer ndgrec, ndg, sgnexp, ndgexp, expcor, expo, code0, k, i
      character*(ndgmax+6) string
      double precision toto
      save             toto

*
*     detdot : a var to put the value of the argument dotdet
*              (this is because at the call, dotdet is a constant 
*              (.true. or .false.) and in this subroutine detdot
*              may change of value
*
*     ndgmax : maximun number of recorded digits (=> when
*              the mantissa have more than ndgmax digit,
*              it may result a relative error of 10^(1-ndgmax)
*              between the initial number and the number that
*              this routine converts as a machine number
*              (this last one may suffer of a relative error
*              of epsm = (approx) 1.11 10^(-16)))
*     digit  : array of length ndgmax to record the mantissa 's digits
*     ndgrec : number of recorded digits (<= ndgmax)
*     ndg    : to count the number of digits of the integer part of
*              the mantissa (which may be superior to ndgmax => in
*              this case a correction must be bring in the exponent)
*     sgnexp : sign of the exponent part (see SYMBOL AFTER)
*     expcor : correction to bring in the exponent (because all
*              the mantissa begin an integer, p.e. 123.456 => 123456
*              in this case the correction is -3)
*     ndgexp : number of digits of the exponent (to control spurious
*              integer overflow if the exponent is something like
*              e+2147483648  (=2^31  (= -2^31 with the usual 32 bits
*              integer arithmetic )))
*     expo   : the exponent (directly computed with integer arithmetic
*              but ndgexp may control integer "overflow")
*     code0  : integer code of the character "0"  
*     string : string to hold the "number" to be converted in double
*              when the "number" is such that a direct straitforward
*              conversion will be not enough accurate  
*     toto   : a var to got an inf with 1/(toto-toto)  


*     CONSTANTS  (to adapt eventualy ...)
      integer    EXPMAX,    EXPMIN,     NDEMAX,  DGLIM,   EXPLIM
      parameter (EXPMAX=309,EXPMIN=-324,NDEMAX=7,DGLIM=15,EXPLIM=22)

*     EXPMAX may be such that 10^EXPMAX > max positive float num
*     EXPMIN may be such that 10^EXPMIN < min positive float num
*     NDEMAX may be such that 10^5 <= 10^NDEMAX < MAX_INTEGER :
*            when we compute (with integer arithmetic) the exponent 
*            the number of digits of the exponent is recorded in 
*            ndgexp and the test  ndgexp <= NDEMAX validate this
*            calculus.
*     DGLIM  all integers with a number of digits <= DGLIM must be 
*            exactly representable as double float (DGLIM = 15 for
*            ieee 754)  
*     EXPLIM all power of 10 up to EXPLIM (included) must be exactly 
*            representable as double float (EXPLIM = 22 for ieee 754)

*     SOME CHAR SYMBOLS (scilab char are integers)
      integer dot,    d,    e,    plus,    minus

*     STATEMENT FUNCTION
      integer c
      logical isDigit

*     DATA 
      data    dot/51/,d/13/,e/14/,plus/45/,minus/46/
      data             toto /0.d0/


      isDigit(c) = abs(c) .le. 9

*     this is from the original getval code... don't know the reason
      fin=0

*     beginning of the code


      detdot = dotdet
      ndg = 0
      ndgrec = 0
      if (.not. detdot) then
*        1) got the integer part of the mantissa of the pattern
*        1-a) may be there is some 0 at the beginning
         do while ( char1 .eq. 0 )
            call getch
         enddo
*        1-b) now record the digits (inside the digit array)
*             (but we record a maximum of ndgmax digits) 
         do while ( isDigit(char1) )
            ndg = ndg + 1
            if ( ndgrec .lt. ndgmax ) then
               ndgrec = ndgrec + 1
               digit(ndgrec) = char1
            endif
            call getch
         enddo
*        1-c) at this point we have detected something which is not a digit
*             may be a point, may be a d,D,e,E, or something else
*             here we only test for the dot and let the others cases
*             to be treated after ...
         if ( abs(char1) .eq. dot ) then
            detdot = .true.
            call getch
         endif
      endif

*     first correction for the (future) exponent : if the first part
*     of the string have more then ndgmax digits we have to add
*     ndg - ndgrec (else we have expcor=0)
      expcor = ndg - ndgrec

      if (detdot) then
*        2) got the "fractionnal" part of the "mantissa"
         if (ndgrec .eq. 0) then
*           we have not passed throw the part 1) or only zeros have been met
*           and may be the number start with .000xxx : so clean up those 0
            do while ( char1 .eq. 0 )
               expcor = expcor - 1
               call getch
            enddo
         endif
*        now we begin to record the digits
         do while ( isDigit(char1) )
            if ( ndgrec .lt. ndgmax ) then
               ndgrec = ndgrec + 1
               expcor = expcor - 1
               digit(ndgrec) = char1
            endif
            call getch
         enddo
      endif

*     3) at this point the "mantissa" of the string decimal number
*        must be recorded, now detect the exponent
      expo = 0
      ndgexp = 0
      sgnexp = plus
      if (abs(char1).eq.d .or. abs(char1).eq.e) then
*        the string have an exponent part (which, in Scilab, may be empty or
*        may had only a sign ! => expo = 0)
         call getch
         if (char1.eq.minus .or. char1.eq.plus) then
            sgnexp = char1
            call getch
         else
            sgnexp = plus
         endif
*        may be the exponent start by some 0
         do while ( char1 .eq. 0 )
            call getch
         enddo
*        now form the exponent : the var ndgexp is here
*        to treat spurious integer overflow ...
         do while ( isDigit(char1) ) 
            expo = 10*expo + char1
            ndgexp = ndgexp + 1
            call getch
         enddo
      endif

*     4) Now we can form the double float number s

*     4-1/ only zeros in the mantissa 
      if (ndgrec .eq. 0) then
*        no digits have been recorded : this is the case
*        when the mantissa part is of the form [000][.][000]
*        the number is 0
         s = 0.d0
         return
      endif

*     4-2/ ndgexp is to large => the exponent expo is perhaps badly 
*     computed (integer "overflow") or in all cases the 
*     exponent is too large (positive or negative) such that it result 
*     (for s) in a overflow or underflow depending the exponent sign
      if (ndgexp .ge. NDEMAX) then
         if (sgnexp .eq. minus) then
*           underflow
            s = 0.d0
         else
*           overflow : got an inf ...
            s = 1.d0/(toto - toto)
         endif
         return
      endif

*     4-3/ now build the final exponent
      if (sgnexp .eq. plus) then
         expo =  expo + expcor
      else
         expo = -expo + expcor
      endif

*     4-4/ here some tests to avoid unnecessary call to the fortran "strtod"
*     Now we have a number s of the form  d_1 d_2 ... d_ndgrec 10^expo
*     which is equal to d_1 . d_2 ... d_ndgrec 10^(expo + ndgrec - 1)
*     with d_1 .ne. 0 
*     so it comes :  s >= 10^(expo + ndgrec - 1)
*                    s <= 10^(expo + ndgrec)
*
*     Suppose given EXPMAX such that  10^EXPMAX > max positive float number
*               and EXPMIN such that  10^EXPMIN < min positive float number
*
*     then if  expo + ndgrec - 1 >= EXPMAX then overflow occurs necessarily
*     and  if  expo + ndgrec <= EXPMIN then underflow occurs
*
*     On IEEE 754 we have : max positive float num = (approx) 1.8E+308
*                           min positive float num = (approx) 4.9E-324
*                           (if denormalised number are used)
*
*     So that EXPMAX = 309
*        and  EXPMIN = -324  are OK (but larger limits are possible to take
*     into account others f.p. arithmetics)
*     Note that after the test (with these values) the exponent have a 
*     maximum of 3 (decimals) digits

      if (expo + ndgrec - 1 .ge. EXPMAX) then
*        overflow : got an inf ...
         s = 1.d0/(toto - toto)
         return
      endif
      if (expo + ndgrec .le. EXPMIN) then
*        underflow : got an 0
         s = 0.d0
         return
      endif


*     4-5/ Now the usual case where we can get the near floating point
*     without any problem
      if (ndgrec .le. DGLIM .and. abs(expo) .le. EXPLIM) then
         s = 0.d0
         do i=1, ndgrec
            s = 10.d0*s + digit(i)
         enddo
         if (expo .lt. 0) then
            s = s/(10.d0**(-expo))
         else
            s = s*10.d0**expo
         endif
         return
      endif

*     4-6/ The other easy case where we can compute s :
*     if expo = EXPLIM + k  but [integer part]*10^k < max_int_coded_in_double
*     then it is OK (retrieve k in the exponent and multiply the integer
*     part by 10^k and do the same job as previus)
      if (expo .gt. EXPLIM  .and. (expo-EXPLIM)+ndgrec .le. DGLIM) then
         s = 0.d0
         do i=1, ndgrec
            s = 10.d0*s + digit(i)
         enddo
*        peut etre dangereux avec des options d'optimisation ?
*        (le compilo peut etre tente d'ecrire directement s = s*10**expo
*        ce qui detruit le truc ...)
         s = s*10.d0**(expo-EXPLIM)
         s = s*10.d0**EXPLIM
         return
      endif

*     4-7/ else use langage routines (as strtod but in fortran) to do the job
*     the overhead is a retranslation into a string...
      code0 = ichar("0")
      do i=1,ndgrec
         string(i:i) = char(digit(i)+code0)
      enddo
      if (expo .lt. 0) then
         string(ndgrec+1:ndgrec+3) = '.d-'
      else
         string(ndgrec+1:ndgrec+3) = '.d+'
      endif
      k = ndgrec+4
*     the following because the Ix format is "right" justified ...      
      if (abs(expo) .le. 9) then 
         write(string(k:k),'(I1)') abs(expo)
         read(string(1:k),*) s
      else if (abs(expo) .le. 99) then 
         write(string(k:k+1),'(I2)') abs(expo)
         read(string(1:k+1),*) s
      else 
         write(string(k:k+2),'(I3)') abs(expo)
         read(string(1:k+2),*) s
      endif

      end





