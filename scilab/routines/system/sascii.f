      subroutine sascii(n,line,job)
c     n: entier, longueur de la chaine a convertir
c     copi de cvstr 
c     line: vecteur entier, contient des caracteres scilab ou ASCII 
C     qui sont transformes de l'un dans l'autre suivant job 
c     job: entier, si egal a 1: scilab-->ascii
c                  si egal a 0: ascii-->scilab
c
c!
c     Copyright INRIA
      include '../stack.h'
c
      integer line(*),mc
c
      if(job.eq.0) goto 40
      do 30 j=1,n
         m=line(j)
         if(abs(m).gt.csiz) m=99
         if(m.eq.99) goto 10
         if(m.lt.0) then
            line(j)=ichar(alfb(abs(m)+1))
         else
            line(j)=ichar(alfa(m+1))
         endif
         goto 30
 10      line(j)=ichar('        !')
 30   continue
      return
 40   do 50 j=1,n
         mc=line(j)
         do 45 k=1,csiz
            k1=k
            if(mc.eq.ichar(alfa(k)).or.mc.eq.ichar(alfb(k))) goto 46
 45      continue
 46      line(j)=k1-1
         if(mc.ne.ichar(alfa(k1))) line(j)=-(k1-1)
 50   continue
      end
      
