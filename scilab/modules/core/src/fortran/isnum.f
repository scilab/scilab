      logical function isnum(s,n,ival)
c!but
c     cette fonction teste si la chaine de caractere codee donnee 
c     dans les n premiers entiers de s represente un nombre entier et si oui 
c     retourne sa valeur ival
c!
c     Copyright INRIA
      integer s(n),ival
c
      ival=0
      k=0
 10   k=k+1
      if(k.gt.n) goto 20
      if(abs(s(k)).gt.9) goto 30
      ival=10*ival+s(k)
      goto 10
c
 20   isnum=.true.
      return
 30   isnum=.false.
      return
      end
