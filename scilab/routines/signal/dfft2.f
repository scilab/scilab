      subroutine dfft2(a,b,nseg,n,nspn,isn,ierr,iw,lw)
c!
c fft avec tableau de travail iw
c
c     voir la routine fft de singleton pour l'appel
c     iw est ici un tableau de travail a dimensionner a lw
c     dans le pgm appelant.Si lw est trop petit on sort avec
c     ierr < 0 et -ierr est le manque de place necessaire pour
c     poursuivre le calcul
c!
      double precision a,b
      dimension iw(*),a(*),b(*)
      iw(1)=0
      iw(2)=10
      iw(3)=10
      iw(4)=lw
      iw(5)=10
      call dfftbi(a,b,nseg,n,nspn,isn,ierr,iw(1),iw(2),
     +            iw(3),iw(4),iw(5),iw(1),iw(1))
      return
      end
