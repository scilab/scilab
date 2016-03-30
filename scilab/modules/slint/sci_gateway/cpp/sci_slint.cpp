/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Antoine ELIAS
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

#include <iostream>
#include <string>

#include "SLint.hxx"
#include "output/SLintScilabResult.hxx"
#include "output/SLintXmlResult.hxx"
#include "output/cnes/CNESXmlResult.hxx"
#include "output/cnes/CNESCsvResult.hxx"
#include "config/cnes/ToolConfiguration.hxx"
#include "config/XMLConfig.hxx"

#include "struct.hxx"
#include "string.hxx"
#include "slint_gw.hxx"

extern "C"
{
#include "Scierror.h"
#include "sciprint.h"
#include "localization.h"
}

static bool contributionMsg = true;
/**
 * slint(String[a,b] files, String[1,c] conf, String[1,1] out)
 * slint(String[a,b] files, String[1,1] out)                   : default conf is etc/slint.xml
 * slint(String[a,b] files)                                    : default conf is etc/slint.xml & out is outputstream
 * slint(String[a,b] files, bool[1,1] printResult)             : default conf is etc/slint.xml
 * slint(String[a,b] files, String[1,c] conf, bool[1,1] printResult)
 */

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_slint(types::typed_list & in, int _iRetCount, types::typed_list & out)
{
    slint::SLintResult * results = nullptr;
    bool printResults = false;
    const int size = (int)in.size();
    types::String * conf = nullptr;
    types::String * outFile = nullptr;

    if (size == 0 || size >= 4)
    {
        Scierror(999, _("%s: Wrong number of input arguments: at least %d expected.\n"), "slint", 1);
        return types::Function::Error;
    }

    if (!in[0]->isString())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "slint", 1);
        return types::Function::Error;
    }

    switch (size)
    {
        case 1:
        {
            printResults = true;
            break;
        }
        case 2:
        {
            if (in[1]->isBool())
            {
                if (in[1]->getAs<types::Bool>()->getSize() == 1)
                {
                    printResults = in[1]->getAs<types::Bool>()->get(0) == 0 ? false : true;
                }
                else
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A single boolean expected.\n"), "slint", 2);
                    return types::Function::Error;
                }
            }
            else if (in[1]->isString())
            {
                outFile = in[1]->getAs<types::String>();
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A string or a boolean expected.\n"), "slint", 2);
                return types::Function::Error;
            }
            break;
        }
        case 3:
        {
            if (in[2]->isBool())
            {
                if (in[2]->getAs<types::Bool>()->getSize() == 1)
                {
                    printResults = in[2]->getAs<types::Bool>()->get(0) == 0 ? false : true;
                }
                else
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A single boolean expected.\n"), "slint", 3);
                    return types::Function::Error;
                }
            }
            else if (in[2]->isString())
            {
                outFile = in[2]->getAs<types::String>();
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A string or a boolean expected.\n"), "slint", 3);
                return types::Function::Error;
            }

            if (!in[1]->isString())
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "slint", 3);
                return types::Function::Error;
            }
            conf = in[1]->getAs<types::String>();
            break;
        }
    }

    try
    {
        slint::SLintOptions options;
        if (conf)
        {
            if (conf->getSize() == 1)
            {
                slint::XMLConfig::getOptions(conf->get(0), options);
            }
            else
            {
                slint::XMLConfig::getOptions(*conf, options);
            }
        }
        else
        {
            slint::XMLConfig::getOptions(L"SCI/modules/slint/etc/slint.xml", options);
        }

        if (outFile)
        {
            if (conf && conf->getSize() >= 2 && (std::wstring(conf->get(0)) == L"cnes"))
            {
                const slint::CNES::ToolConfiguration tc = slint::CNES::ToolConfiguration::createFromXml(conf->get(1));
                const std::wstring out(outFile->get(0));
                const std::size_t pos = out.find_last_of(L'.');
                if (pos != std::string::npos && out.substr(pos) == L".csv")
                {
                    results = new slint::CNES::CNESCsvResult(tc, conf, options.getId(), outFile->get(0));
                }
                else
                {
                    results = new slint::CNES::CNESXmlResult(tc, conf, options.getId(), outFile->get(0));
                }
            }
            else
            {
                results = new slint::SLintXmlResult(outFile->get(0));
            }
        }
        else
        {
            if (printResults)
            {
                results = new slint::SLintScilabResult();
            }
            else
            {
                results = new slint::SLintScilabOut();
            }
        }

        slint::SLint slint(options, *results);
        slint.setFiles(in[0]->getAs<types::String>());
        slint.check();
        results->finalize();

        if (!outFile && !printResults)
        {
            out.emplace_back(static_cast<slint::SLintScilabOut *>(results)->getStruct());
        }
    }
    catch (slint::PCREException & e)
    {
        delete results;
        Scierror(999, _("%s: %s\n"), "slint", e.what(), 1);
        return types::Function::Error;
    }
    catch (slint::FileException & e)
    {
        delete results;
        Scierror(999, _("%s: %s\n"), "slint", e.what(), 1);
        return types::Function::Error;
    }
    catch (slint::SLintXMLException & e)
    {
        delete results;
        Scierror(999, _("%s: %s\n"), "slint", e.what(), 1);
        return types::Function::Error;
    }

    delete results;

    if (contributionMsg)
    {
        sciprint("%s\n", _("Module developed with the contribution of CNES."));
        contributionMsg = false;
    }

    return types::Function::OK;
}
