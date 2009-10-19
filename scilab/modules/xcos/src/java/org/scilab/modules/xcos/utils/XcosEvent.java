/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.utils;

import com.mxgraph.util.mxEvent;

public class XcosEvent extends mxEvent {
    public static final String ADD_PORTS 					= "addPorts";

    public static final String SUPER_BLOCK_UPDATED			= "superBlockUpdated";
    
    public static final String IN_EXPLICIT_VALUE_UPDATED 	= "InExplicitValueUpdated";
    public static final String OUT_EXPLICIT_VALUE_UPDATED 	= "OutExplicitValueUpdated";

    public static final String IN_IMPLICIT_VALUE_UPDATED 	= "InImplicitValueUpdated";
    public static final String OUT_IMPLICIT_VALUE_UPDATED 	= "OutImplicitValueUpdated";
    
    public static final String IN_EVENT_VALUE_UPDATED 		= "InEventValueUpdated";
    public static final String OUT_EVENT_VALUE_UPDATED		= "OutEventValueUpdated";
}
