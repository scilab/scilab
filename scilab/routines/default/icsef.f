      subroutine icsef(indf,t,y,uc,uv,f,fy,fu,b,itu,dtu,
c     Copyright INRIA
      &        t0,tf,dti,dtf,ermx,iu,nuc,nuv,ilin,nti,ntf,ny,nea,
     &        itmx,nex,nob,ntob,ntobi,nitu,ndtu,nomf,nomc,nomi)
c     sous programme appele par icse.f qui donne :
c     pour indf=1,les seconds membres du systeme:f(ny)
c     pour indf=2,la matrice derivee des seconds membres par
c                 rapport a l'etat:fy(ny,ny)
c     pour indf=3,la matrice derivee des seconds membres par
c                 rapport aux parametres:fu(ny,nu)
c
      implicit double precision (a-h,o-z)
      dimension y(ny),uc(*),uv(*),f(ny),fy(ny,ny),fu(ny,*),
     & b(ny),itu(*),dtu(*),iu(5)
      character*6 nomf ,nomc ,nomi
      character*6 nomf1
c
      call majmin(6,nomf,nomf1)
c
c     cas lineaire quadratique
      if(nomf1.eq.' ') then
        if (indf.eq.1) then
          do 50 i=1,ny
          fii=b(i)
          do 20 j=1,ny
20        fii=fii+fy(i,j)*y(j)
          if(nuc.gt.0) then
            do 30 j=1,nuc
30          fii=fii+fu(i,j)*uc(j)
          endif
          if(nuv.gt.0) then
            jj=0
            do 40 j=1+nuc,nuv+nuc
            jj=jj+1
40          fii=fii+fu(i,j)*uv(jj)
          endif
50        f(i)=fii
          return
        endif
      if (indf.eq.2.or.indf.eq.3) return
      endif
c
c     identification de parametres d'un systeme lineaire
c     serotonine G.Launay
c
      if(nomf1.eq.'seros') then
        call seros(indf,t,y,uc,uv,f,fy,fu,itu,dtu,
     &  ny,nuc,nuv,nitu,ndtu)
        return
      endif
c
c     rentree de la navette en forward
c
      if(nomf1.eq.'navetf') then
        call navetf(indf,t,y,uc,uv,f,fy,fu,itu,dtu,
     &  ny,nuc,nuv,nitu,ndtu)
        return
      endif
c
c     rentree de la navette en backward
c
      if(nomf1.eq.'navetb') then
        call navetb(indf,t,y,uc,uv,f,fy,fu,itu,dtu,
     &  ny,nuc,nuv,nitu,ndtu)
        return
      endif
c
c     cas nomf incorrect
      write(*,8000) nomf
 8000 format(1x,'icsef : routine de dynamique inconnue : ',a6)
      indf=-10
      end
