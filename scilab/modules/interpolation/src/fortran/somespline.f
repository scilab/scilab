c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) Bruno Pincon
c
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.

      subroutine SplineCub(x, y, d, n, type, A_d, A_sd, qdy, lll)
*
*     PURPOSE
*        computes a cubic spline interpolation function
*        in Hermite form (ie computes the derivatives d(i) of the
*        spline in each interpolation point (x(i), y(i)))
*
*     ARGUMENTS
*      inputs :
*         n       number of interpolation points (n >= 3)
*         x, y    the n interpolation points, x must be in strict increasing order
*         type    type of the spline : currently
*                    type = 0 correspond to a  NOT_A_KNOT spline where it is
*                             imposed the conditions :
*                                 s'''(x(2)-) = s'''(x(2)+)
*                             and s'''(x(n-1)-) = s'''(x(n-1)+)
*                    type = 1 correspond to a NATURAL spline with the conditions :
*                                 s''(x1) = 0
*                             and s''(xn) = 0
*                    type = 2 correspond to a CLAMPED spline (d(1) and d(n) are given)
*
*                    type = 3 correspond to a PERIODIC spline
*      outputs :
*         d    the derivatives in each x(i) i = 1..n
*
*      work arrays :
*         A_d(1..n), A_sd(1..n-1), qdy(1..n-1)
*         lll(1..n-1) (used only in the periodic case)
*
*    NOTES
*         this routine requires (i)   n >= 3 (for natural) n >=4 (for not_a_knot)
*                               (ii)  strict increasing abscissae x(i)
*                               (iii) y(1) = y(n) in the periodic case
*         THESE CONDITIONS MUST BE TESTED IN THE CALLING CODE
*
*     AUTHOR
*        Bruno Pincon
*
*     July 22 2004 : correction of the case not_a_knot which worked only
*                    for equidistant abscissae
*
      implicit none

      integer n, type
      double precision x(n), y(n), d(n), A_d(n), A_sd(n-1), qdy(n-1),
     $                 lll(n-1)

      include 'constinterp.h.f'
      integer i
      double precision r

      if (n .eq. 2) then
         if (type .ne. CLAMPED) then
            d(1) = (y(2) - y(1)) / (x(2) - x(1))
            d(2) = d(1)
         endif
         return
      endif

      if (n .eq. 3  .and.  type .eq. NOT_A_KNOT) then
         call derivd(x, y, d, n, 1, FAST)
         return
      endif

      do i = 1, n-1
         A_sd(i) = 1.d0 / (x(i+1) - x(i))
         qdy(i) = (y(i+1) - y(i)) * A_sd(i)**2
      enddo

