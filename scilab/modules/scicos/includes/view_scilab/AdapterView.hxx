/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2017-2018 - ESI Group - Clement DAVID
*
* This file is hereby licensed under the terms of the GNU GPL v2.0,
* pursuant to article 5.3.4 of the CeCILL v.2.1.
* This file was originally licensed under the terms of the CeCILL v2.1,
* and continues to be available under such terms.
* For more information, see the COPYING file which you should have received
* along with this program.
*
*/

#ifndef ADAPTER_VIEW_HXX_
#define ADAPTER_VIEW_HXX_

#include "View.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{
/**
* This class will handle partial information on update through Adapters.
*/
struct AdapterView final : public View
{
    AdapterView();
    virtual ~AdapterView();
    AdapterView(const AdapterView& o) = delete;
    AdapterView& operator=(AdapterView o) = delete;

    void objectCreated(const ScicosID& uid, kind_t kind);
    void objectReferenced(const ScicosID& uid, kind_t kind, unsigned refCount);
    void objectUnreferenced(const ScicosID& uid, kind_t kind, unsigned refCount);
    void objectDeleted(const ScicosID& uid, kind_t kind) final;
    void objectCloned(const ScicosID& uid, const ScicosID& cloned, kind_t kind);
    void propertyUpdated(const ScicosID& uid, kind_t kind, object_properties_t property, update_status_t status);
};
} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */

#endif /* ADAPTER_VIEW_HXX_ */
