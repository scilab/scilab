      subroutine genmdsp(is,a,na,m,n,ndgt,lineln,wte,buf,iw) 
      integer is,m,na,n,ndgt,lineln,wte,iw
      character*(*) buf
      integer a(*)

      if(is.eq.4.or.is.eq.14) then
         call imdsp(a,na,m,n,ndgt,lineln,wte,buf,iw) 
      elseif(is.eq.2.or.is.eq.12) then
         call i2mdsp(a,na,m,n,ndgt,lineln,wte,buf,iw) 
      else
         call i1mdsp(a,na,m,n,ndgt,lineln,wte,buf,iw) 
      endif
      return
      end
