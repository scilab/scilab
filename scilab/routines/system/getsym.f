      subroutine getsym
c     get a symbol
C     cette fonction modifie 
C     fin ?
C     char : caractere courant lu 
C     syn(nsiz) : codage du symbole lu 
C     sym : flag de type du symbole lu 
C     stk(lstk(isiz)) si le symbole est un nombre 
C     lpt(6) : mystere 
C     buf : buffer pour imprimer 
c*------------------------------------------------------------------
c     Copyright INRIA
      include '../stack.h'
      double precision syv,s
      integer blank,dot,d,e,plus,minus,name,num,sign,chcnt,eol,achar1
      integer star,slash,bslash,ss,percen,hat,quote
      integer rparen,right
      integer io
      integer namecd(nlgh)
      data blank/40/,dot/51/,d/13/,e/14/,eol/99/,plus/45/
      data minus/46/,name/1/,num/0/,star/47/,slash/48/,bslash/49/
      data percen/56/,hat/62/,quote/53/
      data rparen/42/,right/55/
      fin=1
 10   if (abs(char1) .ne. blank) go to 20
      call getch
      go to 10
 20   lpt(2) = lpt(3)
      lpt(3) = lpt(4)
      if (abs(char1) .le. 9) go to 50
      if (abs(char1) .lt. blank.or. char1.eq.percen) go to 30
c     
c     special character
      ss = abs(sym)
      sym = abs(char1)
      call getch
      if (sym .ne. dot) go to 90

c     is dot part of number or operator
      achar1=abs(char1)
      if ((char1 .ge. 0 .and. char1 .le. 9)) then 
c     part of number
         syv=0.0d0
         goto 55
      else
c     part of operator
         goto 90
      endif
c     
c     name
 30   sym = name 
      chcnt=1
      namecd(chcnt)=char1
 40   call getch
      if (abs(char1).ge.blank) goto 45
      if(chcnt.lt.nlgh) then
         chcnt = chcnt+1
         namecd(chcnt)=char1
      endif
      go to 40
 45   call namstr(syn,namecd,chcnt,0)
      go to 90
c     
c     number
 50   call getval(syv)
      if (char1 .ne. dot) go to 60
      l4=lpt(4)
      call getch
      if (abs(char1).eq.d .or. abs(char1).eq.e) goto 61
c      if (abs(char1).gt.9.and.char1.ne.rparen
c     $     .and.char1.ne.right) then
c     .  dot is part of an operator
c         lpt(4)=l4
c         char1=dot
c         goto 70
c      endif
 55   chcnt = lpt(4)
      call getval(s)
      chcnt = lpt(4) - chcnt
      if (char1 .eq. eol) chcnt = chcnt+1
      syv = syv + s/10.0d+0**chcnt
 60   if (abs(char1).ne.d .and. abs(char1).ne.e) go to 70
 61   call getch
      sign = char1
      if (sign.eq.minus .or. sign.eq.plus) call getch
      call getval(s)
      if (sign .ne. minus) syv = syv*10.0d+0**s
      if (sign .eq. minus) syv = syv/10.0d+0**s
 70   stk(lstk(isiz)) = syv
      sym = num
c     
 90   if (abs(char1) .ne. blank) go to 99
      call getch
      go to 90
 99   if (ddt .lt. 3) return
      if (sym.gt.name .and. sym.lt.csiz) call basout(io,wte,alfa(sym+1))
      if (abs(sym) .ge. csiz) call basout(io,wte, ' eol')
      if (sym .eq. name) call prntid(syn(1),1,wte)
      if (sym .eq. num) then
         write(buf(1:11),'(1x,e10.3))') syv
         call basout(io,wte,buf(1:11))
      endif
      return
      end
