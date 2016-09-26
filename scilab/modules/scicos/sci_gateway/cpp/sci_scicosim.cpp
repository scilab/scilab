/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include <string>
#include <cstdio>
#include <cwchar>
#include <cstring>

#include "gw_scicos.hxx"

#include "internal.hxx"
#include "types.hxx"
#include "context.hxx"
#include "double.hxx"
#include "function.hxx"
#include "macro.hxx"
#include "list.hxx"
#include "tlist.hxx"
#include "string.hxx"
#include "int.hxx"

#include "il_state.hxx"
#include "il_sim.hxx"

extern "C"
{
#include "machine.h" /* C2F */
#include "import.h"
#include "sci_malloc.h"
#include "scicos.h" /* funnum2() */
#include "scicos-def.h"
#include "charEncoding.h"

#include "localization.h"
#include "Scierror.h"

    COSIM_struct C2F(cosim);
    /*--------------------------------------------------------------------------*/
    // Variables defined in scicos.c
    extern COSERR_struct coserr;
    extern CURBLK_struct C2F(curblk);
    /*--------------------------------------------------------------------------*/
}

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

/*--------------------------------------------------------------------------*/
/* intsicosimc scicosim interface routine.
*
* [state,t] = scicosim(state,tcur,tf,sim,str,tol)
*
* rhs 1 state : Tlist
*        - 1  : state(1)     : !xcs  x  z  oz iz  tevts  evtspt  pointi  outtb  !
*        - 2  : state.x      : column vector of real
*        - 3  : state.z      : column vector of real
*        - 4  : state.oz     : list of scilab object
*        - 5  : state.iz     : column vector of real (empty object with flag "finish")
*        - 6  : state.tevts  : column vector of real
*        - 7  : state.evtspt : column vector of real
*        - 8  : state.pointi : real scalar
*        - 9  : state.outtb  : list of scilab object
* rhs 2 tcur  : real scalar
* rhs 3 tf    : real scalar
* rhs 4 sim   : Tlist
*        - 1  : sim(1) : !scs    funs    xptr    zptr    ozptr   zcptr   inpptr
*                         outptr inplnk  outlnk  rpar    rpptr   ipar    ipptr
*                         opar   opptr   clkptr  ordptr  execlk  ordclk  cord
*                         oord   zord    critev  nb      ztyp    nblk    ndcblk
*                         subscr funtyp  iord    labels  modptr  uids !
*        - 2  : sim.funs   : list of strings and/or scilab function
*        - 3  : sim.xptr   : column vector of real
*        - 4  : sim.zptr   : column vector of real
*        - 5  : sim.ozptr  : column vector of real
*        - 6  : sim.zcptr  : column vector of real
*        - 7  : sim.inpptr : column vector of real
*        - 8  : sim.outptr : column vector of real
*        - 9  : sim.inplnk : column vector of real
*        - 10 : sim.outlnk : column vector of real
*        - 11 : sim.rpar   : column vector of real
*        - 12 : sim.rpptr  : column vector of real
*        - 13 : sim.ipar   : column vector of real
*        - 14 : sim.ipptr  : column vector of real
*        - 15 : sim.opar   : list of scilab object
*        - 16 : sim.opptr  : column vector of real
*        - 17 : sim.clkptr : column vector of real
*        - 18 : sim.ordptr : column vector of real
*        - 19 : sim.execlk : matrix of real
*        - 20 : sim.ordclk : matrix of real
*        - 21 : sim.cord   : matrix of real
*        - 22 : sim.oord   : matrix of real
*        - 23 : sim.zord   : column vector ? of real
*        - 24 : sim.critev : column vector of real
*        - 25 : sim.nb     : real scalar
*        - 26 : sim.ztyp   : column vector of real
*        - 27 : sim.nblk   : real scalar
*        - 28 : sim.ndcblk : real scalar
*        - 29 : sim.subscr : column vector of real
*        - 30 : sim.funtyp : column vector of real
*        - 31 : sim.iord   : column vector of real
*        - 32 : sim.labels : column vector of strings
*        - 33 : sim.modptr : column vector of real
*        - 34 : sim.uids : column vector of strings
*
* rhs 5 str   : string flag : 'start','run','finish','linear'
* rhs 6 tol   : real vector of size (7,1) minimum (4,1)
*               [atol rtol ttol [deltat realtimescale solver hmax]]'
*
* 16/03/06, A.Layec : Rewritten from original fortran
*                     source code intsscicos in intcos.f.
*
* 29/03/06, Alan    : Improvement in accordance to c_pass2
*                     (int32 parameters).
*
* 31/05/06, Alan    : Add global variable int *il_state_save
*                     and int *il_sim_save in intcscicos.h to store
*                     stack address of list %cpr.state and %cpr.sim
*                     (to use with get/setscicosvars).
*
* 14/06/06, Alan    : Save common intersci before calling scicos
*                     (to disable scilab crash with scifunc.f).
*
* 13/11/06, Alan    : Get back to double parameters for sim and state
*                     (for better compatibility with scilab-4.x families).
*                     Remove il_sim_save global variable.
*
* 15/12/06, Alan    : Warnings compilation removed.
*                     This can crash scilab/scicos.
*                     Please report.
*
* xx/02/07, Alan    : Add opar/odstate : scilab lists of arbitrary object
*                     parameters/states.
*
* 08/12/14, Paul    : Rewrite to C++.
*
*/
/*--------------------------------------------------------------------------*/

static const std::string funname = "scicosim";

