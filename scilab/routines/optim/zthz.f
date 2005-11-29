      DOUBLE PRECISION FUNCTION ZTHZ(H,IH,Z,IZ,N,I1,I2)
C
C***********************************************************************
C                                                                      *
C                                                                      *
C     Copyright:        Eduardo Casas Renteria                         *
C                       Cecilia Pola Mendez                            *
C                                                                      *
C       Departamento de Matematicas,Estadistica y Computacion          *
C       -----------------------------------------------------          *
C                     UNIVERSIDAD DE CANTABRIA                         *
C                     ------------------------                         *
C                          FEBRERO 1988                                *
C                                                                      *
C***********************************************************************
C
C     OBJETIVO:
C        Este subprograma calcula el elemento  (i1,i2)  del producto
C        matricial  ZTHZ.
C
C     LISTA DE LLAMADA:
C
C        H     Matriz  de  dimension  (IH,N)  que  contiene, en  sus  N
C              primeras filas, la matriz  H.  Es suficiente  suministrar
C              la parte triangular inferior. El resto de la matriz no se
C              utiliza.
C
C        IH    Primera dimension de la matriz  H. IH >= N.
C
C        Z     Matriz de dimension  (IZ,*) que contiene la matriz  Z.
C
C        IZ    Primera dimension de la matriz  Z. IZ >= N.
C
C        N     Numero de filas de  Z, numero de filas y columnas de  H.
C
C      I1,I2   (I1,I2) indica el elemento que se desea calcular.
C
C        Este subprograma trabaja en doble precision via una sentencia
C     "implicit":
C                Implicit double precision (a-h,o-z)
C
C     SUBPROGRAMAS AUXILIARES: ddot
C     FUNCIONES FORTRAN INTRINSECAS: mod
C
C
      implicit double precision(a-h,o-z)
      dimension h(ih,*),z(iz,*)
      zthz=0
      do 10 j=1,n
         jj=j+1
         s=ddot(j,h(j,1),ih,z(1,i2),1)
         s=s+ddot(n-j,h(jj,j),1,z(jj,i2),1)
         zthz=zthz+s*z(j,i1)
10    continue
      end
