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
	String []entireContext = new String[getContext().length + container.getParentDiagram().getContext().length];
	for (int i = 0 ; i < container.getParentDiagram().getContext().length ; ++i) {
	    entireContext[i] = container.getParentDiagram().getContext()[i];
	}
	for (int i = 0 ; i < getContext().length ; ++i) {
	    entireContext[i + container.getParentDiagram().getContext().length] = getContext()[i];
	}
	
	return entireContext;
    }
    
    public void closeDiagram() {

    	// Do not ask the user, the diagram is saved and closed
    	
    	if (isModified()) {
    		container.setRealParameters(BlockWriter.convertDiagramToMList(this));
        	container.getParentDiagram().setModified(true);
    	}

    	ScilabWindow xcosWindow = (ScilabWindow) UIElementMapper.getCorrespondingUIElement(getParentTab().getParentWindowId());
    	xcosWindow.removeTab(getParentTab());
    	getViewPort().close();
    	Xcos.closeDiagram(this);
    	this.removeListener(null);
    	container.setLocked(false);
    }

    public void installSuperBlockListeners() {
	addListener(XcosEvent.CELLS_ADDED, new mxIEventListener() {
	    public void invoke(Object source, mxEventObject evt) {
		getContainer().updateAllBlocksColor();
		getContainer().updateExportedPort();
	    }
	});

	addListener(XcosEvent.CELLS_REMOVED, new mxIEventListener() {
	    public void invoke(Object source, mxEventObject evt) {
		getContainer().updateAllBlocksColor();
		getContainer().updateExportedPort();
	    }
	});

	addListener(XcosEvent.IN_EXPLICIT_VALUE_UPDATED, new mxIEventListener() {
	    public void invoke(Object source, mxEventObject evt) {
		getContainer().updateAllBlocksColor();
		getContainer().updateExportedPort();
	    }
	});

	addListener(XcosEvent.IN_IMPLICIT_VALUE_UPDATED, new mxIEventListener() {
	    public void invoke(Object source, mxEventObject evt) {
		getContainer().updateAllBlocksColor();
		getContainer().updateExportedPort();
	    }
	});

	addListener(XcosEvent.IN_EVENT_VALUE_UPDATED, new mxIEventListener() {
	    public void invoke(Object source, mxEventObject evt) {
		getContainer().updateAllBlocksColor();
		getContainer().updateExportedPort();
	    }
	});

	addListener(XcosEvent.OUT_EXPLICIT_VALUE_UPDATED, new mxIEventListener() {
	    public void invoke(Object source, mxEventObject evt) {
		getContainer().updateAllBlocksColor();
		getContainer().updateExportedPort();
	    }
	});

	addListener(XcosEvent.OUT_IMPLICIT_VALUE_UPDATED, new mxIEventListener() {
	    public void invoke(Object source, mxEventObject evt) {
		getContainer().updateAllBlocksColor();
		getContainer().updateExportedPort();
	    }
	});

	addListener(XcosEvent.OUT_EVENT_VALUE_UPDATED, new mxIEventListener() {
	    public void invoke(Object source, mxEventObject evt) {
		getContainer().updateAllBlocksColor();
		getContainer().updateExportedPort();
	    }
	});
    }

}
