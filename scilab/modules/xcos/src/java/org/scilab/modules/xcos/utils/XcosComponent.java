package org.scilab.modules.xcos.utils;

import com.mxgraph.swing.mxGraphComponent;
import com.mxgraph.view.mxGraph;

public class XcosComponent extends mxGraphComponent{

	private static final long serialVersionUID = 1592816691330208090L;

	public XcosComponent(mxGraph graph) {
		super(graph);
		// TODO Auto-generated constructor stub
	}

	public XcosCanvas createCanvas()
	{
		return new XcosCanvas();
	}

}
