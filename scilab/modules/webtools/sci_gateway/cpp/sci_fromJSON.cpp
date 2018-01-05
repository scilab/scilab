/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2017 - ESI-Group - Antoine ELIAS
*
*
* This file is hereby licensed under the terms of the GNU GPL v2.0,
* pursuant to article 5.3.4 of the CeCILL v.2.1.
* This file was originally licensed under the terms of the CeCILL v2.1,
* and continues to be available under such terms.
* For more information, see the COPYING file which you should have received
* along with this program.
*
*/
/*--------------------------------------------------------------------------*/

#include <list>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <fstream>
#include <chrono>

#define __API_SCILAB_UNSAFE__
#include "json.hxx"

//#define SHOW_TIMER

extern "C"
{
#include "gw_webtools.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "sciprint.h"
#include "sci_malloc.h"
#include <localization.h>
#include "charEncoding.h"
}

const std::string name("fromJSON");

/* ==================================================================== */
int sci_fromJSON(scilabEnv env, int nin, scilabVar *in, int nopt, scilabOpt opt, int nout, scilabVar *out)
{
    std::string json;
#ifdef SHOW_TIMER
    std::chrono::steady_clock::time_point ttotal = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point tinput = std::chrono::steady_clock::now();
#endif

    if (1 > nin || nin > 2)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d to %d expected.\n"), name.data(), 1, 2);
        return STATUS_ERROR;
    }

    if (nin == 2)
    {
        if (!scilab_isString(env, in[1]) || !scilab_isScalar(env, in[1]))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), name.data(), 2);
            return STATUS_ERROR;
        }

        wchar_t *wf = nullptr;
        scilab_getString(env, in[1], &wf);
        if (wcscmp(wf, L"file") != 0)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: \"file\" expected.\n"), name.data(), 2);
            return STATUS_ERROR;
        }

        wchar_t* wfile = nullptr;
        scilab_getString(env, in[0], &wfile);

        char *_filename = wide_string_to_UTF8(wfile);
        if (_filename == NULL)
        {
            return false;
        }

        std::ifstream infile(_filename);
        FREE(_filename);

        json.reserve(infile.tellg());
        infile.seekg(0, std::ios::beg);

        json.assign((std::istreambuf_iterator<char>(infile)),
            std::istreambuf_iterator<char>());
    }
    else
    {
        //must be a string or string matrix
        if (!scilab_isString(env, in[0]))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), name.data(), 1);
            return STATUS_ERROR;
        }

        if (scilab_isScalar(env, in[0]))
        {
            wchar_t* w;
            scilab_getString(env, in[0], &w);
            char* s = wide_string_to_UTF8(w);
            json = s;
            FREE(s);
        }
        else
        {
            wchar_t** w;
            int size = scilab_getSize(env, in[0]);
            scilab_getStringArray(env, in[0], &w);

            for (int i = 0; i < size; ++i)
            {
                char* s = wide_string_to_UTF8(w[i]);
                json += s;
                FREE(s);
            }
        }
    }

#ifdef SHOW_TIMER
    std::chrono::steady_clock::time_point tinputend = std::chrono::steady_clock::now();
    sciprint("Input\t: %d ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(tinputend - tinput).count());

    std::chrono::steady_clock::time_point tparse = std::chrono::steady_clock::now();
#endif

    scilabVar var = fromJSON(env, json);
    if (var == nullptr)
    {
        Scierror(999, _("%s: JSON format expected.\n"), name.data(), 1);
        return STATUS_ERROR;
    }

    out[0] = var;

#ifdef SHOW_TIMER
    std::chrono::steady_clock::time_point toutputend = std::chrono::steady_clock::now();
    sciprint("Output\t: %d ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(toutputend - toutput).count());

    std::chrono::steady_clock::time_point ttotalend = std::chrono::steady_clock::now();
    sciprint("Total\t: %d ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(ttotalend - ttotal).count());
#endif

    return STATUS_OK;
}
