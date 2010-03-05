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

/**
 * Add Xcos specialized events to mxGraph
 */
public final class XcosEvent extends mxEvent {

	/**
	 * Fired when a port as been added by the user
	 */
	public static final String ADD_PORTS = "addPorts";

	/**
	 * Fired when something as been done and the cell need to be updated
	 */
	public static final String FORCE_CELL_VALUE_UPDATE = "ForceCellValueUpdate";

	/**
	 * Fired when an {@link EventInputPort} as been updated by the user
	 */
	public static final String IN_EVENT_VALUE_UPDATED = "InEventValueUpdated";

	/**
	 * Fired when an
	 * {@link org.scilab.modules.xcos.port.input.ExplicitInputPort} as been
	 * updated by the user
	 */
	public static final String IN_EXPLICIT_VALUE_UPDATED = "InExplicitValueUpdated";
	/**
	 * Fired when an
	 * {@link org.scilab.modules.xcos.port.input.ImplicitInputPort} as been
	 * updated by the user
	 */
	public static final String IN_IMPLICIT_VALUE_UPDATED = "InImplicitValueUpdated";
	/**
	 * Fired when an {@link EventOutputPort} as been updated by the user
	 */
	public static final String OUT_EVENT_VALUE_UPDATED = "OutEventValueUpdated";

	/**
	 * Fired when an
	 * {@link org.scilab.modules.xcos.port.output.ExplicitOutputPort} as been
	 * updated by the user
	 */
	public static final String OUT_EXPLICIT_VALUE_UPDATED = "OutExplicitValueUpdated";
	/**
	 * Fired when an
	 * {@link org.scilab.modules.xcos.port.output.ImplicitOutputPort} as been
	 * updated by the user
	 */
	public static final String OUT_IMPLICIT_VALUE_UPDATED = "OutImplicitValueUpdated";
	/**
	 * Fired when an {@link org.scilab.modules.xcos.block.SuperBlock} as been
	 * updated by the user
	 */
	public static final String SUPER_BLOCK_UPDATED = "superBlockUpdated";

	/** This class is a static singleton, thus it must not be instantiated */
	private XcosEvent() { }
}
