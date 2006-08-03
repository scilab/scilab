      subroutine anfm02(q,iq,r,ir,n,m,icol,io)
C     SUBROUTINE ANFM02(Q,IQ,R,IR,N,M,ICOL,IO)
C
C***********************************************************************
C                                                                      *
C                                                                      *
C     Copyright:        Eduardo Casas Renteria                         *
C                       Cecilia Pola Mendez                            *
C                                                                      *
C       Departamento de Matematicas,Estadistica y Computacion          *
C       -----------------------------------------------------          *
C                       UNIVERSIDAD DE CANTABRIA                       *
C                       ------------------------                       *
C                            FEBRERO 1987                              *
C                                                                      *
C***********************************************************************
C
C     OBJETIVO:
C        Esta subrutina modifica la factorizacion QR de una matriz,
C        cuando a esta se le quita una columna.
C
C     LISTA DE LLAMADA:
C     DE ENTRADA:
C
C        Q      Matriz de dimension (IQ,N).En sus N primeras filas
C               contiene a la matriz " q " de la factorizacion QR
C               inicial.
C
C        IQ     Primera dimension de Q. IQ >= N.
C
C        R      Matriz de dimension (IR,N).En sus N primeras filas
C               se tiene a la matriz " r " de la factorizacion QR
C               inicial.
C
C        IR     Primera dimension de R. IR >= N.
C
C        N      Numero de filas de R,numero de filas y columnas de Q.
C
C        M      Numero de columnas de R.
C
C        ICOL   Numero de columna que se desea quitar en la matriz.
C
C        IO     Numero de canal de salida de resultados.
C
C     DE SALIDA:
C
C        Q      Recoge la matriz ortogonal correspondiente a la nueva
C               factorizacion QR.
C
C        R      En R se tiene la matriz triangular superior de la nueva
C               factorizacion QR.
C
C        Esta subrutina trabaja en doble precision via una sentencia
C     "implicit":
C                Implicit double precision (a-h,o-z)
C
C     SUBPROGRAMAS AUXILIARES: daxpy, dcopy,ddot,dscal,d1mach
C     FUNCIONES FORTRAN INTRINSECAS: mod,sign
C
C
      implicit double precision (a-h,o-z)
      dimension q(iq,*),r(ir,*)
C
C     Se comprueba si los valores de las variables son correctos
C
      if(m.lt.icol .or. m.lt.2 .or. icol.lt.1 .or. iq.lt.n .or.
     &   ir.lt.n) then
         write(io,'(10x,A)') 'INCORRECT LIST OF CALLING IN ANFM02.'
         stop
      end if
C
C     Si la columna que se desea quitar ocupa el ultimo lugar en la
C     matriz,el proceso finaliza
C
      if(m.eq.icol) return
C
C     Se modifican adecuadamente las ultimas columnas de R y la matriz Q
C
css      epsmch=d1mach(4)
      epsmch=dlamch('p')

      do 15 i=icol+1,m
         if(r(i,i).ne.0.d0) then
            i1=i-1
            a=dnrm2(2,r(i1,i),1)
            if(a.gt.epsmch) then
               if(r(i1,i).ne.0.d0) a=sign(a,r(i1,i))
               call dscal(2,1.d0/a,r(i1,i),1)
               r(i1,i)=1.d0+r(i1,i)
               s1=r(i1,i)
               s2=r(i,i)
               s=s2/s1
               do 10 j=1,n
                  t=-q(j,i1)-q(j,i)*s
                  q(j,i1)=q(j,i1)+t*s1
                  q(j,i)=q(j,i)+t*s2
10             continue
               do 12 j=i+1,m
                  t=-r(i1,j)-r(i,j)*s
                  r(i1,j)=r(i1,j)+t*s1
                  r(i,j)=r(i,j)+t*s2
12             continue
               r(i1,i)=-a
            end if
         end if
15    continue
C
C     Se coloca en las M-1 primeras columnas la matriz R resultante
C
      do 20 j=icol+1,m
         i1=j-1
         call dcopy(i1,r(1,j),1,r(1,i1),1)
20    continue
      end
