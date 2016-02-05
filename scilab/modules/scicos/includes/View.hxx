/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
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
    virtual void objectReferenced(const ScicosID& uid, kind_t kind, unsigned refCount) = 0;
    virtual void objectUnreferenced(const ScicosID& uid, kind_t kind, unsigned refCount) = 0;
    virtual void objectDeleted(const ScicosID& uid, kind_t kind) = 0;
    virtual void propertyUpdated(const ScicosID& uid, kind_t kind, object_properties_t property, update_status_t status) = 0;
};

} /* namespace org_scilab_modules_scicos */

#endif /* VIEW_HXX_ */
