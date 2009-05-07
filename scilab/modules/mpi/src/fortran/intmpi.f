c     $Log: INTMPI.f,v $
c     Revision 1.4  1999/09/14 12:41:13  fleury
c     Mise a jour pour la version de scilabp
c      - macro de blacs_init: permet de demarrer une grille blacs a partir
c     de la console scilex
c      - script de demarage de scilabp: initialise les blacs et se met en
c     mode recv/exec
c
c     Modif ds pvm
c      - pvm_start prend en argument un fichier de config pvm
c
c     Modif des conversion de complexe
c
c     Modif des blacs
c      - suppression des _ ds le nom des funct car ca fout la merde sous gcc
c
c     Modif ds l interface MPI
c      - TODO: a reprendre pour le nom des function et pour le lancement des
c     prg
c

c  interface function 
c   ********************
       subroutine intmpi
       include 'stack.h'
       rhs = max(0,rhs)
c
       goto (1,2,3,4,5,6,7,8) fin
       return
1      call intsmpi_send('mpi_send')
       return
2      call intsmpi_recv('mpi_recv')
       return
3      call intsmpi_isend('mpi_isend')
       return
4      call intsmpi_irecv('mpi_irecv')
       return
5      call intsmpi_init('mpi_init')
       return
6      call intsmpi_finalize('mpi_finalize')
       return
7      call intsmpi_comm_rank('mpi_comm_rank')
       return
8      call intsmpi_comm_size('mpi_comm_size')
       return
       end
