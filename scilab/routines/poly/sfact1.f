      subroutine sfact1(b,n,w,maxit,ierr)
c!but
c     on cherche une factorisation spectrale d'un polynome a donne
c     par : a*(a(1/z) = b(n)*z**-n+....+b(0)+ ... +b(n)*z**n
c!liste d'appel
c     subroutine sfact1(b,n,w,maxit,ierr)
c
c     double precision b(n+1),w(6*(n+1))
c     integer n,maxit,ierr
c
c     b : contient les coeffs b(n),b(n-1),....,b(0)
c         apres execution b contient les coeff du resultat
c     n : degre de a
c     w : tableau de travail de taille 6*(n+1)
c     maxit : nombre maxi d'iterations admis
c     ierr : indicateur d'erreur
c             si ierr=0 : ok
c             si ierr<0 : convergence a 10**ierr pres
c             si ierr=1 : non convergence 
c             si ierr=2 : b(0) est negatif ou nul.
c          
c!auteur
c     f Delebecque inria (86) d'apres Kucera V
c     modif s. Steer (90)
c!origine
c     V Kucera : Discrete Linear control (john Wiley& Sons) 1979
c!
c     Copyright INRIA
      dimension b(n+1),w(*)
      double precision b,b0,w,a0,temp,b00,s,dlamch,eps,best
c
      eps=dlamch('p')*10.0d+0
c
      lb=n+1
      ierr=0
c
      lomeg=1
      lalpha=lomeg+lb
      lro=lalpha+lb
      leta=lro+lb
      lbold=leta+lb
      lambda=lbold+lb
      lsave=lambda+lb
c     
      call dcopy(lb,b,-1,w(lbold),1)
      call dcopy(lb,w(lbold),1,b,1)
      b00=w(lbold)
      if(b00.le.0.0d+0) goto 91
      b0=sqrt(b00)
      do 1 j=1,lb
         w(lalpha-1+j)=b(j)/b0
 1    continue
c     
      do 40 i=1,maxit
c     
         call dcopy(lb,w(lbold),1,b,1)
         call dcopy(lb,w(lalpha),1,w(lomeg),1)
c     premier tableau
         do 15 k=1,lb-1
            call dcopy(lb-k+1,w(lalpha),-1,w(lro),1)
            w(lambda+k-1)=w(lalpha+lb-k)/w(lro+lb-k)
            do 11 j=1,lb-k
               w(lalpha-1+j)=w(lalpha-1+j)-w(lambda+k-1)*w(lro+j-1)
 11         continue
            a0=w(lalpha)
c     calcul de eta
            w(leta+lb-k)=2.0d+0*b(lb-k+1)/a0
            if (k.lt.lb-1) then
               do 12 j=2,lb-k
                  b(j)=b(j)-0.50d+0*w(leta+lb-k)*w(lalpha+lb-k-j+1)
 12            continue
            endif
 15      continue
         w(leta)=b(1)/w(lalpha)
c     deuxieme tableau
         do 21 k=lb-1,1,-1
            call dcopy(lb-k+1,w(leta),-1,b,1)
            do 19 j=1,lb-k+1
               w(leta+j-1)=w(leta+j-1)-w(lambda+k-1)*b(j)
 19         continue
 21      continue
         s=0.0d+0
         do 22 j=1,lb
            w(lalpha-1+j)=0.50d+0*(w(leta+j-1)+w(lomeg+j-1))
            s=s+w(lalpha-1+j)*w(lalpha-1+j)
 22      continue
c     
c     test de convergence

c     calcul de l'erreur element par elements
c$$$  call dcopy(lb,w(lbold),-1,b,1)
c$$$  do 900 iii=0,n
c$$$  x=b(iii+1)-ddot(iii+1,w(lalpha),1,w(lalpha+n-iii),1)
c$$$  write(6,'(10x,e10.3,'','',e10.3,'';'')') x,b(1+iii)
c$$$  900  continue

         temp=abs(s-b00)/b00
         if(temp.le.eps)  goto 50
         if(i.eq.1) best=temp
         if(temp.lt.best) then
            call dcopy(lb,w(lalpha),1,w(lsave),1)
            best=temp
         endif
 40   continue
      goto 90
c     
 50   do 51 i=1,lb
         b(i)=w(lalpha-1+i)
 51   continue
      return
c     
c     retours en erreur
c     
 90   continue
      if(best.le.1.d-3) then
         call dcopy(lb,w(lsave),1,b,1)
         ierr=nint(log10(best))
      else
c     non convergence
         ierr=1
      endif
      return
 91   continue
c     b00 est negatif ou nul
      ierr=2
      return
      end
