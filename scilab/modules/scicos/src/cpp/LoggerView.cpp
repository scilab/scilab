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

#include <string>
#include <iostream>
#include <sstream>
#include "scilabWrite.hxx"

#include "LoggerView.hxx"
#include "Controller.hxx"

namespace org_scilab_modules_scicos
{

LoggerView::LoggerView()
{
}

LoggerView::~LoggerView()
{
}

// generated with :
// awk ' $2 == "//!<" {sub(",","", $1); print "case " $1 ":\n    os << \"" $1 "\";\n    break;" }' ~/work/branches/YaSp/scilab/modules/scicos/includes/utilities.hxx

std::ostream& operator<<(std::ostream& os, update_status_t u)
{
    switch (u)
    {
        case SUCCESS:
            os << "SUCCESS";
            break;
        case NO_CHANGES:
            os << "NO_CHANGES";
            break;
        case FAIL:
            os << "FAIL";
            break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, kind_t k)
{
    switch (k)
    {
        case ANNOTATION:
            os << "ANNOTATION";
            break;
        case BLOCK:
            os << "BLOCK";
            break;
        case DIAGRAM:
            os << "DIAGRAM";
            break;
        case LINK:
            os << "LINK";
            break;
        case PORT:
            os << "PORT";
            break;
    }
    return os;
}


std::ostream& operator<<(std::ostream& os, object_properties_t p)
{
    switch (p)
    {
        case PARENT_DIAGRAM:
            os << "PARENT_DIAGRAM";
            break;
        case GEOMETRY:
            os << "GEOMETRY";
            break;
        case DESCRIPTION:
            os << "DESCRIPTION";
            break;
        case RELATED_TO:
            os << "RELATED_TO";
            break;
        case INTERFACE_FUNCTION:
            os << "INTERFACE_FUNCTION";
            break;
        case SIM_FUNCTION_NAME:
            os << "SIM_FUNCTION_NAME";
            break;
        case SIM_FUNCTION_API:
            os << "SIM_FUNCTION_API";
            break;
        case SIM_SCHEDULE:
            os << "SIM_SCHEDULE";
            break;
        case SIM_BLOCKTYPE:
            os << "SIM_BLOCKTYPE";
            break;
        case SIM_DEP_UT:
            os << "SIM_DEP_UT";
            break;
        case ANGLE:
            os << "ANGLE";
            break;
        case EXPRS:
            os << "EXPRS";
            break;
        case INPUTS:
            os << "INPUTS";
            break;
        case OUTPUTS:
            os << "OUTPUTS";
            break;
        case EVENT_INPUTS:
            os << "EVENT_INPUTS";
            break;
        case EVENT_OUTPUTS:
            os << "EVENT_OUTPUTS";
            break;
        case STATE:
            os << "STATE";
            break;
        case DSTATE:
            os << "DSTATE";
            break;
        case ODSTATE:
            os << "ODSTATE";
            break;
        case NZCROSS:
            os << "NZCROSS";
            break;
        case NMODE:
            os << "NMODE";
            break;;
        case RPAR:
            os << "RPAR";
            break;
        case IPAR:
            os << "IPAR";
            break;
        case OPAR:
            os << "OPAR";
            break;
        case EQUATIONS:
            os << "EQUATIONS";
            break;
        case UID:
            os << "UID";
            break;
        case PARENT_BLOCK:
            os << "PARENT_BLOCK";
            break;
        case CHILDREN:
            os << "CHILDREN";
            break;
        case PORT_REFERENCE:
            os << "PORT_REFERENCE";
            break;
        case STYLE:
            os << "STYLE";
            break;
        case LABEL:
            os << "LABEL";
            break;
        case DESTINATION_PORT:
            os << "DESTINATION_PORT";
            break;
        case SOURCE_PORT:
            os << "SOURCE_PORT";
            break;
        case CONTROL_POINTS:
            os << "CONTROL_POINTS";
            break;
        case DATATYPE:
            os << "DATATYPE";
            break;
        case DATATYPE_ROWS:
            os << "DATATYPE_ROWS";
            break;
        case DATATYPE_COLS:
            os << "DATATYPE_COLS";
            break;
        case DATATYPE_TYPE:
            os << "DATATYPE_TYPE";
            break;
        case FIRING:
            os << "FIRING";
            break;
        case SOURCE_BLOCK:
            os << "SOURCE_BLOCK";
            break;
        case PORT_KIND:
            os << "PORT_KIND";
            break;
        case IMPLICIT:
            os << "IMPLICIT";
            break;
        case PORT_NUMBER:
            os << "PORT_NUMBER";
            break;
        case CONNECTED_SIGNALS:
            os << "CONNECTED_SIGNALS";
            break;
    }
    return os;
}

static const bool USE_SCILAB_WRITE = true;

void LoggerView::objectCreated(const ScicosID& uid, kind_t k)
{
    std::stringstream ss;

    ss << __FUNCTION__ << "( " << uid << " , " << k << " )" << std::endl;

    if (USE_SCILAB_WRITE)
    {
        scilabForcedWrite(ss.str().data());
    }
    else
    {
        std::cerr << ss.str();
    }
}

void LoggerView::objectDeleted(const ScicosID& uid)
{
    std::stringstream ss;

    ss << __FUNCTION__ << "( " << uid << " )" << std::endl;

    if (USE_SCILAB_WRITE)
    {
        scilabForcedWrite(ss.str().data());
    }
    else
    {
        std::cerr << ss.str();
    }
}

void LoggerView::objectUpdated(const ScicosID& uid, kind_t k)
{
    std::stringstream ss;

    ss << __FUNCTION__ << "( " << uid << " , " << k << " )" << std::endl;

    if (USE_SCILAB_WRITE)
    {
        scilabForcedWrite(ss.str().data());
    }
    else
    {
        std::cerr << ss.str();
    }
}

void LoggerView::propertyUpdated(const ScicosID& uid, kind_t k, object_properties_t p)
{
    // do not log anything on success; the message has already been logged
}

void LoggerView::propertyUpdated(const ScicosID& uid, kind_t k, object_properties_t p,
                                 update_status_t u)
{
    std::stringstream ss;

    ss << __FUNCTION__ << "( " << uid << " , " << k << " , " << p << " ) : " << u << std::endl;

    if (USE_SCILAB_WRITE)
    {
        scilabForcedWrite(ss.str().data());
    }
    else
    {
        std::cerr << ss.str();
    }
}

} /* namespace org_scilab_modules_scicos */