types::Function::ReturnValue sci_scicosim(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    /************************************
    * Variables and constants definition
    ************************************/
    double simpar[7];

    BOOL allocatedError = FALSE;
    const int MAX_ERROR_LEN = 512;

    if (in.size() != 6)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), funname.data(), 6);
        return types::Function::Error;
    }

    if (_iRetCount > 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), funname.data(), 1, 2);
        return types::Function::Error;
    }

    /****************
    * state (rhs 1)
    ****************/
    if (in[0]->isTList() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A tlist expected.\n"), funname.data(), 1);
        return types::Function::Error;
    }
    types::TList* il_state_input = in[0]->getAs<types::TList>();
    types::TList* il_state = new types::TList();

    if (il_state_input->getSize() < 9)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d : %d elements expected.\n"), funname.data(), 1, 9);
        il_state->DecreaseRef();
        il_state->killMe();
        return types::Function::Error;
    }

    // Make a copy of 'il_state' in a global variabe
    set_il_state(il_state_input);

    types::String* header_state = il_state_input->get(0)->getAs<types::String>();
    il_state->append(header_state->clone());

    /*2 : state.x      */
    if (il_state_input->get(1)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 2, 1);
        il_state->DecreaseRef();
        il_state->killMe();
        return types::Function::Error;
    }
    types::Double* il_state_x_input = il_state_input->get(1)->getAs<types::Double>();
    il_state->append(il_state_x_input->clone());
    types::Double* il_state_x = il_state->get(1)->getAs<types::Double>();
    double* l_state_x = il_state_x->get();

    /*3 : state.z      */
    if (il_state_input->get(2)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 3, 1);
        il_state->DecreaseRef();
        il_state->killMe();
        return types::Function::Error;
    }
    types::Double* il_state_z_input = il_state_input->get(2)->getAs<types::Double>();
    il_state->append(il_state_z_input->clone());
    types::Double* il_state_z = il_state->get(2)->getAs<types::Double>();
    double* l_state_z = il_state_z->get();

    /*4 : state.oz     */
    types::List* il_state_oz_input = il_state_input->get(3)->getAs<types::List>();
    types::List* il_state_oz = new types::List();
    for (int i = 0; i < il_state_oz_input->getSize(); ++i)
    {
        types::InternalType* l_state_oz_input = il_state_oz_input->get(i);
        il_state_oz->append(l_state_oz_input->clone());
    }
    il_state->append(il_state_oz);
    int noz = il_state_oz->getSize(); // 'nlnk' is the dimension of the list 'state.oz'

    /*5 : state.iz     */
    if (il_state_input->get(4)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 5, 1);
        return types::Function::Error;
    }
    types::Double* il_state_iz_input = il_state_input->get(4)->getAs<types::Double>();
    il_state->append(il_state_iz_input->clone());
    types::Double* il_state_iz = il_state->get(4)->getAs<types::Double>();
    void** l_state_iz = (void**) il_state_iz->get();
    int m1e5 = il_state_iz->getRows();

    /*6 : state.tevts  */
    if (il_state_input->get(5)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 6, 1);
        il_state->DecreaseRef();
        il_state->killMe();
        return types::Function::Error;
    }
    types::Double* il_state_tevts_input = il_state_input->get(5)->getAs<types::Double>();
    il_state->append(il_state_tevts_input->clone());
    types::Double* il_state_tevts = il_state->get(5)->getAs<types::Double>();
    double* l_state_tevts = il_state_tevts->get();
    int m1e6 = il_state_tevts->getRows();

    /*7 : state.evtspt */
    if (il_state_input->get(6)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 7, 1);
        il_state->DecreaseRef();
        il_state->killMe();
        return types::Function::Error;
    }
    types::Double* il_state_evtspt_input = il_state_input->get(6)->getAs<types::Double>();
    il_state->append(il_state_evtspt_input->clone());
    types::Double* il_state_evtspt = il_state->get(6)->getAs<types::Double>();
    il_state_evtspt->convertToInteger();
    int* l_state_evtspt = (int*) il_state_evtspt->get();
    int m1e7 = il_state_evtspt->getRows();

    /*8 : state.pointi */
    if (il_state_input->get(7)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 8, 1);
        il_state->DecreaseRef();
        il_state->killMe();
        return types::Function::Error;
    }
    types::Double* il_pointi_input = il_state_input->get(7)->getAs<types::Double>();
    il_state->append(il_pointi_input->clone());
    types::Double* il_pointi = il_state->get(7)->getAs<types::Double>();
    il_pointi->convertToInteger();
    int* l_pointi = (int*) il_pointi->get();

    /*9 : state.outtb  */
    if (il_state_input->get(8)->isList() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A list expected.\n"), funname.data(), 9, 1);
        il_state->DecreaseRef();
        il_state->killMe();
        return types::Function::Error;
    }
    types::List* il_state_outtb_input = il_state_input->get(8)->getAs<types::List>();
    types::List* il_state_outtb = new types::List();
    for (int i = 0; i < il_state_outtb_input->getSize(); ++i)
    {
        types::InternalType* l_state_outtb_input = il_state_outtb_input->get(i);
        il_state_outtb->append(l_state_outtb_input->clone());
    }
    il_state->append(il_state_outtb);
    int nlnk = il_state_outtb->getSize(); // 'nlnk' is the dimension of the list 'state.outtb'

    /***************
    * tcur (rhs 2)
    ***************/
    if (in[1]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), funname.data(), 2);
        il_state->DecreaseRef();
        il_state->killMe();
        return types::Function::Error;
    }
    types::Double* il_tcur_input = in[1]->getAs<types::Double>();
    types::Double* il_tcur = il_tcur_input->clone()->getAs<types::Double>();
    if (il_tcur->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d : A scalar expected.\n"), funname.data(), 2);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        return types::Function::Error;
    }
    double* l_tcur = il_tcur->get();

    /*************
    * tf (rhs 3)
    *************/
    if (in[2]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), funname.data(), 3);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        return types::Function::Error;
    }
    types::Double* il_tf = in[2]->getAs<types::Double>();
    if (il_tf->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d : A scalar expected.\n"), funname.data(), 3);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        return types::Function::Error;
    }
    double* l_tf = il_tf->get();

    /*************
    * sim (rhs 4)
    *************/
    if (in[3]->isTList() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A tlist expected.\n"), funname.data(), 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        return types::Function::Error;
    }
    types::TList* il_sim_input = in[3]->getAs<types::TList>();
    types::TList* il_sim = new types::TList();

    // Make a copy of 'il_sim' in a global variabe
    set_il_sim(il_sim_input);

    if (il_sim_input->getSize() < 34)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d : %d elements expected.\n"), funname.data(), 4, 34);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }

    types::String* header_sim = il_sim_input->get(0)->getAs<types::String>();
    il_sim->append(header_sim->clone());

    /*2  : sim.funs*/
    if (il_sim_input->get(1)->isList() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A list expected.\n"), funname.data(), 2, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    types::List* il_sim_fun_input = il_sim_input->get(1)->getAs<types::List>();
    types::List* il_sim_fun = new types::List();
    for (int i = 0; i < il_sim_fun_input->getSize(); ++i)
    {
        types::InternalType* l_sim_fun_input = il_sim_fun_input->get(i);
        il_sim_fun->append(l_sim_fun_input->clone());
    }
    il_sim->append(il_sim_fun);
    int nblk = il_sim_fun->getSize(); // 'nblk' is the dimension of the list 'sim.funs'

    /*3  : sim.xptr   */
    if (il_sim_input->get(2)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 3, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    types::Double* il_sim_xptr_input = il_sim_input->get(2)->getAs<types::Double>();
    il_sim->append(il_sim_xptr_input->clone());
    types::Double* il_sim_xptr = il_sim->get(2)->getAs<types::Double>();
    il_sim_xptr->convertToInteger();
    int* l_sim_xptr = (int*) il_sim_xptr->get();
    int m_xptr = il_sim_xptr->getRows();

    /*4  : sim.zptr   */
    if (il_sim_input->get(3)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 4, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    types::Double* il_sim_zptr_input = il_sim_input->get(3)->getAs<types::Double>();
    il_sim->append(il_sim_zptr_input->clone());
    types::Double* il_sim_zptr = il_sim->get(3)->getAs<types::Double>();
    il_sim_zptr->convertToInteger();
    int* l_sim_zptr = (int*) il_sim_zptr->get();
    int m_zptr = il_sim_zptr->getRows();

    /*5  : sim.ozptr   */
    if (il_sim_input->get(4)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 5, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    types::Double* il_sim_ozptr_input = il_sim_input->get(4)->getAs<types::Double>();
    il_sim->append(il_sim_ozptr_input->clone());
    types::Double* il_sim_ozptr = il_sim->get(4)->getAs<types::Double>();
    il_sim_ozptr->convertToInteger();
    int* l_sim_ozptr = (int*) il_sim_ozptr->get();
    int m_ozptr = il_sim_ozptr->getRows();

    /*6  : sim.zcptr  */
    if (il_sim_input->get(5)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 6, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    types::Double* il_sim_zcptr_input = il_sim_input->get(5)->getAs<types::Double>();
    il_sim->append(il_sim_zcptr_input->clone());
    types::Double* il_sim_zcptr = il_sim->get(5)->getAs<types::Double>();
    il_sim_zcptr->convertToInteger();
    int* l_sim_zcptr = (int*) il_sim_zcptr->get();
    int m_zcptr = il_sim_zcptr->getRows();

    /*7  : sim.inpptr */
    if (il_sim_input->get(6)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 7, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    types::Double* il_sim_inpptr_input = il_sim_input->get(6)->getAs<types::Double>();
    il_sim->append(il_sim_inpptr_input->clone());
    types::Double* il_sim_inpptr = il_sim->get(6)->getAs<types::Double>();
    il_sim_inpptr->convertToInteger();
    int* l_sim_inpptr = (int*) il_sim_inpptr->get();
    int m_inpptr = il_sim_inpptr->getRows();

    /*8  : sim.outptr */
    if (il_sim_input->get(7)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 8, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    types::Double* il_sim_outpptr_input = il_sim_input->get(7)->getAs<types::Double>();
    il_sim->append(il_sim_outpptr_input->clone());
    types::Double* il_sim_outptr = il_sim->get(7)->getAs<types::Double>();
    il_sim_outptr->convertToInteger();
    int* l_sim_outptr = (int*) il_sim_outptr->get();
    int m_outptr = il_sim_outptr->getRows();

    /*9  : sim.inplnk */
    if (il_sim_input->get(8)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 9, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    types::Double* il_sim_inplnk_input = il_sim_input->get(8)->getAs<types::Double>();
    il_sim->append(il_sim_inplnk_input->clone());
    types::Double* il_sim_inplnk = il_sim->get(8)->getAs<types::Double>();
    il_sim_inplnk->convertToInteger();
    int* l_sim_inplnk = (int*) il_sim_inplnk->get();

    /*10  : sim.outlnk */
    if (il_sim_input->get(9)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 10, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    types::Double* il_sim_outlnk_input = il_sim_input->get(9)->getAs<types::Double>();
    il_sim->append(il_sim_outlnk_input->clone());
    types::Double* il_sim_outlnk = il_sim->get(9)->getAs<types::Double>();
    il_sim_outlnk->convertToInteger();
    int* l_sim_outlnk = (int*) il_sim_outlnk->get();

    /*11 : sim.rpar   */
    if (il_sim_input->get(10)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 11, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    types::Double* il_sim_rpar_input = il_sim_input->get(10)->getAs<types::Double>();
    il_sim->append(il_sim_rpar_input->clone());
    types::Double* il_sim_rpar = il_sim->get(10)->getAs<types::Double>();
    double* l_sim_rpar = il_sim_rpar->get();

    /*12 : sim.rpptr  */
    if (il_sim_input->get(11)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 12, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    types::Double* il_sim_rpptr_input = il_sim_input->get(11)->getAs<types::Double>();
    il_sim->append(il_sim_rpptr_input->clone());
    types::Double* il_sim_rpptr = il_sim->get(11)->getAs<types::Double>();
    il_sim_rpptr->convertToInteger();
    int* l_sim_rpptr = (int*) il_sim_rpptr->get();
    int m_rpptr = il_sim_rpptr->getRows();

    /*13 : sim.ipar   */
    if (il_sim_input->get(12)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 13, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    types::Double* il_sim_ipar_input = il_sim_input->get(12)->getAs<types::Double>();
    il_sim->append(il_sim_ipar_input->clone());
    types::Double* il_sim_ipar = il_sim->get(12)->getAs<types::Double>();
    il_sim_ipar->convertToInteger();
    int* l_sim_ipar = (int*) il_sim_ipar->get();

    /*14 : sim.ipptr  */
    if (il_sim_input->get(13)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 14, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    types::Double* il_sim_ipptr_input = il_sim_input->get(13)->getAs<types::Double>();
    il_sim->append(il_sim_ipptr_input->clone());
    types::Double* il_sim_ipptr = il_sim->get(13)->getAs<types::Double>();
    il_sim_ipptr->convertToInteger();
    int* l_sim_ipptr = (int*) il_sim_ipptr->get();
    int m_ipptr = il_sim_ipptr->getRows();

    /*15 : sim.opar   */
    if (il_sim_input->get(14)->isList() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A list expected.\n"), funname.data(), 15, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    types::List* il_sim_opar_input = il_sim_input->get(14)->getAs<types::List>();
    types::List* il_sim_opar = new types::List();
    for (int i = 0; i < il_sim_opar_input->getSize(); ++i)
    {
        types::InternalType* l_sim_opar_input = il_sim_opar_input->get(i);
        il_sim_opar->append(l_sim_opar_input->clone());
    }
    il_sim->append(il_sim_opar);
    int nopar = il_sim_opar->getSize(); // 'nopar' is the dimension of the list 'sim.opar'

    /*16 : sim.opptr  */
    if (il_sim_input->get(15)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 16, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    types::Double* il_sim_opptr_input = il_sim_input->get(15)->getAs<types::Double>();
    il_sim->append(il_sim_opptr_input->clone());
    types::Double* il_sim_opptr = il_sim->get(15)->getAs<types::Double>();
    il_sim_opptr->convertToInteger();
    int* l_sim_opptr = (int*) il_sim_opptr->get();
    int m_opptr = il_sim_opptr->getRows();

    /*17 : sim.clkptr */
    if (il_sim_input->get(16)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 17, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    types::Double* il_sim_clkptr_input = il_sim_input->get(16)->getAs<types::Double>();
    il_sim->append(il_sim_clkptr_input->clone());
    types::Double* il_sim_clkptr = il_sim->get(16)->getAs<types::Double>();
    il_sim_clkptr->convertToInteger();
    int* l_sim_clkptr = (int*) il_sim_clkptr->get();
    int m_clkptr = il_sim_clkptr->getRows();

    /*18 : sim.ordptr */
    if (il_sim_input->get(17)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 18, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    types::Double* il_sim_ordptr_input = il_sim_input->get(17)->getAs<types::Double>();
    il_sim->append(il_sim_ordptr_input->clone());
    types::Double* il_sim_ordptr = il_sim->get(17)->getAs<types::Double>();
    il_sim_ordptr->convertToInteger();
    int* l_sim_ordptr = (int*) il_sim_ordptr->get();
    int m_ordptr = il_sim_ordptr->getRows();

    /*19 : sim.execlk */
    if (il_sim_input->get(18)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 19, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    types::Double* il_sim_execlk_input = il_sim_input->get(18)->getAs<types::Double>();
    il_sim->append(il_sim_execlk_input->clone());

    /*20 : sim.ordclk */
    if (il_sim_input->get(19)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 20, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    types::Double* il_sim_ordclk_input = il_sim_input->get(19)->getAs<types::Double>();
    il_sim->append(il_sim_ordclk_input->clone());
    types::Double* il_sim_ordclk = il_sim->get(19)->getAs<types::Double>();
    il_sim_ordclk->convertToInteger();
    int* l_sim_ordclk = (int*) il_sim_ordclk->get();
    int m_ordclk = il_sim_ordclk->getRows();
    int n_ordclk = il_sim_ordclk->getCols();

    /*21 : sim.cord   */
    if (il_sim_input->get(20)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 21, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    types::Double* il_sim_cord_input = il_sim_input->get(20)->getAs<types::Double>();
    il_sim->append(il_sim_cord_input->clone());
    types::Double* il_sim_cord = il_sim->get(20)->getAs<types::Double>();
    il_sim_cord->convertToInteger();
    int* l_sim_cord = (int*) il_sim_cord->get();
    int m_cord = il_sim_cord->getRows();
    int n_cord = il_sim_cord->getCols();

    /*22 : sim.oord   */
    if (il_sim_input->get(21)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 22, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    types::Double* il_sim_oord_input = il_sim_input->get(21)->getAs<types::Double>();
    il_sim->append(il_sim_oord_input->clone());
    types::Double* il_sim_oord = il_sim->get(21)->getAs<types::Double>();
    il_sim_oord->convertToInteger();
    int* l_sim_oord = (int*) il_sim_oord->get();
    int m_oord = il_sim_oord->getRows();
    int n_oord = il_sim_oord->getCols();

    /*23 : sim.zord   */
    if (il_sim_input->get(22)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 23, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    types::Double* il_sim_zord_input = il_sim_input->get(22)->getAs<types::Double>();
    il_sim->append(il_sim_zord_input->clone());
    types::Double* il_sim_zord = il_sim->get(22)->getAs<types::Double>();
    il_sim_zord->convertToInteger();
    int* l_sim_zord = (int*) il_sim_zord->get();
    int m_zord = il_sim_zord->getRows();
    int n_zord = il_sim_zord->getCols();

    /*24 : sim.critev */
    if (il_sim_input->get(23)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 24, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    types::Double* il_sim_critev_input = il_sim_input->get(23)->getAs<types::Double>();
    il_sim->append(il_sim_critev_input->clone());
    types::Double* il_sim_critev = il_sim->get(23)->getAs<types::Double>();
    il_sim_critev->convertToInteger();
    int* l_sim_critev = (int*) il_sim_critev->get();

    /*25 : sim.nb     */
    if (il_sim_input->get(24)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 25, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    types::Double* il_sim_nb_input = il_sim_input->get(24)->getAs<types::Double>();
    il_sim->append(il_sim_nb_input->clone());
    types::Double* il_sim_nb = il_sim->get(24)->getAs<types::Double>();
    double* l_sim_nb = il_sim_nb->get();

    if (static_cast<int>(l_sim_nb[0]) != nblk) // Value of 'nb' must be equal to 'nblk'
    {
        Scierror(42, _("%s : Incompatible sim.nb RHS parameter.\n"), funname.data());
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }

    /*26 : sim.ztyp   */
    if (il_sim_input->get(25)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 26, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    types::Double* il_sim_ztyp_input = il_sim_input->get(25)->getAs<types::Double>();
    il_sim->append(il_sim_ztyp_input->clone());
    types::Double* il_sim_ztyp = il_sim->get(25)->getAs<types::Double>();
    il_sim_ztyp->convertToInteger();
    int* l_sim_ztyp = (int*) il_sim_ztyp->get();

    /*27 : sim.nblk   */
    if (il_sim_input->get(26)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 27, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    types::Double* il_sim_nblk_input = il_sim_input->get(26)->getAs<types::Double>();
    il_sim->append(il_sim_nblk_input->clone());

    /*28 : sim.ndcblk */
    if (il_sim_input->get(27)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 28, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    types::Double* il_sim_ndcblk_input = il_sim_input->get(27)->getAs<types::Double>();
    il_sim->append(il_sim_ndcblk_input->clone());

    /*29 : sim.subscr */
    if (il_sim_input->get(28)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 29, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    types::Double* il_sim_subscr_input = il_sim_input->get(28)->getAs<types::Double>();
    il_sim->append(il_sim_subscr_input->clone());
    types::Double* il_sim_subscr = il_sim->get(28)->getAs<types::Double>();
    il_sim_subscr->convertToInteger();
    int* l_sim_subscr = (int*) il_sim_subscr->get();
    int m_subscr = il_sim_subscr->getRows();

    /*30 : sim.funtyp */
    if (il_sim_input->get(29)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 30, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    types::Double* il_sim_funtyp_input = il_sim_input->get(29)->getAs<types::Double>();
    il_sim->append(il_sim_funtyp_input->clone());
    types::Double* il_sim_funtyp = il_sim->get(29)->getAs<types::Double>();
    il_sim_funtyp->convertToInteger();
    int* l_sim_funtyp = (int*) il_sim_funtyp->get();

    /*31 : sim.iord   */
    if (il_sim_input->get(30)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 31, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    types::Double* il_sim_iord_input = il_sim_input->get(30)->getAs<types::Double>();
    il_sim->append(il_sim_iord_input->clone());
    types::Double* il_sim_iord = il_sim->get(30)->getAs<types::Double>();
    il_sim_iord->convertToInteger();
    int* l_sim_iord = (int*) il_sim_iord->get();
    int m_iord = il_sim_iord->getRows();
    int n_iord = il_sim_iord->getCols();

    /*32 : sim.labels */
    if (il_sim_input->get(31)->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A string matrix expected.\n"), funname.data(), 32, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    types::String* il_sim_lab_input = il_sim_input->get(31)->getAs<types::String>();
    il_sim->append(il_sim_lab_input->clone());
    types::String* il_sim_lab = il_sim->get(31)->getAs<types::String>();
    int* il_sim_labptr;
    try
    {
        il_sim_labptr = new int[il_sim_lab->getSize()];
    }
    catch (const std::bad_alloc& /*e*/)
    {
        Scierror(999, _("%s: Memory allocation error.\n"), funname.data());
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        return types::Function::Error;
    }
    char** l_sim_lab;
    try
    {
        l_sim_lab = new char*[il_sim_lab->getSize()];
    }
    catch (const std::bad_alloc& /*e*/)
    {
        Scierror(999, _("%s: Memory allocation error.\n"), funname.data());
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        delete[] il_sim_labptr;
        return types::Function::Error;
    }
    for (int i = 0; i < il_sim_lab->getSize(); ++i)
    {
        l_sim_lab[i] = wide_string_to_UTF8(il_sim_lab->get(i));
        il_sim_labptr[i] = static_cast<int>(strlen(l_sim_lab[i]));
    }

    /*33 : sim.modptr */
    if (il_sim_input->get(32)->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A matrix expected.\n"), funname.data(), 33, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        delete[] il_sim_labptr;
        delete[] l_sim_lab;
        return types::Function::Error;
    }
    types::Double* il_sim_modptr_input = il_sim_input->get(32)->getAs<types::Double>();
    il_sim->append(il_sim_modptr_input->clone());
    types::Double* il_sim_modptr = il_sim->get(32)->getAs<types::Double>();
    il_sim_modptr->convertToInteger();
    int* l_sim_modptr = (int*) il_sim_modptr->get();

    /*34 : sim.uids */
    if (il_sim_input->get(33)->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for element #%d of argument #%d : A string matrix expected.\n"), funname.data(), 34, 4);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        delete[] il_sim_labptr;
        delete[] l_sim_lab;
        return types::Function::Error;
    }
    types::String* il_sim_uid_input = il_sim_input->get(33)->getAs<types::String>();
    il_sim->append(il_sim_uid_input->clone());
    types::String* il_sim_uid = il_sim->get(33)->getAs<types::String>();
    int* il_sim_uidptr;
    try
    {
        il_sim_uidptr = new int[il_sim_uid->getSize()];
    }
    catch (const std::bad_alloc& /*e*/)
    {
        Scierror(999, _("%s: Memory allocation error.\n"), funname.data());
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        delete[] il_sim_labptr;
        delete[] l_sim_lab;
        return types::Function::Error;
    }
    char** l_sim_uid;
    try
    {
        l_sim_uid = new char*[il_sim_uid->getSize()];
    }
    catch (const std::bad_alloc& /*e*/)
    {
        Scierror(999, _("%s: Memory allocation error.\n"), funname.data());
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        delete[] il_sim_labptr;
        delete[] l_sim_lab;
        delete[] il_sim_uidptr;
        return types::Function::Error;
    }
    for (int i = 0; i < il_sim_uid->getSize(); ++i)
    {
        l_sim_uid[i] = wide_string_to_UTF8(il_sim_uid->get(i));
        il_sim_uidptr[i] = static_cast<int>(strlen(l_sim_uid[i]));
    }

    /*************
    * str (rhs 5)
    *************/
    if (in[4]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), funname.data(), 5);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        delete[] il_sim_labptr;
        delete[] l_sim_lab;
        delete[] il_sim_uidptr;
        delete[] l_sim_uid;
        return types::Function::Error;
    }
    types::String* il_str = in[4]->getAs<types::String>();
    if (il_str->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d : A scalar expected.\n"), funname.data(), 5);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        delete[] il_sim_labptr;
        delete[] l_sim_lab;
        delete[] il_sim_uidptr;
        delete[] l_sim_uid;
        return types::Function::Error;
    }

    const std::wstring start  (L"start");
    const std::wstring run    (L"run");
    const std::wstring finish (L"finish");
    const std::wstring linear (L"linear");
    const std::wstring Kinsol (L"Kinsol");
    int flag;
    if (il_str->get(0) == start)
    {
        flag = 1;
    }
    else if (il_str->get(0) == run)
    {
        flag = 2;
    }
    else if (il_str->get(0) == finish)
    {
        flag = 3;
    }
    else if (il_str->get(0) == linear)
    {
        flag = 4;
    }
    else if (il_str->get(0) == Kinsol)
    {
        flag = 5;
    }
    else
    {
        Scierror(44, _("%s: Wrong value for input argument #%d : ""%s"", ""%s"" ""%s"" ""%s"" or ""%s"" expected.\n"), funname.data(), 5,
                 "start", "run", "finish", "linear", "Kinsol");
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        delete[] il_sim_labptr;
        delete[] l_sim_lab;
        delete[] il_sim_uidptr;
        delete[] l_sim_uid;
        return types::Function::Error;
    }

    /*************
    * tol (rhs 6)
    *************/
    if (in[5]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), funname.data(), 6);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        delete[] il_sim_labptr;
        delete[] l_sim_lab;
        delete[] il_sim_uidptr;
        delete[] l_sim_uid;
        return types::Function::Error;
    }
    types::Double* il_tol = in[5]->getAs<types::Double>();
    int m6 = il_tol->getSize();
    if (m6 < 4 || m6 > 7) // Check if 'tol' has 4 to 7 elements
    {
        Scierror(999, _("%s: Wrong size for input argument #%d : %d to %d elements expected.\n"), funname.data(), 6, 4, 7);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        delete[] il_sim_labptr;
        delete[] l_sim_lab;
        delete[] il_sim_uidptr;
        delete[] l_sim_uid;
        return types::Function::Error;
    }

    /******************
    * Set 'simpar' array
    ******************/
    for (int i = 0; i < m6; ++i)
    {
        simpar[i] = il_tol->get(i);
    }
    for (int i = m6; i < 7; ++i)
    {
        simpar[i] = 0;
    }
    int solver = static_cast<int>(simpar[5]); // Set 'solver' variable

    /******************************
    * Cross variable size checking
    ******************************/
    int err_check = 0;
    if (m1e6 != m1e7)
    {
        err_check = 1;    // tevts vs evtspt
    }
    else if (m_xptr != m_zptr)
    {
        err_check = 2;    // xptr vs zptr
    }
    else if (m_xptr != m_ozptr)
    {
        err_check = 3;    // xptr vs ozptr
    }
    else if (m_xptr != m_zcptr)
    {
        err_check = 4;    // xptr vs zcptr
    }
    else if (m_xptr != m_inpptr)
    {
        err_check = 5;    // xptr vs inpptr
    }
    else if (m_xptr != m_outptr)
    {
        err_check = 6;    // xptr vs outptr
    }
    else if (m_xptr != m_rpptr)
    {
        err_check = 7;    // xptr vs rpptr
    }
    else if (m_xptr != m_ipptr)
    {
        err_check = 8;    // xptr vs ipptr
    }
    else if (m_xptr != m_opptr)
    {
        err_check = 8;    // xptr vs opptr
    }
    else if (m_xptr != m_clkptr)
    {
        err_check = 10;    // xptr vs clkptr
    }
    else if ((n_ordclk != 2) & (m_ordclk != 0))
    {
        err_check = 11;    // sim.ordclk
    }
    else if ((n_cord != 2) & (m_cord != 0))
    {
        err_check = 12;    // sim.cord
    }
    else if ((n_oord != 2) & (m_oord != 0))
    {
        err_check = 13;    // sim.oord
    }
    else if ((n_zord != 2) & (m_zord != 0))
    {
        err_check = 14;    // sim.zord
    }
    else if ((n_iord != 2) & (m_iord != 0))
    {
        err_check = 15;    // sim.iord
    }
    if (err_check != 0)
    {
        // Please write an error table here
        Scierror(42, _("%s : error in cross variable size checking : %d\n"), funname.data(), err_check);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        delete[] il_sim_labptr;
        delete[] l_sim_lab;
        delete[] il_sim_uidptr;
        delete[] l_sim_uid;
        return types::Function::Error;
    }

    /*******************************
    * Set function table for blocks
    *******************************/
    // Define new variable 'lfunpt'
    void** lfunpt;
    try
    {
        lfunpt = new void*[nblk];
    }
    catch (const std::bad_alloc& /*e*/)
    {
        Scierror(999, _("%s: Memory allocation error.\n"), funname.data());
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        delete[] il_sim_labptr;
        delete[] l_sim_lab;
        delete[] il_sim_uidptr;
        delete[] l_sim_uid;
        return types::Function::Error;
    }

    // For each block
    for (int i = 0; i < nblk; ++i) // For each block
    {
        types::InternalType* pIT = il_sim_fun->get(i);
        // Block is defined by a Scilab function
        if (pIT->isCallable())
        {
            lfunpt[i] = (void*)pIT;
            // Keep 'l_sim_funtyp' negative for Scilab macros
            if (l_sim_funtyp[i] > 0)
            {
                l_sim_funtyp[i] *= -1;
            }
        }
        // Block is defined by a function described by a string
        else if (pIT->isString())
        {
            types::String* funStr = pIT->getAs<types::String>();
            if (funStr->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong size for element #%d of input argument #%d : A scalar expected.\n"), funname.data(), i + 1, 4);
                il_state->DecreaseRef();
                il_state->killMe();
                il_tcur->DecreaseRef();
                il_tcur->killMe();
                il_sim->DecreaseRef();
                il_sim->killMe();
                delete[] il_sim_labptr;
                delete[] l_sim_lab;
                delete[] il_sim_uidptr;
                delete[] l_sim_uid;
                delete[] lfunpt;
                return types::Function::Error;
            }

            wchar_t* w_str = funStr->get(0);
            char* c_str = wide_string_to_UTF8(w_str);
            if (strcmp(c_str, "ifthel") == 0)
            {
                l_sim_funtyp[i] = 11; // Magic value for "if-then-else" block
            }
            else if (strcmp(c_str, "eselect") == 0)
            {
                l_sim_funtyp[i] = 12; // Magic value for "eselect" block
            }
            else
            {
                void* f = funnum2(c_str); // Search associated function number of function name
                // Block is defined by a C or Fortran function
                if (f != nullptr)
                {
                    // C interface from "tabsim" defined in blocks.h
                    lfunpt[i] = f;
                    if (l_sim_funtyp[i] < 0)
                    {
                        // Keep 'l_sim_funtyp' positive for Fortran functions
                        l_sim_funtyp[i] *= -1;
                    }
                }
                // Block is defined by a predefined scilab function
                else
                {
                    ConfigVariable::EntryPointStr* pEP = ConfigVariable::getEntryPoint(w_str);
                    if (pEP)
                    {
                        //linked functions
                        lfunpt[i] = (void*)pEP->functionPtr;
                    }
                    else
                    {
                        types::InternalType* pMacro = symbol::Context::getInstance()->get(symbol::Symbol(w_str));
                        if (pMacro && pMacro->isCallable())
                        {
                            //macros
                            lfunpt[i] = (void*)pMacro;
                            l_sim_funtyp[i] *= -1;
                        }
                        else
                        {
                            Scierror(888, _("%s : unknown block : %s\n"), funname.data(), c_str);
                            il_state->DecreaseRef();
                            il_state->killMe();
                            il_tcur->DecreaseRef();
                            il_tcur->killMe();
                            il_sim->DecreaseRef();
                            il_sim->killMe();
                            delete[] il_sim_labptr;
                            delete[] l_sim_lab;
                            delete[] il_sim_uidptr;
                            delete[] l_sim_uid;
                            delete[] lfunpt;
                            FREE(c_str);
                            return types::Function::Error;
                        }
                    }
                }
            }
            FREE(c_str);
        }
        else
        {
            Scierror(999, _("%s: Wrong type for element #%d of input argument #%d : string or macro expected.\n"), funname.data(), i + 1, 4);
            il_state->DecreaseRef();
            il_state->killMe();
            il_tcur->DecreaseRef();
            il_tcur->killMe();
            il_sim->DecreaseRef();
            il_sim->killMe();
            delete[] il_sim_labptr;
            delete[] l_sim_lab;
            delete[] il_sim_uidptr;
            delete[] l_sim_uid;
            delete[] lfunpt;
            return types::Function::Error;
        }
    }

    /**********************
    * Set oz, ozsz, oztyp
    **********************/
    void** oz = nullptr;
    int* ozsz = nullptr;
    int* oztyp = nullptr;
    if (noz > 0)
    {
        // Allocation of 'oz'
        try
        {
            oz = new void*[noz];
        }
        catch (const std::bad_alloc& /*e*/)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), funname.data());
            il_state->DecreaseRef();
            il_state->killMe();
            il_tcur->DecreaseRef();
            il_tcur->killMe();
            il_sim->DecreaseRef();
            il_sim->killMe();
            delete[] il_sim_labptr;
            delete[] l_sim_lab;
            delete[] il_sim_uidptr;
            delete[] l_sim_uid;
            delete[] lfunpt;
            return types::Function::Error;
        }
        // Allocation of 'ozsz'
        try
        {
            ozsz = new int[2 * noz];
        }
        catch (const std::bad_alloc& /*e*/)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), funname.data());
            il_state->DecreaseRef();
            il_state->killMe();
            il_tcur->DecreaseRef();
            il_tcur->killMe();
            il_sim->DecreaseRef();
            il_sim->killMe();
            delete[] il_sim_labptr;
            delete[] l_sim_lab;
            delete[] il_sim_uidptr;
            delete[] l_sim_uid;
            delete[] lfunpt;
            delete[] oz;
            return types::Function::Error;
        }
        // Allocation of 'oztyp'
        try
        {
            oztyp = new int[noz];
        }
        catch (const std::bad_alloc& /*e*/)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), funname.data());
            il_state->DecreaseRef();
            il_state->killMe();
            il_tcur->DecreaseRef();
            il_tcur->killMe();
            il_sim->DecreaseRef();
            il_sim->killMe();
            delete[] il_sim_labptr;
            delete[] l_sim_lab;
            delete[] il_sim_uidptr;
            delete[] l_sim_uid;
            delete[] lfunpt;
            delete[] oz;
            delete[] ozsz;
            return types::Function::Error;
        }

        // Set vectors of 'oz'
        for (int j = 0; j < noz; ++j)
        {
            int subtype = il_state_oz->get(j)->getType();

            switch (subtype) // Store type and address
            {
                case types::InternalType::ScilabDouble :
                {
                    types::Double* ozDouble = il_state_oz->get(j)->getAs<types::Double>();
                    if (ozDouble->isComplex() == false)
                    {
                        oztyp[j] = SCSREAL_N; // Double real matrix
                        oz[j] = (SCSREAL_COP *) ozDouble->get();
                    }
                    else
                    {
                        oztyp[j] = SCSCOMPLEX_N; // Double complex matrix
                        oz[j] = (SCSCOMPLEX_COP *) ozDouble->get();
                    }
                    ozsz[j] = ozDouble->getRows();
                    ozsz[j + noz] = ozDouble->getCols();
                    break;
                }
                case types::InternalType::ScilabInt8 :
                {
                    types::Int8* ozInt8 = il_state_oz->get(j)->getAs<types::Int8>();
                    oztyp[j] = SCSINT8_N; // int8
                    ozsz[j] = ozInt8->getRows();
                    ozsz[j + noz] = ozInt8->getCols();
                    oz[j] = (SCSINT8_COP *) ozInt8->get();
                    break;
                }
                case types::InternalType::ScilabInt16 :
                {
                    types::Int16* ozInt16 = il_state_oz->get(j)->getAs<types::Int16>();
                    oztyp[j] = SCSINT16_N; // int16
                    ozsz[j] = ozInt16->getRows();
                    ozsz[j + noz] = ozInt16->getCols();
                    oz[j] = (SCSINT16_COP *) ozInt16->get();
                    break;
                }
                case types::InternalType::ScilabInt32 :
                {
                    types::Int32* ozInt32 = il_state_oz->get(j)->getAs<types::Int32>();
                    oztyp[j] = SCSINT32_N; // int32
                    ozsz[j] = ozInt32->getRows();
                    ozsz[j + noz] = ozInt32->getCols();
                    oz[j] = (SCSINT32_COP *) ozInt32->get();
                    break;
                }
                case types::InternalType::ScilabUInt8 :
                {
                    types::UInt8* ozUInt8 = il_state_oz->get(j)->getAs<types::UInt8>();
                    oztyp[j] = SCSUINT8_N; // uint8
                    ozsz[j] = ozUInt8->getRows();
                    ozsz[j + noz] = ozUInt8->getCols();
                    oz[j] = (SCSUINT8_COP *) ozUInt8->get();
                    break;
                }
                case types::InternalType::ScilabUInt16 :
                {
                    types::UInt16* ozUInt16 = il_state_oz->get(j)->getAs<types::UInt16>();
                    oztyp[j] = SCSUINT16_N; // uint16
                    ozsz[j] = ozUInt16->getRows();
                    ozsz[j + noz] = ozUInt16->getCols();
                    oz[j] = (SCSUINT16_COP *) ozUInt16->get();
                    break;
                }
                case types::InternalType::ScilabUInt32 :
                {
                    types::UInt32* ozUInt32 = il_state_oz->get(j)->getAs<types::UInt32>();
                    oztyp[j] = SCSUINT32_N; // uint32
                    ozsz[j] = ozUInt32->getRows();
                    ozsz[j + noz] = ozUInt32->getCols();
                    oz[j] = (SCSUINT32_COP *) ozUInt32->get();
                    break;
                }
                default :
                {
                    Scierror(999, _("%s: Wrong type for element #%d of element #%d of argument #%d : A matrix expected.\n"), funname.data(), j + 1, 4, 1);
                    il_state->DecreaseRef();
                    il_state->killMe();
                    il_tcur->DecreaseRef();
                    il_tcur->killMe();
                    il_sim->DecreaseRef();
                    il_sim->killMe();
                    delete[] il_sim_labptr;
                    delete[] l_sim_lab;
                    delete[] il_sim_uidptr;
                    delete[] l_sim_uid;
                    delete[] lfunpt;
                    delete[] oz;
                    delete[] ozsz;
                    delete[] oztyp;
                    return types::Function::Error;
                }
            }
        }
    }

    /****************************
    * Set opar, oparsz, opartyp
    ****************************/
    void** opar = nullptr;
    int* oparsz = nullptr;
    int* opartyp = nullptr;
    if (nopar > 0)
    {
        // Allocation of 'opar'
        try
        {
            opar = new void*[nopar];
        }
        catch (const std::bad_alloc& /*e*/)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), funname.data());
            il_state->DecreaseRef();
            il_state->killMe();
            il_tcur->DecreaseRef();
            il_tcur->killMe();
            il_sim->DecreaseRef();
            il_sim->killMe();
            delete[] il_sim_labptr;
            delete[] l_sim_lab;
            delete[] il_sim_uidptr;
            delete[] l_sim_uid;
            delete[] lfunpt;
            delete[] oz;
            delete[] ozsz;
            delete[] oztyp;
            return types::Function::Error;
        }
        // Allocation of 'oparsz'
        try
        {
            oparsz = new int[2 * nopar];
        }
        catch (const std::bad_alloc& /*e*/)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), funname.data());
            il_state->DecreaseRef();
            il_state->killMe();
            il_tcur->DecreaseRef();
            il_tcur->killMe();
            il_sim->DecreaseRef();
            il_sim->killMe();
            delete[] il_sim_labptr;
            delete[] l_sim_lab;
            delete[] il_sim_uidptr;
            delete[] l_sim_uid;
            delete[] lfunpt;
            delete[] oz;
            delete[] ozsz;
            delete[] oztyp;
            delete[] opar;
            return types::Function::Error;
        }
        // Allocation of 'opartyp'
        try
        {
            opartyp = new int[nopar];
        }
        catch (const std::bad_alloc& /*e*/)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), funname.data());
            il_state->DecreaseRef();
            il_state->killMe();
            il_tcur->DecreaseRef();
            il_tcur->killMe();
            il_sim->DecreaseRef();
            il_sim->killMe();
            delete[] il_sim_labptr;
            delete[] l_sim_lab;
            delete[] il_sim_uidptr;
            delete[] l_sim_uid;
            delete[] lfunpt;
            delete[] oz;
            delete[] ozsz;
            delete[] oztyp;
            delete[] opar;
            delete[] oparsz;
            return types::Function::Error;
        }

        // Set vectors of 'opar'
        for (int j = 0; j < nopar; ++j)
        {
            int subtype = il_sim_opar->get(j)->getType();

            switch (subtype) // Store type and address
            {
                case types::InternalType::ScilabDouble :
                {
                    types::Double* oparDouble = il_sim_opar->get(j)->getAs<types::Double>();
                    if (oparDouble->isComplex() == false)
                    {
                        opartyp[j] = SCSREAL_N; // Double real matrix
                        opar[j] = (SCSREAL_COP *) oparDouble->get();
                    }
                    else
                    {
                        opartyp[j] = SCSCOMPLEX_N; // Double complex matrix
                        // Allocate a long array in order to make the real and complex parts contiguous (oparDouble->get() and oparDouble->getImg())
                        try
                        {
                            opar[j] = new double[2 * oparDouble->getSize()];
                        }
                        catch (const std::bad_alloc& /*e*/)
                        {
                            Scierror(999, _("%s: Memory allocation error.\n"), funname.data());
                            il_state->DecreaseRef();
                            il_state->killMe();
                            il_tcur->DecreaseRef();
                            il_tcur->killMe();
                            il_sim->DecreaseRef();
                            il_sim->killMe();
                            delete[] il_sim_labptr;
                            delete[] l_sim_lab;
                            delete[] il_sim_uidptr;
                            delete[] l_sim_uid;
                            delete[] lfunpt;
                            delete[] oz;
                            delete[] ozsz;
                            delete[] oztyp;
                            delete[] opar;
                            delete[] oparsz;
                            delete[] opartyp;
                            return types::Function::Error;
                        }
                        memcpy(opar[j], oparDouble->get(), oparDouble->getSize() * sizeof(double)); // Real part
                        memcpy((double*)(opar[j]) + oparDouble->getSize(), oparDouble->getImg(), oparDouble->getSize() * sizeof(double)); // Complex part
                        // FIXME: delete oparDouble because we copied it instead of using it?
                    }
                    oparsz[j] = oparDouble->getRows();
                    oparsz[j + nopar] = oparDouble->getCols();
                    break;
                }
                case types::InternalType::ScilabInt8 :
                {
                    types::Int8* oparInt8 = il_sim_opar->get(j)->getAs<types::Int8>();
                    opartyp[j] = SCSINT8_N; // int8
                    oparsz[j] = oparInt8->getRows();
                    oparsz[j + nopar] = oparInt8->getCols();
                    opar[j] = (SCSINT8_COP *) oparInt8->get();
                    break;
                }
                case types::InternalType::ScilabInt16 :
                {
                    types::Int16* oparInt16 = il_sim_opar->get(j)->getAs<types::Int16>();
                    opartyp[j] = SCSINT16_N; // int16
                    oparsz[j] = oparInt16->getRows();
                    oparsz[j + nopar] = oparInt16->getCols();
                    opar[j] = (SCSINT16_COP *) oparInt16->get();
                    break;
                }
                case types::InternalType::ScilabInt32 :
                {
                    types::Int32* oparInt32 = il_sim_opar->get(j)->getAs<types::Int32>();
                    opartyp[j] = SCSINT32_N; // int32
                    oparsz[j] = oparInt32->getRows();
                    oparsz[j + nopar] = oparInt32->getCols();
                    opar[j] = (SCSINT32_COP *) oparInt32->get();
                    break;
                }
                case types::InternalType::ScilabUInt8 :
                {
                    types::UInt8* oparUInt8 = il_sim_opar->get(j)->getAs<types::UInt8>();
                    opartyp[j] = SCSUINT8_N; // uint8
                    oparsz[j] = oparUInt8->getRows();
                    oparsz[j + nopar] = oparUInt8->getCols();
                    opar[j] = (SCSUINT8_COP *) oparUInt8->get();
                    break;
                }
                case types::InternalType::ScilabUInt16 :
                {
                    types::UInt16* oparUInt16 = il_sim_opar->get(j)->getAs<types::UInt16>();
                    opartyp[j] = SCSUINT16_N; // uint16
                    oparsz[j] = oparUInt16->getRows();
                    oparsz[j + nopar] = oparUInt16->getCols();
                    opar[j] = (SCSUINT16_COP *) oparUInt16->get();
                    break;
                }
                case types::InternalType::ScilabUInt32 :
                {
                    types::UInt32* oparUInt32 = il_sim_opar->get(j)->getAs<types::UInt32>();
                    opartyp[j] = SCSUINT32_N; // uint32
                    oparsz[j] = oparUInt32->getRows();
                    oparsz[j + nopar] = oparUInt32->getCols();
                    opar[j] = (SCSUINT32_COP *) oparUInt32->get();
                    break;
                }
                default :
                {
                    Scierror(999, _("%s: Wrong type for element #%d of element #%d of argument #%d : A matrix expected.\n"), funname.data(), j + 1, 15, 4);
                    il_state->DecreaseRef();
                    il_state->killMe();
                    il_tcur->DecreaseRef();
                    il_tcur->killMe();
                    il_sim->DecreaseRef();
                    il_sim->killMe();
                    delete[] il_sim_labptr;
                    delete[] l_sim_lab;
                    delete[] il_sim_uidptr;
                    delete[] l_sim_uid;
                    delete[] lfunpt;
                    delete[] oz;
                    delete[] ozsz;
                    delete[] oztyp;
                    delete[] opar;
                    delete[] oparsz;
                    delete[] opartyp;
                    return types::Function::Error;
                }
            }
        }
    }

    // Declaration of 'outtb_elem'
    outtb_el* outtb_elem = nullptr;
    int nelem = 0;

    /*******************************
    * Set outtbptr, outtbsz, outtbtyp
    *******************************/
    void** outtbptr = nullptr;
    int* outtbsz = nullptr;
    int* outtbtyp = nullptr;
    if (nlnk > 0)
    {
        // Allocation of 'outtbptr'
        try
        {
            outtbptr = new void*[nlnk];
        }
        catch (const std::bad_alloc& /*e*/)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), funname.data());
            il_state->DecreaseRef();
            il_state->killMe();
            il_tcur->DecreaseRef();
            il_tcur->killMe();
            il_sim->DecreaseRef();
            il_sim->killMe();
            delete[] il_sim_labptr;
            delete[] l_sim_lab;
            delete[] il_sim_uidptr;
            delete[] l_sim_uid;
            delete[] lfunpt;
            delete[] oz;
            delete[] ozsz;
            delete[] oztyp;
            delete[] opar;
            delete[] oparsz;
            delete[] opartyp;
            return types::Function::Error;
        }
        // Allocation of 'outtbptrsz'
        try
        {
            outtbsz = new int[2 * nlnk];
        }
        catch (const std::bad_alloc& /*e*/)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), funname.data());
            il_state->DecreaseRef();
            il_state->killMe();
            il_tcur->DecreaseRef();
            il_tcur->killMe();
            il_sim->DecreaseRef();
            il_sim->killMe();
            delete[] il_sim_labptr;
            delete[] l_sim_lab;
            delete[] il_sim_uidptr;
            delete[] l_sim_uid;
            delete[] lfunpt;
            delete[] oz;
            delete[] ozsz;
            delete[] oztyp;
            delete[] opar;
            delete[] oparsz;
            delete[] opartyp;
            delete[] outtbptr;
            return types::Function::Error;
        }
        // Allocation of 'outtbtyp'
        try
        {
            outtbtyp = new int[nlnk];
        }
        catch (const std::bad_alloc& /*e*/)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), funname.data());
            il_state->DecreaseRef();
            il_state->killMe();
            il_tcur->DecreaseRef();
            il_tcur->killMe();
            il_sim->DecreaseRef();
            il_sim->killMe();
            delete[] il_sim_labptr;
            delete[] l_sim_lab;
            delete[] il_sim_uidptr;
            delete[] l_sim_uid;
            delete[] lfunpt;
            delete[] oz;
            delete[] ozsz;
            delete[] oztyp;
            delete[] opar;
            delete[] oparsz;
            delete[] opartyp;
            delete[] outtbptr;
            delete[] outtbsz;
            return types::Function::Error;
        }

        // Set vectors of 'outtbptr'
        for (int j = 0; j < nlnk; ++j)
        {
            int subtype = il_state_outtb->get(j)->getType();

            switch (subtype) // Store type and address
            {
                case types::InternalType::ScilabDouble :
                {
                    types::Double* outtbDouble = il_state_outtb->get(j)->getAs<types::Double>();
                    if (outtbDouble->isComplex() == false)
                    {
                        outtbtyp[j] = SCSREAL_N; // Double real matrix
                        outtbptr[j] = (SCSREAL_COP *) outtbDouble->get();
                    }
                    else
                    {
                        outtbtyp[j] = SCSCOMPLEX_N; // Double complex matrix
                        // Allocate a long array in order to make the real and complex parts contiguous (outtbDouble->get() and outtbDouble->getImg())
                        try
                        {
                            outtbptr[j] = new double[2 * outtbDouble->getSize()];
                        }
                        catch (const std::bad_alloc& /*e*/)
                        {
                            Scierror(999, _("%s: Memory allocation error.\n"), funname.data());
                            il_state->DecreaseRef();
                            il_state->killMe();
                            il_tcur->DecreaseRef();
                            il_tcur->killMe();
                            il_sim->DecreaseRef();
                            il_sim->killMe();
                            delete[] il_sim_labptr;
                            delete[] l_sim_lab;
                            delete[] il_sim_uidptr;
                            delete[] l_sim_uid;
                            delete[] lfunpt;
                            delete[] oz;
                            delete[] ozsz;
                            delete[] oztyp;
                            delete[] opar;
                            delete[] oparsz;
                            delete[] opartyp;
                            delete[] outtbptr;
                            delete[] outtbsz;
                            return types::Function::Error;
                        }
                        memcpy(outtbptr[j], outtbDouble->get(), outtbDouble->getSize() * sizeof(double)); // Real part
                        memcpy((double*)(outtbptr[j]) + outtbDouble->getSize(), outtbDouble->getImg(), outtbDouble->getSize() * sizeof(double)); // Complex part
                        // FIXME: delete outtbDouble because we copied it instead of using it?
                    }
                    outtbsz[j] = outtbDouble->getRows();
                    outtbsz[j + nlnk] = outtbDouble->getCols();
                    break;
                }
                case types::InternalType::ScilabInt8 :
                {
                    types::Int8* outtbInt8 = il_state_outtb->get(j)->getAs<types::Int8>();
                    outtbtyp[j] = SCSINT8_N; // int8
                    outtbsz[j] = outtbInt8->getRows();
                    outtbsz[j + nlnk] = outtbInt8->getCols();
                    outtbptr[j] = (SCSINT8_COP *) outtbInt8->get();
                    break;
                }
                case types::InternalType::ScilabInt16 :
                {
                    types::Int16* outtbInt16 = il_state_outtb->get(j)->getAs<types::Int16>();
                    outtbtyp[j] = SCSINT16_N; // int16
                    outtbsz[j] = outtbInt16->getRows();
                    outtbsz[j + nlnk] = outtbInt16->getCols();
                    outtbptr[j] = (SCSINT16_COP *) outtbInt16->get();
                    break;
                }
                case types::InternalType::ScilabInt32 :
                {
                    types::Int32* outtbInt32 = il_state_outtb->get(j)->getAs<types::Int32>();
                    outtbtyp[j] = SCSINT32_N; // int32
                    outtbsz[j] = outtbInt32->getRows();
                    outtbsz[j + nlnk] = outtbInt32->getCols();
                    outtbptr[j] = (SCSINT32_COP *) outtbInt32->get();
                    break;
                }
                case types::InternalType::ScilabUInt8 :
                {
                    types::UInt8* outtbUInt8 = il_state_outtb->get(j)->getAs<types::UInt8>();
                    outtbtyp[j] = SCSUINT8_N; // uint8
                    outtbsz[j] = outtbUInt8->getRows();
                    outtbsz[j + nlnk] = outtbUInt8->getCols();
                    outtbptr[j] = (SCSUINT8_COP *) outtbUInt8->get();
                    break;
                }
                case types::InternalType::ScilabUInt16 :
                {
                    types::UInt16* outtbUInt16 = il_state_outtb->get(j)->getAs<types::UInt16>();
                    outtbtyp[j] = SCSUINT16_N; // uint16
                    outtbsz[j] = outtbUInt16->getRows();
                    outtbsz[j + nlnk] = outtbUInt16->getCols();
                    outtbptr[j] = (SCSUINT16_COP *) outtbUInt16->get();
                    break;
                }
                case types::InternalType::ScilabUInt32 :
                {
                    types::UInt32* outtbUInt32 = il_state_outtb->get(j)->getAs<types::UInt32>();
                    outtbtyp[j] = SCSUINT32_N; // uint32
                    outtbsz[j] = outtbUInt32->getRows();
                    outtbsz[j + nlnk] = outtbUInt32->getCols();
                    outtbptr[j] = (SCSUINT32_COP *) outtbUInt32->get();
                    break;
                }
                default :
                {
                    Scierror(999, _("%s: Wrong type for element #%d of element #%d of argument #%d : A matrix expected.\n"), funname.data(), j + 1, 9, 1);
                    il_state->DecreaseRef();
                    il_state->killMe();
                    il_tcur->DecreaseRef();
                    il_tcur->killMe();
                    il_sim->DecreaseRef();
                    il_sim->killMe();
                    delete[] il_sim_labptr;
                    delete[] l_sim_lab;
                    delete[] il_sim_uidptr;
                    delete[] l_sim_uid;
                    delete[] lfunpt;
                    delete[] outtbptr;
                    delete[] outtbtyp;
                    delete[] outtbsz;
                    delete[] opar;
                    delete[] oparsz;
                    delete[] opartyp;
                    delete[] oz;
                    delete[] ozsz;
                    delete[] oztyp;
                    FREE(outtb_elem);
                    return types::Function::Error;
                }
            }

            // Store 'lnk' and 'pos' in 'outtb_elem'
            int k = nelem;
            nelem += outtbsz[j] * outtbsz[j + nlnk];
            if ((outtb_elem = (outtb_el *) REALLOC(outtb_elem, nelem * sizeof(outtb_el))) == nullptr)
            {
                Scierror(999, _("%s : No more free memory.\n"), funname.data());
                il_state->DecreaseRef();
                il_state->killMe();
                il_tcur->DecreaseRef();
                il_tcur->killMe();
                il_sim->DecreaseRef();
                il_sim->killMe();
                delete[] il_sim_labptr;
                delete[] l_sim_lab;
                delete[] il_sim_uidptr;
                delete[] l_sim_uid;
                delete[] lfunpt;
                delete[] outtbptr;
                delete[] outtbtyp;
                delete[] outtbsz;
                delete[] opar;
                delete[] oparsz;
                delete[] opartyp;
                delete[] oz;
                delete[] ozsz;
                delete[] oztyp;
                FREE(outtb_elem);
                return types::Function::Error;
            }
            for (int i = 0; i < outtbsz[j]*outtbsz[j + nlnk]; ++i)
            {
                outtb_elem[k + i].lnk = j;
                outtb_elem[k + i].pos = i;
            }
        }
    }

    /************************
    * Call scicos simulator
    ************************/
    /* Syntax :
    * int C2F(scicos)(double *x_in, int *xptr_in, double *z__,
    *                 void **work,int *zptr,int *modptr_in,
    *                 void **oz,int *ozsz,int *oztyp,int *ozptr,
    *                 char **iz,int *izptr,double *t0_in,
    *                 double *tf_in,double *tevts_in,int *evtspt_in,
    *                 int *nevts,int *pointi_in,void **outtbptr_in,
    *                 int *outtbsz_in,int *outtbtyp_in,
    *                 outtb_el *outtb_elem_in,int *nelem1,int *nlnk1,
    *                 int *funptr,int *funtyp_in,int *inpptr_in,
    *                 int *outptr_in, int *inplnk_in,int *outlnk_in,
    *                 double *rpar,int *rpptr,int *ipar,int *ipptr,
    *                 void **opar,int *oparsz,int *opartyp,int *opptr,
    *                 int *clkptr_in,int *ordptr_in,int *nordptr1,
    *                 int *ordclk_in,int *cord_in,int *ncord1,
    *                 int *iord_in,int *niord1,int *oord_in,
    *                 int *noord1,int *zord_in,int *nzord1,
    *                 int *critev_in,int *nblk1,int *ztyp,
    *                 int *zcptr_in,int *subscr,int *nsubs,
    *                 double *simpar,int *flag__,int *ierr_out)
    */

    int ierr = 0;
    C2F(scicos)(l_state_x, l_sim_xptr, l_state_z,
                l_state_iz, l_sim_zptr, l_sim_modptr,
                oz, ozsz, oztyp, l_sim_ozptr,
                l_sim_lab, il_sim_labptr, l_sim_uid, il_sim_uidptr, l_tcur,
                l_tf, l_state_tevts, l_state_evtspt,
                &m1e5, l_pointi, outtbptr,
                outtbsz, outtbtyp,
                outtb_elem, &nelem, &nlnk,
                lfunpt, l_sim_funtyp, l_sim_inpptr,
                l_sim_outptr, l_sim_inplnk, l_sim_outlnk,
                l_sim_rpar, l_sim_rpptr, l_sim_ipar, l_sim_ipptr,
                opar, oparsz, opartyp, l_sim_opptr,
                l_sim_clkptr, l_sim_ordptr, &m_ordptr,
                l_sim_ordclk, l_sim_cord, &m_cord,
                l_sim_iord, &m_iord, l_sim_oord,
                &m_oord, l_sim_zord, &m_zord,
                l_sim_critev, &nblk, l_sim_ztyp,
                l_sim_zcptr, l_sim_subscr, &m_subscr,
                simpar, &flag, &ierr);

    /**********************
    * Free allocated arrays
    **********************/
    delete[] outtbptr;
    delete[] outtbtyp;
    delete[] outtbsz;
    delete[] opar;
    delete[] oparsz;
    delete[] opartyp;
    delete[] oz;
    delete[] ozsz;
    delete[] oztyp;
    delete[] lfunpt;
    FREE(outtb_elem);

    /*************************************
    * Switch to appropriate message error
    *************************************/
    if (ierr > 0)
    {
        char* error;
        switch (ierr)
        {
            case 1  :
                error = _("scheduling problem");
                C2F(curblk).kfun = 0;
                break;

            case 2  :
                error = _("input to zero-crossing stuck on zero");
                C2F(curblk).kfun = 0;
                break;

            case 3  :
                error = _("event conflict");
                C2F(curblk).kfun = 0;
                break;

            case 4  :
                error = _("algebraic loop detected");
                C2F(curblk).kfun = 0;
                break;

            case 5  :
                error = _("cannot allocate memory");
                C2F(curblk).kfun = 0;
                break;

            case 6  :
                try
                {
                    error = new char[MAX_ERROR_LEN];
                    allocatedError = TRUE;
                    snprintf(error, MAX_ERROR_LEN, _("the block %d has been called with input out of its domain"), C2F(curblk).kfun);
                }
                catch (const std::bad_alloc& /*e*/)
                {
                    error = _("cannot allocate memory");
                }
                break;

            case 7  :
                error = _("singularity in a block");
                break;

            case 8  :
                error = _("block produces an internal error");
                break;

            case 10  :
                error = coserr.buf;
                break;

            case 20  :
                error = _("initial conditions not converging");
                break;

            case 21  :
                try
                {
                    error = new char[MAX_ERROR_LEN];
                    allocatedError = TRUE;
                    snprintf(error, MAX_ERROR_LEN, _("cannot allocate memory in block=%d"), C2F(curblk).kfun);
                }
                catch (const std::bad_alloc& /*e*/)
                {
                    error = _("cannot allocate memory");
                }
                break;

            case 22  :
                error = _("sliding mode condition, cannot integrate");
                break;

            case 23  :
                error = _("Cannot find the initial mode, maybe there is a sliding mode condition");
                break;
            case 24  :
                error = _("You have changed a parameter in your model, but the model has been compiled to use an XML file containing initial values and parameters. So you should either recompile your Scicos diagram or [re]launch the initialization interface to regenerate the XML file  with new parameters.");
                break;

            case 25  :
                error = _("Undefined data type.");
                break;
            case 26  :
                error = _("The number of parameter provided by Scicos blocks is different from what expected by the code generated by the Modelica compiler. You might have relaxed a parameter using FIXED property (i.e., fixed=false) in a Modelica model. This will be corrected in the next version");
                break;
                // In this case, you need to turn off the parameter embedded code generation mode by setting 'Modelica_ParEmb=%f' in the Scilab command window, and recompile the Scicos diagram

            default  :
                if (ierr >= 1000)
                {
                    error = _("unknown or erroneous block");
                }
                else if (ierr >= 201 && ierr <= 416) // Sundials error messages, stored in coserr.buf
                {
                    error = coserr.buf;
                }
                else if (ierr >= 100)
                {
                    int istate = -(ierr - 100);
                    try
                    {
                        error = new char[MAX_ERROR_LEN];
                        allocatedError = TRUE;
                        snprintf(error, MAX_ERROR_LEN, _("integration problem istate=%d"), istate);
                    }
                    catch (const std::bad_alloc& /*e*/)
                    {
                        error = _("cannot allocate memory");
                    }
                    C2F(curblk).kfun = 0;
                }
                else
                {
                    error = _("scicos unexpected error, please report...");
                    C2F(curblk).kfun = 0;
                }
                break;
        }

        Scierror(888, "%s\n", error);
        il_state->DecreaseRef();
        il_state->killMe();
        il_tcur->DecreaseRef();
        il_tcur->killMe();
        il_sim->DecreaseRef();
        il_sim->killMe();
        delete[] il_sim_labptr;
        delete[] l_sim_lab;
        delete[] il_sim_uidptr;
        delete[] l_sim_uid;
        if (allocatedError)
        {
            delete[] error;
        }
        return types::Function::Error;
    }

    /*********************
    * Return Lhs variables
    *********************/
    // Convert integer parameters of 'il_state' into double parameters
    il_state_evtspt->convertFromInteger();
    il_pointi->convertFromInteger();

    out.push_back(il_state);
    out.push_back(il_tcur);

    // End
    il_sim->DecreaseRef();
    il_sim->killMe();
    delete[] il_sim_labptr;
    delete[] l_sim_lab;
    delete[] il_sim_uidptr;
    delete[] l_sim_uid;
    return types::Function::OK;
}
