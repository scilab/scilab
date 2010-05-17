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
import java.awt.dnd.DnDConstants;
import java.awt.dnd.DragGestureEvent;
import java.awt.dnd.DragGestureListener;
import java.awt.dnd.DragSource;
import java.awt.dnd.InvalidDnDOperationException;
import java.awt.event.MouseListener;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BlockFactory;
import org.scilab.modules.xcos.block.BlockFactory.BlockInterFunction;
import org.scilab.modules.xcos.io.scicos.H5RWHandler;
import org.scilab.modules.xcos.palette.listener.PaletteBlockMouseListener;
import org.scilab.modules.xcos.palette.model.PaletteBlock;
import org.scilab.modules.xcos.palette.view.PaletteBlockView;
import org.scilab.modules.xcos.utils.BlockPositioning;

import com.mxgraph.swing.util.mxGraphTransferable;
import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxRectangle;

/**
 * A palette block is the representation of the block in the palette. All the
 * operations there are used to render, load and put (on a diagram) a block.
 */
public final class PaletteBlockCtrl {

	private static final MouseListener MOUSE_LISTENER = new PaletteBlockMouseListener();
	private static PaletteBlockCtrl previouslySelected;
	
	private final PaletteBlock model;
	private final PaletteBlockView view;
	
	private mxGraphTransferable transferable;
	
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
	public mxGraphTransferable getTransferable() {
		if (transferable == null) {
			BasicBlock block = loadBlock();
			BlockPositioning.updateBlockView(block);
			transferable = new mxGraphTransferable(new Object[] {block},
					(mxRectangle) block.getGeometry().clone());
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

			if (block.getStyle().compareTo("") == 0) {
				block.setStyle(block.getInterfaceFunctionName());
				block.setValue(block.getInterfaceFunctionName());
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
				try {
				e.startDrag(null, mxConstants.EMPTY_IMAGE, new Point(),
						getTransferable(), null);
				} catch (InvalidDnDOperationException exception) {
					LogFactory.getLog(PaletteBlockCtrl.class)
						.warn(exception);
				}
			}

		};

		DragSource dragSource = new DragSource();
		dragSource.createDefaultDragGestureRecognizer(this.getView(),
				DnDConstants.ACTION_COPY, dragGestureListener);
	}

}
