// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Yann COLLETTE <yann.collette@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// test overloading of the uicontrol functions

// create a mlist
mymlist = mlist(['objid','A','B'],[],[]);

// overload set / get for objid
function result = %objid_uicontrol(varargin)
  // res = uicontrol(mymlist,'A');
  obj_tmp   = varargin(1);
  field_tmp = varargin(2);
  printf('uicontrol on an object of type %s, field = %s\n', typeof(obj_tmp), field_tmp);
  result = %t;
endfunction

ierr = execstr('uicontrol(mymlist,''property'');','errcatch');

if ierr then pause; end

