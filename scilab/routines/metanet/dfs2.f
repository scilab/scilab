      subroutine dfs2(i0,lp1,ls1,m,n,nn,num,pw,fin,nart,iart)
      implicit integer (a-z)
      dimension lp1(*),ls1(m),nn(n),num(n),pw(n),fin(n),nart(n)
      if (i0 .lt. 0 .or. i0 .gt. n) then
         call erro('bad internal node number')
         return
      endif
      iart=1
      do 10 i=1,n
         nart(i)=0
         pw(i)=0
         nn(i)=lp1(i+1)-lp1(i)
         num(i)=0
         fin(i)=100000
 10   continue
      pw(i0)=i0
      k=1
      num(i0)=k
      i=i0
 100  if(nn(i).eq.0) goto 200
      ll=lp1(i)+nn(i)-1
      j=ls1(ll)
      nn(i)=nn(i)-1
      if(pw(j).ne.0) goto 101
      fin(i)=min(fin(i),num(i))
      pw(j)=i
      k=k+1
      num(j)=k
      i=j
 101  fin(i)=min(fin(i),num(j))
      go to 100
 200  if(i.eq.i0) go to 1000
      q=fin(i)
      i=pw(i)
      fin(i)=min(q,fin(i))
      if(fin(i).ne.num(i)) goto 100
      if(fin(i).eq.1) goto 100
      nart(iart)=i
      iart=iart+1
      goto 100
 1000 continue
      return
      end
