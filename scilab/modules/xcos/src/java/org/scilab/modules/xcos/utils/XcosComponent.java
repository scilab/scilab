package org.scilab.modules.xcos.utils;

import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;

import org.scilab.modules.xcos.graph.PaletteDiagram;

import com.mxgraph.swing.mxGraphComponent;
import com.mxgraph.view.mxGraph;

public class XcosComponent extends mxGraphComponent{

    private static final long serialVersionUID = 1592816691330208090L;

    public XcosComponent(mxGraph graph) {
	super(graph);
	
	addComponentListener(new ComponentListener() {
	    
	    public void componentShown(ComponentEvent arg0) {
		if(getGraph() instanceof PaletteDiagram) {
		    ((PaletteDiagram)getGraph()).updateDiagram(getSize().getWidth());
		}		
	    }
	    
	    public void componentResized(ComponentEvent arg0) {
		if(getGraph() instanceof PaletteDiagram) {
		    ((PaletteDiagram)getGraph()).updateDiagram(getSize().getWidth());
		}		
	    }
	    
	    public void componentMoved(ComponentEvent arg0) {
	    }
	    
	    public void componentHidden(ComponentEvent arg0) {
	    }
	});
    }

    public XcosCanvas createCanvas()
    {
	return new XcosCanvas();
    }
    
    public String toString() {
	if(getGraph() instanceof PaletteDiagram) {
	    String fileName = ((PaletteDiagram)getGraph()).getName();
	    return fileName.substring(0, fileName.lastIndexOf('.'));
	} else {
	    return super.toString();
	}
    }
}
