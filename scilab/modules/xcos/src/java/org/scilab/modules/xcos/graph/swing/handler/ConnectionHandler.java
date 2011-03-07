/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2010 - DIGITEO - Clément DAVID <clement.david@scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.graph.swing.handler;

import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.List;

import org.scilab.modules.xcos.graph.swing.GraphComponent;

import com.mxgraph.model.mxICell;
import com.mxgraph.model.mxIGraphModel;
import com.mxgraph.swing.handler.mxConnectPreview;
import com.mxgraph.swing.handler.mxConnectionHandler;
import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxPoint;
import com.mxgraph.view.mxCellState;
import com.mxgraph.view.mxGraph;

/**
 * Connection handler used to handle multi point links.
 * 
 * @see <a
 *      href="http://www.jgraph.org/bugzilla/show_bug.cgi?id=20">http://www.jgraph.org/bugzilla/show_bug.cgi?id=20</a>
 */
public class ConnectionHandler extends mxConnectionHandler {
	private boolean multiPointLinkStarted;

	/**
	 * Default constructor.
	 * 
	 * @param graphComponent
	 *            the component
	 */
	public ConnectionHandler(GraphComponent graphComponent) {
		super(graphComponent);
	}

	/**
	 * @return {@link ConnectPreview} instance
	 * @see com.mxgraph.swing.handler.mxConnectionHandler#createConnectPreview()
	 */
	@Override
	protected mxConnectPreview createConnectPreview() {
		return new ConnectPreview((GraphComponent) graphComponent);
	}

	/**
	 * Enable or disable the reset handler which reset any action on graph
	 * modification.
	 * 
	 * @param status the enable status
	 */
	protected void setResetEnable(boolean status) {
		final mxIGraphModel model = graphComponent.getGraph().getModel();
		
		if (status) {
			model.addListener(mxEvent.CHANGE, resetHandler);
		} else {
			model.removeListener(resetHandler, mxEvent.CHANGE);
		}
	}
	
	
	/*
	 * mxMouseAdapter specific reimplementation
	 */

	/**
	 * Handle first release and click on the empty background during connection.
	 * 
	 * @param e
	 *            the event
	 * @see com.mxgraph.swing.handler.mxConnectionHandler#mouseReleased(java.awt.event.MouseEvent)
	 */
	@Override
	public void mouseReleased(MouseEvent e) {
		if (error != null && error.isEmpty() && !e.isConsumed()
				&& first != null && connectPreview.isActive()
				&& !marker.hasValidState()) {
			final mxGraph graph = graphComponent.getGraph();
			final double x = graph.snap(e.getX());
			final double y = graph.snap(e.getY());

			// we are during a link creation on an invalid area
			mxICell cell = (mxICell) connectPreview.getPreviewState().getCell();

			// allocate points if applicable
			List<mxPoint> points = cell.getGeometry().getPoints();
			if (points == null) {
				points = new ArrayList<mxPoint>();
				cell.getGeometry().setPoints(points);
			}
			
			// scale and set the point
			// extracted from mxConnectPreview#transformScreenPoint
			{
				final mxPoint tr = graph.getView().getTranslate();
				final double scale = graph.getView().getScale();
				points.add(new mxPoint(x / scale - tr.getX(), y / scale - tr.getY()));
			}

			// update the preview and set the flag
			connectPreview.update(e, null, x, y);
			multiPointLinkStarted = true;

			e.consume();
		} else {
			multiPointLinkStarted = false;
			super.mouseReleased(e);
		}
	}

	/**
	 * Only chain up when the multi point link feature is disable, drag
	 * otherwise.
	 * 
	 * @param e
	 *            the event
	 * @see com.mxgraph.swing.handler.mxConnectionHandler#mouseMoved(java.awt.event.MouseEvent)
	 */
	@Override
	public void mouseMoved(MouseEvent e) {
		if (multiPointLinkStarted) {
			mouseDragged(e);
		} else {
			super.mouseMoved(e);
		}
	}
	
	/**
	 * Only chain up when multi point link feature is disable.
	 * 
	 * This will not update the first point on multi point link creation.
	 * 
	 * @param e the mouse event
	 * @param state the marker valid state
	 * @see com.mxgraph.swing.handler.mxConnectionHandler#start(java.awt.event.MouseEvent, com.mxgraph.view.mxCellState)
	 */
	@Override
	public void start(MouseEvent e, mxCellState state) {
		if (!multiPointLinkStarted) {
			super.start(e, state);
		}
	}
}
