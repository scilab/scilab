      subroutine mintyq(b,c,coderf,delta,ex,i1,infr,j1,la2,lp2,ma,     
     &     mm,n,or,phibar,phir,pile,piv,predw,
     &     sufval,type,u0,waqc)
      implicit integer (a-z)
      dimension lp2(*),la2(mm)
      dimension or(ma),ex(ma),b(ma),c(ma)
      dimension type(ma),predw(n),pile(n)
      doubleprecision sufval(n),gamma1,gamma2,delta,eps
      doubleprecision sf,piv(n),waqc(ma),phir(ma),cumul,infr
      doubleprecision phibar(ma)
      coderf=0
      cumul=0.
      do 50 i=1,n
         predw(i)=0
         sufval(i)=infr
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
         if(type(u).eq.1.or.type(u).eq.8)go to 110
         if(type(u).eq.11.or.type(u).eq.51)go to 110
         if(type(u).eq.4.or.type(u).eq.41)go to 115
         if(type(u).eq.6.or.type(u).eq.9)go to 115
         if(type(u).eq.7)go to 112
         go to 140
 110     continue
         gamma1=waqc(u)*(phir(u)+delta*.5-phibar(u))
         sf=gamma1-piv(ex(u))+piv(or(u))+cumul
         if(sf.ge.sufval(jj)) goto 140
         predw(jj)=-u
         sufval(jj)=sf
         go to 140
 112     continue
         gamma2=waqc(u)*(phir(u)-delta*.5-phibar(u))
         sf=piv(ii)-piv(jj)+gamma2+cumul
         if(sf.gt.sufval(jj)) go to 140
         sufval(jj)=sf
         predw(jj)=-u
         go to 140
 115     continue
         top=top+1
         pile(top)=jj
         predw(jj)=u
         sufval(jj)=cumul
         goto 140
 120     continue
         if(type(u).eq.3.or.type(u).eq.8)go to 135
         if(type(u).eq.11.or.type(u).eq.41)go to 135
         if(type(u).eq.5.or.type(u).eq.51)go to 130
         if(type(u).eq.7.or.type(u).eq.10)go to 130
         if(type(u).eq.6)go to 125
         go to 140
 125     continue
         gamma1=waqc(u)*(phir(u)+delta*.5-phibar(u))
         sf=piv(ii)-piv(jj)-gamma1+cumul
         if(sf.gt.sufval(jj)) go to 140
         sufval(jj)=sf
         predw(jj)=-u
         go to 140
 130     continue
         top=top+1
         pile(top)=jj
         predw(jj)=u
         sufval(jj)=cumul
         go to 140
 135     continue
         gamma2=waqc(u)*(phir(u)-delta*.5-phibar(u))
         sf=piv(ex(u))-piv(or(u))-gamma2+cumul
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
      do 250 i=1,n
         if(predw(i).ge.0)go to 250
         if(sufval(i).gt.eps)go to 250
         umin=-predw(i)
         if(type(umin).eq.6.or.type(umin).eq.7)goto 230
         if(sufval(i).ge.eps)goto 250
 230     eps=sufval(i)
         imin=i
 250  continue
      if(eps.lt.infr) goto 260
      coderf = 2
      return
 260  continue
      umin=-predw(imin)
      if(type(umin).ne.6.and.type(umin).ne.7)goto 270
      cumul=eps
      goto 300
 270  continue
      predw(imin)=-predw(imin)
      cumul=eps
      top=top+1
      if(top.le.n)goto 280
      coderf=1
      return
 280  pile(top)=imin
      goto 100      
 300  continue
      do 350 i=1,n
         if(predw(i).gt.0)go to 330
         piv(i)=piv(i)+cumul
         go to 350
 330     piv(i)=piv(i)+sufval(i)
 350  continue
      end
