/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2021 - Stéphane MOTTELET
 *
 * This file is hereby licensed under the terms of the GNU GPL v3.0,
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include <cwctype>
#include "core_gw.hxx"
#include "types.hxx"
#include "function.hxx"
#include "string.hxx"
#include "double.hxx"
#include "file.hxx"
#include "filemanager.hxx"
#include "crc32.hxx"
#include "md5.hxx"
#include "sha1.hxx"
#include "sha256.hxx"
#include "sha3.hxx"

extern "C"
{
#include "Scierror.h"
#include "sci_malloc.h"
}

types::Function::ReturnValue sci_hash(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    bool bStringMode = false;
    char* pstInput = NULL;
    int iSize = 0;
    types::String* pIn = NULL;
    types::String* pOutput = NULL;
    types::Double* pDbl = NULL;
    Hash* digest = NULL;

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "hash", 1);
        return types::Function::Error;
    }

    if (in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "hash", 2);
        return types::Function::Error;
    }

    if (in[0]->isString() == false && in[0]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type of input argument #%d: String or File descriptor expected.\n"), "hash", 1);
        return types::Function::Error;
    }

    if (in[1]->isString() == false || in[1]->getAs<types::String>()->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong type of input argument #%d: a single string expected.\n"), "hash", 2);
        return types::Function::Error;
    }

    std::wstring wstrAlgo(in[1]->getAs<types::String>()->get()[0]);
    std::transform(wstrAlgo.begin(), wstrAlgo.end(), wstrAlgo.begin(),std::towlower);

    if (wstrAlgo == L"crc32")
    {
        digest = new CRC32();
    }
    else if (wstrAlgo == L"md5")
    {
        digest = new MD5();
    }
    else if (wstrAlgo == L"sha1")
    {
        digest = new SHA1();
    }
    else if (wstrAlgo == L"sha2" || wstrAlgo == L"sha256")
    {
        digest = new SHA256();
    }
    else if (wstrAlgo == L"sha3-224")
    {
        digest = new SHA3(SHA3::Bits224);
    }
    else if (wstrAlgo == L"sha3-256")
    {
        digest = new SHA3(SHA3::Bits256);
    }
    else if (wstrAlgo == L"sha3-384")
    {
        digest = new SHA3(SHA3::Bits384);
    }
    else if (wstrAlgo == L"sha3-512")
    {
        digest = new SHA3(SHA3::Bits512);
    }
    else
    {
        Scierror(999, _("%s: algorithm %ls is unknown.\n"), "hash", wstrAlgo.c_str());
        return types::Function::Error;
    }

    if (in[0]->isString())
    {
        pIn = in[0]->getAs<types::String>();
        pOutput = new types::String(pIn->getRows(), pIn->getCols());
        bStringMode = true;
        iSize = pIn->getSize();
    }
    else
    {
        pDbl = in[0]->getAs<types::Double>();
        pOutput = new types::String(pDbl->getRows(), pDbl->getCols());
        iSize = pDbl->getSize();
    }

    for (int i = 0; i < iSize; ++i)
    {
        wchar_t *pwstHash = NULL;

        if (bStringMode)
        {
            wchar_t *pwstInput = pIn->get(i);
            pstInput = wide_string_to_UTF8(pwstInput);
            digest->add(pstInput, strlen(pstInput));
            FREE(pstInput);
        }
        else
        {
            FILE *fp = NULL;
            char pStData[64];
            size_t iLen = 0;
            int iFile = (int) pDbl->get(i);
            types::File* pF = NULL;

            switch (iFile)
            {
                case 0: // stderr
                case 5: // stdin
                case 6: // stdout
                    break;
                default :
                    pF =  FileManager::getFile(iFile);
            }

            if (pF == NULL)
            {
                Scierror(999, _("%s: Wrong file descriptor: %d.\n"), "hash", iFile);
                return types::Function::Error;
            }
            else if (pF->getFileMode()[0] != L'r')
            {
                Scierror(999, _("%s: File must be opened for reading.\n"), "hash", iFile);
                fclose(pF->getFiledesc());
                FileManager::deleteFile(iFile);
                return types::Function::Error;
            }

            fp = pF->getFiledesc();
            if (fp) // argument is a valid path to an existing file
            {
                while (!feof(fp))
                {
                    iLen = fread(pStData, 1, sizeof(pStData), fp);
                    digest->add(pStData, iLen);
                }
                //fclose(fp);
                // after closing, don't forget to remove file from the list of opened files
                //FileManager::deleteFile(iFile);
            }
        }

        std::string strHash = digest->getHash();
        if (strHash.length() > 0)
        {
            pwstHash = to_wide_string(strHash.c_str());
            pOutput->set(i, pwstHash);
            FREE(pwstHash);
        }
        else
        {
            pOutput->set(i, L"");
        }

        digest->reset();
    }

    delete digest;
    out.push_back(pOutput);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