*     compute the coef matrix and r.h.s. for rows 2..n-1
*     (which don't relies on the type)
      do i = 2, n-1
         A_d(i) = 2.d0*( A_sd(i-1) + A_sd(i) )
         d(i) = 3.d0 * ( qdy(i-1) + qdy(i) )
      enddo

*     compute equ 1 and n in function of the type
      if (type .eq. NATURAL) then
         A_d(1) =  2.d0*A_sd(1)
         d(1) = 3.d0 * qdy(1)
         A_d(n) =  2.d0*A_sd(n-1)
         d(n) =  3.d0 * qdy(n-1)
         call TriDiagLDLtSolve(A_d, A_sd, d, n)

      else if (type .eq. NOT_A_KNOT) then
         !  s'''(x(2)-) = s'''(x(2)+)
         r = A_sd(2)/A_sd(1)
         A_d(1) = A_sd(1)/(1.d0 + r)
         d(1) = ((3.d0*r+2.d0)*qdy(1)+r*qdy(2))/(1.d0+r)**2
         !  s'''(x(n-1)-) = s'''(x(n-1)+)
         r = A_sd(n-2)/A_sd(n-1)
         A_d(n) = A_sd(n-1)/(1.d0 + r)
         d(n) = ((3.d0*r+2.d0)*qdy(n-1)+r*qdy(n-2))/(1.d0+r)**2
         call TriDiagLDLtSolve(A_d, A_sd, d, n)

      else if (type .eq. CLAMPED) then
         ! d(1) and d(n) are already known
         d(2) = d(2) - d(1)*A_sd(1)
         d(n-1) = d(n-1) - d(n)*A_sd(n-1)
         call TriDiagLDLtSolve(A_d(2), A_sd(2), d(2), n-2)

      else if (type .eq. PERIODIC) then
         A_d(1) = 2.d0*( A_sd(1) + A_sd(n-1) )
         d(1) = 3.d0 * ( qdy(1) + qdy(n-1) )
         lll(1) = A_sd(n-1)
         call dset(n-2, 0.d0, lll(2),1)  ! mise a zero
         lll(n-2) = A_sd(n-2)
         call CyclicTriDiagLDLtSolve(A_d, A_sd, lll, d, n-1)
         d(n) = d(1)

      endif

      end ! subroutine SplineCub


      subroutine TriDiagLDLtSolve(d, l, b, n)
*
*     PURPOSE
*        solve a linear system A x = b with a symmetric tridiagonal positive definite
*        matrix A by using an LDL^t factorization
*
*     PARAMETERS
*        d(1..n)   : on input the diagonal of A
*                    on output the diagonal of the (diagonal) matrix D
*        l(1..n-1) : on input the sub-diagonal of A
*                    on output the sub-diagonal of L
*        b(1..n)   : on input contains the r.h.s. b
*                    on output the solution x
*        n         : the dimension
*
*     CAUTION
*        no zero pivot detection
*
      implicit none
      integer n
      integer flag
      double precision d(n), l(n-1), b(n)

      integer i
      double precision temp

      do i = 2, n
         temp = l(i-1)
         l(i-1) = l(i-1) / d(i-1)
         d(i) = d(i) - temp * l(i-1)
         b(i) = b(i) - l(i-1)*b(i-1)
      enddo

      b(n) = b(n) / d(n)
      do i = n-1, 1, -1
         b(i) = b(i)/d(i) - l(i)*b(i+1)
      enddo

      end ! subroutine TriDiagLDLtSolve

      subroutine CyclicTriDiagLDLtSolve(d, lsd, lll, b, n)
*
*     PURPOSE
*        solve a linear system A x = b with a symmetric "nearly" tridiagonal
*        positive definite matrix A by using an LDL^t factorization,
*        the matrix A has the form :
*
*          |x x         x|                        |1            |
*          |x x x        |                        |x 1          |
*          |  x x x      |                        |  x 1        |
*          |    x x x    |  and so the L is like  |    x 1      |
*          |      x x x  |                        |      x 1    |
*          |        x x x|                        |        x 1  |
*          |x         x x|                        |x x x x x x 1|
*
*     PARAMETERS
*        d(1..n)     : on input the diagonal of A
*                      on output the diagonal of the (diagonal) matrix D
*        lsd(1..n-2) : on input the sub-diagonal of A (without  A(n,n-1))
*                      on output the sub-diagonal of L (without  L(n,n-1))
*        lll(1..n-1) : on input the last line of A (without A(n,n))
*                      on output the last line of L (without L(n,n))
*        b(1..n)     : on input contains the r.h.s. b
*                      on output the solution x
*        n           : the dimension
*
*     CAUTION
*        no zero pivot detection
*
      implicit none
      integer n
      double precision d(n), lsd(n-2), lll(n-1), b(n)

      integer i, j
      double precision temp1, temp2

*     compute the LDL^t factorization
      do i = 1, n-2
         temp1 = lsd(i)
         temp2 = lll(i)
         lsd(i) = lsd(i) / d(i) ! elimination coef L(i,i-1)
         lll(i) = lll(i) / d(i) ! elimination coef L(n,i-1)

         d(i+1)   = d(i+1)   - lsd(i) * temp1 ! elimination on line i+1
         lll(i+1) = lll(i+1) - lll(i) * temp1 ! elimination on line n
         d(n)     = d(n)     - lll(i) * temp2 ! elimination on line n
      enddo
      temp2 = lll(n-1)
      lll(n-1) = lll(n-1) / d(n-1)
      d(n)     = d(n) - lll(n-1) * temp2

*     solve LDL^t x = b  (but use b for x and for the intermediary vectors...)
      do i = 2, n-1
         b(i) = b(i) - lsd(i-1)*b(i-1)
      enddo
      do j = 1, n-1
         b(n) = b(n) - lll(j)*b(j)
      enddo

      do i = 1, n
         b(i) = b(i) / d(i)
      enddo

      b(n-1) = b(n-1) - lll(n-1)*b(n)
      do i = n-2, 1, -1
         b(i) = b(i) - lsd(i)*b(i+1) - lll(i)*b(n)
      enddo

      end ! subroutine CyclicTriDiagLDLtSolve


      integer function isearch(t, x, n)
*
*     PURPOSE
*        x(1..n) being an array (with strict increasing order and n >=2)
*        representing intervals, this routine return i such that :
*
*           x(i) <= t <= x(i+1)
*
*        and 0 if t is not in [x(1), x(n)]
*
      implicit none
      integer n
      double precision t, x(n)

      integer i1, i2, i

      if ( x(1) .le. t  .and.  t .le. x(n)) then
*        dichotomic search
         i1 = 1
         i2 = n
         do while ( i2 - i1 .gt. 1 )
            i = (i1 + i2)/2
            if ( t .le. x(i) ) then
               i2 = i
            else
               i1 = i
            endif
         enddo
         isearch = i1
      else
         isearch = 0
      endif

      end

      subroutine EvalPWHermite(t, st, dst, d2st, d3st, m, x, y, d, n,
     $                         outmode)
*
*     PURPOSE
*        evaluation at the abscissae t(1..m) of the piecewise hermite function
*        define by x(1..n), y(1..n), d(1..n) (d being the derivatives at the
*        x(i)) together with its derivative, second derivative and third derivative
*
*     PARAMETERS
*
*        outmode : define what return in case t(j) not in [x(1), x(n)]

      implicit none
      integer m, n, outmode
      double precision t(m), st(m), dst(m), d2st(m), d3st(m),
     $                 x(n), y(n), d(n)

      include 'constinterp.h.f'
      integer i, j
      integer  isearch, isanan
      external isearch, isanan
      double precision tt
      external         returnananfortran
      logical new_call
      common /INFO_HERMITE/new_call

      new_call = .true.
      i = 0
      do j = 1, m
         tt = t(j)
         call fast_int_search(tt, x, n, i)  ! recompute i only if necessary

         if ( i .ne. 0 ) then
            call EvalHermite(tt, x(i), x(i+1), y(i), y(i+1), d(i),
     $                       d(i+1), st(j), dst(j), d2st(j), d3st(j), i)

         else   ! t(j) is outside [x(1), x(n)] evaluation depend upon outmode

            if (outmode .eq. BY_NAN  .or.  isanan(tt) .eq. 1) then
               CALL returnananfortran(st(j))
               dst(j) = st(j)
               d2st(j) = st(j)
               d3st(j) = st(j)

            elseif (outmode .eq. BY_ZERO) then
               st(j) = 0.d0
               dst(j) = 0.d0
               d2st(j) = 0.d0
               d3st(j) = 0.d0

            elseif (outmode .eq. C0) then
               dst(j) = 0.d0
               d2st(j) = 0.d0
               d3st(j) = 0.d0
               if ( tt .lt. x(1) ) then
                  st(j) = y(1)
               else
                  st(j) = y(n)
               endif

            elseif (outmode .eq. LINEAR) then
               d2st(j) = 0.d0
               d3st(j) = 0.d0
               if ( tt .lt. x(1) ) then
                  dst(j) = d(1)
                  st(j) = y(1) + (tt - x(1))*d(1)
               else
                  dst(j) = d(n)
                  st(j) = y(n) + (tt - x(n))*d(n)
               endif

            else
               if (outmode .eq. NATURAL) then
                  call near_interval(tt, x, n, i)
               elseif (outmode .eq. PERIODIC) then
                  call coord_by_periodicity(tt, x, n, i)
               endif
               call EvalHermite(tt, x(i), x(i+1), y(i), y(i+1), d(i),
     $                       d(i+1), st(j), dst(j), d2st(j), d3st(j), i)
            endif
         endif
      enddo

      end ! subroutine EvalPWHermite

      subroutine EvalHermite(t, xa, xb, ya, yb, da, db, h, dh, ddh,
     $                       dddh, i)

      implicit none
      double precision t, xa, xb, ya, yb, da, db, h, dh, ddh, dddh
      integer i

      logical new_call
      common /INFO_HERMITE/new_call

      double precision tmxa, dx, p, c2, c3
      integer          old_i
      save             old_i,       c2, c3
      data old_i/0/

      if (old_i .ne. i  .or.  new_call) then
*        compute the following Newton form :
*           h(t) = ya + da*(t-xa) + c2*(t-xa)^2 + c3*(t-xa)^2*(t-xb)
         dx = 1.d0/(xb - xa)
         p = (yb - ya) * dx
         c2 = (p - da) * dx
         c3 = ( (db - p) + (da - p) ) * (dx*dx)
         new_call = .false.
      endif
      old_i = i

*     eval h(t), h'(t), h"(t) and h"'(t), by a generalized Horner 's scheme
      tmxa = t - xa
      h = c2 + c3*(t - xb)
      dh = h + c3*tmxa
      ddh = 2.d0*( dh + c3*tmxa )
      dddh = 6.d0*c3
      h = da + h*tmxa
      dh = h + dh*tmxa
      h = ya + h*tmxa

      end ! subroutine EvalHermite

      subroutine fast_int_search(xx, x, nx, i)

      implicit none
      integer nx, i
      double precision xx, x(nx)

      integer  isearch
      external isearch

      if ( i .eq. 0 ) then
         i = isearch(xx, x, nx)
      elseif ( .not. (x(i) .le. xx  .and.  xx .le. x(i+1))) then
         i = isearch(xx, x, nx)
      endif

      end

      subroutine coord_by_periodicity(t, x, n, i)
*
*     PURPOSE
*        recompute t such that t in [x(1), x(n)] by periodicity :
*        and then the interval i of this new t
*
      implicit none
      integer n, i
      double precision t, x(n)
      integer  isearch
      external isearch

      double precision r, dx

      dx = x(n) - x(1)
      r = (t - x(1)) / dx

      if (r .ge. 0.d0) then
         t = x(1) + (r - aint(r))*dx
      else
         r = abs(r)
         t = x(n) - (r - aint(r))*dx
      endif

      ! some cautions in case of roundoff errors (is necessary ?)
      if (t .lt. x(1)) then
         t = x(1)
         i = 1
      else if (t .gt. x(n)) then
         t = x(n)
         i  = n-1
      else
         i = isearch(t, x, n)
      endif

      end ! subroutine coord_by_periodicity


      subroutine near_grid_point(xx, x, nx, i)
*
*     calcule le point de la grille le plus proche ... a detailler
*
      implicit none
      integer i, nx
      double precision xx, x(nx)

      if (xx .lt. x(1)) then
         i = 1
         xx = x(1)
      else    !  xx > x(nx)
         i = nx-1
         xx = x(nx)
      endif

      end

      subroutine near_interval(xx, x, nx, i)
*
*     idem sans modifier xx
*
      implicit none
      integer i, nx
      double precision xx, x(nx)

      if (xx .lt. x(1)) then
         i = 1
      else    !  xx > x(nx)
         i = nx-1
      endif

      end

      subroutine proj_by_per(t, xmin, xmax)
*
*     PURPOSE
*        recompute t such that t in [xmin, xmax] by periodicity.
*
      implicit none
      double precision t, xmin, xmax
      double precision r, dx

      dx = xmax - xmin
      r = (t - xmin) / dx

      if (r .ge. 0.d0) then
         t = xmin + (r - aint(r))*dx
      else
         r = abs(r)
         t = xmax - (r - aint(r))*dx
      endif

      ! some cautions in case of roundoff errors (is necessary ?)
      if (t .lt. xmin) then
         t = xmin
      else if (t .gt. xmax) then
         t = xmax
      endif

      end ! subroutine proj_by_per


      subroutine proj_on_grid(xx, xmin, xmax)
*
      implicit none
      double precision xx, xmin, xmax

      if (xx .lt. xmin) then
         xx = xmin
      else
         xx = xmax
      endif

      end

      subroutine BiCubicSubSpline(x, y, u, nx, ny, C, p, q, r, type)
*
*     PURPOSE
*        compute bicubic subsplines
*
      implicit none
      integer nx, ny, type
      double precision x(nx), y(ny), u(nx, ny), C(4,4,nx-1,ny-1),
     $                 p(nx, ny), q(nx, ny), r(nx, ny)
      integer i, j
      include 'constinterp.h.f'

      if (type .eq. MONOTONE) then
*        approximation des derivees par SUBROUTINE DPCHIM(N,X,F,D,INCFD)
         ! p = du/dx
         do j = 1, ny
            call dpchim(nx, x, u(1,j), p(1,j), 1)
         enddo
         ! q = du/dy
         do i = 1, nx
            call dpchim(ny, y, u(i,1), q(i,1), nx)
         enddo
         ! r = d2 u/ dx dy  approchee via  dq / dx
         do j = 1, ny
            call dpchim(nx, x, q(1,j), r(1,j), 1)
         enddo

      elseif (type .eq. FAST  .or.  type .eq. FAST_PERIODIC) then
*        approximation des derivees partielles par methode simple

         ! p = du/dx
         do j = 1, ny
            call derivd(x, u(1,j), p(1,j), nx, 1, type)
         enddo
         ! q = du/dy
         do i = 1, nx
            call derivd(y, u(i,1), q(i,1), ny, nx, type)
         enddo
         ! r = d2 u/ dx dy  approchee via  dq / dx
         do j = 1, ny
            call derivd(x, q(1,j), r(1,j), nx, 1, type)
         enddo

      endif

*     calculs des coefficients dans les bases (x-x(i))^k (y-y(j))^l  0<= k,l <= 3
*     pour evaluation rapide via Horner par la suite
      call coef_bicubic(u, p, q, r, x, y, nx, ny, C)

      end ! subroutine BiCubicSubSpline

      subroutine BiCubicSpline(x, y, u, nx, ny, C, p, q, r,
     $                         A_d, A_sd, d, ll, qdu, u_temp, type)
*
*     PURPOSE
*        compute bicubic splines
*
      implicit none
      integer nx, ny, type
      double precision x(nx), y(ny), u(nx, ny), C(4,4,nx-1,ny-1),
     $                 p(nx, ny), q(nx, ny), r(nx, ny), A_d(*),
     $                 A_sd(*), d(ny), ll(*), qdu(*), u_temp(ny)
      include 'constinterp.h.f'
      integer i, j

      ! compute du/dx
      do j = 1, ny
         call SplineCub(x, u(1,j), p(1,j), nx, type, A_d, A_sd, qdu, ll)
      enddo

      ! compute du/dy
      do i = 1, nx
         call dcopy(ny, u(i,1), nx, u_temp, 1)
         call SplineCub(y, u_temp, d, ny, type, A_d, A_sd, qdu, ll)
         call dcopy(ny, d, 1, q(i,1), nx)
      enddo

      ! compute ddu/dxdy
      call SplineCub(x, q(1,1),  r(1,1),  nx, type, A_d, A_sd, qdu, ll)
      call SplineCub(x, q(1,ny), r(1,ny), nx, type, A_d, A_sd, qdu, ll)

      do i = 1, nx
         call dcopy(ny, p(i,1), nx, u_temp, 1)
         d(1) = r(i,1)
         d(ny) = r(i,ny)
         call SplineCub(y, u_temp, d, ny, CLAMPED, A_d, A_sd, qdu, ll)
         call dcopy(ny-2, d(2), 1, r(i,2), nx)
      enddo

*     calculs des coefficients dans les bases (x-x(i))^k (y-y(j))^l  0<= k,l <= 3
*     pour evaluation rapide via Horner par la suite
      call coef_bicubic(u, p, q, r, x, y, nx, ny, C)

      end  ! subroutine BiCubicSpline


      subroutine derivd(x, u, du, n, inc, type)
*
*     PURPOSE
*        given functions values u(i) at points x(i),  i = 1, ..., n
*        this subroutine computes approximations du(i) of the derivative
*        at the points x(i).
*
*     METHOD
*        For i in [2,n-1], the "centered" formula of order 2 is used :
*            d(i) = derivative at x(i) of the interpolation polynomial
*                   of the points {(x(j),u(j)), j in [i-1,i+1]}
*
*         For i=1 and n, if type = FAST_PERIODIC  (in which case u(n)=u(1)) then
*         the previus "centered" formula is also used else (type = FAST), d(1)
*         is the derivative at x(1) of the interpolation polynomial of
*         {(x(j),u(j)), j in [1,3]} and the same method is used for d(n)
*
*     ARGUMENTS
*      inputs :
*         n       integer : number of point (n >= 2)
*         x, u    double precision : the n points, x must be in strict increasing order
*         type    integer : FAST (the function is non periodic) or FAST_PERIODIC
*                 (the function is periodic), in this last case u(n) must be equal to u(1))
*         inc     integer : to deal easily with 2d applications, u(i) is in fact
*                 u(1,i) with u declared as u(inc,*) to avoid the direct management of
*                 the increment inc (the i th value given with u(1 + inc*(i-1) ...)
*      outputs :
*         d       the derivatives in each x(i) i = 1..n
*
*    NOTES
*         this routine requires (i)   n >= 2
*                               (ii)  strict increasing abscissae x(i)
*                               (iii) u(1)=u(n) if type = FAST_PERIODIC
*         ALL THESE CONDITIONS MUST BE TESTED IN THE CALLING CODE
*
*     AUTHOR
*        Bruno Pincon
*

      implicit none
      integer n, inc, type
      double precision x(n), u(inc,*), du(inc,*)

      include 'constinterp.h.f'
      double precision dx_l, du_l, dx_r, du_r, w_l, w_r
      integer i, k


      if (n .eq. 2) then  ! special case used linear interp
         du(1,1) = (u(1,2) - u(1,1))/(x(2)-x(1))
         du(1,2) = du(1,1)
         return
      endif

      if (type .eq. FAST_PERIODIC) then

         dx_r  = x(n)-x(n-1)
         du_r  = (u(1,1) - u(1,n-1)) / dx_r
         do i = 1, n-1
            dx_l = dx_r
            du_l = du_r
            dx_r = x(i+1) - x(i)
            du_r = (u(1,i+1) - u(1,i))/dx_r
            w_l  = dx_r/(dx_l + dx_r)
            w_r = 1.d0 - w_l
            du(1,i) = w_l*du_l + w_r*du_r
         enddo
         du(1,n) = du(1,1)

      else if (type .eq. FAST) then

         dx_l = x(2) - x(1)
         du_l = (u(1,2) - u(1,1))/dx_l
         dx_r = x(3) - x(2)
         du_r = (u(1,3) - u(1,2))/dx_r
         w_l = dx_r/(dx_l + dx_r)
         w_r = 1.d0 - w_l
         du(1,1) = (1.d0 + w_r)*du_l - w_r*du_r
         du(1,2) = w_l*du_l + w_r*du_r
         do i = 3, n-1
            dx_l = dx_r
            du_l  = du_r
            dx_r = x(i+1) - x(i)
            du_r  = (u(1,i+1) - u(1,i))/dx_r
            w_l  = dx_r/(dx_l + dx_r)
            w_r = 1.d0 - w_l
            du(1,i) = w_l*du_l + w_r*du_r
         enddo
         du(1,n) = (1.d0 + w_l)*du_r - w_l*du_l

      endif

      end


      subroutine coef_bicubic(u, p, q, r, x, y, nx, ny, C)
*
*     PURPOSE
*        compute for each polynomial (i,j)-patch (defined on
*        [x(i),x(i+1)]x[y(i),y(i+1)]) the following base
*        representation :
*           i,j        _4_  _4_   i,j       k-1       l-1
*          u   (x,y) = >__  >__  C   (x-x(i))  (y-y(j))
*                      k=1  l=1   k,l
*
*        from the "Hermite" representation (values of u, p = du/dx,
*        q = du/dy, r = ddu/dxdy at the 4 vertices (x(i),y(j)),
*        (x(i+1),y(j)), (x(i+1),y(j+1)), (x(i),y(j+1)).
*
      implicit none

      integer nx, ny
      double precision u(nx, ny), p(nx, ny), q(nx, ny), r(nx, ny),
     $                 x(nx), y(ny), C(4,4,nx-1,ny-1)

      integer i, j
      double precision a, b, cc, d, dx, dy

      do j = 1, ny-1
         dy = 1.d0/(y(j+1) - y(j))

         do i = 1, nx-1
            dx = 1.d0/(x(i+1) - x(i))

            C(1,1,i,j) = u(i,j)
            C(2,1,i,j) = p(i,j)
            C(1,2,i,j) = q(i,j)
            C(2,2,i,j) = r(i,j)

            a = (u(i+1,j) - u(i,j))*dx
            C(3,1,i,j) = (3.d0*a -2.d0*p(i,j) - p(i+1,j))*dx
            C(4,1,i,j) = (p(i+1,j) + p(i,j) - 2.d0*a)*(dx*dx)

            a = (u(i,j+1) - u(i,j))*dy
            C(1,3,i,j) = (3.d0*a -2.d0*q(i,j) - q(i,j+1))*dy
            C(1,4,i,j) = (q(i,j+1) + q(i,j) - 2.d0*a)*(dy*dy)

            a = (q(i+1,j) - q(i,j))*dx
            C(3,2,i,j) = (3.d0*a - r(i+1,j) - 2.d0*r(i,j))*dx
            C(4,2,i,j) = (r(i+1,j) + r(i,j) - 2.d0*a)*(dx*dx)

            a = (p(i,j+1) - p(i,j))*dy
            C(2,3,i,j) = (3.d0*a - r(i,j+1) - 2.d0*r(i,j))*dy
            C(2,4,i,j) = (r(i,j+1) + r(i,j) - 2.d0*a)*(dy*dy)

            a = (u(i+1,j+1)+u(i,j)-u(i+1,j)-u(i,j+1))*(dx*dx*dy*dy)
     $          - (p(i,j+1)-p(i,j))*(dx*dy*dy)
     $          - (q(i+1,j)-q(i,j))*(dx*dx*dy)
     $          + r(i,j)*(dx*dy)
            b = (p(i+1,j+1)+p(i,j)-p(i+1,j)-p(i,j+1))*(dx*dy*dy)
     $          - (r(i+1,j)-r(i,j))*(dx*dy)
            cc = (q(i+1,j+1)+q(i,j)-q(i+1,j)-q(i,j+1))*(dx*dx*dy)
     $          - (r(i,j+1)-r(i,j))*(dx*dy)
            d = (r(i+1,j+1)+r(i,j)-r(i+1,j)-r(i,j+1))*(dx*dy)


            C(3,3,i,j) =  9.d0*a - 3.d0*b - 3.d0*cc + d
            C(3,4,i,j) =(-6.d0*a + 2.d0*b + 3.d0*cc - d)*dy
            C(4,3,i,j) =(-6.d0*a + 3.d0*b + 2.d0*cc - d)*dx
            C(4,4,i,j) =( 4.d0*a - 2.d0*b - 2.d0*cc + d)*dx*dy

         enddo
      enddo
      end

      double precision function EvalBicubic(xx, yy, xk, yk, Ck)

      implicit none
      double precision xx, yy, xk, yk, Ck(4,4)

      double precision dx, dy, u
      integer i

      dx = xx - xk
      dy = yy - yk

      u = 0.d0
      do i = 4, 1, -1
         u = Ck(i,1) + dy*(Ck(i,2) + dy*(Ck(i,3) + dy*Ck(i,4))) + u*dx
      enddo
      EvalBicubic = u

      end ! function EvalBicubic

      subroutine EvalBicubic_with_grad(xx, yy, xk, yk, Ck,
     $                                 u, dudx, dudy)

      implicit none
      double precision xx, yy, xk, yk, Ck(4,4), u, dudx, dudy

      double precision dx, dy
      integer i

      dx = xx - xk
      dy = yy - yk
      u = 0.d0
      dudx = 0.d0
      dudy = 0.d0
      do i = 4, 1, -1
         u = Ck(i,1) + dy*(Ck(i,2) + dy*(Ck(i,3) + dy*Ck(i,4))) + u*dx
         dudx = Ck(2,i)+dx*(2.d0*Ck(3,i) + dx*3.d0*Ck(4,i)) + dudx*dy
         dudy = Ck(i,2)+dy*(2.d0*Ck(i,3) + dy*3.d0*Ck(i,4)) + dudy*dx
      enddo

      end ! subroutine EvalBicubic_with_grad


      subroutine EvalBicubic_with_grad_and_hes(xx, yy, xk, yk, Ck,
     $                                         u, dudx, dudy,
     $                                         d2udx2, d2udxy, d2udy2)

      implicit none
      double precision xx, yy, xk, yk, Ck(4,4), u, dudx, dudy, d2udx2,
     $                 d2udxy, d2udy2

      double precision dx, dy
      integer i

      dx = xx - xk
      dy = yy - yk
      u = 0.d0
      dudx = 0.d0
      dudy = 0.d0
      d2udx2 = 0.d0
      d2udy2 = 0.d0
      d2udxy = 0.d0
      do i = 4, 1, -1
         u = Ck(i,1) + dy*(Ck(i,2) + dy*(Ck(i,3) + dy*Ck(i,4))) + u*dx
         dudx = Ck(2,i)+dx*(2.d0*Ck(3,i) + dx*3.d0*Ck(4,i)) + dudx*dy
         dudy = Ck(i,2)+dy*(2.d0*Ck(i,3) + dy*3.d0*Ck(i,4)) + dudy*dx
         d2udx2 = 2.d0*Ck(3,i) + dx*6.d0*Ck(4,i) + d2udx2*dy
         d2udy2 = 2.d0*Ck(i,3) + dy*6.d0*Ck(i,4) + d2udy2*dx
      enddo
      d2udxy =            Ck(2,2)+dy*(2.d0*Ck(2,3)+dy*3.d0*Ck(2,4))
     .        + dx*(2.d0*(Ck(3,2)+dy*(2.d0*Ck(3,3)+dy*3.d0*Ck(3,4)))
     .        + dx*(3.d0*(Ck(4,2)+dy*(2.d0*Ck(4,3)+dy*3.d0*Ck(4,4)))))
      end ! subroutine EvalBicubic_with_grad_and_hes


      subroutine BiCubicInterp(x, y, C, nx, ny, x_eval, y_eval, z_eval,
     $                         m, outmode)
*
*     PURPOSE
*        bicubic interpolation :
*          the grid is defined by x(1..nx), y(1..ny)
*          the known values are z(1..nx,1..ny), (z(i,j) being the value
*          at point (x(i),y(j)))
*          the interpolation is done on the points x_eval,y_eval(1..m)
*          z_eval(k) is the result of the bicubic interpolation of
*          (x_eval(k), y_eval(k))
*
      implicit none
      integer nx, ny, m, outmode
      double precision x(nx), y(ny), C(4,4,nx-1,ny-1),
     $                 x_eval(m), y_eval(m), z_eval(m)

      double precision xx, yy
      integer i, j, k
      include 'constinterp.h.f'
      integer  isanan
      double precision EvalBicubic
      external isanan, returnananfortran, EvalBicubic

      i = 0
      j = 0
      do k = 1, m
         xx = x_eval(k)
         call fast_int_search(xx, x, nx, i)
         yy = y_eval(k)
         call fast_int_search(yy, y, ny, j)

         if (i .ne. 0  .and.  j .ne. 0) then
            z_eval(k) = EvalBicubic(xx, yy, x(i), y(j), C(1,1,i,j))

         elseif (outmode .eq. BY_NAN  .or.  isanan(xx) .eq. 1
     $                                .or.  isanan(yy) .eq. 1) then
            CALL returnananfortran(z_eval(k))

         elseif (outmode .eq. BY_ZERO) then
            z_eval(k) = 0.d0

         elseif (outmode .eq. PERIODIC) then
            if (i .eq. 0) call coord_by_periodicity(xx, x, nx, i)
            if (j .eq. 0) call coord_by_periodicity(yy, y, ny, j)
            z_eval(k) = EvalBicubic(xx, yy, x(i), y(j), C(1,1,i,j))

         elseif (outmode .eq. C0) then
            if (i .eq. 0) call near_grid_point(xx, x, nx, i)
            if (j .eq. 0) call near_grid_point(yy, y, ny, j)
            z_eval(k) = EvalBicubic(xx, yy, x(i), y(j), C(1,1,i,j))

         elseif (outmode .eq. NATURAL) then
            if (i .eq. 0) call near_interval(xx, x, nx, i)
            if (j .eq. 0) call near_interval(yy, y, ny, j)
            z_eval(k) = EvalBicubic(xx, yy, x(i), y(j), C(1,1,i,j))
         endif

      enddo

      end

      subroutine BiCubicInterpWithGrad(x, y, C, nx, ny, x_eval, y_eval,
     $                                 z_eval, dzdx_eval, dzdy_eval,m,
     $                                 outmode)
*
*     PURPOSE
*        bicubic interpolation :
*          the grid is defined by x(1..nx), y(1..ny)
*          the known values are z(1..nx,1..ny), (z(i,j) being the value
*          at point (x(i),y(j)))
*          the interpolation is done on the points x_eval,y_eval(1..m)
*          z_eval(k) is the result of the bicubic interpolation of
*          (x_eval(k), y_eval(k)) and dzdx_eval(k), dzdy_eval(k) is the gradient
*
      implicit none
      integer nx, ny, m, outmode
      double precision x(nx), y(ny), C(4,4,nx-1,ny-1),
     $                 x_eval(m), y_eval(m), z_eval(m),
     $                 dzdx_eval(m), dzdy_eval(m)

      double precision xx, yy
      integer k, i, j
      include 'constinterp.h.f'
      integer  isanan
      double precision EvalBicubic
      external isanan, returnananfortran, EvalBicubic
      logical change_dzdx, change_dzdy

      i = 0
      j = 0
      do k = 1, m
         xx = x_eval(k)
         call fast_int_search(xx, x, nx, i)
         yy = y_eval(k)
         call fast_int_search(yy, y, ny, j)

         if (i .ne. 0  .and.  j .ne. 0) then
            call Evalbicubic_with_grad(xx, yy, x(i), y(j), C(1,1,i,j),
     $                                 z_eval(k),
     $                                 dzdx_eval(k), dzdy_eval(k))

         elseif ( outmode .eq. BY_NAN  .or.  isanan(xx) .eq. 1
     $                                 .or.  isanan(yy) .eq. 1) then
            CALL returnananfortran(z_eval(k))
            dzdx_eval(k) = z_eval(k)
            dzdy_eval(k) = z_eval(k)

         elseif (outmode .eq. BY_ZERO ) then
            z_eval(k) = 0.d0
            dzdx_eval(k) = 0.d0
            dzdy_eval(k) = 0.d0

         elseif (outmode .eq. PERIODIC) then
            if (i .eq. 0) call coord_by_periodicity(xx, x, nx, i)
            if (j .eq. 0) call coord_by_periodicity(yy, y, ny, j)
            call Evalbicubic_with_grad(xx, yy, x(i), y(j), C(1,1,i,j),
     $                                 z_eval(k),
     $                                 dzdx_eval(k), dzdy_eval(k))

         elseif (outmode .eq. C0) then
            if (i .eq. 0) then
               call near_grid_point(xx, x, nx, i)
               change_dzdx = .true.
            else
               change_dzdx = .false.
            endif
            if (j .eq. 0) then
               call near_grid_point(yy, y, ny, j)
               change_dzdy = .true.
            else
               change_dzdy = .false.
            endif
            call Evalbicubic_with_grad(xx, yy, x(i), y(j), C(1,1,i,j),
     $                                 z_eval(k),
     $                                 dzdx_eval(k), dzdy_eval(k))
            if (change_dzdx) dzdx_eval(k) = 0.d0
            if (change_dzdy) dzdy_eval(k) = 0.d0

         elseif (outmode .eq. NATURAL) then
            if (i .eq. 0) call near_interval(xx, x, nx, i)
            if (j .eq. 0) call near_interval(yy, y, ny, j)
            call Evalbicubic_with_grad(xx, yy, x(i), y(j), C(1,1,i,j),
     $                                 z_eval(k),
     $                                 dzdx_eval(k), dzdy_eval(k))

         endif

      enddo

      end

      subroutine BiCubicInterpWithGradAndHes(x, y, C, nx, ny,
     $                                       x_eval, y_eval, z_eval,
     $                                         dzdx_eval, dzdy_eval,
     $                        d2zdx2_eval, d2zdxy_eval, d2zdy2_eval,
     $                                                    m, outmode)
*
*     PURPOSE
*        bicubic interpolation :
*          the grid is defined by x(1..nx), y(1..ny)
*          the known values are z(1..nx,1..ny), (z(i,j) being the value
*          at point (x(i),y(j)))
*          the interpolation is done on the points x_eval,y_eval(1..m)
*          z_eval(k) is the result of the bicubic interpolation of
*          (x_eval(k), y_eval(k)), [dzdx_eval(k), dzdy_eval(k)] is the gradient
*          and [d2zdx2(k), d2zdxy(k), d2zdy2(k)] the Hessian
*
      implicit none
      integer nx, ny, m, outmode
      double precision x(nx), y(ny), C(4,4,nx-1,ny-1),
     $                 x_eval(m), y_eval(m), z_eval(m), dzdx_eval(m),
     $   dzdy_eval(m), d2zdx2_eval(m), d2zdxy_eval(m), d2zdy2_eval(m)

      double precision xx, yy
      integer k, i, j
      include 'constinterp.h.f'
      integer  isanan
      double precision EvalBicubic
      external isanan, returnananfortran, EvalBicubic
      logical change_dzdx, change_dzdy

      i = 0
      j = 0
      do k = 1, m
         xx = x_eval(k)
         call fast_int_search(xx, x, nx, i)
         yy = y_eval(k)
         call fast_int_search(yy, y, ny, j)

         if (i .ne. 0  .and.  j .ne. 0) then
            call Evalbicubic_with_grad_and_hes(xx, yy, x(i), y(j),
     $                                      C(1,1,i,j), z_eval(k),
     $                                 dzdx_eval(k), dzdy_eval(k),
     $             d2zdx2_eval(k), d2zdxy_eval(k), d2zdy2_eval(k))

         elseif ( outmode .eq. BY_NAN  .or.  isanan(xx) .eq. 1
     $                                 .or.  isanan(yy) .eq. 1) then
            CALL returnananfortran(z_eval(k))
            dzdx_eval(k) = z_eval(k)
            dzdy_eval(k) = z_eval(k)
            d2zdx2_eval(k) = z_eval(k)
            d2zdxy_eval(k) = z_eval(k)
            d2zdy2_eval(k) = z_eval(k)

         elseif (outmode .eq. BY_ZERO ) then
            z_eval(k) = 0.d0
            dzdx_eval(k) = 0.d0
            dzdy_eval(k) = 0.d0
            d2zdx2_eval(k) = 0.d0
            d2zdxy_eval(k) = 0.d0
            d2zdy2_eval(k) = 0.d0

         elseif (outmode .eq. PERIODIC) then
            if (i .eq. 0) call coord_by_periodicity(xx, x, nx, i)
            if (j .eq. 0) call coord_by_periodicity(yy, y, ny, j)
            call Evalbicubic_with_grad_and_hes(xx, yy, x(i), y(j),
     $                                      C(1,1,i,j), z_eval(k),
     $                                 dzdx_eval(k), dzdy_eval(k),
     $             d2zdx2_eval(k), d2zdxy_eval(k), d2zdy2_eval(k))

         elseif (outmode .eq. C0) then
            if (i .eq. 0) then
               call near_grid_point(xx, x, nx, i)
               change_dzdx = .true.
            else
               change_dzdx = .false.
            endif
            if (j .eq. 0) then
               call near_grid_point(yy, y, ny, j)
               change_dzdy = .true.
            else
               change_dzdy = .false.
            endif
            call Evalbicubic_with_grad_and_hes(xx, yy, x(i), y(j),
     $                                      C(1,1,i,j), z_eval(k),
     $                                 dzdx_eval(k), dzdy_eval(k),
     $             d2zdx2_eval(k), d2zdxy_eval(k), d2zdy2_eval(k))
            if (change_dzdx) then
               dzdx_eval(k) = 0.d0
               d2zdx2_eval(k) = 0.d0
               d2zdxy_eval(k) = 0.d0
            endif
            if (change_dzdy) then
               dzdy_eval(k) = 0.d0
               d2zdxy_eval(k) = 0.d0
               d2zdy2_eval(k) = 0.d0
            endif

         elseif (outmode .eq. NATURAL) then
            if (i .eq. 0) call near_interval(xx, x, nx, i)
            if (j .eq. 0) call near_interval(yy, y, ny, j)
            call Evalbicubic_with_grad_and_hes(xx, yy, x(i), y(j),
     $                                      C(1,1,i,j), z_eval(k),
     $                                 dzdx_eval(k), dzdy_eval(k),
     $             d2zdx2_eval(k), d2zdxy_eval(k), d2zdy2_eval(k))
         endif

      enddo

      end

      subroutine driverdb3val(xp, yp, zp, fp, np, tx, ty, tz,
     $                        nx, ny, nz, kx, ky, kz, bcoef, work,
     $                        xmin, xmax, ymin, ymax, zmin, zmax,
     $                        outmode)
*
*     PURPOSE
*        driver on to db3val
*
      implicit none
      integer np, nx, ny, nz, kx, ky, kz, outmode
      double precision xp(np), yp(np), zp(np), fp(np),
     $                 tx(*), ty(*), tz(*), bcoef(*), work(*),
     $                 xmin, xmax, ymin, ymax, zmin, zmax

      integer k
      logical flag_x, flag_y, flag_z
      double precision x, y, z
      include 'constinterp.h.f'
      integer  isanan
      double precision db3val
      external isanan, returnananfortran, db3val

      do k = 1, np
         x = xp(k)
         if ( xmin .le. x .and. x .le. xmax ) then
            flag_x = .true.
         else
            flag_x = .false.
         endif
         y = yp(k)
         if ( ymin .le. y .and. y .le. ymax ) then
            flag_y = .true.
         else
            flag_y = .false.
         endif
         z = zp(k)
         if ( zmin .le. z .and. z .le. zmax ) then
            flag_z = .true.
         else
            flag_z = .false.
         endif

         if ( flag_x .and. flag_y .and. flag_z ) then
            fp(k) = db3val(x, y, z, 0, 0, 0, tx, ty, tz,
     $                     nx, ny, nz, kx, ky, kz, bcoef, work)

         elseif (outmode .eq. BY_NAN  .or.  isanan(x) .eq. 1
     $                                .or.  isanan(y) .eq. 1
     $                                .or.  isanan(z) .eq. 1) then
            CALL returnananfortran(fp(k))

         elseif (outmode .eq. BY_ZERO) then
            fp(k) = 0.d0

         else
            if (outmode .eq. PERIODIC) then
               if (.not. flag_x) call proj_by_per(x, xmin, xmax)
               if (.not. flag_y) call proj_by_per(y, ymin, ymax)
               if (.not. flag_z) call proj_by_per(z, zmin, zmax)
            elseif (outmode .eq. C0) then
               if (.not. flag_x) call proj_on_grid(x, xmin, xmax)
               if (.not. flag_y) call proj_on_grid(y, ymin, ymax)
               if (.not. flag_z) call proj_on_grid(z, zmin, zmax)
            endif
            fp(k) = db3val(x, y, z, 0, 0, 0, tx, ty, tz, nx, ny, nz,
     $                     kx, ky, kz, bcoef, work)
         endif
      enddo

      end

      subroutine driverdb3valwithgrad(xp, yp, zp, fp,
     $                        dfpdx, dfpdy, dfpdz, np, tx, ty, tz,
     $                        nx, ny, nz, kx, ky, kz, bcoef, work,
     $                        xmin, xmax, ymin, ymax, zmin, zmax,
     $                        outmode)
*
*     PURPOSE
*        driver on to db3val with gradient computing
*
      implicit none
      integer np, nx, ny, nz, kx, ky, kz, outmode
      double precision xp(np), yp(np), zp(np), fp(np),
     $                 dfpdx(np), dfpdy(np), dfpdz(np),
     $                 tx(*), ty(*), tz(*), bcoef(*), work(*),
     $                 xmin, xmax, ymin, ymax, zmin, zmax

      integer k
      logical flag_x, flag_y, flag_z
      double precision x, y, z
      include 'constinterp.h.f'
      integer  isanan
      double precision db3val
      external isanan, returnananfortran, db3val

      do k = 1, np
         x = xp(k)
         if ( xmin .le. x .and. x .le. xmax ) then
            flag_x = .true.
         else
            flag_x = .false.
         endif
         y = yp(k)
         if ( ymin .le. y .and. y .le. ymax ) then
            flag_y = .true.
         else
            flag_y = .false.
         endif
         z = zp(k)
         if ( zmin .le. z .and. z .le. zmax ) then
            flag_z = .true.
         else
            flag_z = .false.
         endif

         if ( flag_x .and. flag_y .and. flag_z ) then
            fp(k) = db3val(x, y, z, 0, 0, 0, tx, ty, tz,
     $                     nx, ny, nz, kx, ky, kz, bcoef, work)
            dfpdx(k) = db3val(x, y, z, 1, 0, 0, tx, ty, tz,
     $                     nx, ny, nz, kx, ky, kz, bcoef, work)
            dfpdy(k) = db3val(x, y, z, 0, 1, 0, tx, ty, tz,
     $                     nx, ny, nz, kx, ky, kz, bcoef, work)
            dfpdz(k) = db3val(x, y, z, 0, 0, 1, tx, ty, tz,
     $                     nx, ny, nz, kx, ky, kz, bcoef, work)

         elseif (outmode .eq. BY_NAN  .or.  isanan(x) .eq. 1
     $                                .or.  isanan(y) .eq. 1
     $                                .or.  isanan(z) .eq. 1) then
            CALL returnananfortran(fp(k))
            dfpdx(k) = fp(k)
            dfpdy(k) = fp(k)
            dfpdz(k) = fp(k)

         elseif (outmode .eq. BY_ZERO) then
            fp(k) = 0.d0
            dfpdx(k) = 0.d0
            dfpdy(k) = 0.d0
            dfpdz(k) = 0.d0

         elseif (outmode .eq. PERIODIC) then
            if (.not. flag_x) call proj_by_per(x, xmin, xmax)
            if (.not. flag_y) call proj_by_per(y, ymin, ymax)
            if (.not. flag_z) call proj_by_per(z, zmin, zmax)
            fp(k) = db3val(x, y, z, 0, 0, 0, tx, ty, tz, nx, ny, nz,
     $                     kx, ky, kz, bcoef, work)
            dfpdx(k) = db3val(x, y, z, 1, 0, 0, tx, ty, tz,
     $                     nx, ny, nz, kx, ky, kz, bcoef, work)
            dfpdy(k) = db3val(x, y, z, 0, 1, 0, tx, ty, tz,
     $                     nx, ny, nz, kx, ky, kz, bcoef, work)
            dfpdz(k) = db3val(x, y, z, 0, 0, 1, tx, ty, tz,
     $                     nx, ny, nz, kx, ky, kz, bcoef, work)

         elseif (outmode .eq. C0) then
            if (.not. flag_x) call proj_on_grid(x, xmin, xmax)
            if (.not. flag_y) call proj_on_grid(y, ymin, ymax)
            if (.not. flag_z) call proj_on_grid(z, zmin, zmax)
            fp(k) = db3val(x, y, z, 0, 0, 0, tx, ty, tz, nx, ny, nz,
     $                     kx, ky, kz, bcoef, work)
            if ( flag_x ) then
               dfpdx(k) = 0.d0
            else
               dfpdx(k) = db3val(x, y, z, 1, 0, 0, tx, ty, tz,
     $                           nx, ny, nz, kx, ky, kz, bcoef, work)
            endif
            if ( flag_y ) then
               dfpdy(k) = 0.d0
            else
               dfpdy(k) = db3val(x, y, z, 0, 1, 0, tx, ty, tz,
     $                           nx, ny, nz, kx, ky, kz, bcoef, work)
            endif
            if ( flag_z ) then
               dfpdz(k) = 0.d0
            else
               dfpdz(k) = db3val(x, y, z, 0, 0, 1, tx, ty, tz,
     $                           nx, ny, nz, kx, ky, kz, bcoef, work)
            endif

         endif
      enddo

      end
