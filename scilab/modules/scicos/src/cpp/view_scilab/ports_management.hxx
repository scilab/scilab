/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
 *  Copyright (C) 2014 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef PORTS_MANAGEMENT_HXX_
#define PORTS_MANAGEMENT_HXX_

#include <string>
#include <vector>

#include "internal.hxx"

#include "Controller.hxx"
#include "GraphicsAdapter.hxx"

namespace org_scilab_modules_scicos
{
namespace view_scilab
{

/*
 * Return a Scilab encoded value for a property.
 */
types::InternalType* get_ports_property(const GraphicsAdapter& adaptor, object_properties_t port_kind, const Controller& controller, object_properties_t p);

/*
 * Set a Scilab encoded values as a property.
 *
 * \note this method will return false if one of the ports does not exist
 */
bool set_ports_property(const GraphicsAdapter& adaptor, object_properties_t port_kind, Controller& controller, object_properties_t p, types::InternalType* v);

/**
 * Update the ports with a specific property.
 *
 * Create ports if needed, remove ports if needed and set a default property on each port.
 */
bool update_ports_with_property(const GraphicsAdapter& adaptor, object_properties_t port_kind, Controller& controller, object_properties_t p, types::InternalType* v);


} /* view_scilab */
} /* namespace org_scilab_modules_scicos */

#endif /* PORTS_MANAGEMENT_HXX_ */
