      subroutine dfs(i0,lp1,ls1,m,n,nn,num,pw)
      implicit integer (a-z)
      dimension lp1(*),ls1(m),pw(n),num(n),nn(n)
      if (i0 .lt. 0 .or. i0 .gt. n) then
         call erro('bad internal node number')
         return
      endif
      do 10 i=1,n
         pw(i)=0
         nn(i)=lp1(i+1)-lp1(i)
         num(i)=0
 10   continue
      pw(i0)=i0
      k=1
      num(i0)=k
      i=i0
 100  if(nn(i).eq.0) goto 200
      ll=lp1(i)+nn(i)-1
      j=ls1(ll)
      nn(i)=nn(i)-1
      if(pw(j).ne.0) goto 100
      pw(j)=i
      k=k+1
      num(j)=k
      i=j
      go to 100
 200  if(i.eq.i0) go to 1000
      i=pw(i)
      goto 100
 1000 return
      end
