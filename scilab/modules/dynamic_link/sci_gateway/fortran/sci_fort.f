c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine intfort(fname)
c     =====================================
c     interface for the scilab fort command 
c     =====================================
      include 'stack.h'
cc      implicit undefined (a-z)
      character*(*) fname
      character*1   type
      logical getsmat,checkval,cresmat2,bufstore
      logical flag,getscalar ,getmat,getrhsvar,cremat,lcres
      integer gettype,sadr,iadr,top2,tops,topl,topk,loc

      parameter (fortname=24)
      character  name*25
      common /cinter/ name
      common /ibfu/ ibuf(intersiz*6)
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      call ref2val
      
      nbvars = 0
      flag=.false.
c     maximum number of variables ( see stack.h) 
c     
      namax=intersiz
      call iset(namax,0,ladr,1)
c     get a scilab string the fort function name 
      top2=top-rhs+1
      if(.not.getsmat(fname,top,top2,m1,n1,1,1,lrc,nc))return
      name=' '
      if ( nc.gt.fortname ) then 
         buf = fname // ' first argument must be of length < 24'
         call error(999)
         return
      endif
      call cvstr(nc,istk(lrc),name,1)
      name(nc+1:nc+1)=char(0)
C     Check the name in the <<fort>> table 
      call setinterf(name,irep)
      if ( irep.eq.1) then 
         buf = name
         call error(50)
         return
      endif
C     test the argument list : search for 'out' or 'sort'
c     ----------------------------------------------------
      do 81 i=1,rhs-1
         tops=top2+i
         itype = gettype(tops) 
         if ( itype.eq.10 ) then 
            if(.not.getsmat(fname,top,tops,m1,n1,1,1,lr1,nlr1)) return
            if(.not.checkval(fname,m1*n1,1)) return
            lbuf=1
            if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return
            if (buf(lbufi1:lbuff1).eq.'sort'.or.
     $           buf(lbufi1:lbuff1).eq.'out') goto 82 
         endif
 81   continue
c     'sort' or 'out' are not in the calling list we use the brief form 
C     we call the routine directly 
C     the routine must deal itself with the stack 
c     ----------------------------------------------------
      call interf1(name(1:nc))
      nbvars = 0
      return
c     Now the long form 
c     -----------------
 82   continue
c     for the long form the call to interf is hardcoded 
c     with a limit of 30 arguments
      namax=min(intersiz,30)
c     ie : number of input arguments 
      ie=(tops-1-top2)/3
c     computing iss : 
c     number of output variables described after the 'out'  string 
c     -----------------
      jjj=tops+1
      iss=0
 86   if ( jjj.gt.top) goto 861
      itype = gettype(jjj)
      if (itype.eq.1) then 
         if(.not.getmat(fname,top,jjj,it1,m,n,lr1,lc1)) return
         if( m*n.eq.2 ) then 
            jjj = jjj+3 
         else
            jjj = jjj+1 
         endif
         iss = iss+1
      else
         err= jjj-top2+1
         call cvname(ids(1,pt+1),fname,0)
         call error(81) 
         return
      endif
      goto 86 
 861  continue
      if( iss.gt.0 .and. lhs.gt.iss) then
         call error(41)
         return
      endif
      if( err.gt.0) return
      ieis=ie+iss
      if(ie.ne.0) then 
c     input arguments 
c     storing information in ibuf for each input variable
c     ibuf(6*(i-1)+ j) 
c       j=1,2,3,4,5,6 ==>[call position,adress of data,type, nrow,ncol
c         number-in-the-caling-stack]
c     -------------------------------------------------------------
         do 87 i=1,ie
            ir1=top2+1+3*(i-1)
            if(.not.getsmat(fname,top,ir1+2,m1,n1,1,1,lr1,nlr1)) return
            if( m1*n1.ne.1.or.nlr1.ne.1) then 
               buf = fname // ': argument must be "c","d","i",or "r"'
               call error(999)
               return
            endif
