/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * Copyright (C) ENPC
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SCI_TYPES_H__
#define __SCI_TYPES_H__

/*-------------------------------------------------
 * types
 * -------------------------------------------------*/

typedef enum {
  sci_matrix = 1 ,
  sci_poly = 2 ,
  sci_boolean = 4 ,
  sci_sparse = 5  ,
  sci_boolean_sparse = 6,
  sci_matlab_sparse = 7, /* matlab sparse matrix  */
  sci_ints = 8,
  sci_handles =9,
  sci_strings = 10,
  sci_u_function = 11,
  sci_c_function = 13 ,
  sci_lib =  14,
  sci_list = 15,
  sci_tlist = 16,
  sci_mlist = 17,
  sci_pointer = 128, /* pointer */
  sci_implicit_poly = 129,
  sci_intrinsic_function = 130

} sci_types;


/* These values are also used in:
 * modules/api_scilab/includes/api_int.h */
typedef enum {
    sci_int8 = 1,
    sci_int16 = 2,
    sci_int32 = 4,
    sci_int64 = 8,
    sci_uint8 = 11,
    sci_uint16 = 12,
    sci_uint32 = 14,
    sci_uint64 = 18
} sci_int_types;


 /* lufact pointer... Done for backward compatibility. 
  *	sci_lufact_pointer has been added to be able to handle umfpack data type
  * Since the use of pointer data structure is very interesting for Scilab 
  * we decided to rename it to remove the reference of the initial use.
  */
#define sci_lufact_pointer sci_pointer

#endif /* __SCI_TYPES_H__ */
