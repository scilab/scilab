      subroutine mesh2b(nbs,n6,n4,lfront,cr,c,nu,tri,front,nbt,err)
c-----------------------------------------------------------------------
c triangulation  2d a partir d'un ensemble de points et d'une frontiere
c de domaine decrite par ses composantes connexes
c-----------------------------------------------------------------------
c entrees :
c     cr     tableau des coordonnees des points c(1:2,1:nbs)
c     nbs    nombre de points
c      n6=6*(nbs+nbs-2)       n4=4*nbs-4
c     front  tableau definisant la frontiere par composantes connexes de la
c            frontiere (i1,i2) front(i1)=front(i2) front(i+1) est le
c            point frontiere suivant de front(i) pour i=i1,i2-1 telle que
c            la normale a la frontiere soit interne ( la  composante
c            connexe exterieure tourne dans le sens trigo )
c     lfront est la longueur du tableau front
c----------------------------------------------------------------------
c tableaux de travail
c     c(2,nbs) tableau d'entiers (copie de coordonnees)
c     tri(4*nbs-4) tableau d'entiers
c     nu (6*(2*nbs-2)) tableau d entiers contiendra le tableau
c             des connectivites ( au debut) a la sortie du sp
c
c sorties :
c     nbt         nombre de triangles generes
c     nu(1:3,nbt) sommets des triangles (tableau des connections)
c                        (!! nu a dimensionner en entree nu(1:6*(2*nbs-2))
c     err         si err = 0 alors pas de probleme  (ok)
c                        sinon nbt = 0 et pas de triangulation
c     c(1:2,nbs)  coordonnees des sommets (en entier) cr n est pas modifie
c---------------------------------------------------------------------------
c hecht-marrocco  inria-rocquencourt  (39 63 55 14)
c-------------------------------------------------------------------------
      integer nbs,nbt,lfront,n6,n4,err
      integer c(2,nbs),tri(n4),front(lfront),nu(n6)
      double precision cr(2,nbs)
      integer i,j,k,tete,i1,i2
c
      err = 0
      nbt = 0
      do 1,i=1,nbs
         c(1,i)=0
         c(2,i)=0
 1    continue
      do 2,i=1,n6
         nu(i)=0
 2    continue
c preparation des donnees
      call mshtri (cr,c,nbs,tri,tri(nbs+1),err)
      if(err.ne.0) return
c maillage de l enveloppe convexe
      call mshcxi (c,nu,tri,nbs,tete,err,n4)
      if(err.ne.0) return
c
      do 10 i=1,nbs
      tri(i)=0
10    continue
      i=tete
20    continue
      j=nu(6*(i-1)+4)
      tri(nu(6*(i-1)+1))=nu(6*(j-1)+1)
      i=j
      if(i.ne.tete) goto 20
c traitement frontiere
      k=0
      if(lfront.gt.0) then
      call mshfrt(c,nu,nbs,front,lfront,tri,err,n4)
      if(err.ne.0) return
      do 30 i=1,nbs
      tri(i)=0
30    continue
      k  = 0
      i2 = 0
      do 40 i=1,lfront
      i1=i2
      i2=front(i)
      if(i1.eq.k) then
      k = -i2
      elseif(i1.eq.-k) then
      k=-k
      tri(i1)=i2
      else
      tri(i1)=i2
      endif
40    continue
      endif
c     construction du tableau nu(1:3,1:nbt)
      nbt=0
      k = 0
      do 200 j=1,6*(nbs+nbs-2),6
      if(nu(j+5).ne.0) then
      nbt=nbt + 1
      do 190 i=0,2
      k=k+1
      nu(k)=nu(j+i)
190   continue
      endif
200   continue
      end
c**********************************************************************
      integer function mshlcl(c,nu,tete,s,nbs)
      integer nbs,c(2,nbs),nu(6,nbs+nbs-2),tete,s
      integer x,y,pt,ppt,det
      logical init
c
      x=c(1,s)
      y=c(2,s)
      init=.true.
      pt=tete
10    continue
      ppt=pt
      pt=nu(4,pt)
      if(pt.ne.tete) then
      det=x*c(2,nu(1,pt)) -y*c(1,nu(1,pt))
      if(det.lt.0) then
      init=.false.
      goto 10
      elseif(init.and.det.eq.0) then
      goto 10
      endif
      endif
      mshlcl=ppt
      end
c**********************************************************************
      subroutine mshtri (cr,c,nbs,tri,nu,err)
      integer nbs,c(1:2,1:nbs),tri(1:nbs),nu(1:nbs),err
      double precision cr(1:2,1:nbs)
      integer iii,ic,xx,ip,i,j,jc,k,trik,tri3,det,ierr
      double precision aa1,aa2,xmin,xmax,ymin,ymax
      double precision precis
      parameter (precis=2.**15-1.)
c
      err = 0
      ierr = 0
      iii=1
      xmin=cr(1,1)
      ymin=cr(2,1)
      xmax=cr(1,1)
      ymax=cr(2,1)
      do 10 ic=1,nbs
       xmin=min(cr(1,ic),xmin)
       ymin=min(cr(2,ic),ymin)
       xmax=max(cr(1,ic),xmax)
       ymax=max(cr(2,ic),ymax)
       tri(ic)=ic
       if(cr(1,ic).lt.cr(1,iii)) then
        iii=ic
       endif
10    continue
      aa1 = precis/(xmax-xmin)
      aa2 = precis/(ymax-ymin)
      aa1 = min(aa1,aa2)
      aa2 = aa1*(cr(2,iii)-ymin)
      do 20 ic=1,nbs
       c(1,ic) = nint(aa1*(cr(1,ic)-cr(1,iii)))
       c(2,ic) = nint(aa1*(cr(2,ic)-ymin)-aa2)
       nu(ic)= c(1,ic)**2 + c(2,ic)**2
20    continue
c----------------------------------------------------------
      call mshtr1 (nu,tri,nbs)
      ip = 1
      xx=nu(ip)
      do 30 jc=1,nbs
      if(nu(jc).gt.xx)then
      call mshtr1 (nu(ip),tri(ip),jc-ip)
      do 25 i=ip,jc-2
      if(nu(i).eq.nu(i+1)) then
      ierr=ierr+1
c      print *,' error les points ',tri(i),tri(i+1),' sont egaux'
      endif
25    continue
      xx=nu(jc)
      ip=jc
      endif
      ic=tri(jc)
      nu(jc)=c(2,ic)
30    continue
      call mshtr1 (nu(ip),tri(ip),nbs-ip)
      do 35 i=ip,jc-2
      if(nu(i).eq.nu(i+1)) then
      ierr=ierr+1
c      print *,' error les points ',tri(i),tri(i+1),' sont egaux'
      endif
35    continue
      if(ierr.ne.0) then
      err = 2
c      print *,' fatal error mshtri:il y a des points confondus'
      return
      endif
      k=2
50    continue
      if(k.le.nbs) then
      k=k+1
      det = c(1,tri(2))*c(2,tri(k)) - c(2,tri(2))*c(1,tri(k))
      if(det.eq.0) goto 50
      else
c      print *,'fatal error mshtri tous les points sont alignes'
c      print *,'tri =',(tri(k),k=1,nbs)
      err = 3
      stop 'fatal error'
      endif
      trik = tri(k)
      do 60 j=k-1,3,-1
      tri(j+1)=tri(j)
60    continue
      tri(3)=trik
      if(det.lt.0) then
      tri3=tri(3)
      tri(3)=tri(2)
      tri(2)=tri3
      endif
      end
c**********************************************************************
      subroutine mshtr1 (criter,record,n)
      integer record(n)
      integer criter(n)
c
      integer i,l,r,j,n
      integer rec
      integer crit
c
      if(n.le.1) return
      l=n/2+1
      r=n
2     if(l.le.1)goto 20
      l=l-1
      rec=record(l)
      crit=criter(l)
      goto 3
20    continue
      rec=record(r)
      crit=criter(r)
      record(r)=record(1)
      criter(r)=criter(1)
      r=r-1
      if(r.eq.1)goto 999
3     j=l
4     i=j
      j=2*j
      CRES=j-r
      if (CRES .lt. 0) then
         goto 5
      elseif (CRES .eq. 0) then
         goto 6
      else
         goto 8
      endif
5     if(criter(j).lt.criter(j+1))j=j+1
6     if(crit.ge.criter(j))goto 8
      record(i)=record(j)
      criter(i)=criter(j)
      goto 4
8     record(i)=rec
      criter(i)=crit
      goto 2
999   record(1)=rec
      criter(1)=crit
      return
      end
c**********************************************************************
      subroutine mshcvx(direct,c,nu,pfold,nbs,err)
      integer nbs,c(2,nbs),nu(6,nbs+nbs-2),pfold,err
      logical direct
      integer pp,ps,i1,i2,i3,i4,i5,i6
      integer pf,psf,ppf,s1,s2,s3,t,t4,t5,a4,a5,det,tt4,tt5
      if(direct) then
      pp=3
      ps=4
      i1=1
      i2=3
      i3=2
      i4=6
      i5=5
      i6=4
      else
      pp=4
      ps=3
      i1=1
      i2=2
      i3=3
      i4=4
      i5=5
      i6=6
      endif
10    continue
      ppf=pfold
      pf =nu(ps,pfold)
      psf=nu(ps,pf)
      s1=nu(1,ppf)
      s2=nu(1,pf)
      s3=nu(1,psf)
      det =   ( c(1,s2) - c(1,s1) ) * ( c(2,s3) - c(2,s1) )
     &      - ( c(2,s2) - c(2,s1) ) * ( c(1,s3) - c(1,s1) )
      if(((.not.direct).and.det.gt.0).or.(direct.and.det.lt.0)) then
      if(direct) then
      tt4 = nu(2,ppf)
      tt5 = nu(2,pf)
      else
      tt4 = nu(2,pf)
      tt5 = nu(2,psf)
      endif
      t4 = tt4/(2**3)
      t5 = tt5/(2**3)
      a4 = tt4 -8 * t4
      a5 = tt5 -8 * t5
      nu(ps,ppf) = psf
      nu(pp,psf) = ppf
      t = pf
      if(direct) then
      nu(2,ppf) = (2**3) * t + i6
      else
      nu(2,psf) = (2**3) * t + i6
      endif
      nu(i1,t ) = s1
      nu(i2,t ) = s2
      nu(i3,t ) = s3
      nu(i4,t ) = (2**3) * t4 + a4
      nu(i5,t ) = (2**3) * t5 + a5
      if(direct) then
      nu(i6,t ) = -ppf
      else
      nu(i6,t ) = -psf
      endif
      nu(a4,t4) = (2**3) * t + i4
      nu(a5,t5) = (2**3) * t + i5
      call mshopt (c,nu,t5,a5,nbs,err)
      if(err.ne.0) return
      goto 10
      endif
      end
c**********************************************************************
      subroutine mshcxi (c,nu,tri,nbs,tete,err,n4)
      integer nbs,c(2,nbs),nu(6,2*nbs-2),tri(n4),tete
      integer mshlcl,err,n4
      integer i,j,s,t,pf,ppf,psf,npf,pp,ps,taf,iaf,free,ttaf
      parameter (pp=3,ps=4)
      do 10 i=1,nbs+nbs-2
      nu(1,i)=i+1
      do 10 j=2,6
      nu(j,i)=0
10    continue
      nu(1,nbs+nbs-2)=0
      free = 1
      t=free
      free = nu(1,free)
      tete=free
      pf  =free
      do 20 i=1,3
      nu(i  ,t) = tri(i)
      nu(3+i,t) = -pf
      ppf       = pf
      free      = nu(1,pf)
      pf        = free
      if(i.eq.3) pf=tete
      nu(1,ppf) = tri(i)
      nu(2,ppf) = i + 3 + (2**3) * t
      nu(ps,ppf) = pf
      nu(pp,pf ) = ppf
20    continue
      do 30 i=4,nbs
      s=tri(i)
      pf=mshlcl(c,nu,tete,s,nbs)
      t=free
      free = nu(1,free)
      npf  = free
      free = nu(1,free)
      ppf  = nu(pp,pf)
      psf  = nu(ps,pf)
      ttaf  = nu(2,pf)
      taf  = ttaf / (2**3)
      iaf  = ttaf - (2**3) * taf
      nu(1,t) = s
      nu(2,t) = nu(1,psf)
      nu(3,t) = nu(1,pf )
      nu(4,t) = -npf
      nu(5,t) = (2**3) * taf + iaf
      nu(6,t) = -pf
      nu(iaf,taf) = (2**3) * t + 5
      nu(ps,npf) = psf
      nu(ps,pf ) = npf
      nu(pp,npf) = pf
      nu(pp,psf) = npf
      nu(1,npf)  = s
      nu(2,npf)  = (2**3) * t + 4
      nu(2,pf )  = (2**3) * t + 6
      call mshopt (c,nu,t,5,nbs,err)
      if(err.ne.0) return
      call mshcvx (.true. ,c,nu,npf,nbs,err)
      if(err.ne.0) return
      call mshcvx (.false.,c,nu,npf,nbs,err)
      if(err.ne.0) return
30    continue
      end
c**********************************************************************
      subroutine mshopt (c,nu,t,a,nbs,err)
      integer nbs,c(2,nbs),nu(6,nbs+nbs-2),t,a,err
      integer vide
      parameter (vide=-2**30)
      integer mxpile
      parameter (mxpile=512)
      integer pile(2,mxpile)
      integer t1,t2,i,s1,s2,s3,s4,sin1,cos1,sin2,cos2,sgn
      integer tt1,tt,i11,i12,i13,i21,i22,i23,a1,a2,aa,mod3(1:3)
      double precision reel1,reel2
      double precision reel8
      data mod3/2,3,1/
      i=1
      pile(1,i) = t 
      pile(2,i) = a
10    continue
      if(i.gt.0) then
      t1=pile(1,i)
      a1=pile(2,i)
      i=i-1
      if(t1.le.0) goto 10
      tt1 = nu(a1,t1)
      if(tt1.le.0) goto 10
      t2 = tt1/(2**3)
      a2 = tt1-t2*(2**3)
      i11 =   a1 -3
      i12 =   mod3(i11) 
      i13 =   mod3(i12)
      i21 =   a2 -3
      i22 =   mod3(i21)
      i23 =   mod3(i22)
      s1 = nu(i13,t1)
      s2 = nu(i11,t1)
      s3 = nu(i12,t1)
      s4 = nu(i23,t2)
        sin1 =   (c(2,s3)-c(2,s1)) * (c(1,s2)-c(1,s1))
     &         - (c(1,s3)-c(1,s1)) * (c(2,s2)-c(2,s1))
        cos1 =   (c(1,s3)-c(1,s1)) * (c(1,s3)-c(1,s2))
     &         + (c(2,s3)-c(2,s1)) * (c(2,s3)-c(2,s2))
        if(sin1.eq.0.and.cos1.eq.0) then
c          print *,'fatal error mshopt:'
c     &           ,'3 points confondus ',s1,s2,s3
          err = 12
          return
        end if
        sin2  =   (c(1,s4)-c(1,s1)) * (c(2,s2)-c(2,s1))
     &          - (c(2,s4)-c(2,s1)) * (c(1,s2)-c(1,s1))
        cos2  =   (c(1,s4)-c(1,s2)) * (c(1,s4)-c(1,s1))
     &          + (c(2,s4)-c(2,s2)) * (c(2,s4)-c(2,s1))
        reel1=float(cos2)*float(sin1)
        reel2=float(cos1)*float(sin2)
        if(abs(reel1)+abs(reel2).ge.2**30) then
        reel8=dble(cos2)*dble(sin1)
     &       +dble(cos1)*dble(sin2)
        reel8=min(max(reel8,-1.d0),1.d0)
        sgn=reel8
        else
        sgn = cos2*sin1 + cos1*sin2
        endif
        if(min(max(sgn,-1),+1)*sin1.ge.0) goto 10
        nu(i12,t1) = s4
        nu(i22,t2) = s1
        tt1 = nu(i22+3,t2)
        nu(a1 ,t1) = tt1
        if(tt1.gt.0) then
         tt=tt1/(2**3)
         aa = tt1-(2**3)*tt
         nu(aa,tt)=  a1 +  (2**3) * t1
        elseif(tt1.ne.vide) then
         nu(2,-tt1)= a1 +  (2**3) * t1
        endif
        tt1 = nu(i12+3,t1)
        nu(a2 ,t2) = tt1
        if(tt1.gt.0) then
         tt=tt1/(2**3)
         aa=tt1-(2**3)*tt
         nu(aa,tt)= a2 +  (2**3) * t2
        elseif(tt1.ne.vide) then
         nu(2,-tt1)= a2 +  (2**3) * t2
        endif
        nu(i12+3,t1) =   i22+3 + (2**3)*t2
        nu(i22+3,t2) =   i12+3 + (2**3)*t1
        if(i+4.gt.mxpile) then
c          print *,' fatal error mshopt la pile est trop petite ',mxpile
          err =13
          return
        endif
c
        i=i+1
        pile(1,i)=t1
        pile(2,i)=a1
        i=i+1
        pile(1,i)=t2
        pile(2,i)=a2
        i=i+1
        pile(1,i)=t1
        pile(2,i)=i13+3
        i=i+1
        pile(1,i)=t2
        pile(2,i)=i23+3
        goto 10
      endif
      end
c**********************************************************************
      subroutine mshfrt (c,nu,nbs,frt,lfrt,w,err,n4)
      integer nbs,c(2,nbs),nu(6,nbs+nbs-2)
      integer lfrt,frt(lfrt),err,w(n4)
      integer i,ifrt,sinit,lnu,is,ie,tinter,nbac,nbaf,nbacpp
      integer s0,s1,s2,ta,is1,it,s2t,s3t,det2,det3
      integer p3(1:3)
      integer vide
      parameter (vide=-2**30)
      logical fin
      data p3/2,3,1/
      if(lfrt.eq.0) return
      tinter =0
      ifrt=0
      lnu = nbs+nbs-2
c       inite du tableau w
      do 10 i=1,nbs
       w(i)=-1
10    continue
      nbaf = 0
      s1 = 0
      sinit= 0
      fin =.true.
      do 20 i=1,lfrt
      s0 = s1
      s1 = frt(i)
      if(s1.le.0.or.s1.gt.nbs) then
      err=5
c      print *,' fatal error mshfrt '
c      print *,' le tableau des la frontiere est mauvais en ',i,s1
      return
      endif
      if(s0.eq.sinit) then
      if(fin) then
      sinit=s1
      else
      nbaf = nbaf + 1
      if(w(s0).ne.-1) then
c      print *,'fatal error mshfrt : la frontiere est croisee '
c     &       ,' en ',s0                    
      err=6
      endif
      w(s0)=i
      endif
      fin=.not.fin
      else
      nbaf = nbaf + 1
      if(w(s0).ne.-1) then
c      print *,'fatal error mshfrt : la frontiere est croisee '
c     &       ,' en ',s0                    
      err=6
      endif
      w(s0)=i
      endif
20    continue
      if(sinit.ne.s1) then
c      print *,'warning mshfrt:la frontiere n''est pas fermee'
c     &         ,' on la ferme avec l''arete ',s1,sinit
      if(w(s1).ne.-1) then
c      print *,'fatal error mshfrt : la frontiere est croisee '
c     &           ,' en ',s1
      err=6
      endif
      w(s1)=sinit
      nbaf = nbaf + 1
      endif
      nbac = 0
      nbacpp = 1
30    continue
      if(err.ne.0) return
      if(nbac.lt.nbaf) then
      if(nbacpp.eq.0) then
      err = 7
c      print *,' fatal error mshfrt :l''algorithme boucle :'
c     &           ,nbaf,nbac
c      print *,' la frontiere est certainement mal orientee '
      return
      endif
c     on s'occupe des aretes du maillage et frontiere de omega
c---------------------------------------------------------------------
      nbacpp = 0
      do 60 ie=1,lnu
      if(nu(5,ie).ne.0) then
      do 50 is=1,3
      s1  =nu(    is ,ie)
      s2t =nu( p3(is),ie)
      if(w(s1).gt.0) then
      s2   = frt(w(s1))
      if(s2.eq.s2t) then
      tinter = ie
      nbacpp = nbacpp + 1 
      w(s1) = 0
      if(nu(is+3,ie).gt.0) then
      ta = nu(is+3,ie) /(2**3)
      i  = nu(is+3,ie)-(2**3) * ta
      nu(i,ta)=vide
      endif
      nu(is+3,ie)=vide
      else
      it   = ie
      is1  = is
      s3t  = nu(p3(p3(is)),it)
      det2 =  (c(1,s2t)-c(1,s1))*(c(2,s2)-c(2,s1))
     &      - (c(2,s2t)-c(2,s1))*(c(1,s2)-c(1,s1))
      det3 =  (c(1,s3t)-c(1,s1))*(c(2,s2)-c(2,s1))
     &      - (c(2,s3t)-c(2,s1))*(c(1,s2)-c(1,s1))
      if(det2.ge.0.and.det3.le.0) then
      if(det2.eq.0) then
      if(w(s2t).eq.-1) then
c      print *,' fatal error mshfrt: le point ',s2t
c     &       ,' qui ne doit pas etre frontiere , l''est'
      err = 10
      endif
      goto 50
      endif
      if(det3.eq.0) then
      if(w(s3t).eq.-1) then
c      print *,' fatal error mshfrt: le point ',s3t
c     &      ,' qui ne doit pas etre frontiere , l''est'
      err = 10
      endif
      goto 50
      endif
      call mshfr1 (c,nu,nbs,it,is1,s2,err)
      if(err.ne.0) return
      tinter=it
      w(s1) = 0
      nbacpp = nbacpp + 1
      endif
      endif
      endif
50    continue
      endif
60    continue
      nbac = nbac + nbacpp
      goto 30
      endif
      i=2
      w(1)=tinter
      w(2)=3
      nu(1,tinter) = -nu(1,tinter)
70    continue
      if(i.gt.0) then
      w(i)=w(i)+1
      if(w(i).le.6) then
      ta=nu(w(i),w(i-1))
      if(ta.gt.0) then
      ta = ta / (2**3)
      if(nu(1,ta).gt.0) then
      w(i+1)=ta
      w(i+2)=3
      i=i+2
      nu(1,ta)=-nu(1,ta)
      endif
      endif
      else
      i=i-2
      endif
      goto  70
      endif
      do 90 ie=1,lnu
      if(nu(1,ie).lt.0) then
      nu(1,ie)=-nu(1,ie)
      else
      do 80 i=1,6
      nu(i,ie)=0
80    continue
      endif
90    continue
      end
c**********************************************************************
      subroutine mshfr1 (c,nu,nbs,it1,is1,s2,err)
      integer nbs,c(2,nbs),nu(6,nbs+nbs-2),is1,s2,err,it1
      integer lstmx
      parameter (lstmx=256)
      integer lst(3,lstmx)
      integer s1,s3,x,y,det,nbac,s2t,s3t,t,ta
      integer l1,l2,l3,la,p3(1:5)
      logical direct
      data p3 /2,3,1,2,3/
      direct = .true.
      t = it1
      s1 = nu(is1,t)
      x = c(1,s2)-c(1,s1)
      y = c(2,s2)-c(2,s1)
      nbac = 0
      l1 = is1
      l2 = p3(l1)
      l3 = p3(l2)
      s2t = nu(l2,t)
      s3t = nu(l3,t)
      la = l2 + 3
20    continue
      nbac = nbac + 1
      if(nbac.gt.lstmx) then
c        print *,' fatal error mshfr1 : lst trop petit ',nbac,lstmx
        err =8
        return
      endif
      lst(2,nbac) = t
      lst(3,nbac) = la
      ta = nu(la,t)
      if(ta.le.0) then
c        print *,' fatal error mshfr1:la frontiere est croisee en ',t
        err =9
        return
      endif
      t  = ta/8
      la = ta-8*t 
      s3 = nu(p3(la-2),t)
      if(s3.ne.s2) then
        det = x*(c(2,s3)-c(2,s1))-y*(c(1,s3)-c(1,s1))
        if(det.gt.0) then
          la = 3+p3(la-3)
        elseif(det.lt.0) then
          la = 3+p3(la-2)
        else
c          print *,' fatal error mshfr1: le point ',s3
c     &           ,' qui ne doit pas etre frontiere , l''est'
          err = 10
          return
        endif
        goto 20
      endif
      call mshfr2 (c,nu,nbs,lst,nbac,it1,s1,s2)
      return
      end
c**********************************************************************
      subroutine mshfr2 (c,nu,nbs,lst,nbac,t,ss1,ss2)
      integer nbs,nbac,c(2,nbs),nu(6,nbs+nbs-2),lst(3,nbac)
      integer t,ss1,ss2
      integer ptlst,ttlst,pslst,pplst,s1,s2,s3,s4,x41,y41,x,y
      integer i,t1,a1,tt1,t2,a2,tt,i11,i12,i13,i21,i22,i23,aas,aa
      integer det1,det4,det2,det3
      integer mod3(3)
      integer vide
      parameter (vide=-2**30)
      data mod3/2,3,1/
      x = c(1,ss1)-c(1,ss2)
      y = c(2,ss1)-c(2,ss2)
      do 10 i=1,nbac-1
      lst(1,i)=i+1
10    continue
      lst(1,nbac)=0
      ttlst = 1
20    continue
      ptlst  = ttlst
      pplst  = 0
30    continue
      if(ptlst.gt.0) then
      t1=lst(2,ptlst)
      a1=lst(3,ptlst)
      tt1 = nu(a1,t1)
      t2 = tt1/(2**3)
      a2 = tt1-t2*(2**3)
      i11 =   a1 -3
      i12 =   mod3(i11) 
      i13 =   mod3(i12)
      i21 =   a2 -3
      i22 =   mod3(i21)
      i23 =   mod3(i22)
      s1 = nu(i13,t1)
      s2 = nu(i11,t1)
      s3 = nu(i12,t1)
      s4 = nu(i23,t2)
      x41 = c(1,s4)-c(1,s1)
      y41 = c(2,s4)-c(2,s1)
      det2 = (c(1,s2)-c(1,s1))*y41-(c(2,s2)-c(2,s1))*x41
      det3 = (c(1,s3)-c(1,s1))*y41-(c(2,s3)-c(2,s1))*x41
      if(det2.gt.0.and.det3.lt.0) then
      nu(i12,t1) = s4
      nu(i22,t2) = s1
      pslst=lst(1,ptlst)
      if(pslst.gt.0) then
      aas=lst(3,pslst)
      if(aas.eq.i22+3) then
      lst(2,pslst) = t1
      lst(3,pslst) = i11 + 3
      endif
      endif
      tt1 = nu(i22+3,t2)
      nu(a1 ,t1) = tt1
      if(tt1.gt.0) then
      tt=tt1/(2**3)
      aa = tt1-(2**3)*tt
      nu(aa,tt)=  a1 +  (2**3) * t1
      elseif(tt1.ne.vide) then
      nu(2,-tt1)= a1 +  (2**3) * t1
      endif
      tt1 = nu(i12+3,t1)
      nu(a2 ,t2) = tt1
      if(tt1.gt.0) then
      tt=tt1/(2**3)
      aa=tt1-(2**3)*tt
      nu(aa,tt)= a2 +  (2**3) * t2
      elseif(tt1.ne.vide) then
      nu(2,-tt1)= a2 +  (2**3) * t2
      endif
      nu(i12+3,t1) =   i22+3 + (2**3)*t2
      nu(i22+3,t2) =   i12+3 + (2**3)*t1
      det1 = (c(1,s1)-c(1,ss1))*y-(c(2,s1)-c(2,ss1))*x
      det4 = (c(1,s4)-c(1,ss1))*y-(c(2,s4)-c(2,ss1))*x
      if(det1.lt.0.and.det4.gt.0) then
      lst(2,ptlst) = t2
      lst(3,ptlst) = i22+3
      elseif(det1.gt.0.and.det4.lt.0) then
      lst(2,ptlst) = t1
      lst(3,ptlst) = i12+3
      else
      if(pplst.eq.0) then
      ttlst = lst(1,ptlst)
      ptlst = ttlst
      else
      ptlst        = lst(1,ptlst)
      lst(1,pplst) = ptlst
      endif
      goto 30
      endif
      endif
      pplst = ptlst
      ptlst = lst(1,ptlst)
      goto 30
      endif
      if(ttlst.ne.0) goto 20
      nu(i12+3,t1) =  vide
      nu(i22+3,t2) =  vide
      t = t2      
      do 40 i=1,nbac
      call mshopt (c,nu,lst(2,i),4,nbs,ierr)
      call mshopt (c,nu,lst(2,i),5,nbs,ierr)
      call mshopt (c,nu,lst(2,i),6,nbs,ierr)
40    continue
      end 
