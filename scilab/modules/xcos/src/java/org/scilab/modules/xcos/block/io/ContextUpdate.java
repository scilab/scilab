/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.block.io;

import java.io.File;
import java.io.IOException;

import org.scilab.modules.graph.utils.ScilabInterpreterManagement;
import org.scilab.modules.graph.utils.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.types.scilabTypes.ScilabDouble;
import org.scilab.modules.types.scilabTypes.ScilabList;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.io.scicos.H5RWHandler;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.scilab.modules.xcos.utils.FileUtils;

/**
 * @author Clement DAVID
 *
 */
public abstract class ContextUpdate extends BasicBlock {

    private static final long serialVersionUID = 6076826729067963560L;
    
    /**
     * This enum represent all the subclasses of ContextUpdate .
     *
     * It is used to easily loop over a BasicBlock I/O blocks
     */
    public enum IOBlocks {
	EventInBlock(EventInBlock.class, ControlPort.class),
	EventOutBlock(EventOutBlock.class, CommandPort.class),
	ExplicitInBlock(ExplicitInBlock.class, ExplicitInputPort.class),
	ExplicitOutBlock(ExplicitOutBlock.class, ExplicitOutputPort.class),
	ImplicitInBlock(ImplicitInBlock.class, ImplicitInputPort.class),
	ImplicitOutBlock(ImplicitOutBlock.class, ImplicitOutputPort.class);
	
	private final Class< ? extends ContextUpdate> ioBlock;
	private final Class< ? extends BasicPort> port;
	
	/**
	 * @param ioBlock input/output block
	 * @param port the associated port class
	 */
	private IOBlocks(Class< ? extends ContextUpdate> ioBlock, Class< ? extends BasicPort> port) {
	    this.ioBlock = ioBlock;
	    this.port = port;
	}
	
	/**
	 * @return referenced class
	 */
	public Class< ? extends ContextUpdate> getReferencedClass() {
	    return ioBlock;
	}
	
	/**
	 * @return the port referenced class
	 */
	public Class< ? extends BasicPort> getReferencedPortClass() {
	    return port;
	}
	
    }
    
	/**
	 * Constructor
	 */
	public ContextUpdate() {
		super();
	}

	/**
	 * @param label block labek
	 */
	protected ContextUpdate(String label) {
		this();
		setValue(label);
	}

	/**
	 * Initialize the block with the default values
	 */
	@Override
	protected void setDefaultValues() {
		super.setDefaultValues();
		setNbZerosCrossing(new ScilabDouble(0));
		setNmode(new ScilabDouble(0));
		setODState(new ScilabList());
		setValue(1);
	}
    
    /**
     * @param context new context
     */
    public void onContextChange(String[] context) {
	//prevent to open twice
	if (isLocked()) {
	    return;
	}
	
	final File tempOutput;
	final File tempInput;
	final File tempContext;
	try {
	    tempInput = FileUtils.createTempFile();
	    tempInput.deleteOnExit();

	    // Write scs_m
	    tempOutput = exportBlockStruct();
	    // Write context
	    tempContext = exportContext(context);

	    String cmd = ScilabInterpreterManagement.buildCall("xcosBlockEval", 
	    		tempOutput.getAbsolutePath(),
	    		tempInput.getAbsolutePath(),
	    		getInterfaceFunctionName(),
	    		tempContext.getAbsolutePath());

		try {
			ScilabInterpreterManagement.synchronousScilabExec(cmd);
		} catch (InterpreterException e) {
			e.printStackTrace();
		}
		BasicBlock modifiedBlock = new H5RWHandler(tempInput).readBlock();
		updateBlockSettings(modifiedBlock);
	    
	} catch (IOException e) {
	    e.printStackTrace();
	}
    }
}