c           stack-position 
            ibuf(6+6*(i-1)) = ir1-top2+1
            call cvstr(nlr1,istk(lr1),type,1)
            ibuf(3+6*(i-1)) = ichar(type)
            if(.not.getscalar(fname,top,ir1+1,lr1))return
            ibuf(1+6*(i-1)) = int(stk(lr1))
            if(.not.getrhsvar(2+3*(i-1),type,m1,n1,lr1)) return
            ibuf(2+6*(i-1)) = lr1 
            ibuf(4+6*(i-1)) = m1
            ibuf(5+6*(i-1)) = n1
            if (type.eq.'d' ) then 
               ladr(ibuf(1+6*(i-1))) = lr1 
            else if (type.eq.'r') then 
               ladr(ibuf(1+6*(i-1))) = sadr(lr1)
            else if (type.eq.'i') then 
               ladr(ibuf(1+6*(i-1))) = sadr(lr1)
            else if (type.eq.'c') then 
               call cs2st(lr1,lr2)
               ladr(ibuf(1+6*(i-1))) = lr2
            endif

 87      continue
      endif
c     Output variables 
c     -------------------------------------------------------------
c     icre will output variables which need to be created
      icre=0
      jjj=tops+1
      if(iss.eq.0) goto 95
      do 94 i=1,iss
         ilm=iadr(lstk(jjj))
         if(istk(ilm+1)*istk(ilm+2).eq.1) then 
c           output variable is described by position only 
            ipla=int(stk(sadr(ilm+4)))
c           get the data of the associated input var 
            do 91 j=1,ie
               jj=6*(j-1)
               if(ibuf(jj+1).eq.ipla) then 
                  call icopy(6,ibuf(jj+1),1,ibuf(6*(ie+i-1)+1),1)
                  jjj=jjj+1
                  goto 94 
               endif
 91         continue
            buf = fname // ': output variable described by position'
     $          // ' must be an input variable'
            call error(999)
         else
c           explicit dimension of the output var 
            if(.not.getsmat(fname,top,jjj+2,m1,n1,1,1,lr1,nlr1)) return
            if( m1*n1.ne.1.or.nlr1.ne.1) then 
               buf = fname // ': argument must be "c","d","i",or "r"'
               call error(999)
               return
            endif
            call cvstr(nlr1,istk(lr1),type,1)
            if(.not.getscalar(fname,top,jjj+1,lr1))return
            ipla = int(stk(lr1))
            if(.not.getmat(fname,top,jjj,it1,m1,n1,lr1,lc1)) return
            if (m1*n1.ne.2) then 
               buf = fname 
     $              // 'Output argument dimension must be [nr,nc]'
               call error(999)
               return
            endif
            m1 = int(stk(lr1))
            n1 = int(stk(lr1+1))
c           maybe this output variable was also an input variable
c           we must check that dimensions and type are compatible 
            iecor=0
            do 910 j=1,ie
               jj=6*(j-1)
               if(ibuf(jj+1).eq.ipla) then 
                  iecor= j
                  goto 911
               endif
 910        continue
 911        continue 
            if ( iecor.eq.0 ) then 
c     .        we must create a new entry for an output variable 
               icre=icre+1
C     .        bug 2119 fix, create a variable of type "d" in any case
C     .        to avoid overlaping
               if (.not.cremat(fname,top+icre,0,m1,n1,lr1,lc1)) return
               if (type.eq.'d') then
                  lr2 = lr1
               else if (type.eq.'r'.or. (type.eq.'i')) then 
                  lr2=iadr(lr1)
               else if (type.eq.'c') then 
                  lr2 = 4*lr1-1
               endif
               ipos=6*(ie+i-1)
               ibuf(ipos+1) = ipla  
               ibuf(ipos+2) = lr2
               ibuf(ipos+3) = ichar(type)
               ibuf(ipos+4) = m1
               ibuf(ipos+5) = n1 
               ibuf(ipos+6) = rhs + icre 
               narg = ibuf(6*(ie+i-1)+1)
               if(narg.gt.namax) then
                  call error(70)
                  return
               endif    
               ladr(ipla) = lr1         
          else
