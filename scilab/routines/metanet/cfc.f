      subroutine cfc(i0,inf,lp1,ls1,m,n,nc,nfcomp,
     &     nn,num,pw,pile)
      implicit integer (a-z)
      dimension lp1(*),ls1(m),pw(n),nn(n),nfcomp(n),
     &     inf(n),pile(n),num(n)
      pw(i0)=i0
      k=1
      num(i0)=k
      inf(i0)=k
      i=i0
      np=1
      pile(np)=i0
 100  if(nn(i).eq.0) goto 200
      ll=lp1(i)+nn(i)-1
      j=ls1(ll)
      nn(i)=nn(i)-1
      if(pw(j).lt.0) goto 130
      if(pw(j).gt.0) goto 120
      pw(j)=i
      k=k+1
      num(j)=k
      np=np+1
      pile(np)=j
      inf(j)=num(j)
      i=j
      goto 100
 120  inf(i)=min0(num(j),inf(i))
      goto 100
 130  if(nfcomp(j).ne.0) goto 140
      inf(i)=min0(inf(i),inf(j))
 140  goto 100
 200  if(inf(i).ne.num(i)) goto 220
 205  if(pile(np).eq.i) goto 210
      j=pile(np)
      nfcomp(j)=nc
      np=np-1
      goto 205
 210  nfcomp(i)=nc
      np=np-1
      nc=nc+1
      if(i.eq.i0) goto 300
 220  j=pw(i)
      pw(i)=-pw(i)
      inf(j)=min0(inf(j),inf(i))
      i=j
      goto 100
 300  pw(i0)=-pw(i0)
      end
