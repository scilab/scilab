
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Sylvestre LEDRU <sylvestre.ledru@inria.fr>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*
 * Local declaration file for mexlib
 *
 */

/**
 * TODO : comment
 * @param ptr_lstk
 * @return
 */
vraiptrst stkptr(long int ptr_lstk);


/**
 * TODO : comment
 * @param ptr
 * @return
 */
int *Header(const mxArray *ptr);

/**
 * TODO : comment
 * @param header
 * @return
 */
mxArray *header2ptr(int *header);


/**
 * TODO : comment
 * @param header
 * @param i
 * @return
 */
int *listentry(int *header, int i);


/**
 * TODO : comment
 * @param header
 * @return
 */
int theMLIST(int *header);


/**
 * TODO : comment
 * @param ptr
 * @return
 */
int *RawHeader(const mxArray *ptr);


/**
 * TODO : comment
 * @param ptr
 * @return
 */
int Is1x1(const mxArray *ptr);


/**
 * TODO : comment
 * @param m
 * @return
 */
mxArray *mxCreateData(int m);


/**
 * TODO : comment
 * @param expr
 * @param error_message
 */
void mxAssert(int expr, char *error_message);


/**
 * TODO : comment
 * @param ptr
 * @param nsize
 */
void *mxRealloc(void *ptr, size_t nsize);


/**
 * TODO : comment
 * @param ptr
 */
void mexMakeMemoryPersistent(void *ptr);


/**
 * TODO : comment
 * @param ptr
 * @return
 */
int IsstOrce(mxArray *ptr);


/**
 * TODO : comment
 * @param ptr
 * @param number
 * @param size
 */
void  numberandsize(const mxArray  *ptr, int *number, int *size);

/**
 * TODO : comment
 * @param ptr
 * @return
 */
bool mexIsGlobal(const mxArray *ptr);

/**
 * Unreference objects in a struct or cell 
 * @param ptr
 * @return
*/
mxArray *UnrefStruct(mxArray *ptr);

/**
 * TODO : comment
 * @param m
 * @param n
 * @return
*/
mxArray *mxCreateLogicalMatrix(int m, int n);

/**
 * TODO : comment
 * @param value
 * @return
*/
mxArray *mxCreateLogicalScalar(mxLOGICAL *value);

/**
 * TODO : comment
 * @param pa
 * @return
*/
bool mxIsLogicalScalarTrue(mxArray *pa);


/**
 * TODO : comment
 * @param pa
 * @return
*/
bool mxIsLogicalScalar(mxArray *pa);


/**
 * TODO : comment
 * @return
*/
const char *mexFunctionName(void);


/**
 * TODO : comment
 * @param array_ptr
 * @param workspace
 * @return
*/
int mexPutArray(mxArray *array_ptr, char *workspace);


/**
 * TODO : comment
 * @param workspace
 * @param var_name
 * @param array_ptr
 * @return
*/
int mexPutVariable(const char *workspace, char *var_name, mxArray *array_ptr);


/**
 * TODO : comment
 * @param ptr
 * @return
*/
const char *mxGetClassName(const mxArray *ptr);


/**
 * TODO : comment
 * @param m
 * @param n
 * @param it
 * @return
*/
mxArray *C2F(mxcreatedoublematrix)(int *m, int *n, int *it);


/**
 * TODO : comment
 * @param y
 * @param ptr
 * @param pti
 * @param n
 * @return
*/
int C2F(mxcopycomplex16toptr)(double *y, mxArray *ptr, mxArray *pti, integer *n);


/**
 * TODO : comment
 * @param ptr
 * @param pti
 * @param y
 * @param n
 * @return
*/
int C2F(mxcopyptrtocomplex16)(mxArray *ptr, mxArray *pti, double *y, integer *n);



