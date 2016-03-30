/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
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
#include <vector>

#include "gw_scicos.hxx"
#include "internal.hxx"
#include "types.hxx"
#include "double.hxx"
#include "int.hxx"
#include "string.hxx"
#include "tlist.hxx"
#include "function.hxx"

extern "C"
{
#include "sciprint.h"
#include "scicos-def.h"
#include "charEncoding.h"
#include "Scierror.h"
#include "localization.h"
#include "import.h"

    /*--------------------------------------------------------------------------*/
    // Variable defined in sci_scicosim.cpp
    extern COSIM_struct C2F(cosim);
    /*--------------------------------------------------------------------------*/
}

#include "il_state.hxx"
#include "il_sim.hxx"
#include "createblklist.hxx"

/*--------------------------------------------------------------------------*/
/* getscicosvars interface routine retrieves some information during simulation.
*
* [myvar]=getscicosvars(str)
*
* rhs 1  : str : a character string matrix with choice,
*               - 'x' to retrieve continuous state
*               - 'xptr' to retrieve ptr of continuous state
*               - 'z' to retrieve discrete state
*               - 'zptr' to retrieve ptr of discrete state
*               - 'rpar' to retrieve real parameters
*               - 'rpptr' to retrieve ptr of real parameters
*               - 'ipar' to retrieve int parameters
*               - 'ipptr' to retrieve  ptr of int parameters
*               - 'outtb' to retrieve output register (list of scilb object)
*               - 'inpptr' to retrieve number of input ports
*               - 'outptr' to retrieve number of output ports
*               - 'inplnk' to retrieve link number of input ports
*               - 'outlnk' to retrieve link number of output ports
*               ...... -see below-
*
* lhs 1  : myvar : matrix of int32 or double, or list or a Tlist
*
* 31/05/06, Alan : Rewritten from original fortran
* source code intgetscicosvars in coselm.f.
*
* 22/06/06, Alan : Allow multiple string in rhs(1).
*                  Create Tlist for Lhs(1).
*
* 23/06/06, Alan : Create blocks list for Lhs(1).
*
* 13/11/06, Alan : Remove il_sim_save global variable (all in sim
*                  come from import struct now)
*                  evtspt & pointi of state come from import struct
*
* 09/02/07, Alan : Update with oz/opar and restore il_sim_save only for opar
*
* 08/12/14, Paul : Rewrote to C++.
*
*/
/*--------------------------------------------------------------------------*/

static const std::string funname = "getscicosvars";

