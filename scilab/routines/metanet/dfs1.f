      subroutine dfs1(i0,l,lp1,ls1,m,n,ncomp,nn,pw)
      implicit integer (a-z)
      dimension lp1(*),ls1(m),pw(n),nn(n),ncomp(n)
      pw(i0)=i0
      i=i0
 100  if(nn(i).eq.0) goto 200
      ll=lp1(i)+nn(i)-1
      j=ls1(ll)
      nn(i)=nn(i)-1
      if(pw(j).ne.0) goto 100
      pw(j)=i
      ncomp(j)=l
      i=j
      goto 100
 200  if(i.eq.i0) goto 1000
      i=pw(i)
      goto 100
 1000 return
      end
