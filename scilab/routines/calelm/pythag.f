C/MEMBR ADD NAME=PYTHAG,SSI=0
c     Copyright INRIA
      double precision function pythag(a,b)
c!but
c     pythag calcule (a**2+b**2)**(1/2), par une methode iterative
c!liste d'appel
c     double precision function pythag(a,b)
c     double precision a,b
c!
      double precision a,b,dlamch
      double precision p,q,r,s,t
c     --------testing Nans 
      if (isanan(a).eq.1) then 
         pythag=a 
         return
      endif
      if (isanan(b).eq.1) then 
         pythag=b
         return
      endif
c     --------testing Inf 
      if ( a.ge.dlamch('o').or.-a.ge.dlamch('o')) then 
         pythag = abs(a)
         return
      endif
      if ( b.ge.dlamch('o').or.-b.ge.dlamch('o')) then 
         pythag = abs(b)
         return
      endif
c     --------generic case 
      p = max(abs(a),abs(b))
      q = min(abs(a),abs(b))
      if (q .eq. 0.0d+0) go to 20
   10 r = (q/p)**2
      t = 4.0d+0 + r
      if (t .eq. 4.0d+0) go to 20
      s = r/t
      p = p + 2.0d+0*p*s
      q = q*s
      go to 10
   20 pythag = p
      return
      end
