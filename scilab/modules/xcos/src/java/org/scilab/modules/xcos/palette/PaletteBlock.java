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
import java.awt.event.MouseListener;

import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BlockFactory;
import org.scilab.modules.xcos.block.BlockFactory.BlockInterFunction;
import org.scilab.modules.xcos.io.BlockReader;
import org.scilab.modules.xcos.palette.listener.PaletteBlockMouseListener;
import org.scilab.modules.xcos.palette.model.PaletteBlockModel;
import org.scilab.modules.xcos.palette.view.PaletteBlockView;
import org.scilab.modules.xcos.utils.BlockPositioning;

import com.mxgraph.swing.util.mxGraphTransferable;
import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxRectangle;

/**
 * A palette block is the representation of the block in the palette. All the
 * operations there are used to render, load and put (on a diagram) a block.
 */
public final class PaletteBlock {

	private static final double DEFAULT_POSITION = 10.0;
	private static final String BLOCK_PATH = System.getenv("SCI")
			+ "/modules/scicos_blocks/blocks/";
	private static final MouseListener MOUSE_LISTENER = new PaletteBlockMouseListener();
	private static PaletteBlock previouslySelected;
	
	
	private PaletteBlockModel model;
	private PaletteBlockView view;
	
	private mxGraphTransferable transferable;
	
	/**
	 * Default constructor
	 * @param blockName the block name
	 */
	protected PaletteBlock(String blockName) {
		this.model = new PaletteBlockModel(blockName);
		this.view = new PaletteBlockView(this);
		installListeners(this.view);
	}
	
	/**
	 * @param view The view to setup
	 */
	private void installListeners(PaletteBlockView view) {
		view.addMouseListener(MOUSE_LISTENER);
	}

	/**
	 * @return the model
	 */
	public PaletteBlockModel getModel() {
		return model;
	}

	/**
	 * @return the view
	 */
	public PaletteBlockView getView() {
		return view;
	}
	
	/**
	 * This function is the only access to get the block.
	 * @return the transferable object
	 */
	public mxGraphTransferable getTransferable() {
		if (transferable == null) {
			BasicBlock block = loadBlock();
			block.getGeometry().setX(DEFAULT_POSITION);
			block.getGeometry().setY(DEFAULT_POSITION);
			BlockPositioning.updateBlockView(block);
			new mxGraphTransferable(new Object[] {block}, (mxRectangle) block
					.getGeometry().clone());
		}
		return transferable;
	}

	/**
	 * @return the loaded block.
	 */
	private BasicBlock loadBlock() {
		BasicBlock block;
		if (getModel().getName().compareTo("TEXT_f") != 0) {
			// Load the block from the file
			block = BlockReader.readBlockFromFile(BLOCK_PATH
					+ getModel().getName() + ".h5");

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
				e.startDrag(null, mxConstants.EMPTY_IMAGE, new Point(),
						getTransferable(), null);
			}

		};

		DragSource dragSource = new DragSource();
		dragSource.createDefaultDragGestureRecognizer(this.getView(),
				DnDConstants.ACTION_COPY, dragGestureListener);
	}

}
