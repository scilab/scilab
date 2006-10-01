c      
c     SUBROUTINE wheel
c     
c     Copyright ENPC 
      subroutine wheel(neq,t,z,zdot)
        implicit double precision (t)
        parameter (kn=3)
        doubleprecision t,z(8),zdot(8),r,j(3),m
        doubleprecision me3s(kn,kn)
        doubleprecision const(kn,1),w(3*kn),rcond
        integer i,k,neq,ierr
        data g / 9.81/
        data r / 1.0/
        data m / 1.0/
        data j / 0.3,0.4,1.0/
      t1 = r**2
      t2 = t1*m
      t3 = t2+J(3)
      t5 = cos(z(2))*t3
      t9 = cos(2*z(2))
         me3s(3,3) = t3
         me3s(1,3) = t5
         me3s(2,1) = 0
         me3s(2,3) = 0
         me3s(2,2) = J(1)+t2
         me3s(3,1) = t5
         me3s(1,2) = 0
         me3s(3,2) = 0
         me3s(1,1) = J(1)/2+t1*m*t9/2+t2/2+J(3)*t9/2+J(3)/2-J(1)*t9/2
      t1 = r**2
      t2 = z(4)**2
      t4 = sin(2*z(2))
      t5 = t2*t4
      t11 = sin(z(2))
      t12 = z(4)*z(6)
      t34 = z(4)*t4
         const(2,1) = -t1*m*t5/2+J(1)*t5/2-t1*t11*m*t12-r*cos(z(2))*m*g-
     +J(3)*t11*t12-J(3)*t5/2
         const(3,1) = t11*z(4)*z(5)*(2*t1*m+J(3))
         const(1,1) = -z(5)*(-t1*m*t34-z(6)*t11*J(3)+J(1)*t34-J(3)*t34)
c         
        do 1000, i =1,kn ,1
          zdot(i) = z(i+kn)
 1000   continue
c       
c        we must solve  M z =const 
        call dlslv(me3s,kn,kn,Const,kn,1,w, rcond,ierr,1)
        if (ierr.ne.0) then
          write(6,2000) 
 2000     format('Ill conditioned matrix!')
        endif
c         
        do 1001, i =1,kn ,1
          zdot(kn+i) = const(i,1)
 1001   continue
c       
      t2 = sin(z(1))
        zdot(7) = r*cos(z(2))*t2*z(4)+r*sin(z(2))*cos(z(1))*z(5)+r*t2*z(
     +6)
      t2 = cos(z(1))
        zdot(8) = -r*cos(z(2))*t2*z(4)+r*sin(z(2))*sin(z(1))*z(5)-r*t2*z
     +(6)
        return
      end