types::Function::ReturnValue sci_getscicosvars(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() > 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d or %d expected.\n"), funname.data(), 0, 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), funname.data(), 1);
        return types::Function::Error;
    }

    int isrun = C2F(cosim).isrun;
    if (!isrun)
    {
        Scierror(999, _("%s: scicosim is not running.\n"), funname.data());
        return types::Function::Error;
    }

    bool ok;
    // Define accepted entries of getscicosvars -please update me-
    const std::vector<const char*> entry
    {
        "x"       , "nx"       , "xptr"   , "zcptr"      , "z"        ,
        "nz"      , "zptr"     , "noz"    , "oz"         , "ozsz"     ,
        "oztyp"   , "ozptr"    , "rpar"   , "rpptr"      , "ipar"     ,
        "ipptr"   , "opar"     , "oparsz" , "opartyp"    , "opptr"    ,
        "outtb"   , "inpptr"   , "outptr" , "inplnk"     , "outlnk"   ,
        "subs"    , "tevts"    , "evtspt" , "pointi"     , "iord"     ,
        "oord"    , "zord"     , "funtyp" , "ztyp"       , "cord"     ,
        "ordclk"  , "clkptr"   , "ordptr" , "critev"     , "mod"      ,
        "nmod"    , "iz"       , "nblk"   , "izptr"      , "outtbptr" ,
        "outtbsz" , "outtbtyp" , "nlnk"   , "nsubs"      , "nevts"    ,
        "niord"   , "noord"    , "nzord"  , "funptr"     , "ncord"    ,
        "nordptr" , "iwa"      , "blocks" , "ng"         , "g"        ,
        "t0"      , "tf"       , "Atol"   , "rtol"       , "ttol"     ,
        "deltat"  , "hmax"     , "nelem"  , "outtb_elem"
    };
    // Number of entries
    const size_t nentries = entry.size();

    // Display usage of getscicosvars function if in.size()==0
    if (in.size() == 0)
    {
        sciprint(_("\ngetscicosvars: utility function to retrieve\n"));
        sciprint(_("                scicos arrays during simulation.\n\n"));
        sciprint(_("Usage: [myvar]=getscicosvars([\"str1\";\"str2\";...]);\n\n"));
        sciprint(_("- myvar: an int32 or double matrix or a Tlist.\n"));
        sciprint(_("- [\"str1\";\"str2\",...] is a string matrix\n"));
        sciprint(_("  that must be informed with the following values:\n"));

        // Display allowed entries
        int i = 0;
        for (size_t j = 0; j < nentries; ++j)
        {
            if (j == nentries - 1)
            {
                sciprint("\"%s\" ", entry[j]);
            }
            else
            {
                sciprint("\"%s\", ", entry[j]);
            }
            i++;
            if (i == 6)
            {
                sciprint("\n");
                i = 0;
            }
        }
        sciprint("\n");
        return types::Function::OK;
    }

    /*******************
    * Check str (rhs 1)
    *******************/
    if (!in[0]->isString())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string matrix expected.\n"), funname.data(), 1);
        return types::Function::Error;
    }
    types::String* il_str = in[0]->getAs<types::String>();

    // Retrieve dimension of input string matrix
    const int m1 = il_str->getRows();
    const int n1 = il_str->getCols();

    // Create header of TList
    types::String* dyn_char = new types::String(1 + m1 * n1, 1);

    // Type of list is scicosvar
    const std::string scicosvar("scicosvar");
    dyn_char->set(0, "scicosvar");

    // Check string matrix
    for (int j = 0; j < m1 * n1; ++j)
    {
        ok = false;
        for (size_t i = 0; i < nentries; ++i)
        {
            char* field = wide_string_to_UTF8(il_str->get(j));
            if (strcmp(field, entry[i]) == 0)
            {
                dyn_char->set(j + 1, entry[i]);

                ok = true;
                FREE(field);
                break;
            }
            FREE(field);
        }

        // If failed then display an error message and exit
        if (!ok)
        {
            Scierror(999, _("%s: Undefined field in string matrix position: %d.\n"), funname.data(), j + 1);
            dyn_char->killMe();
            return types::Function::Error;
        }
    }

    // Preparing the return
    types::TList* ret = new types::TList();
    ret->append(dyn_char); // Header for the returned tlist

    types::InternalType* element; // Declare 'element' out of the loop to be able to return it alone in case il_str->getSize()==1
    for (int j = 0; j < m1 * n1; j++)
    {
        char* field = wide_string_to_UTF8(il_str->get(j));

        /*****************************************************************
        * Entries that can be retrieved by 'il_state_save' global variable
        *****************************************************************/
        if (strcmp(field, "x") == 0)          /* Retrieve continuous state */
        {
            types::TList* il_state = get_il_state()->getAs<types::TList>();
            if (il_state->getSize() < 2)
            {
                ok = false;
            }
            else
            {
                element = il_state->get(1);
                ok = true;
            }
        }
        else if (strcmp(field, "z") == 0)     /* Retrieve discrete state */
        {
            types::TList* il_state = get_il_state()->getAs<types::TList>();
            if (il_state->getSize() < 3)
            {
                ok = false;
            }
            else
            {
                element = il_state->get(2);
                ok = true;
            }
        }
        else if (strcmp(field, "oz") == 0)    /* Retrieve object discrete state */
        {
            types::TList* il_state = get_il_state()->getAs<types::TList>();
            if (il_state->getSize() < 4)
            {
                ok = false;
            }
            else
            {
                element = il_state->get(3);
                ok = true;
            }
        }
        else if (strcmp(field, "outtb") == 0) /* Retrieve outtb */
        {
            types::TList* il_state = get_il_state()->getAs<types::TList>();
            if (il_state->getSize() < 9)
            {
                ok = false;
            }
            else
            {
                element = il_state->get(8);
                ok = true;
            }
        }
        else if (strcmp(field, "tevts") == 0) /* Retrieve tevts */
        {
            types::TList* il_state = get_il_state()->getAs<types::TList>();
            if (il_state->getSize() < 6)
            {
                ok = false;
            }
            else
            {
                element = il_state->get(5);
                ok = true;
            }
        }

        /*****************************************************************
        * Entries that can be retrieved by 'il_sim_save' global variable
        *****************************************************************/
        if (strcmp(field, "opar") == 0)       /* retrieve object parameters */
        {
            types::TList* il_sim = get_il_sim()->getAs<types::TList>();
            if (il_sim->getSize() < 15)
            {
                ok = false;
            }
            else
            {
                element = il_sim->get(14);
                ok = true;
            }
        }

        /*************************************************
        * int variables coming from import structure
        *************************************************/
        else if ((strcmp(field, "mod") == 0)      || /* retrieve mode */
                 (strcmp(field, "nmod") == 0)     || /* retrieve nmode */
                 (strcmp(field, "iz") == 0)       || /* label int code of blocks */
                 (strcmp(field, "nblk") == 0)     || /* number of block */
                 (strcmp(field, "izptr") == 0)    || /* label int code of blocks ptr*/
                 (strcmp(field, "outtbptr") == 0) || /* outtb ptr */
                 (strcmp(field, "outtbsz") == 0)  || /* outtb size */
                 (strcmp(field, "outtbtyp") == 0) || /* outtb type */
                 (strcmp(field, "nlnk") == 0)     || /* number of link */
                 (strcmp(field, "nsubs") == 0)    || /* length of nsubs */
                 (strcmp(field, "nevts") == 0)    || /* length of evtspt & tevts */
                 (strcmp(field, "niord") == 0)    || /* length of iord */
                 (strcmp(field, "noord") == 0)    || /* length of oord */
                 (strcmp(field, "nzord") == 0)    || /* length of zord */
                 (strcmp(field, "funptr") == 0)   || /* retrieve function ptr */
                 (strcmp(field, "ncord") == 0)    || /* retrieve ncord */
                 (strcmp(field, "nordptr") == 0)  || /* retrieve nordptr */
                 (strcmp(field, "iwa") == 0)      || /* retrieve iwa */
                 (strcmp(field, "ng") == 0)       || /* retrieve ng */
                 (strcmp(field, "nx") == 0)       || /* retrieve nx */
                 (strcmp(field, "nz") == 0)       || /* retrieve nz */
                 (strcmp(field, "noz") == 0)      || /* retrieve noz */
                 (strcmp(field, "ozptr") == 0)    || /* retrieve ozptr */
                 (strcmp(field, "ozsz") == 0)     || /* retrieve ozsz */
                 (strcmp(field, "oztyp") == 0)    || /* retrieve oztyp */
                 (strcmp(field, "nelem") == 0)    || /* retrieve nelem */
                 (strcmp(field, "xptr") == 0)     || /* retrieve xptr */
                 (strcmp(field, "zcptr") == 0)    || /* retrieve zcptr */
                 (strcmp(field, "zptr") == 0)     || /* retrieve zptr */
                 (strcmp(field, "rpptr") == 0)    || /* retrieve rpptr */
                 (strcmp(field, "ipar") == 0)     || /* retrieve ipar */
                 (strcmp(field, "ipptr") == 0)    || /* retrieve ipptr */
                 (strcmp(field, "opptr") == 0)    || /* retrieve opptr */
                 (strcmp(field, "oparsz") == 0)   || /* retrieve oparsz */
                 (strcmp(field, "opartyp") == 0)  || /* retrieve opartyp */
                 (strcmp(field, "inpptr") == 0)   || /* retrieve inpptr */
                 (strcmp(field, "outptr") == 0)   || /* retrieve outptr */
                 (strcmp(field, "inplnk") == 0)   || /* retrieve inplnk */
                 (strcmp(field, "outlnk") == 0)   || /* retrieve outlnk */
                 (strcmp(field, "subs") == 0)     || /* retrieve subs */
                 (strcmp(field, "iord") == 0)     || /* retrieve iord */
                 (strcmp(field, "oord") == 0)     || /* retrieve iord */
                 (strcmp(field, "zord") == 0)     || /* retrieve iord */
                 (strcmp(field, "funtyp") == 0)   || /* retrieve funtyp */
                 (strcmp(field, "ztyp") == 0)     || /* retrieve ztyp */
                 (strcmp(field, "cord") == 0)     || /* retrieve cord */
                 (strcmp(field, "ordclk") == 0)   || /* retrieve ordclk */
                 (strcmp(field, "clkcpr") == 0)   || /* retrieve clkcpr */
                 (strcmp(field, "ordptr") == 0)   || /* retrieve ordptr */
                 (strcmp(field, "critev") == 0)   || /* retrieve critev */
                 /* state */
                 (strcmp(field, "evtspt") == 0)   || /* retrieve evtspt */
                 (strcmp(field, "pointi") == 0)      /* retrieve nelem */
                )
        {
            /* Retrieve dims and 'ptr' of asked array with getscicosvarsfromimport() */
            void* ptr = nullptr;
            int nv, mv;
            ok = getscicosvarsfromimport(field, &ptr, &nv, &mv) != 0;

            if (ok)
            {
                int* data;
                element = new types::Int32(nv, mv, &data);
                data = static_cast<int*>(ptr);
            }
        }

        /*************************************************
        * double variables coming from import structure
        *************************************************/
        else if ((strcmp(field, "rpar") == 0)   || /* retrieve rpar      */
                 (strcmp(field, "g") == 0)      || /* retrieve g      */
                 (strcmp(field, "t0") == 0)     || /* retrieve t0     */
                 (strcmp(field, "tf") == 0)     || /* retrieve tf     */
                 (strcmp(field, "Atol") == 0)   || /* retrieve Atol   */
                 (strcmp(field, "rtol") == 0)   || /* retrieve rtol   */
                 (strcmp(field, "ttol") == 0)   || /* retrieve ttol   */
                 (strcmp(field, "deltat") == 0) || /* retrieve deltat */
                 (strcmp(field, "hmax") == 0)      /* retrieve hmax   */
                )
        {
            /* Retrieve dims and 'ptr' of asked array with getscicosvarsfromimport() */
            void* ptr = nullptr;
            int nv, mv;
            ok = getscicosvarsfromimport(field, &ptr, &nv, &mv) != 0;

            if (ok)
            {
                double* data;
                element = new types::Double(nv, mv, &data);
                data = static_cast<double*>(ptr);
            }
        }

        /*************************************************
        * scicos_block ptr coming from import structure
        *************************************************/
        else if (strcmp(field, "blocks") == 0)
        {
            /* Retrieve scicos_block 'ptr' of asked array with getscicosvarsfromimport() */
            void* ptr = nullptr;
            int nv, mv;
            ok = getscicosvarsfromimport(field, &ptr, &nv, &mv) != 0;

            if (ok)
            {
                /* Store ptr in ptrscs_blk */
                scicos_block* ptr_scsblk = static_cast<scicos_block*>(ptr);

                /* Retrieve 'nblk' by import structure */
                strcpy(field, "nblk");
                ok = getscicosvarsfromimport(field, &ptr, &nv, &mv) != 0;
                int nblk = ((int*)ptr)[0];

                /* Retrieve 'ng' by import structure */
                strcpy(field, "ng");
                ok = getscicosvarsfromimport(field, &ptr, &nv, &mv) != 0;
                //int ng = ((int*)ptr)[0];

                /* Retrieve 'xptr' by import structure */
                strcpy(field, "xptr");
                ok = getscicosvarsfromimport(field, &ptr, &nv, &mv) != 0;
                int* xptr = ((int*)ptr);

                /* Retrieve 'zcptr' by import structure */
                strcpy(field, "zcptr");
                ok = getscicosvarsfromimport(field, &ptr, &nv, &mv) != 0;
                int* zcptr = ((int*)ptr);

                /* Retrieve 'x' and 'xd' by import structure */
                strcpy(field, "x");
                ok = getscicosvarsfromimport(field, &ptr, &nv, &mv) != 0;
                double* x = ((double*)ptr);
                //double* xd = &x[xptr[nblk] - 1];

                /* Retrieve 'g' by import structure */
                strcpy(field, "g");
                ok = getscicosvarsfromimport(field, &ptr, &nv, &mv) != 0;
                double* g = ((double*)ptr);

                /* Retrieve 'funtyp' by import structure */
                strcpy(field, "funtyp");
                ok = getscicosvarsfromimport(field, &ptr, &nv, &mv) != 0;
                //int* funtyp = ((int*)ptr);

                for (int k = 0; k < nblk; ++k)
                {
                    /* For each block, call createblklist() */

                    /* Set 'flag_imp' <0 for createblklst() */
                    int i = -1;

                    /* The following test is done in order to know if block k
                    * have been already called with callf in scicos.c
                    */
                    if (ptr_scsblk[k].nx != 0)
                    {
                        if (ptr_scsblk[k].x != &x[xptr[k] - 1])
                        {
                            /*fprintf(stderr,"k=%d,X,xd Non initialise",k);*/
                            /* Set 'flag_imp'=k for createblklst() <0 */
                            i = k;
                        }
                    }
                    if (ptr_scsblk[k].ng != 0)
                    {
                        if ((ptr_scsblk[k].g != &g[zcptr[k] - 1]) && (ptr_scsblk[k].g != &x[xptr[k] - 1]))
                        {
                            /*fprintf(stderr,"k=%d,g Non initialise",k);*/
                            /* Set 'flag_imp'=k for createblklst() <0 */
                            i = k;
                        }
                    }
                    /* Call createblklist() */
                    element = createblklist(&ptr_scsblk[k], i, k + 1);
                }
            }
        }

        /*******************************************
        * outtb_elem coming from import structure
        *******************************************/
        else if ((strcmp(field, "outtb_elem") == 0)) /* retrieve outtb_elem */
        {
            /* Retrieve dims and 'ptr' of asked array with getscicosvarsfromimport */
            void* ptr = nullptr;
            int nv, mv;
            ok = getscicosvarsfromimport(field, &ptr, &nv, &mv) != 0;

            /* Check 'ok' flag */
            if (ok == true)
            {
                int* data;
                element = new types::Int32(nv, mv, &data);
                outtb_el* ptr_elem = static_cast<outtb_el*>(ptr);
                for (int i = 0; i < nv; ++i) /* Copy returned array in 'element' */
                {
                    data[i] = ptr_elem[i].lnk + 1; /* +1 is for the connection with outtb list */
                    data[nv + i] = ptr_elem[i].pos + 1;
                }
            }
        }

        // If failed then display an error message.
        if (ok == false)
        {
            Scierror(999, _("%s: Error with parameter \"%s\".\n"), funname.data(), il_str->get(j));
            dyn_char->killMe();
            ret->killMe();
            FREE(field);
            return types::Function::Error;
        }

        FREE(field);
        ret->append(element);
    }

    if (il_str->getSize() == 1)
    {
        out.push_back(element);
        dyn_char->killMe();
        ret->killMe();
    }
    else
    {
        out.push_back(ret);
    }
    return types::Function::OK;
}

