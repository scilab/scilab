      function getpro()
c!but
c     cette fontion est un artifice pour appeler getarg en C
c     si le programme principal est en fortran on n'a pas acces directement
c     aux variables C argc et argv. getarg est un fonction SUN permettant
c     de recuperer les argument d'appel du programme principal.
c     getarg(0,..) retourne les caracteres compris entre le debut de
c     l'instruction et le premier blanc , c'est a dire a peu pres la
c     facon dont on a designe l'executable
c!
c     Copyright INRIA
      character*(*) getpro
      getpro=' '
      call fgetarg(0,getpro)
      ln=len(getpro)+1
 01   continue
      ln=ln-1
      if(ln.le.0) goto 02
      if(getpro(ln:ln).eq.' ') goto 01
 02   continue
      ln=max(1,ln)
      getpro=getpro(1:ln)//char(0)
c
      end
