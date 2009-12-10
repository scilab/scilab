package org.scilab.modules.xcos.block;

import java.io.File;
import java.io.IOException;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.xcos.io.BlockReader;
import org.scilab.modules.xcos.utils.Signal;

public abstract class ContextUpdate extends BasicBlock{

    private static final long serialVersionUID = 6076826729067963560L;
    private static Object _mutex_ = new Object();
    
    /**
     * This enum represent all the subclasses of ContextUpdate .
     *
     * It is used to easily loop over a BasicBlock I/O blocks
     */
    public enum IOBlocks {
	EventInBlock(EventInBlock.class),
	EventOutBlock(EventOutBlock.class),
	ExplicitInBlock(ExplicitInBlock.class),
	ExplicitOutBlock(ExplicitOutBlock.class),
	ImplicitInBlock(ImplicitInBlock.class),
	ImplicitOutBlock(ImplicitOutBlock.class),
	Unknow(ContextUpdate.class);
	
	private Class<? extends ContextUpdate> klass;
	
	private IOBlocks(Class<? extends ContextUpdate> klass) {
	    this.klass = klass;
	}
	
	public Class<? extends ContextUpdate> getReferencedClass() {
	    return klass;
	}
    }
    
    public ContextUpdate() {
	super();
    }

    public ContextUpdate(String label) {
	super(label);
    }

    public void onContextChange(String[] context) {
	//prevent to open twice
	if(isLocked()) {
	    return;
	}
	
	final File tempOutput;
	final File tempInput;
	final File tempContext;
	try {
	    tempInput = File.createTempFile("xcos",".h5");
	    tempInput.deleteOnExit();

	    // Write scs_m
	    tempOutput = exportBlockStruct();
	    // Write context
	    tempContext = exportContext(context);

	    String cmd;
	    
	    cmd = "xcosBlockEval(\""+tempOutput.getAbsolutePath()+"\"";
	    cmd += ", \""+tempInput.getAbsolutePath()+"\"";
	    cmd += ", "+getInterfaceFunctionName();
	    cmd += ", \""+tempContext.getAbsolutePath()+"\");";

	    synchronized (_mutex_) {
		InterpreterManagement.putCommandInScilabQueue(cmd);
		Signal.wait(tempInput.getAbsolutePath());
		BasicBlock modifiedBlock = BlockReader.readBlockFromFile(tempInput.getAbsolutePath());
		updateBlockSettings(modifiedBlock);
	    }
	    
	} catch (IOException e) {
	    e.printStackTrace();
	}
    }
}
