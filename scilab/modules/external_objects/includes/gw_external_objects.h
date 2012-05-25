/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __GW_EXTERNAL_OBJECTS_H__
#define __GW_EXTERNAL_OBJECTS_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_external_objects_scilab.h"
/*--------------------------------------------------------------------------*/
EXTERNAL_OBJECTS_SCILAB_IMPEXP int gw_external_objects(void);

/*--------------------------------------------------------------------------*/
int sci_percent_EObj_e(char * fname, unsigned long fname_len);
int sci_percent_EObj_p(char * fname, unsigned long fname_len);
int sci_percent_EClass_e(char * fname, unsigned long fname_len);
int sci_percent_foo_i_EObj(char * fname, unsigned long fname_len);
int sci_percent_EObj_a_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_d_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_g_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_h_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_j_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_k_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_l_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_m_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_n_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_o_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_p_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_q_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_r_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_s_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_t_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_x_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_y_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_z_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_1_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_2_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_3_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_4_foo(char * fname, unsigned long fname_len);
int sci_percent_EObj_0(char * fname, unsigned long fname_len);
int sci_percent_EObj_5(char * fname, unsigned long fname_len);
int sci_percent_EObj_t(char * fname, unsigned long fname_len);
int sci_invoke_lu(char * fname, unsigned long fname_len);
int sci_doubleExclam_invoke_(char * fname, unsigned long fname_len);
int sci_percent_EObj_disp(char * fname, unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /* __GW_EXTERNAL_OBJECTS_H__ */
/*--------------------------------------------------------------------------*/
