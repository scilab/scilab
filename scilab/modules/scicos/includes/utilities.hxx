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

#ifndef UTILITIES_HXX_
#define UTILITIES_HXX_

#include <iostream>

/**
 * A unique ID is used to represent a reference to any object in the model.
 *
 * The '0' value is used indicate that BaseObject is not handled by the controller.
 */
typedef long long ScicosID;
//struct ScicosID
//{
//    ScicosID(const ScicosID& u) : v(u.v) {}
//    typedef unsigned long long uid_container_type;
//    ScicosID(uid_container_type u) : v(u) {}
//
//    inline bool operator<(const ScicosID& u) const
//    {
//        return v < u.v;
//    }
//    inline bool operator==(const ScicosID& u) const
//    {
//        return v == u.v;
//    }
//    inline void operator++(int added)
//    {
//        v = v + added;
//    }
//    friend std::ostream& operator<< (std::ostream &out, const ScicosID& u);
//private:
//    uid_container_type v;
//};

/**
 * Return status of get and set
 */
enum update_status_t
{
    SUCCESS,        //!< Property updated with new values
    NO_CHANGES,     //!< Property unchanged
    FAIL            //!< Update failed
};

/**
 * Kind of model object.
 *
 * All model::BaseObject sub-classes should be listed there. This enum is used to emulate RTTI per Model object.
 */
enum kind_t
{
    ANNOTATION,     //!< model::Annotation object
    BLOCK,          //!< model::Block object
    DIAGRAM,        //!< model::Diagram object
    LINK,           //!< model::Link object
    PORT            //!< model::Port object
};

/**
 * Set / Get identifier
 *
 * For each fields of any model::BaseObject, a corresponding identifier exists and is used on the Controller to store and view-dispatch any modification. This field value will be then used by each view to filter out / in important event per-view.
 */
enum object_properties_t
{
    PARENT_DIAGRAM,     //!< model::Annotation::parentDiagram or model::Block::parentDiagram or model::Link::parentDiagram value
    GEOMETRY,           //!< model::Annotation::geometry or model::Block::geometry value
    DESCRIPTION,        //!< model::Annotation::description
    RELATED_TO,         //!< model::Annotation::relatedTo
    INTERFACE_FUNCTION, //!< model::Block::interfaceFunction value
    SIM_FUNCTION_NAME,  //!< model::Descriptor::functionName value (stored into model::Block::sim)
    SIM_FUNCTION_API,   //!< model::Descriptor::functionApi value (stored into model::Block::sim)
    SIM_SCHEDULE,       //!< model::Descriptor::schedulingProperties value (stored into model::Block::sim)
    ANGLE,              //!< model::Block::flip and theta values
    EXPRS,              //!< model::Block::exprs value
    INPUTS,             //!< model::Block::in value
    OUTPUTS,            //!< model::Block::out value
    EVENT_INPUTS,       //!< model::Block::ein value
    EVENT_OUTPUTS,      //!< model::Block::eout value
    PARENT_BLOCK,       //!< model::Block::parentBlock value
    CHILDREN,           //!< model::Block::children value
    PORT_REFERENCE,     //!< model::Block::portReference value
    DESTINATION_PORT,   //!< model::Link::destinationPort value
    SOURCE_PORT,        //!< model::Link::sourcePort value
    CONTROL_POINTS,     //!< model::Link::controlPoints value
    DATATYPE,           //!< model::Port::dataType value
    SOURCE_BLOCK,       //!< model::Port::sourceBlock value
    PORT_KIND,          //!< model::Port::kind value
    PORT_NUMBER,        //!< model::Port::portNumber value
    CONNECTED_SIGNALS,  //!< model::Port::connectedSignals value
};

#endif /* UTILITIES_HXX_ */
