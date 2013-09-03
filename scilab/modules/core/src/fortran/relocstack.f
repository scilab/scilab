c     
c     Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c     Copyright (C) 2008-2008 - INRIA - Simon LIPP
c     
c     This file must be used under the terms of the CeCILL.
c     This source file is licensed as described in the file COPYING, which
c     you should have received as part of this distribution.  The terms
c     are also available at    
c     http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c     
      subroutine relocstack(new_stk, new_istk, new_sstk, new_cstk,
     $ new_zstk, nstk, nistk, nsstk, ncstk, nzstk)
      include 'stack.h'
      
      double precision, target, dimension(nstk)  ::  new_stk
      integer,          target, dimension(nistk) :: new_istk
      real,             target, dimension(nsstk) :: new_sstk
      character,        target                   :: new_cstk*(4*vsiz)
      complex*16,       target, dimension(nzstk) :: new_zstk

      stk  => new_stk
      istk => new_istk
      sstk => new_sstk
      cstk => new_cstk
      zstk => new_zstk

      return
      end
