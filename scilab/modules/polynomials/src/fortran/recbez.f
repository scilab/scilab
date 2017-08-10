c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 1989-2008 - INRIA - Serge STEER
c
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
      subroutine recbez(A,n,B,m,best,ipb,w,err)
c!but
c calcule la factorisation de bezout de deux polynomes A,B c'est a dire
c les polynomes x,y,u, v et pgcd de degre minimal et tels que :
c                      [x  u]
c            [A B] *   [    ] = [0 Pgcd]
c                      [y  v]
c!liste d'appel
c     subroutine recbez(A,n,B,m,best,ipb,w,err)
c     double precision  A(n+1),B(m+1),w(*),best(*),err
c
c     A : vector of A polynomial coefficients, in increasing degrees order
c     n : degree of A polynomial
c     B : vector of B polynomial coefficients, in increasing degrees order
c     m : degree of B polynomial
c
c     best : Array where the result is stored
c            (2*(n+m)+min(n,m)+3)  [gcd u,v,x,y]
c            gcd=best(1:ipb(2)-1),
c            u=best(ipb(2):ipb(3)-1) v=best(ipb(3):ipb(4)-1)
c            x=best(ipb(4):ipb(5)-1) y=best(ipb(5):ipb(6)-1)
c     ipb : integer array that gives the way gcd u,v,x,y are stored in best
c     w: working array  w(7*n0+3*n0*n0) ou n0=max(n,m)+1
c     err : estimated equation error
c! method
c     If A and B are polynomials of degree m and n : A in R[x] , B in R[x]
c                                                          n           m
c     The application: (P,Q) in R[x] x R[x]  --> P*A+Q*B in R[x]
c                                m-l    n-l                  n+m-l
c
c     is linear. It can be represented by a linear application from 
c     R^(n+m-2*l+2) to R^(n+m-l+1) X--> X*S. Let call S the matrix S 
c     of this linear application. If l=1 this is the Sylvester matrix. 
c     example for A of degree 5 (n=5) and B of degree 4 (m=4) and l=2.
c             [A_0  A_1  A_2  A_3  A_4  A_5    0    0] 
c             [  0  A_0  A_1  A_2  A_3  A_4  A_5    0]
c             [  0    0  A_0  A_1  A_2  A_3  A_4  A_5]
c         S = [B_0  B_1  B_2  B_3  B_4    0    0    0]
c             [  0  B_0  B_1  B_2  B_3  B_4    0    0]
c             [  0    0  B_0  B_1  B_2  B_3  B_4    0]
c             [  0    0    0  B_0  B_1  B_2  B_3  B_4]
c        
c         X = [P_0 P_1 P_2 Q_0 Q_1 Q_2 Q_3]
c     The greater l for which S becomes not full rank is the GCD degree.
c     The kernel of S then gives the x and y value
c     The GCD is the element of the image of S that has the smaller
c     degree.
c     The determination of the S rank is a difficult problem. To avoid
c     the use of a tolerance The algorithm try all l from min(m,n)-1 to
c     1. and keeps the solution that minimise a criterion based on
c     forward  equation errors ||A*x+B*y|| and ||A*u+B*V-GCD|| and
c     backward equation errors ||y*CGD+A|| and ||x*GCD-B||
c     Finally instead of computing QR decomposition of each S matrices a
c     recursive method is used that updates  previous QR decomposition
c     using givens rotations. This algorithm also requires to store only
c     a matrix which has max(m,n)+1 columns.
c
c! called routines
c     dest dcopy bezstp dlamch
      double precision  A(n+1),B(m+1),w(*),best(*),err
      double precision  dlamch
      integer ipb(6)
c
      integer nn,mm
c
      err=dlamch('o')
      nn = n
      mm = m
c
c     compute the true degree for A and B taking care of 0 higer degree
c
      nn=n+1
 01   nn=nn-1
      if(nn.lt.0) goto 30
      if(A(nn+1).eq.0.0d+0) goto 01
c
      mm=m+1
 02   mm=mm-1
      if(mm.lt.0) goto 30
      if(B(mm+1).eq.0.0d+0) goto 02
c
      n0=max(nn,mm)+1
      n02=n0*(n0+1)
c     leading dimension of the S matrix 
      ldS=n0+1
c     leading dimension of the Q matrix of the QR factorization 
      ldQ=n0+1

c     allocate working arrays
c     start index of S array in w
      iS=1
c     start index of Q array in w
      iQ=iS+n02
c     start index of working array for bezstp
      iw=iQ+2*n02
      ifree=iw+7*n0

