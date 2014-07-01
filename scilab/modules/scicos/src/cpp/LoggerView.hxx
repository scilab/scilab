/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef LOGGERVIEW_HXX_
#define LOGGERVIEW_HXX_

#include "View.hxx"

namespace org_scilab_modules_scicos
{

class LoggerView: public View
{
public:
    LoggerView();
    ~LoggerView();

    void objectCreated(const ScicosID& uid, kind_t k);
    void objectDeleted(const ScicosID& uid);
    void objectUpdated(const ScicosID& uid, kind_t k);
    void propertyUpdated(const ScicosID& uid, kind_t k, object_properties_t p);
    void propertyUpdated(const ScicosID& uid, kind_t k, object_properties_t p, update_status_t u);
};

} /* namespace org_scilab_modules_scicos */

#endif /* LOGGERVIEW_HXX_ */
