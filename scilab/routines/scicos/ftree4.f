      subroutine ftree4(vec,nb,nd,nnd,typr,outoin,outoinptr,
c     Copyright INRIA
     $     r1,r2,nr)
c make sure nb > 0
      integer vec(*),nb,nd(*),outoin(*),outoinptr(*)
      integer r1(*),r2(*),fini,typr(*)
c
      nr=0
      do 150 j=1,nb-1
         fini=1
         do 100 i=1,nb
            if(vec(i).gt.-1) then
               if(outoinptr(i+1)-outoinptr(i).ne.0) then
                  do 60 k=outoinptr(i),outoinptr(i+1)-1
                     ii=outoin(k)
                     if(typr(ii).eq.1) then
                        nprt=outoin(k+outoinptr(nb+1)-1)
                        if(nd(nprt+1+(ii-1)*nnd).eq.0) then
                           nr=nr+1
                           r1(nr)=ii
                           r2(nr)=nprt
                           fini=0
                           vec(ii)=0
                           nd(nprt+1+(ii-1)*nnd)=1
                        endif
                     endif
 60               continue
               endif
            endif
 100     continue
         if(fini.eq.1) goto 200
 150  continue
 200  continue
      return
      end

