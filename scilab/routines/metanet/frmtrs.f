      subroutine frmtrs(cfcl,cfcp,cw,inf,lp1,lpft,ls1,lsft,m,
     &     mftdim,n,nc,nfcomp,nn,num,p,p1,pile,s,
     &     sdim,som,suc)
      implicit integer (a-z)
      dimension lp1(*),cfcp(*),ls1(m)
      dimension p(n),nn(n),nfcomp(n)
      dimension pile(n),num(n),inf(n),cfcl(n)
      dimension som(*),suc(m),p1(*),s(sdim),cw(n)
      dimension lpft(*),lsft(mftdim)
      n1=n+1
      do 10 i=1,n1
         cfcp(i)=1
 10   continue
      call compfc(inf,lp1,ls1,m,n,nc,nfcomp,nn,
     &     num,p,pile)
      do 20 i=1,nc
 20      som(i)=0
         do 21 j=1,n
            i=nfcomp(j)
            som(i)=som(i)+1
 21      continue
         cfcp(1)=1
         do 22 i=1,nc
            cfcp(i+1)=cfcp(i)+som(i)
 22      continue
         do 23 i=1,nc
 23         som(i)=cfcp(i)
            do 24 j=1,n
               i=nfcomp(j)
               ll=som(i)
               cfcl(ll)=j
               som(i)=som(i)+1
 24         continue
            do 30 i=1,nc+1
 30            som(i)=1
               do 80 ncfc=1,nc
                  nsuc=0
                  ind1=cfcp(ncfc)
                  ind2=cfcp(ncfc+1)-1
                  if(ind2.lt.ind1) goto 80
                  do 70 l=ind1,ind2
                     i=cfcl(l)
                     ind3=lp1(i)
                     ind4=lp1(i+1)-1
                     if(ind4.lt.ind3) goto 70
                     do 60 ll=ind3,ind4
                        j=ls1(ll)
                        ncfc1=nfcomp(j)
                        if(ncfc.eq.ncfc1) goto 60
                        if(nsuc.eq.0) goto 50
                        ind5=som(ncfc)+nsuc-1
                        do 40 lll=som(ncfc),ind5
                           if(suc(lll).eq.ncfc1) goto 60
 40                     continue
 50                     continue
                        ind6=som(ncfc)+nsuc
                        suc(ind6)=ncfc1
                        nsuc=nsuc+1
 60                  continue
 70               continue
                  som(ncfc+1)=som(ncfc)+nsuc
 80            continue
               nq=nc
               mq=som(nc+1)-1
               call ftrans(cw,m,mq,n,nq,p1,s,sdim,som,suc)
               do 301 i=1,n+1
                  lpft(i)=1
 301           continue
               do 330 i=1,n
                  ids=lpft(i)
                  nbsom=0
                  ncfc=nfcomp(i)
                  ind1=cfcp(ncfc+1)-1
                  do 302 i1=cfcp(ncfc),ind1
                     is=ids+nbsom
                     if(cfcl(i1).eq.i) goto 302
                     lsft(is)=cfcl(i1)
                     nbsom=nbsom+1
 302              continue
                  ind2=p1(ncfc+1)-1
                  do 320 ics=p1(ncfc),ind2
                     nscfc=s(ics)
                     ind3=cfcp(nscfc+1)-1
                     do 310 i1=cfcp(nscfc),ind3
                        is=ids+nbsom
                        lsft(is)=cfcl(i1)
                        nbsom=nbsom+1
 310                 continue
 320              continue
                  lpft(i+1)=lpft(i)+nbsom
 330           continue
               end
