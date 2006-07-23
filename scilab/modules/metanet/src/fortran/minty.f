      subroutine minty(coderk,ex,gamma,i1,infr,j1,la2,lp2,ma,
     &     mm,n,or,pile,piv,predw,sufval,type,u0)
      implicit integer (a-z)
      dimension lp2(*),la2(mm),or(ma),ex(ma)
      dimension type(ma),predw(n),pile(n)
      doubleprecision sufval(n),cumul,infr
      doubleprecision gamma(ma),sf,piv(n),eps
      coderk=0
      cumul=0.
      do 50 i=1,n
         predw (i)=0
         sufval(i)=infr
         pile(i)=0
 50   continue
      predw(j1)=u0
      sufval(j1)=cumul
      top=1
      bottom=0
      pile(top)=j1
 100  continue
      bottom=bottom+1
      ii=pile(bottom)
      if(lp2(ii).eq.lp2(ii+1))goto 145
      do 140 ll=lp2(ii),lp2(ii+1)-1
         u=la2(ll)
         if(ii.eq.ex(u)) goto 105
         jj=ex(u)
         if(predw(jj).gt.0) go to 140
         goto 108
 105     jj=or(u)
         if(predw(jj).gt.0) go to 140
         goto 120
 108     continue
         goto (110,115,140,115,140,115,112,110,115,140),type(u)
 110     continue
         sf=gamma(u)-piv(jj)+piv(ii)+sufval(ii)
         if(sf.ge.sufval(jj)) go to 140
         predw(jj)=-u
         sufval(jj)=sf
         go to 140
 112     continue
         sf=piv(ii)-piv(jj)+gamma(u)+sufval(ii)
         if(sf.gt.sufval(jj)) go to 140
         predw(jj)=-u
 113     sufval(jj)=sf
         go to 140
 115     continue
         top=top+1
         if(top.le.n) goto 119
         coderk=1
         return
 119     pile(top)=jj
         predw(jj)=u
         sufval(jj)=cumul
         goto 140
 120     continue
         goto (140,130,135,140,130,125,130,135,140,130),type(u)
 125     continue
         sf=piv(ii)-piv(jj)-gamma(u)+sufval(ii)
         if(sf.gt.sufval(jj)) go to 140
         predw(jj)=-u
 128     sufval(jj)=sf
         go to 140
 130     continue
         top=top+1
         if(top.le.n) goto 132
         coderk=1
         return
 132     pile(top)=jj
         predw(jj)=u
         sufval(jj)=cumul
         go to 140
 135     continue
         sf=piv(ii)-piv(jj)-gamma(u)+sufval(ii)
         if(sf.ge.sufval(jj)) go to 140
         sufval(jj)=sf
         predw(jj)=-u
         go to 140
 140  continue
 145  continue
      if(predw(i1).gt.0) goto 300
      if(bottom.ge.top) goto 200
      goto 100
 200  continue
      eps=infr
      do 240 i=1,n
         if(predw(i).ge.0)go to 240
         if(sufval(i).gt.eps)go to 240
         umin=-predw(i)
         if(type(umin).eq.6.or.type(umin).eq.7)goto 230
         if(sufval(i).ge.eps)goto 240
 230     eps=sufval(i)
         imin=i
 240  continue
      if(eps.lt.infr) goto 245
      coderk = 2
      return
 245  continue
      umin=-predw(imin)
      if(type(umin).ne.6.and.type(umin).ne.7)goto 250
      cumul=eps
      goto 300
 250  continue
      predw(imin)=-predw(imin)
      cumul=eps
      top=top+1
      if(top.le.n)goto 260
      coderk=1
      return
 260  pile(top)=imin
      go to 100
 300  continue
      do 350 i=1,n
         if(predw(i).gt.0)go to 330
         piv(i)=piv(i)+cumul
         go to 350
 330     piv(i)=piv(i)+sufval(i)
 350  continue
      end