c           we must check input-output consistency 
               ii=6*(iecor-1)
               if (m1*n1.gt.ibuf(ii+4)*ibuf(ii+5)
     $              .or.ichar(type).ne.ibuf(ii+3)) then
                  buf = fname // ': incompatibility between '
     $                 //'input and output variable'
                  call error(999)
                  return
               endif
               ibuf(ii+4)=m1
               ibuf(ii+5)=n1
               call icopy(6,ibuf(ii+1),1,ibuf(6*(ie+i-1)+1),1)
            endif
            jjj=jjj+3
         endif
 94   continue
 95   continue
      call interf(stk(ladr(1)),stk(ladr(2)),
     $     stk(ladr(3)),stk(ladr(4)),stk(ladr(5)),stk(ladr(6)),
     $     stk(ladr(7)),stk(ladr(8)),stk(ladr(9)),stk(ladr(10)),
     $     stk(ladr(11)),stk(ladr(12)),stk(ladr(13)),stk(ladr(14)),
     $     stk(ladr(15)),stk(ladr(16)),stk(ladr(17)),stk(ladr(18)),
     $     stk(ladr(19)),stk(ladr(20)),stk(ladr(21)),stk(ladr(22)),
     $     stk(ladr(23)),stk(ladr(24)),stk(ladr(25)),stk(ladr(26)),
     $     stk(ladr(27)),stk(ladr(28)),stk(ladr(29)),stk(ladr(30)))
      if(iss.le.0) then
         top=top2
         call objvide(fname,top)
      else
c        check if output variables are in increasing order in the stack 
         lcres=.true.
C     .   bug 2119 fix verify that the  variables are in increasing
C     .   order in the stack and not in the routine calling sequence
         ibufprec=0
         do 105 i=1,lhs 
            ir1= 6*(ie+i-1)
            type=char(ibuf(ir1+3))
            if(type.eq.'r'.or.type.eq.'i') then
               loc=sadr(ibuf(ir1+2))
            elseif(type.eq.'r') then
               loc=sadr((lr1/4)+1)
            else
               loc=ibuf(ir1+2)
            endif
            if ( loc.lt.ibufprec) then 
               lcres=.false.
               goto 106 
            else
               ibufprec = loc
            endif
 105     continue
 106     continue
         if ( lcres) then 
            top=top2-1
c     .     next lines to have topl and topk initialized in any case
            topl=top
            topk=top
         else
            topk=top2-1
            topl=top+icre
            top=topl
         endif
         do 104 i=1,lhs
            ir1= 6*(ie+i-1)
            top=top+1
            type=char(ibuf(ir1+3)) 
            m = ibuf(ir1+4)
            n = ibuf(ir1+5)
c     Warning : in the following code copy for 
c     overlapping object is necessary 
            if (type.eq.'d') then 
               if (.not.cremat(fname,top,0,m,n,lr1,lc1)) return
               call unsfdcopy(m*n,stk(ibuf(ir1+2)),1,stk(lr1),1)
            else if (type.eq.'i') then 
               if (.not.cremat(fname,top,0,m,n,lr1,lc1)) return
               call stacki2d(m*n,ibuf(ir1+2),lr1)
            else if (type.eq.'r') then 
               if (.not.cremat(fname,top,0,m,n,lr1,lc1)) return
               call stackr2d(m*n,ibuf(ir1+2),lr1)
            else if (type.eq.'c') then 
               if (.not.cresmat2(fname,top,m*n,lr1)) return
               l1=ibuf(ir1+2)
               call stackc2i(m*n,l1,lr1)
            endif
 104     continue
         if (.not.lcres) then 
            do 107 i=1,lhs 
               call copyobj(fname,topl+i,topk+i)
 107        continue
            top=topk+lhs
         endif
      endif
      nbvars = 0
      return
      end
