/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef VIEW_HXX_
#define VIEW_HXX_

#include "utilities.hxx"

#include "dynlib_scicos.h"

namespace org_scilab_modules_scicos
{

class SCICOS_IMPEXP View
{
public:
    View() {};
    virtual ~View() {};

    virtual void objectCreated(const ScicosID& uid, kind_t kind) = 0;
    virtual void objectReferenced(const ScicosID& uid, kind_t kind) = 0;
    virtual void objectUnreferenced(const ScicosID& uid, kind_t kind) = 0;
    virtual void objectDeleted(const ScicosID& uid, kind_t kind) = 0;
    virtual void propertyUpdated(const ScicosID& uid, kind_t kind, object_properties_t property, update_status_t status) = 0;
};

} /* namespace org_scilab_modules_scicos */

#endif /* VIEW_HXX_ */
