      subroutine carete (nu,nt,ns,ma,na,naf,k,lma)
c--------------------------------------------------------------
c   but: construire les aretes d'une triangulation
c--------------------------------------------------------------
c in:
c       nu(1:3,1:nt) sommets des triangle
c       nt : nb de triangle
c       ns : nb de sommet
c       lma : nb de mot du tableau ma > 3*na 
c               et na = nt+ns-1 + nb trou dans l'ouvert 
c out:      
c     ma(1:3,1:na)   tableau des aretes du maillage
c                   (ma(1,i),ma(2,i))  2 sommets de l'arete i
c                    ma(3,i) = 0 => l'arete i est interne 
c                              1 => l'arete i est frontiere
c     na  :  nb d'arete                                
c     naf : nb d'arete frontiere
c
c  work
c     k(1:ns) tableau de travail 
c--------------------------------------------------------------
      dimension nu(3,nt),ma(3,1),k(ns)
      dimension ka(2,3)
      data ka/1,2,2,3,3,1/
      na=0
      do 10 i=1,ns
10      k(i)=0
      do 100 it=1,nt
       do 100 is=1,3
        k1=nu(ka(1,is),it)
        k2=nu(ka(2,is),it)
        kx=max(k1,k2)
        ki=min(k1,k2)
        ip=k(kx)
        ip1=0
20       if(ip.eq.0) goto 60
         ip1=ip
         ip=ma(3,ip)
         if(ma(1,ip1).ne.ki.and.ma(2,ip1).ne.ki) goto 20
         ma(1,ip1)=-ma(1,ip1)
         goto 100
60      continue
        if(lma.le.na*3) goto 10000
        na=na+1
        ma(1,na)=k1
        ma(2,na)=k2
        ma(3,na)=0
        if(ip1.eq.0) then
         k(kx)=na
         else
         ma(3,ip1)=na
         end if
100   continue
      naf=0
      do 200 ia=1,na
       if(ma(1,ia).lt.0) then
        ma(1,ia)=-ma(1,ia)
        ma(3,ia)=0
        else
        ma(3,ia)=1
        naf=naf+1
        end if
200   continue
      lma=na*3
      return
10000 continue
c     call tilt CLG
      call erro('Problem in triangulation')
      return
      end
