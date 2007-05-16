      subroutine feval
C     --------------------------------------------
C     feval(x1,x2,external) -> external(x1(i),x2(j))
C     feval(x1,external)    -> external(x1(i))
c      implicit undefined (a-z)
c     Copyright ENPC (Jean-Philippe Chancelier 
      include '../stack.h'
      character*(5) fname
      character*(nlgh+1)   ename
      integer m1,n1,lb,m2,n2,la,i,j,nn,lr,lc,lb1,lbc1,lrr,lcr
      integer topk,itype,kx1top,kx2top,lr1,kfeval,gettype
      double precision x1,x2,fval(2)
      external setfeval 
      logical type,getexternal,getrmat,cremat
C     External names (colname), Position in stack (coladr), type (coltyp)
      common / fevalname / ename
      common / fevaladr / kfeval,kx1top,kx2top
      common / fevaltyp / itfeval
      fname='feval'
      if(rhs.lt.2) then
         call error(39)
         return
      endif
      itype=0
      type=.false.
      kfeval=top
      topk=top
      if (.not.getexternal(fname,topk,top,ename,type,
     $     setfeval)) return
      itfeval=gettype(top)
      top=top-1
      if (.not.getrmat(fname,topk,top,m1,n1,lb))  return
      x2=stk(lb)
      nn=1
      if (rhs.eq.3) then 
         nn=2
         top=top-1
         if (.not.getrmat(fname,topk,top,m2,n2,la))  return
         x1=stk(la)
      endif
C     place pour le resultat si on a deux arguments 
      top=topk+1
      if (nn.eq.2) then 
         if (.not.cremat(fname,top,1,m1*n1,m2*n2,lr,lc)) return
      else
         if (.not.cremat(fname,top,0,m1,n1,lb1,lbc1)) return
      endif
c     external scilab
C     une variable de taille 1 qui permet de gerer le type d'argument
      top=top+1
      kx1top=top
      if (.not.cremat(fname,top,0,1,1,lrr,lcr)) return
      if (nn.eq.2) then 
         top=top+1
         kx2top=top
         if (.not.cremat(fname,top,0,1,1,lrr,lcr)) return
      endif
      if(type) then 
         if (nn.eq.2) then 
            do 182 i=1,m2*n2
               do 192 j=1,m1*n1
                  call ffeval(nn,stk(la+i-1),stk(lb+j-1),
     $                 fval,itype,ename)
                  if(err.gt.0) return
                  stk(lr+i-1+m2*n2*(j-1))=fval(1)
                  if (itype.eq.1) stk(lc+i-1+m2*n2*(j-1))=fval(2)
 192           continue
 182        continue
         else
            do 183 i=1,m1*n1
               call ffeval(nn,stk(lb+i-1),1.0d0,fval,itype,ename)
               if(err.gt.0) return
               stk(lb+i-1)=fval(1)
               if (itype.eq.1) stk(lb1+i-1)=fval(2)
 183        continue
         endif
      else
         if (nn.eq.2) then 
            do 172 i=1,m2*n2
               do 174 j=1,m1*n1
                  call bfeval(nn,stk(la+i-1),stk(lb+j-1),
     $                 fval,itype,ename)
                  if(err.gt.0.or.err1.gt.0) return
                  stk(lr+i-1+m2*n2*(j-1))=fval(1)
                  if (itype.eq.1) stk(lc+i-1+m2*n2*(j-1))=fval(2)
 174           continue
 172        continue
         else
            do 173 i=1,m1*n1
               call bfeval(nn,stk(lb+i-1),1.0D0,fval,itype,ename)
               if(err.gt.0.or.err1.gt.0) return
               stk(lb+i-1)=fval(1)
               if (itype.eq.1) stk(lb1+i-1)=fval(2)
 173        continue
         endif
      endif
 162  continue
      top=topk-rhs+1
      if (nn.eq.2) then 
         if (.not.cremat(fname,top,itype,m2*n2,m1*n1,lr1,lc)) return
         call unsfdcopy(m1*n1*m2*n2*(itype+1),stk(lr),1,stk(lr1),1)
      else
         if (itype.eq.1)then 
            if (.not.cremat(fname,top,itype,m1,n1,lr,lc)) return
            call unsfdcopy(m1*n1,stk(lb1),1,stk(lc),1)
         endif
      endif
      return
      end
