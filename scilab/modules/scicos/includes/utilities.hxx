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

#ifndef UTILITIES_HXX_
#define UTILITIES_HXX_

/**
 * A unique ID is used to represent a reference to any object in the model.
 *
 * The 'ScicosID()' zero initialization value is used indicate that BaseObject is not handled by the controller.
 */
typedef long long ScicosID;

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
    BLOCK,          //!< model::Block object
    DIAGRAM,        //!< model::Diagram object
    LINK,           //!< model::Link object
    ANNOTATION,     //!< model::Annotation object
    PORT            //!< model::Port object
};

/**
 * Set / Get identifier
 *
 * For each fields of any model::BaseObject, a corresponding identifier exists and is used on the Controller to store and view-dispatch any modification. This field value will be then used by each view to filter out / in important event per-view.
 */
enum object_properties_t
{
    PARENT_DIAGRAM,     //!< model::*::parentDiagram value (used to locate the diagram layer)
    PARENT_BLOCK,       //!< model::*::parentBlock value (used to locate the upper layer in case of SuperBlocks hierarchy)
    GEOMETRY,           //!< model::Annotation::geometry or model::Block::geometry value
    DESCRIPTION,        //!< model::Annotation::description text
    FONT,               //!< model::Annotation::description font
    FONT_SIZE,          //!< model::Annotation::description font size
    RELATED_TO,         //!< model::Annotation::relatedTo
    INTERFACE_FUNCTION, //!< model::Block::interfaceFunction value
    SIM_FUNCTION_NAME,  //!< model::Descriptor::functionName value (stored into model::Block::sim)
    SIM_FUNCTION_API,   //!< model::Descriptor::functionApi value (stored into model::Block::sim)
    SIM_SCHEDULE,       //!< model::Descriptor::schedulingProperties value (stored into model::Block::sim)
    SIM_BLOCKTYPE,      //!< model::Descriptor::blocktype value (stored into model::Block::sim)
    SIM_DEP_UT,         //!< model::Descriptor::dep_ut value (stored into model::Block::sim)
    EXPRS,              //!< model::Block::exprs value
    INPUTS,             //!< model::Block::in value
    OUTPUTS,            //!< model::Block::out value
    EVENT_INPUTS,       //!< model::Block::ein value
    EVENT_OUTPUTS,      //!< model::Block::eout value
    STATE,              //!< model::Block::state value
    DSTATE,             //!< model::Block::dstate value
    ODSTATE,            //!< model::Block::odstate value
    NZCROSS,            //!< model::Block::nzcross value
    NMODE,              //!< model::Block::nmode value
    RPAR,               //!< model::Block::rpar value
    IPAR,               //!< model::Block::ipar value
    OPAR,               //!< model::Block::opar value
    EQUATIONS,          //!< model::Block::equations value
    UID,                //!< model::Block::uid value
    CHILDREN,           //!< model::Block::children for superblocks or model::Diagram::children value
    PORT_REFERENCE,     //!< model::Block::portReference value
    STYLE,              //!< model::Block & Port::style value
    LABEL,              //!< model::Block & Port & Link::label or id value
    DESTINATION_PORT,   //!< model::Link::destinationPort value
    SOURCE_PORT,        //!< model::Link::sourcePort value
    CONTROL_POINTS,     //!< model::Link::controlPoints value
    THICK,              //!< model::Link::thick value
    COLOR,              //!< model::Link & Block & Diagram::color value
    KIND,               //!< model::Link::kind value
    FROM,               //!< model::Link::from value
    TO,                 //!< model::Link::to value
    DATATYPE,           //!< model::Port::dataType value
    DATATYPE_ROWS,      //!< model::Port::dataType adapter helper
    DATATYPE_COLS,      //!< model::Port::dataType adapter helper
    DATATYPE_TYPE,      //!< model::Port::dataType adapter helper
    FIRING,             //!< model::Port::firing value
    SOURCE_BLOCK,       //!< model::Port::sourceBlock value
    PORT_KIND,          //!< model::Port::kind value
    IMPLICIT,           //!< model::Port::implicit value
    PORT_NUMBER,        //!< model::Port::portNumber value
    CONNECTED_SIGNALS,  //!< model::Port::connectedSignals value
    TITLE,              //!< model::Diagram::title file name value
    PATH,               //!< model::Diagram::title file path value
    PROPERTIES,         //!< model::Diagram::tol & tf values
    DEBUG_LEVEL,        //!< model::Diagram::debug_level value
    DIAGRAM_CONTEXT,    //!< model::Diagram::context value
    VERSION_NUMBER,     //!< model::Diagram::version value
    MAX_OBJECT_PROPERTIES //!< last valid value of the object_properties_t enum
};

/**
 * PORT_KIND valid values
 */
enum portKind
{
    PORT_UNDEF,
    PORT_IN,
    PORT_OUT,
    PORT_EIN,
    PORT_EOUT
};


#endif /* UTILITIES_HXX_ */
