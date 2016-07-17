/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Scilab Enterprises - 2015 - Antoine ELIAS
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

#include <vector>
#include "api_scilab.h"
#include "configvariable.hxx"

extern "C"
{
#include "gw_dynamic_link.h"
#include "Scierror.h"
#include "localization.h"
}

bool isOut(scilabEnv env, scilabVar var);

typedef void(*fct)(void* p0, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8, void* p9,
                   void* p10, void* p11, void* p12, void* p13, void* p14, void* p15, void* p16, void* p17, void* p18, void* p19,
                   void* p20, void* p21, void* p22, void* p23, void* p24, void* p25, void* p26, void* p27, void* p28, void* p29);


struct Parameter
{
    Parameter() : data(nullptr), type(L'\0'), alloc(false), row(0), col(0) {}
    ~Parameter()
    {
        if (alloc)
        {
            free(data);
        }
    }

    void* data;
    wchar_t type;
    bool alloc;
    int row;
    int col;
};

static const char fname[] = "call";

int sci_call(scilabEnv env, int nin, scilabVar* in, int nopt, scilabOpt opt, int nout, scilabVar* out)
{
    std::vector<Parameter> params(30);
    std::vector<int> output_order(nout);
    wchar_t* interf = NULL;
    if (nin < 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), fname, 1);
        return 1;
    }

    //1st is the interface name
    if (scilab_isString(env, in[0]) == 0 || scilab_isScalar(env, in[0]) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 1);
        return 1;
    }

    scilab_getString(env, in[0], &interf);

    ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(interf);
    if (func == NULL)
    {
        Scierror(999, _("%s: unable to find entry point %ls.\n"), fname, interf);
        return 1;
    }

    int pos = 1;
    bool hasOutputs = true;
    //inputs
    while (1)
    {
        //check "out" to break loop
        if (isOut(env, in[pos]))
        {
            hasOutputs = true;
            break;
        }

        if (pos > nin)
        {
            break;
        }

        int type = 0;
        if (nin < pos + 2)
        {
            Scierror(77, _("%s: Wrong number of input argument(s).\n"), fname);
            return 1;
        }

        type = scilab_getType(env, in[pos]);
        if (type != sci_matrix && type != sci_strings)
        {
            Scierror(77, _("%s: Wrong type for input argument #%d: A real matrix or a string expected.\n"), fname, pos + 1);
            return 1;
        }

        //data

        //position
        if (scilab_isDouble(env, in[pos + 1]) == 0 || scilab_isScalar(env, in[pos + 1]) == 0)
        {
            Scierror(77, _("%s: Wrong type for input argument #%d : A real scalar expected.\n"), fname, pos + 2);
            return 1;
        }

        double param_pos = 0;
        scilab_getDouble(env, in[pos + 1], &param_pos);

        //type
        if (scilab_isString(env, in[pos + 2]) == 0 || scilab_isScalar(env, in[pos + 2]) == 0)
        {
            Scierror(77, _("%s: Wrong type for input argument #%d : string expected.\n"), fname, pos + 3);
            return 1;
        }

        void* data = NULL;
        int row = 0;
        int col = 0;

        wchar_t* param_type = NULL;
        scilab_getString(env, in[pos + 2], &param_type);

        if (param_type[0] == L'c' || type == sci_strings)
        {
            if (param_type[0] != L'c' || type != sci_strings)
            {
                Scierror(77, _("%s: Wrong type for input argument #%d : string expected.\n"), fname, pos + 1);
                return 1;
            }
        }

        bool alloc = false;
        switch (param_type[0])
        {
            case L'c':
            {
                wchar_t* strs = NULL;
                scilab_getString(env, in[pos], &strs);
                char* c = wide_string_to_UTF8(strs);
                data = c;
                alloc = true;
                break;
            }
            case L'd':
            {
                double* dbls = NULL;
                scilab_getDoubleArray(env, in[pos], &dbls);
                data = dbls;
                break;
            }
            case L'r':
            {
                double* dbls = NULL;
                int size = scilab_getSize(env, in[pos]);
                scilab_getDoubleArray(env, in[pos], &dbls);
                float* f = (float*)malloc(size * sizeof(float));
                for (int i = 0; i < size; ++i)
                {
                    f[i] = (float)dbls[i];
                }

                data = f;
                alloc = true;
                break;
            }
            case L'i':
            {
                double* dbls = NULL;
                int size = scilab_getSize(env, in[pos]);
                scilab_getDoubleArray(env, in[pos], &dbls);
                int* ints = (int*)malloc(size * sizeof(int));
                for (int i = 0; i < size; ++i)
                {
                    ints[i] = (int)dbls[i];
                }

                data = ints;
                alloc = true;
                break;
            }
            default:
            {
                Scierror(77, _("%s: Wrong value for input argument #%d: '%s', '%s', '%s' or '%s' expected.\n"), fname, pos + 3, "d", "r", "i", "c");
                return 1;
            }
        }

        scilab_getDim2d(env, in[pos], &row, &col);

        Parameter& p = params[(int)param_pos - 1];
        p.alloc = alloc;
        p.data = data;
        p.row = row;
        p.col = col;
        p.type = param_type[0];

        pos += 3;
    }

    int output_pos = 0;
    //outputs
    if (hasOutputs)
    {
        ++pos; //avoid "out"
        while (1)
        {
            //check if is 3 or 1 arg ...
            if (scilab_isDouble(env, in[pos]) == 0)
            {
                Scierror(77, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), fname, pos + 1);
                return 1;
            }

            if (scilab_isScalar(env, in[pos]))
            {
                double dorder = 0;
                scilab_getDouble(env, in[pos], &dorder);
                int order = (int)dorder;
                if (params[order - 1].data == nullptr)
                {
                    Scierror(77, _("%s: Wrong value for input argument #%d.\n"), fname, pos + 1);
                    return 1;
                }

                pos += 1;
                output_order[output_pos] = order - 1;
            }
            else
            {
                //dims
                double* dims = 0;
                scilab_getDoubleArray(env, in[pos], &dims);
                int size = (int)dims[0] * (int)dims[1];

                //pos
                if (scilab_isDouble(env, in[pos + 1]) == 0 || scilab_isScalar(env, in[pos + 1]) == 0)
                {
                    Scierror(77, _("%s: Wrong type for input argument #%d : A real scalar expected.\n"), fname, pos + 2);
                    return 1;
                }

                double param_pos = 0;
                scilab_getDouble(env, in[pos + 1], &param_pos);

                //type
                if (scilab_isString(env, in[pos + 2]) == 0 || scilab_isScalar(env, in[pos + 2]) == 0)
                {
                    Scierror(77, _("%s: Wrong type for input argument #%d : string expected.\n"), fname, pos + 3);
                    return 1;
                }

                wchar_t* param_type = NULL;
                scilab_getString(env, in[pos + 2], &param_type);

                void* data = NULL;

                switch (param_type[0])
                {
                    case L'c':
                    {
                        data = malloc((size + 1) * sizeof(char));
                        break;
                    }
                    case L'd':
                    {
                        data = malloc(size * sizeof(double));
                        break;
                    }
                    case L'r':
                    {
                        data = malloc(size * sizeof(float));
                        break;
                    }
                    case L'i':
                    {
                        data = malloc(size * sizeof(int));
                        break;
                    }
                }
                Parameter& p = params[(int)param_pos - 1];
                p.row = (int)dims[0];
                p.col = (int)dims[1];
                p.alloc = true;
                p.type = param_type[0];
                p.data = data;
                pos += 3;
                output_order[output_pos] = (int)param_pos - 1;
            }

            ++output_pos;

            if (pos + 1 > nin)
            {
                break;
            }
        }

    }
    //the unbelievable call !
    ((fct)func->functionPtr)(params[0].data, params[1].data, params[2].data, params[3].data, params[4].data, params[5].data, params[6].data, params[7].data, params[8].data, params[9].data,
                             params[10].data, params[11].data, params[12].data, params[13].data, params[14].data, params[15].data, params[16].data, params[17].data, params[18].data, params[19].data,
                             params[20].data, params[21].data, params[22].data, params[23].data, params[24].data, params[25].data, params[26].data, params[27].data, params[28].data, params[29].data);

    //create output variables
    for (int i = 0; i < nout; ++i)
    {
        Parameter& p = params[output_order[i]];

        switch (p.type)
        {
            case L'c':
            {
                wchar_t* w = to_wide_string((char*)p.data);
                scilabVar var = scilab_createString(env, w);
                out[i] = var;
                FREE(w);
                break;
            }
            case L'd':
            {
                scilabVar var = scilab_createDoubleMatrix2d(env, p.row, p.col, 0);
                scilab_setDoubleArray(env, var, (double*)p.data);
                out[i] = var;
                break;
            }
            case L'r':
            {
                double* d = NULL;
                scilabVar var = scilab_createDoubleMatrix2d(env, p.row, p.col, 0);
                scilab_getDoubleArray(env, var, &d);
                int size = p.row * p.col;
                for (int j = 0; j < size; ++j)
                {
                    d[j] = (double)((float*)p.data)[j];
                }

                out[i] = var;
                break;
            }
            case L'i':
            {
                double* d = NULL;
                scilabVar var = scilab_createDoubleMatrix2d(env, p.row, p.col, 0);
                scilab_getDoubleArray(env, var, &d);
                int size = p.row * p.col;
                for (int j = 0; j < size; ++j)
                {
                    d[j] = (double)((int*)p.data)[j];
                }

                out[i] = var;
                break;
            }
        }
    }
    return STATUS_OK;
}

bool isOut(scilabEnv env, scilabVar var)
{
    if (scilab_isString(env, var) && scilab_isScalar(env, var))
    {
        wchar_t* strs = NULL;
        scilab_getString(env, var, &strs);
        if (wcscmp(strs, L"out") == 0 || wcscmp(strs, L"sort") == 0)
        {
            return true;
        }
    }

    return false;
}
