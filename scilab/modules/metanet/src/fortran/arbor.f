      subroutine arbor(alphi,beta,f,g,i0,ind,la1,lp1,ls1,m,
     &     n,nndim,pred,w,z,zsom)
      implicit integer (a-z)
      dimension la1(m),lp1(*),ls1(m),pred(nndim),
     &     f(nndim),g(nndim)
      doubleprecision w(m),z(nndim),zsom(nndim)
      dimension alphi(nndim),beta(nndim),ind(nndim)
      doubleprecision wr,infr
      if (i0 .lt. 0 .or. i0 .gt. n) then
         call erro('bad internal node number')
         return
      endif
      infr=10.e6
      do 10 i=1,2*n
         pred(i)=0
         z(i)=infr
         zsom(i)=0
         f(i)=0
         g(i)=0
         alphi(i)=i
         beta(i)=i
 10   continue
      nnew=1
      ntot=n
 100  continue
      do 110 i=1,n
         do 110 ll=lp1(i),lp1(i+1)-1
            u=la1(ll)
            j=ls1(ll)
            if(j.eq.i0) goto 110
            ii=beta(i)
            jj=beta(j)
            if(ii.eq.jj) goto 110
            if(jj.lt.nnew) goto 110
            wr=w(u)-zsom(j)
            if(wr.ge.z(jj)) goto 110
            z(jj)=wr
            pred(jj)=ii
            f(jj)=i
            g(jj)=j
 110     continue
         do 210 j=1,ntot
            ind(j)=0
 210     continue
         k=ntot
         kc=0
         do 240 j=1,ntot
            if(j.eq.i0) goto 240
            if(ind(j).ne.0) goto 240
            if(j.ne.beta(j)) goto 240
            kc=kc+1
            ind(j)=kc
            jc=j
 215        continue
            i=pred(jc)
            if(i.eq.0) then
               do 1234,ii=1,n
                  pred(ii)=0
 1234          continue
               return
            endif
            if(i.eq.i0)goto 240
            i=beta(i)
            if(ind(i).eq.kc) goto 220
            if(ind(i).ne.0)goto 240
            ind(i)=kc  
            jc=i
            goto 215
 220        continue
            k=k+1
            alphi(k)=k
            beta(k)=k
            ind(k)=kc
            ideb=i
 230        continue
            i=pred(i) 
            i=beta(i)
            alphi(i)=k
            if(i.ne.ideb) goto 230
 240     continue
         if(k.eq.ntot) goto 600
         nnew=ntot + 1
         ntot=k
         do 310 j=1,ntot
            jj=beta(j)
            beta(j)=alphi(jj)
 310     continue
         do 410 i=1,ntot
            zsom(i)=infr
 410     continue
         do 470 i=1,n
            if(i.eq.i0)goto 470
            zsom(i)=z(i)
            ii=i
 420        i1=alphi(ii)
            if(alphi(i1).eq.i1) goto 435
            if(zsom(i1).lt.infr) goto 430
            zsom(i1)=zsom(ii)+z(i1)
            ii=i1
            goto 420
 430        zsom(i)=zsom(i)+zsom(i1)
            goto 440
 435        zsom(i)=zsom(ii)
 440        continue
            ii=i
            ilast=i1
 450        i1=alphi(ii)
            if(i1.eq.ilast) goto 470
            zsom(i1)=zsom(ii)-z(ii)
            ii=i1
            goto 450
 470     continue
         goto 100
 600     continue
         k=ntot
         if(k.le.n) goto 999
 610     continue
         j=g(k)
         j1=j
 620     continue
         pred(j1)=f(k)
         g(j1)=g(k)
         f(j1)=f(k)
         j2=alphi(j1)
         if(j2.eq.j1) goto 630
         j1=j2
         goto 620
 630     continue
         k=k-1
         if(k.le.n) goto 999
         goto 610
 999     continue
         end