c     initialize S (n0+1) x n0 matrix array. Take care that it is a
c     compact  form of the S matrix described in the method part.
      
      call dset(n02,0.0d+0,w(iS),1)
c     the last 2 rows form the current S matrix
c     lS points to the beginning of last row 
c     and lS-1 to the beginning of the previous one
      lS=iS-1+ldS
c     initialize with x and y of degree 0
c        [0        ]
c     S= [coeff(A)]
c        [coeff(B)]
c            no

      call dcopy(nn+1,A,1,w(lS-1),ldS)
      call dcopy(mm+1,B,1,w(lS),ldS)

c     initialize U  (n0+1) x (2*n0) array
c        [0...0|0...0 0 0]
c     U= [0...0|0...0 1 0]
c        [0...0|0...0 0 1]
C          n0      n0
      call dset(2*n02,0.0d+0,w(iQ),1)
      lQ=iQ-1+ldQ+2*n0*ldQ
c     the last 2 rows form the current Q matrix
      call dset(2,1.0d+0,w(lQ-1-2*ldQ),ldQ+1)

c     loop on GCD degree degree(gcd)=n0+1-l
      do 20 l=1,n0
         lS=lS-1
         lQ=lQ-1-2*ldQ
         lx=lS-1+ldS
         lv=lQ-1-2*ldQ
c     .  lS : points on the upper left corner of the current A matrix,
c     .  lQ : points  on the upper left corner of the current Q matrix
         call bezstp(A,nn,B,mm,w(lS),ldS,w(lQ),ldQ,l,w(lx),
     $        w(lv),w(iw),best,ipb,err)
 20   continue
      return


 30   continue
c     Handle special cases at least one polynomial is a scalar
      err=0.0d+0
      ipb(1)=1
      if(min(nn,mm).eq.0) goto 70
      CRES=nn-mm
      if (CRES .lt. 0) then
         goto 40
      elseif (CRES .eq. 0) then
         goto 50
      else
         goto 60
      endif
 40   continue
c     A=0
      np=mm
      call dcopy(mm+1,B,1,best(1),1)
      ipb(2)=ipb(1)+mm+1
      best(ipb(2))=0.0d+0
      ipb(3)=ipb(2)+1
      best(ipb(3))=1.0d+0
      ipb(4)=ipb(3)+1
      best(ipb(4))=1.0d+0
      ipb(5)=ipb(4)+1
      best(ipb(5))=0.0d+0
      ipb(6)=ipb(5)+1
      return
 50   continue
c     A=0,B=0  la decomposition n'est pas definie
      np=0
      best(1)=0.0d+0
      ipb(2)=ipb(1)+1
      best(ipb(2))=1.0d+0
      ipb(3)=ipb(2)+1
      best(ipb(3))=0.0d+0
      ipb(4)=ipb(3)+1
      best(ipb(4))=0.0d+0
      ipb(5)=ipb(4)+1
      best(ipb(5))=1.0d+0
      ipb(6)=ipb(5)+1
      return
 60   continue
c     B=0
      np=nn
      call dcopy(nn+1,A,1,best(1),1)
      ipb(2)=ipb(1)+nn+1
      best(ipb(2))=1.0d+0
      ipb(3)=ipb(2)+1
      best(ipb(3))=0.0d+0
      ipb(4)=ipb(3)+1
      best(ipb(4))=0.0d+0
      ipb(5)=ipb(4)+1
      best(ipb(5))=1.0d+0
      ipb(6)=ipb(5)+1
      return
c
 70   best(1)=1.0d+0
      ipb(2)=2
      CRES=nn-mm
      if (CRES .lt. 0) then
         goto 90
      elseif (CRES .eq. 0) then
         goto 95
      else
         goto 100
      endif
 90   continue
c     A=cte
      best(ipb(2))=1.0d+0/A(1)
      ipb(3)=ipb(2)+1
      best(ipb(3))=0.0d+0
      ipb(4)=ipb(3)+1
      call dcopy(mm+1,B,1,best(ipb(4)),1)
      ipb(5)=ipb(4)+mm+1
      best(ipb(5))=-A(1)
      ipb(6)=ipb(5)+1
      return
  95  continue
c     A=cte,B=cte
      if(abs(A(1)).gt.abs(B(1))) goto 90
c
 100  continue
c     B=cte
      best(ipb(2))=0.0d+0
      ipb(3)=ipb(2)+1
      best(ipb(3))=1.0d+0/B(1)
      ipb(4)=ipb(3)+1
      best(ipb(4))=-B(1)
      ipb(5)=ipb(4)+1
      call dcopy(nn+1,A,1,best(ipb(5)),1)
      ipb(6)=ipb(5)+nn+1
      return
c
      end
