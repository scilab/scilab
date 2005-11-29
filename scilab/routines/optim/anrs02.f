      subroutine anrs02 (a,ia,b,w,ipvt,n,io)
C     SUBROUTINE ANRS02 (A,IA,B,W,IPVT,N,IO)
C
C***********************************************************************
C                                                                      *
C                                                                      *
C     Copyright:        Eduardo Casas Renteria                         *
C                       Cecilia Pola Mendez                            *
C                                                                      *
C       Departamento de Matematicas,estadistica y Computacion          *
C       -----------------------------------------------------          *
C                       UNIVERSIDAD DE CANTABRIA                       *
C                       ------------------------                       *
C                               MAYO  1987                             *
C                                                                      *
C***********************************************************************
C
C     OBJETIVO:
C        Esta subrutina resuelve un sistema del tipo:
C                                P'LL'Px=b ,
C        donde  P  es una matriz de permutacion y  L  es una matriz
C        triangular inferior.
C
C     LISTA DE LLAMADA:
C     DE ENTRADA:
C
C        A      Matriz de dimension (IA,N).  En sus N primeras filas,
C               almacena a la matriz triangular superior L'. La parte
C               subdiagonal de A no es utilizada.
C
C        IA     Primera dimension de la matriz A. IA >= N.
C
C        B      Vector N-dimensional. Guarda los terminos independientes
C               del sistema.
C
C        W      Vector de trabajo N-dimensional.
C
C        IPVT   Vector entero de trabajo de dimension N. Se utiliza para
C               interpretar el cambio de filas producido al multiplicar
C               por la derecha por la matriz de permutacion P :
C                    IPVT(I)=J, indica que en el lugar I-esimo se coloca
C               la fila J-esima
C        N      Numero de filas y columnas de la matriz de coeficientes.
C
C        IO     Numero de canal de salida de resultados.
C
C     DE SALIDA:
C
C        B      Vector que recoge la solucion del sistema.
C
C
C        Esta subrutina trabaja en doble precision via una sentencia
C     "implicit":
C                implicit double precision (a-h,o-z)
C
C     SUPROGRAMAS AUXILIARES: anrs01,ddot,d1mach
C     FUNCIONES FORTRAN INTRINSECAS: abs,mod
C
C
      implicit double precision (a-h,o-z)
      dimension a(ia,*),b(*),w(*),ipvt(*)
C
C     Se resuelve el sistema Ly=Pb
C
      do 10 i=1,n
10    w(i)=b(ipvt(i))
      ind=1
      call anrs01(a,ia,n,w,w,ind,io)
C
C     Se resuelve el sistema L'z=y
C
      ind=2
      call anrs01(a,ia,n,w,w,ind,io)
C
C     Se calcula P'z
C
      do 20 i=1,n
20    b(ipvt(i))=w(i)
      end
