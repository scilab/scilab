      subroutine intnorm(fname)
c     -------------------------------
c     norm of matrix or vector
c     -------------------------------
      character*(*) fname
      character*(4) type
      INCLUDE '../stack.h'
      logical checkrhs,checklhs,getsmat,getscalar,cremat,getmat
      logical inf,fro 
      integer gettype,itype,topk,isanan
      double precision infinity
      double precision p,wnrm2
      double precision dnrm2,dasum,dlamch,dres,t
      ires=0
      inf = .false.
      fro = .false.
      p=2.0d+0
      topk=top
      if (.not.checklhs(fname,1,1)) return
      if (.not.checkrhs(fname,1,2)) return
c     second argument : optional 'inf','fro' or p ( p: double 
c                       or %inf or %nan )
c     --------------------------------
      if ( rhs.eq.2 ) then 
         itype=gettype(topk)
         if ( itype.eq.10 ) then 
            if(.not.getsmat(fname,topk,top,m2,n2,1,1,lr2,nlr2))return
            call cvstr(min(nlr2,3),istk(lr2),type,1)
            if ( type(1:1).eq.'i') then 
               inf = .true. 
            else if ( type(1:1).eq.'f') then 
               fro = .true.
            else
               buf=fname//' : second argument string inf or fro '
               call error(999)         
               return
            endif
         else if (itype.eq.1) then 
            if(.not.getscalar(fname,topk,top,lr2))return
            p=stk(lr2)
         else 
            buf=fname
     $           //' : second argument string inf or fro or integer '
            call error(999)         
            return
         endif
         top=top-1
      endif
c     the first argument matrix or vector  
c     ---------------------------------
      itype = gettype(top) 
      if ( itype.ne.1 ) then 
         call putfunnam('norm',top)
         top=topk
         fun=-1
         return
      endif
      if(.not.getmat(fname,topk,top,it1,m,n,lr1,lc1)) return      
      mn = m*n
      if (mn.eq.0.or. m.eq.1.or.n.eq.1) then 
c     ==============================vector case 
         if (mn.eq.0) then 
            dres = 0.0d+0
         else if ( isanan(p).eq.1) then 
	    dres = p
         else if (p.gt.dlamch('o').or.inf) then 
            if (it1.eq.1) call vpythag(mn,stk(lr1),stk(lc1))
            i = idamax(mn,stk(lr1),1)
            dres= abs(stk(lr1+i-1))
         else if ( p.eq.1.0d+0) then
            if (it1.eq.1) call vpythag(mn,stk(lr1),stk(lc1))
            dres = dasum(mn,stk(lr1),1)
         else if (fro.or. p.eq.2.0d+0) then 
            if (it1.eq.1) then 
               dres=  wnrm2(mn,stk(lr1),stk(lc1),1)
            else
               dres=  dnrm2(mn,stk(lr1),1)
            endif
         else
            if (it1.eq.1) call vpythag(mn,stk(lr1),stk(lc1))
            i=idamax(mn,stk(lr1),1)+lr1-1
            dres =abs(stk(i))
            if ( dres .ne.0.0d+0) then 
               t=0.0d+0
               do 12 i=0,mn-1
                  t = t + (abs(stk(lr1+i))/dres)** p
 12            continue
               if (p.ne.0.0d+0) then 
                  dres = dres*t**(1.0d+0/p)
               else
c     Must return %inf : 
                  dres = infinity(0.0d0)
               endif
            endif
         endif
         if (.not.cremat(fname,top,0,1,1,lr1,lc1)) return
         stk(lr1)=dres
      else
c     ==============================Matrix case 
         if ( isanan(p).eq.1) then 
            dres = p
         else if (p.gt.dlamch('o').or.inf) then 
            if (it1.eq.1) call vpythag(mn,stk(lr1),stk(lc1))
            dres = 0.0d+0
            do 45 i = 1, m
               li = lr1+i-1
               t = dasum(n,stk(li),m)
               dres = max(dres,t)
 45         continue
         else if (fro) then
            if (it1.eq.1) then 
               dres=  wnrm2(mn,stk(lr1),stk(lc1),1)
            else
               dres=  dnrm2(mn,stk(lr1),1)
            endif
         else if ( p.eq.1.0d+0) then
            if (it1.eq.1) call vpythag(mn,stk(lr1),stk(lc1))
            dres=0.0d+0
            do 48 j = 1, n
               lj = lr1+(j-1)*m
               t = dasum(m,stk(lj),1)
               dres = max(dres,t)
 48         continue
         else if ( p.eq.2.0d+0) then 
            m1=min(m+1,n)
            if (.not.cremat(fname,topk+1,it1,1,m1,ld,lcd)) return            
            if (.not.cremat(fname,topk+2,it1,1,n,l1,l1c)) return            
            if (.not.cremat(fname,topk+3,it1,1,m,l2,l2c)) return            
            ierr=1
            if(it1.eq.0) then 
               call dsvdc(stk(lr1),m,m,n,stk(ld),stk(l1),
     $              t,1,t,1,stk(l2),0,ierr)
            else
               call wsvdc(stk(lr1),stk(lc1),m,m,n,stk(ld),stk(lcd),
     $              stk(l1),stk(l1c),t,t,1,t,t,1,
     $              stk(l2),stk(l2c),0,ierr)
            endif
            if (ierr.gt.1) call msgs(3,ierr)
            dres = stk(ld)
         else
            call error(23)
            return
         endif
         if (.not.cremat(fname,top,0,1,1,lr1,lc1)) return
         stk(lr1)=dres
      endif
      return
      end


