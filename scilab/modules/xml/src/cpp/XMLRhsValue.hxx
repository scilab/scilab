/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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

#ifndef __XMLRHSVALUE_HXX__
#define __XMLRHSVALUE_HXX__

#include "xmlObjects.h"

extern "C"
{
#include "Scierror.h"
#include "api_scilab.h"
#include "xml_mlist.h"
}

namespace org_modules_xml
{

/**
 * @file
 * @author Calixte DENIZET <calixte.denizet@scilab.org>
 *
 * Base class for the XML objects.
 */
class XMLRhsValue
{

public:
    /**
     * @anchor get
     * @param fname the function name
     * @param addr the Scilab's variable address
     * @param obj a pointer to the object to retrieve
     * @return true if all is ok
     */
    static bool get(char *fname, int *addr, XMLDocument ** obj, void *pvApiCtx)
    {
        return get < XMLDocument > (fname, addr, obj, pvApiCtx);
    }

    /**
     * @ref get
     */
    static bool get(char *fname, int *addr, XMLElement ** obj, void *pvApiCtx)
    {
        return get < XMLElement > (fname, addr, obj, pvApiCtx);
    }

    /**
     * @ref get
     */
    static bool get(char *fname, int *addr, XMLAttr ** obj, void *pvApiCtx)
    {
        return get < XMLAttr > (fname, addr, obj, pvApiCtx);
    }

    /**
     * @ref get
     */
    static bool get(char *fname, int *addr, XMLNs ** obj, void *pvApiCtx)
    {
        return get < XMLNs > (fname, addr, obj, pvApiCtx);
    }

    /**
     * @ref get
     */
    static bool get(char *fname, int *addr, XMLNodeList ** obj, void *pvApiCtx)
    {
        return get < XMLNodeList > (fname, addr, obj, pvApiCtx);
    }

    /**
     * @ref get
     */
    static bool get(char *fname, int *addr, std::string ** obj, void *pvApiCtx)
    {
        char **str = 0;
        int row = 0;
        int col = 0;

        std::string * code = new std::string("");

        if (getAllocatedMatrixOfString(pvApiCtx, addr, &row, &col, &str))
        {
            delete code;
            return false;
        }

        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                *code += str[i + row * j];
            }

            if (i != row - 1)
            {
                *code += "\n";
            }
        }
        freeAllocatedMatrixOfString(row, col, str);
        *obj = code;

        return true;
    }

private:

    /**
     * @ref get
     */
    template < class T > static bool get(char *fname, int *addr, T ** obj, void *pvApiCtx)
    {
        int id = getXMLObjectId(addr, pvApiCtx);

        *obj = XMLObject::getFromId < T > (id);
        if (!*obj)
        {
            Scierror(999, "%s: XML object does not exist\n", fname);
            return false;
        }

        return true;
    }
};
}

#endif
