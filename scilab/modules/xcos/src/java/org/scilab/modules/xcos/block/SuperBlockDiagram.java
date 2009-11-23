package org.scilab.modules.xcos.block;

import java.io.Serializable;

import javax.swing.JOptionPane;

import org.scilab.modules.gui.utils.UIElementMapper;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.XcosDiagram;
import org.scilab.modules.xcos.io.BlockWriter;
import org.scilab.modules.xcos.utils.XcosEvent;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.util.mxEventObject;

public class SuperBlockDiagram extends XcosDiagram implements Serializable {

    private SuperBlock container = null;

    public SuperBlockDiagram() {
	super();
    }

    public SuperBlockDiagram(SuperBlock superBlock) {
	super();
	this.container = superBlock;
    }

    public SuperBlock getContainer() {
	return container;
    }

    public void setContainer(SuperBlock container) {
	this.container = container;
    }

    public String[] buildEntireContext() {
	
	String[] parentContext = getContainer().getParentDiagram().buildEntireContext();
	String []entireContext = new String[getContext().length + parentContext.length];
	
	for (int i = 0 ; i < parentContext.length ; ++i) {
	    entireContext[i] = parentContext[i];
	}

	for (int i = 0 ; i < getContext().length ; ++i) {
	    entireContext[i + parentContext.length] = getContext()[i];
	}
	
	return entireContext;
    }
    
    public void closeDiagram() {
	getContainer().closeBlockSettings();
    }

    public class GenericSuperBlockListener implements mxIEventListener {
	public void invoke(Object arg0, mxEventObject arg1) {
	    getContainer().updateAllBlocksColor();
	    getContainer().updateExportedPort();	    
	}	
    }
    
    public void installSuperBlockListeners() {
	addListener(XcosEvent.CELLS_ADDED, new GenericSuperBlockListener());

	addListener(XcosEvent.CELLS_REMOVED, new GenericSuperBlockListener());

	addListener(XcosEvent.IN_EXPLICIT_VALUE_UPDATED, new GenericSuperBlockListener());

	addListener(XcosEvent.IN_IMPLICIT_VALUE_UPDATED, new GenericSuperBlockListener());

	addListener(XcosEvent.IN_EVENT_VALUE_UPDATED, new GenericSuperBlockListener());

	addListener(XcosEvent.OUT_EXPLICIT_VALUE_UPDATED, new GenericSuperBlockListener());

	addListener(XcosEvent.OUT_IMPLICIT_VALUE_UPDATED, new GenericSuperBlockListener());

	addListener(XcosEvent.OUT_EVENT_VALUE_UPDATED, new GenericSuperBlockListener());
    }

    public void setModified(boolean modified) {
        super.setModified(modified);
        if (getContainer() != null &&
        	getContainer().getParentDiagram() != null) {
            getContainer().getParentDiagram().setModified(modified);
        }
    }
    
}
