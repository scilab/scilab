/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.graph;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.File;
import java.io.IOException;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.xcos.utils.FileUtils;

import com.mxgraph.util.mxEventObject;
import com.mxgraph.util.mxEventSource.mxIEventListener;

/**
 * Contains the current Scicos engine status.
 */
public class CompilationEngineStatus implements mxIEventListener, PropertyChangeListener {
	private static final Log LOG = LogFactory.getLog(CompilationEngineStatus.class);
	
	private boolean compilationNeeded;
	private File compilationData;
	
	/**
	 * Default constructor.
	 */
	public CompilationEngineStatus() {
		setCompilationNeeded(true);
	}

	/**
	 * @param status
	 *            true, when the associated diagram need to be compiled, false
	 *            otherwise.
	 */
	public void setCompilationNeeded(boolean status) {
		compilationNeeded = status;
	}

	/**
	 * @return always true as we don't use scicos internal modification checking
	 */
	public boolean isCompilationNeeded() {
		return true;
	}

	/**
	 * @param compilationData the source and compiled diagram data
	 */
	public void setCompilationData(File compilationData) {
		this.compilationData = compilationData;
		
		/*
		 * When the compiled data change, we remove the previous stored data.
		 */
		if (compilationData != null) {
			boolean status = compilationData.delete();
			if (!status) {
				LOG.warn("Unable to delete temp file");
			}
		}
	}

	/**
	 * @return the compilationData the source and compiled diagram data
	 */
	public File getCompilationData() {
		return compilationData;
	}
	
	/**
	 * Get the command used to store the simulated data out of the stack.
	 * @return the scilab command
	 */
	public String getStoreSimulationDataCommand() {
		final StringBuilder command = new StringBuilder();
		
		/*
		 * Create a data file if needed
		 */
		if (getCompilationData() == null || !getCompilationData().exists()) {
			try {
				setCompilationData(FileUtils.createTempFile());
			} catch (IOException e) {
				LOG.warn(e);
				
				/*
				 * Restart compilation next time
				 */
				setCompilationNeeded(true);
			}
		}
		
		/*
		 * Create the commands
		 */
		command.append("path='" + getCompilationData().getAbsolutePath() + "'; ");
		command.append("if and([exists('%cpr') exists('scs_m')]) <> %t then ");
		command.append("  deletefile(path);");
		command.append("else ");
		command.append("  export_to_hdf5(path, '%cpr', 'scs_m'); ");
		command.append("end ");
		
		return command.toString();
	}
	
	/**
	 * Get the command used to load the simulated data into the stack.
	 * @return a Scilab command
	 * throws IllegalStateException when the file cannot be loaded.
	 */
	public String getLoadSimulationDataCommand() {
		final StringBuilder command = new StringBuilder();
		
		/*
		 * Check state
		 */
		if (getCompilationData() == null || !getCompilationData().exists()) {
			throw new IllegalStateException("compilation data doesn't exist.");
		}
		
		/*
		 * Create the commands
		 */
		command.append("path = '" + getCompilationData().getAbsolutePath() + "' ; ");
		command.append("import_from_hdf5(path); ");
		
		return command.toString();
	}

	/*
	 * Property change listener
	 */
	
	/**
	 * Listener used for any interesting diagram change. 
	 * 
	 * @param sender the associated diagram
	 * @param evt the current event.
	 * @see com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object, com.mxgraph.util.mxEventObject)
	 */
	@Override
	public void invoke(Object sender, mxEventObject evt) {
		setCompilationNeeded(true);
	}

	/**
	 * Property change listener used to update compilation status when the
	 * context has changed.
	 * 
	 * @param evt the current event
	 * @see java.beans.PropertyChangeListener#propertyChange(java.beans.PropertyChangeEvent)
	 */
	@Override
	public void propertyChange(PropertyChangeEvent evt) {
		setCompilationNeeded(true);
	}
}
