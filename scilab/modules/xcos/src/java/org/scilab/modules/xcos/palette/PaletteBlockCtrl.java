/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.palette;

import java.awt.Point;
import java.awt.datatransfer.Transferable;
import java.awt.dnd.DnDConstants;
import java.awt.dnd.DragGestureEvent;
import java.awt.dnd.DragGestureListener;
import java.awt.dnd.DragSource;
import java.awt.dnd.InvalidDnDOperationException;
import java.awt.event.MouseListener;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BlockFactory;
import org.scilab.modules.xcos.block.BlockFactory.BlockInterFunction;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.H5RWHandler;
import org.scilab.modules.xcos.palette.listener.PaletteBlockMouseListener;
import org.scilab.modules.xcos.palette.model.PaletteBlock;
import org.scilab.modules.xcos.palette.view.PaletteBlockView;
import org.scilab.modules.xcos.palette.view.PaletteManagerView;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.swing.handler.mxGraphTransferHandler;
import com.mxgraph.util.mxConstants;

/**
 * A palette block is the representation of the block in the palette. All the
 * operations there are used to render, load and put (on a diagram) a block.
 */
public final class PaletteBlockCtrl {

	private static final MouseListener MOUSE_LISTENER = new PaletteBlockMouseListener();
	private static final Log LOG = LogFactory.getLog(PaletteBlockCtrl.class);
	
	private static final String UNABLE_TO_LOAD_BLOCK = Messages.gettext("Unable to load block from %s .");
	private static final String LOADING_THE_BLOCK = Messages.gettext("Loading the block") + XcosMessages.DOTS;
	
	/**
	 * Internal graph used to render each block.
	 */
	public static final XcosDiagram INTERNAL_GRAPH;
	static {
		INTERNAL_GRAPH = new XcosDiagram();
		INTERNAL_GRAPH.installListeners();
	}
	
	private static PaletteBlockCtrl previouslySelected;
	
	private final PaletteBlock model;
	private final PaletteBlockView view;
	
	private Transferable transferable;
	
	/**
	 * Default constructor
	 * @param model the block data
	 */
	public PaletteBlockCtrl(PaletteBlock model) {
		this.model = model;
		this.view = new PaletteBlockView(this);
		installListeners(this.view);
	}
	
	/**
	 * @param view The view to setup
	 */
	private void installListeners(PaletteBlockView view) {
		view.addMouseListener(MOUSE_LISTENER);
		installDnd();
	}

	/**
	 * @return the view
	 */
	public PaletteBlockView getView() {
		return view;
	}
	
	/**
	 * @return the model
	 */
	public PaletteBlock getModel() {
		return model;
	}
	
	
	/**
	 * This function is the only access to get the block.
	 * @return the transferable object
	 */
	public Transferable getTransferable() {
		if (transferable == null) {
			/* Load the block from the H5 file */
			BasicBlock block = loadBlock();
			if (block == null) {
				if (LOG.isInfoEnabled()) {
					LOG.info(String.format(UNABLE_TO_LOAD_BLOCK,
							getModel().getData().getEvaluatedPath()));
				}
				getView().setEnabled(false);
				return null;
			}
			getView().setEnabled(true);
			
			/* Render it and export it */
			INTERNAL_GRAPH.addCell(block);
			INTERNAL_GRAPH.selectAll();
			
			INTERNAL_GRAPH.updateCellSize(block);
			
			mxGraphTransferHandler handler = ((mxGraphTransferHandler) INTERNAL_GRAPH
					.getAsComponent().getTransferHandler());
			transferable = handler.createTransferable(INTERNAL_GRAPH.getAsComponent());
			
			INTERNAL_GRAPH.removeCells();
		}
		return transferable;
	}

	/**
	 * @return the loaded block.
	 */
	public BasicBlock loadBlock() {
		BasicBlock block;
		if (model.getName().compareTo("TEXT_f") != 0) {
			// Load the block from the file
			String realPath = model.getData().getEvaluatedPath(); 
			block = new H5RWHandler(realPath).readBlock();
			
			// invalid block case
			if (block == null) {
				return null;
			}
			
			if (block.getStyle().compareTo("") == 0) {
				block.setStyle(block.getInterfaceFunctionName());
			}
		} else {
			block = BlockFactory.createBlock(BlockInterFunction.TEXT_f);
		}
		return block;
	}
	
	/**
	 * @return true if it is selected, false otherwise
	 */
	public boolean isSelected() {
		return this == previouslySelected;
	}

	/**
	 * @param selected the selected state to set
	 */
	public void setSelected(boolean selected) {
		if (selected) {
			if (previouslySelected != null) {
				previouslySelected.setSelected(false);
			}
			previouslySelected = this;
		}
		getView().setSelectedUI(selected);
	}

	/**
	 * Install the Drag'n'Drop on this instance.
	 */
	public void installDnd() {
		// Install the handler for dragging nodes into a graph
		DragGestureListener dragGestureListener = new DragGestureListener() {
			public void dragGestureRecognized(DragGestureEvent e) {
				final PaletteManagerView winView = PaletteManager.getInstance()
						.getView();
				final DragGestureEvent event = e;
				final String msg = String.format(UNABLE_TO_LOAD_BLOCK,
						getModel().getData().getEvaluatedPath());

				winView.setInfo(LOADING_THE_BLOCK);
				try {
					Transferable transfer = getTransferable();

					if (transfer != null) {
						event.startDrag(null, mxConstants.EMPTY_IMAGE, new Point(),
								transfer, null);
					} else {
						throw new NullPointerException();
					}
				} catch (InvalidDnDOperationException exception) {
					ScilabModalDialog.show(winView, msg,
							XcosMessages.XCOS_ERROR, IconType.ERROR_ICON);
				} finally {
					winView.setInfo(XcosMessages.EMPTY_INFO);
				}
			}

		};

		DragSource dragSource = DragSource.getDefaultDragSource();
		dragSource.createDefaultDragGestureRecognizer(this.getView(),
				DnDConstants.ACTION_COPY, dragGestureListener);
	}

}
