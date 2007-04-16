c     ====================================       
      subroutine icomp(xe1,ue1,f1,g1,h1,k1,l1,br1,pas1,nn1,pp,ii)
c     ====================================             
      real br1(nn1),pas1,h1(2),l1(2),xe1(2),k1(2),pp(7)
      real f1(2,2),g1(2),ue1
      integer nn1,i,j,ii 
      real br(1000),pas,ppr,ppk,ppa,ppb,ppm,pps,ppl,
     $     h(2),l(2),k(2),xe(2),f(2,2),g(2),ue
      integer nn 
      common / bcompc/ br,pas,ppr,ppk,ppa,ppb,ppm,pps,ppl,
     $     h,l,k,xe,f,g,ue,nn
     
      ppr=pp(1)
      ppk=pp(2)
      ppa=pp(3)
      ppb=pp(4)
      ppm=pp(5)
      pps=pp(6)
      ppl=pp(7) 
      
      do 771 i=1,nn1
        br(i)=br1(i)
c      end do

 771  continue
      pas=pas1
      nn=nn1
      do 772 i=1,2
      do 773 j=1,2
        f(i,j)=f1(i,j)
c      end do

 773  continue
        h(i)=h1(i)
        l(i)=l1(i)
        xe(i)=xe1(i)
        k(i)=k1(i)
        g(i)=g1(i)
        
 772  continue
      ue=ue1
      if (ii.eq.1) then 
         write(06,*) 'pp r,k,a,b,m,s,l', ppr,ppk,ppa,ppb,ppm,pps,ppl
         write(06,*) 'pas=',pas,' n=',nn
         write(06,*) 'h',(h(i),i=1,2)
         write(06,*) 'f',((f(i,j),i=1,2),j=1,2)
         write(06,*) 'l',(l(i),i=1,2)
         write(06,*) 'xe',(xe(i),i=1,2)
         write(06,*) 'k',(k(i),i=1,2)
         write(06,*) 'g',(g(i),i=1,2)
         write(06,*) 'u',ue
      endif

      return
      end
c     ====================================       
