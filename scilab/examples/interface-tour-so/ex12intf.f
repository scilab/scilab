       subroutine intex12f(fname)
c      optional arguments 
c      ------------------------
       character*(*) fname
       logical checkrhs,checklhs
       include 'stack.h'
       logical getrhsvar,createvar
       integer v1ok,v2ok,v1pos,v2pos
       character name*(nlgh+1)
c     
       v1ok=0
       v2ok=0

       minrhs = 1
       maxrhs = 10
       minlhs = 1
       maxlhs = 3
c     
       nopt = numopt()
       if(.not.checkrhs(fname,minrhs,maxrhs+nopt)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return
c      first argument a string 
c     ------------------------
       if(.not.getrhsvar(1,'c',m,n,l1)) return
c      optional arguments 
c      ------------------
c      v1= arg1,v2=arg2 with arg1 and arg2 of type matrix 
c      v1ok is set to 1 if v1 is present and v1pos is set to its 
c      position in the argument list 
       do 10 k=rhs-nopt+1,rhs
          if (isopt(k,name).eq.0 ) then 
             buf = fname // ' optional arguments name=val'
     $            // ' must be at the end '
             call error(998) 
             return
          else
c             write (06,*) '[',name,']'
             if (name(1:3).eq.'v1'//char(0)) then 
                if (.not.getrhsvar(k,'d',mo1,no1,lo1))return
                v1ok=1
                v1pos=k
             else if ( name(1:3).eq.'v2'//char(0)) then 
                if (.not.getrhsvar(k,'d',mo2,no2,lo2))return
                v2ok=1
                v2pos=k
             else
                buf = fname // 'unrecognized optional arguments '
     $               // name 
                call error(998)
                return
             endif
          endif
 10    continue
c      default values if optional arguments are not given 
c     v1=[99] and v2=[3]
c     ----------------------------------------------------
       iopos=rhs 
       if ( v1ok.eq.0) then 
          mo1=1
          no1=1
          v1pos= iopos+1
          iopos= v1pos
          if(.not.createvar(iopos,'d',1,1,lo1)) return
          stk(lo1)=99
       endif
       if ( v2ok.eq.0) then 
          iopos=iopos+1
          v2pos=iopos
          mo2=1
          no2=1
          if(.not.createvar(iopos,'d',1,1,lo2)) return
          stk(lo2)=3
       endif
c     -----------------------------------------------
c     computation on data v1=2*v1 v2=3*v2
c     ---------------------------------------------
       call ex2f(stk(lo1),mo1,no1,stk(lo2),mo2,no2,err)
       if(err .gt. 0) then 
        buf = fname // 'Internal Error' 
        call error(998)
        return
       endif
c     return first argument,v1,v2 
c       ---------------------------
       lhsvar(1)=1
       lhsvar(2)=v1pos 
       lhsvar(3)=v2pos 
       end

      subroutine ex2f(a,ma,na,b,mb,nb,err) 
      integer err,ma,na,mb,nb
      double precision a(*),b(*)
      do 10 i=1,ma*na
         a(i)=2*a(i)
 10   continue 
      do 20 i=1,mb*nb
         b(i)=3*b(i)
 20   continue
      return
      end




