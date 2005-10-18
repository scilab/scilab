      double precision function mpddot(n,dx,incx,dy,incy)
      include 'mpstack.h'
c
c     forms the max-plus dot product of two vectors.(modified SG)
c
c     uses unrolled loops for increments equal to one.
c     jack dongarra, linpack, 3/11/78.
c     modified 12/3/93, array(1) declarations changed to array(*)
c
      double precision dx(*),dy(*),dtemp
      double precision u,v
      integer i,incx,incy,ix,iy,m,mp1,n
c    
      mpddot=PCZERO
      dtemp=PCZERO
      if(n.le.0)return
      if(incx.eq.1.and.incy.eq.1)go to 20
c
c        code for unequal increments or equal increments
c          not equal to 1
c
      ix = 1
      iy = 1
      if(incx.lt.0)ix = (-n+1)*incx + 1
      if(incy.lt.0)iy = (-n+1)*incy + 1
      do 10 i = 1,n
        u=dx(ix)
        v=dy(iy)
        if(u.ne.PCZERO) then
           if(v.ne.PCZERO) then
              if((u.ne.PCTOP).and.(v.ne.PCTOP)) then
                 dtemp = max(dtemp , u+v)
              else
                 dtemp =PCTOP
              endif
           endif
        endif
        ix = ix + incx
        iy = iy + incy
   10 continue
      mpddot = dtemp
      return
c
c        code for both increments equal to 1
c
c
c        clean-up loop
c
   20 do 50 i = 1,n
         u=dx(i)
         v=dy(i)
         if(u.ne.PCZERO) then
            if(v.ne.PCZERO) then
               if((u.ne.PCTOP).and.(v.ne.PCTOP)) then
                  dtemp = max(dtemp , u+v)
               else
                  dtemp =PCTOP
               endif   
            endif
         endif
   50 continue
   60 mpddot = dtemp
      return
      end
