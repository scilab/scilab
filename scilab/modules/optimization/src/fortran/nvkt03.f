      subroutine nvkt03(a,ia,c,ic,g,v,w,ipvt,dnorma,n,m,mi1,mi,nmd,ndf)
C     SUBROUTINE NVKT03(A,IA,C,IC,G,V,W,IPVT,DNORMA,N,M,MI1,MI,NMD,NDF)
C
C***********************************************************************
C                                                                      *
C                                                                      *
C      Copyright:       Eduardo Casas Renteria                         *
C                       Cecilia Pola Mendez                            *
C                                                                      *
C       Departamento de Matematicas,Estadistica y Computacion          *
C       -----------------------------------------------------          *
C                       UNIVERSIDAD DE CANTABRIA                       *
C                       ------------------------                       *
C                              AGOSTO  1987                            *
C                                                                      *
C***********************************************************************
C
C     OBJETIVO:
C        Esta es una subrutina auxiliar a  OPTR03. Calcula la norma del
C        vector de Kuhn-Tucker.
C
C     LISTA DE LLAMADA:
C
C        A      Matriz de dimension (IA,NDF-NMD). (Referencia en OPTR03)
C
C        C      Matriz  de  dimension   (IC,MI+NMD-N).  ( Referencia  en
C               OPTR03).
C
C        G      Vector de dimension  N que contiene el vector gradiente.
C
C        V      Vector  de  dimension  M  que contiene  el vector de los
C               multiplicadores de Lagrange.
C
C        W      Vector de trabajo de dimension  N+M.
C
C        IPVT   Vector de dimension  M. (Referencia en  OPTR03).
C
C        DNORMA Variable de salida que contiene la norma del vector de
C               Kuhn-Tucker.
C
C        MI1    (Referencia en OPTR03).
C
C        MI     (Referencia en  OPTR03).
C
C        NMD    N+MD (Referencia en OPTR03).
C
C        NDF    NMD+MIF+MDF+1 (Referencia en OPTR03).
C
C
C        Esta subrutina trabaja en doble precision via una sentencia
C     "implicit":
C                Implicit double precision (a-h,o-z)
C
C     SUBPROGRAMAS AUXILIARES:  ddot,dadd,dnrm2
C     FUNCIONES FORTRAN INTRINSECAS:  abs,mod,sqrt
C
      implicit double precision(a-h,o-z)
      dimension a(ia,*),c(ic,*),v(*),w(*),ipvt(*),g(*)
      m1=m+1
      mi2=mi1+1
      ni=mi-n
      do 30 i=1,n
         do 10 j=1,mi1
10       w(j)=c(i,ipvt(j))
         do 20 j=mi2,m
            ij=ipvt(j)
            if(ij.lt.0) then
               if(i.eq.-ij) then
                  w(j)=-1
               else
                  w(j)=0
               end if
            else if(ij.le.n) then
               if(i.eq.ij) then
                  w(j)=1
               else
                  w(j)=0
               end if
            else if(ij.le.nmd) then
               w(j)=c(i,ij+ni)
            else if(ij.lt.ndf) then
               w(j)=a(i,ij-nmd)
            end if
20       continue
         w(m+i)=ddot(m,w,1,v,1)
30    continue
      call dadd(n,g,1,w(m1),1)
      dnorma=dnrm2(n,w(m1),1)
      end
