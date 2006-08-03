      subroutine anrs01(r,ir,m,b,x,ind,io)
C     SUBROUTINE ANRS01(R,IR,M,B,X,IND,IO)
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
C                          FEBRERO 1987                                *
C                                                                      *
C***********************************************************************
C
C     OBJETIVO:
C        Esta subrutina resuelve un sistema de ecuaciones lineales en el
C        que la matriz de coeficientes es triangular.
C
C     LISTA DE LLAMADA:
C     DE ENTRADA:
C
C        R      Matriz de dimension (IR,M),triangular superior.Contiene
C               en sus M primeras filas a la matriz de coeficientes del
C               sistema.La parte subdiagonal de R no es utilizada.
C
C        IR     Primera dimension de la matriz R. IR >= N.
C
C        M      Numero de filas y columnas de la matriz de coeficientes.
C
C        B      Vector M-dimensional.Guarda los terminos independientes
C               del sistema.
C
C        IND    Indica el tipo de sistema a resolver,con los valores:
C                  1  : Se resuelve R'x=b
C                  2  : Se resuelve Rx=b
C
C        IO     Numero de canal de salida de resultados.
C
C     DE SALIDA:
C
C        X      Vector m-dimensional en el que se recoge  la solucion
C               del sistema.
C
C        Esta subrutina trabaja en doble precision via una sentencia
C     "implicit":
C                Implicit double precision (a-h,o-z)
C
C     SUBPROGRAMAS AUXILIARES: ddot,d1mach
C     FUNCIONES FORTRAN INTRINSECAS: abs,mod
C
C
      implicit double precision(a-h,o-z)
      dimension r(ir,*),b(*),x(*)
C
C     Se comprueba si los valores de las variables son correctos
C
      if(m.lt.1 .or. ir.lt.1 .or. m.gt.ir .or. ind.lt.1 .or. ind.gt.2)
     &   then
         write(io,1000) 'INCORRECT LIST OF CALLING IN ANRS01.'
         stop
      end if
C
C     Se calcula un parametro para detectar la posible singularidad de
C     la matriz de coeficientes
C
css   epsmch=d1mach(4)**0.9
      epsmch=dlamch('p')**0.9
C
C     Se comienza la resolucion del sistema segun sea el indicador
C
      if(ind.eq.1) then
         j=1
      else
         j=m
      end if
      if(abs(r(j,j)).lt.epsmch) then
         write(io,1000) 'SINGULAR MATRIX IN ANRS01.'
         stop
      end if
      x(j)=b(j)/r(j,j)
      if(m.eq.1) return
      do 10 i=2,m
         i1=i-1
         if(ind.eq.1) then
            j=i
            j1=1
            j2=i
            j3=1
            k=1
         else
            j=m-i1
            j1=j
            j2=j+1
            j3=j2
            k=ir
         end if
         if(abs(r(j,j)).lt.epsmch) then
            write(io,1000) 'SINGULAR MATRIX IN ANRS01.'
            stop
         end if
         x(j)=(b(j)-ddot(i1,r(j1,j2),k,x(j3),1))/r(j,j)
10    continue
1000  format(10x,A)
      end
