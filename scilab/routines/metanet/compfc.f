      subroutine compfc(inf,lp1,ls1,m,n,nc,nfcomp,
     &     nn,num,pw,pile)
      implicit integer (a-z)
      dimension lp1(*),ls1(m),pw(n),nn(n),nfcomp(n),
     &     pile(n),num(n),inf(n)
      do 10 i=1,n
         pw(i)=0
         nn(i)=lp1(i+1)-lp1(i)
         num(i)=0
         nfcomp(i)=0
 10   continue
      i0=1
      nc=1
 100  call cfc(i0,inf,lp1,ls1,m,n,nc,nfcomp,
     &     nn,num,pw,pile)
      do 210 ii=1,n
         if(nfcomp(ii).gt.0) goto 210
         i0=ii
         goto 100
 210  continue
      nc=nc-1
      end
