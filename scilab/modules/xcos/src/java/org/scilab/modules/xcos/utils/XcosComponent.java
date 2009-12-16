package org.scilab.modules.xcos.utils;

import java.awt.Dimension;
import java.awt.Rectangle;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;

import org.scilab.modules.xcos.graph.PaletteDiagram;

import com.mxgraph.swing.mxGraphComponent;
import com.mxgraph.util.mxRectangle;
import com.mxgraph.view.mxGraph;
import com.mxgraph.view.mxGraphView;

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
    
    /**
     * Zoom the whole graph and center the view on it.
     */
    public void zoomAndCenterToCells() {
	mxGraphView view = graph.getView();
	Rectangle preference = (Rectangle) view.getBounds(graph.getChildCells(graph.getDefaultParent())).getRectangle().clone();
	
	// Feeling values to keep the zoom smart 
	preference.x -= 20;
	preference.y += 20;
	preference.width += 40;
	preference.height += 40;
	
	Dimension actual = viewport.getSize();

	double newScale = 1.0;
	double heightScale = actual.getHeight() / preference.getHeight();
	double widthScale = actual.getWidth() / preference.getWidth();
	
	if (heightScale > 1.0) {
	    if (widthScale > 1.0) {
		// We need to zoom in (the max applicable zoom is the lowest)
		newScale = Math.min(heightScale, widthScale);
	    } else {
		// we need to zoom out (only widthScale is < 1.0)
		newScale = widthScale;
	    }
	} else {
	    if (widthScale > 1.0) {
		// we need to zoom out (only heightScale is < 1.0)
		newScale = heightScale;
	    } else {
		// We need to zoom out (the max applicable zoom is the lowest)
		newScale = Math.min(heightScale, widthScale);
	    }
	}

	zoom(newScale);
	
	view.revalidate();
	Rectangle orig = view.getBounds(graph.getChildCells(graph.getDefaultParent())).getRectangle();
	
	getGraphControl().scrollRectToVisible(orig);
    }
}
