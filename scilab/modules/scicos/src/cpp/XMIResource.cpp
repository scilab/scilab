/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2016-2016 - Scilab Enterprises - Clement DAVID
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

#include "XMIResource.hxx"

extern "C" {
#include <libxml/xmlversion.h>
}

namespace org_scilab_modules_scicos
{

XMIResource::XMIResource(ScicosID id) : controller(), root(id), processed(), parent(NB_XCOS_NAMES), references()
{
    references.insert(std::make_pair("/", id));

    LIBXML_TEST_VERSION;
}

XMIResource::~XMIResource()
{
}

}
