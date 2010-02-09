/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 * Copyright (C) 2009 - DIGITEO - Clément DAVID
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.block.actions;

import java.awt.event.ActionEvent;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.EnumMap;
import java.util.List;
import java.util.Map;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.VertexSelectionDependantAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;
import org.scilab.modules.hdf5.scilabTypes.ScilabList;
import org.scilab.modules.hdf5.scilabTypes.ScilabString;
import org.scilab.modules.xcos.XcosUIDObject;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BlockFactory;
import org.scilab.modules.xcos.block.SplitBlock;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.block.io.ContextUpdate;
import org.scilab.modules.xcos.block.io.EventInBlock;
import org.scilab.modules.xcos.block.io.EventOutBlock;
import org.scilab.modules.xcos.block.io.ExplicitInBlock;
import org.scilab.modules.xcos.block.io.ExplicitOutBlock;
import org.scilab.modules.xcos.block.io.ImplicitInBlock;
import org.scilab.modules.xcos.block.io.ImplicitOutBlock;
import org.scilab.modules.xcos.block.io.ContextUpdate.IOBlocks;
import org.scilab.modules.xcos.graph.SuperBlockDiagram;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.BasicBlockInfo;
import org.scilab.modules.xcos.io.BlockWriter;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.link.commandcontrol.CommandControlLink;
import org.scilab.modules.xcos.link.explicit.ExplicitLink;
import org.scilab.modules.xcos.link.implicit.ImplicitLink;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.scilab.modules.xcos.utils.BlockPositioning;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxGeometry;

/**
 * Transform selection to a new superblock.
 */
public final class RegionToSuperblockAction extends VertexSelectionDependantAction {
	public static final String NAME = XcosMessages.REGION_TO_SUPERBLOCK;
	public static final String SMALL_ICON = "";
	public static final int MNEMONIC_KEY = 0;
	public static final int ACCELERATOR_KEY = 0;
	
    /**
     * @author Antoine ELIAS
     *
     */
    private class BrokenLink {
	private BasicLink link;
	private BasicPort port;
	private mxGeometry geom;
	private boolean outGoing;
	private int portNumber;

	/**
	 * @param link broken link
	 * @param port seleted port of broken link
	 * @param geom original geometry
	 * @param outGoing direction
	 */
	public BrokenLink(BasicLink link, BasicPort port, mxGeometry geom, boolean outGoing) {
	    this.link = link;
	    this.port = port;
	    this.outGoing = outGoing;
	    this.geom = geom;
	}

	/**
	 * @return link redirection from port
	 */
	public boolean getOutGoing() {
	    return outGoing;
	}

	/**
	 * @return broken link
	 */
	public BasicLink getLink() {
	    return link;
	}
	
	/**
	 * @return selected port
	 */
	public BasicPort getPort() {
	    return port;
	}

	/**
	 * @return broken link geometry
	 */
	public mxGeometry getGeometry() {
	    return geom;
	}

	/**
	 * @param portNumber set port number
	 */
	public void setPortNumber(int portNumber) {
	    this.portNumber = portNumber;
	}

	/**
	 * @return get port number
	 */
	public int getPortNumber() {
	    return portNumber;
	}
    }

    /**
     * Default constructor
     * @param scilabGraph graph
     */
    public RegionToSuperblockAction(ScilabGraph scilabGraph) {
	super(scilabGraph);
    }

    /**
     * @param scilabGraph graph
     * @return menu item
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
	return createMenu(scilabGraph, RegionToSuperblockAction.class);
    }

	/**
	 * @param e parameter
	 * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
	 */
	@Override
	public void actionPerformed(ActionEvent e) {

	XcosDiagram graph = (XcosDiagram) getGraph(null);
	graph.info(XcosMessages.GENERATE_SUPERBLOCK);
	graph.getModel().beginUpdate();

	/*
	 * Update selection and return it.
	 */
	List<XcosUIDObject> selectedCells = updateForNotSelectedLinks(graph);
	
	/*
	 * Sort the selected cells to avoid misplacement
	 */
	Collections.sort(selectedCells);
	
	/*
	 * Clone cells and generate a translation matrix between old and new mxCells
	 */
	Object[] cellArrays = getGraph(null).cloneCells(selectedCells.toArray());
	Collection<Object> cells = Arrays.asList(cellArrays);
	XcosUIDObject[] typedCells = new XcosUIDObject[cellArrays.length];
	cells.toArray(typedCells);
	List<XcosUIDObject> cellsCopy = Arrays.asList(typedCells);
	Object[] translationMatrix = new Object[cellsCopy.size()]; 
	for (int i = 0; i < translationMatrix.length; i++) {
	    translationMatrix[i] = selectedCells.get(i);
	}
	
	List<BasicBlock> blocksCopyWithoutSplitBlocks = getBlocks(cellsCopy);
	
	/*
	 * Getting selection rectangle
	 */
	double minX = Double.MAX_VALUE;
	double minY = Double.MAX_VALUE;
	double maxX = Double.MIN_VALUE;
	double maxY = Double.MIN_VALUE;

	for (BasicBlock current  : blocksCopyWithoutSplitBlocks) {
		minX = Math.min(minX, current.getGeometry().getX());
		minY = Math.min(minY, current.getGeometry().getY());
		maxX = Math.max(maxX, current.getGeometry().getX());
		maxY = Math.max(maxY, current.getGeometry().getY());
	}

	/*
	 * Creating the superblock
	 */
	SuperBlock superBlock = (SuperBlock) BlockFactory.createBlock("SUPER_f");
	superBlock.setStyle("SUPER_f");
	superBlock.getGeometry().setX((maxX + minX) / 2.0);
	superBlock.getGeometry().setY((maxY + minY) / 2.0);



	/*
	 * Creating the child graph
	 */
	SuperBlockDiagram diagram = new SuperBlockDiagram(superBlock);

	diagram.getModel().beginUpdate();
	diagram.addCells(cellsCopy.toArray());
	diagram.getModel().endUpdate();

	/*
	 * Find broken links, to insert input/output blocks And update the child
	 * graph
	 */
	List<BrokenLink> breaks = getBrokenLinks(selectedCells, cellsCopy);
	List<Integer> maxValues = getMaxBlocksValues(selectedCells);
	updateChildGraph(diagram, breaks, maxValues);

	/*
	 * Delete the selected cells from the parent graph
	 */
	graph.removeCells(graph.getSelectionCells());
	
	/*
	 * Update block with real parameters
	 */
	superBlock.setRealParameters(BlockWriter.convertDiagramToMList(diagram));
	superBlock.createChildDiagram();
	
	/*
	 * Update the parent
	 */
	graph.getModel().beginUpdate();
	graph.addCell(superBlock);
	graph.setSelectionCell(superBlock);
	graph.getModel().endUpdate();
	
	/*
	 * Calculate angle/mirrored/flipped statistics
	 */
	int angle = 0;
	int flipped = 0;
	int mirrored = 0;
	for (BasicBlock basicBlock : blocksCopyWithoutSplitBlocks) {
	    angle += basicBlock.getAngle();
	    flipped += basicBlock.getFlip() ? 1 : 0;
	    mirrored += basicBlock.getMirror() ? 1 : 0;
	}
	
	/*
	 * Apply statistics to the superblock
	 */
	int midBlockIndex = blocksCopyWithoutSplitBlocks.size() / 2;
	superBlock.setAngle(BlockPositioning.roundAngle(angle / blocksCopyWithoutSplitBlocks.size()));
	superBlock.setFlip((flipped > midBlockIndex) ? true : false);
	superBlock.setMirror((mirrored > midBlockIndex) ? true : false);
	
	/*
	 * Update the view
	 */
	superBlock.updateExportedPort();

	// change source or target of old link
	createLinks(graph, superBlock, breaks);
	superBlock.closeBlockSettings();
	
	/*
	 * Update the visible attributes
	 */
	BlockPositioning.updateBlockView(superBlock);
	
	graph.getModel().endUpdate();
	graph.refresh();
	diagram.refresh();
	graph.info(XcosMessages.EMPTY_INFO);
    }

    /**
     * Get all the non-SplitBlock blocks in the cellsCopy.
     * @param cellsCopy list of selected cells
     * @return list of blocks
     */
    private List<BasicBlock> getBlocks(List<XcosUIDObject> cellsCopy) {
	List<BasicBlock> list = new ArrayList<BasicBlock>(cellsCopy.size());
	for (XcosUIDObject cell : cellsCopy) {
	    if (cell instanceof BasicBlock) {
		if (!(cell instanceof SplitBlock)) {
		    list.add((BasicBlock) cell);
		}
	    }
	}
	return list;
    }

    /**
     * Check for missing links or selected ports, to add or exclude them.
     * @param graph parent diagram
     * @return new selected list
     */
    private List<XcosUIDObject> updateForNotSelectedLinks(XcosDiagram graph) {

	graph.getModel().beginUpdate();

	for (int i = 0; i < graph.getSelectionCells().length; i++) {
	    XcosUIDObject current = (XcosUIDObject) graph.getSelectionCells()[i];
	    if (current instanceof BasicBlock) {
		BasicBlock block = (BasicBlock) current;
		for (int j = 0; j < block.getChildCount(); j++) {
		    if (block.getChildAt(j) instanceof BasicPort) {
			BasicPort port = (BasicPort) block.getChildAt(j);
			if (port.getEdgeCount() > 0) {
			    if (port.getEdgeAt(0) instanceof BasicLink) {
				BasicLink link = (BasicLink) port.getEdgeAt(0);
				BasicBlock otherSide = null;
				if (link.getTarget() == port) {
				    otherSide = (BasicBlock) link.getSource()
					    .getParent();
				} else {
				    otherSide = (BasicBlock) link.getTarget()
					    .getParent();
				} // target == port

				if (isInSelection(graph.getSelectionCells(), otherSide)) {
				    graph.addSelectionCell(link);
				} // isInSelection
				
				if (otherSide instanceof SplitBlock) {
				    graph.addSelectionCell(otherSide);
				} // otherSide is a SplitBlock

			    } // BasicLink
			} // Edge > 0
		    } // BasicPort
		} // for child
	    } else if (current instanceof BasicPort) {
		// remove orphan port and connected link
		graph.removeSelectionCell(current.getEdgeAt(0));
		graph.removeSelectionCell(current);

		// restart loop
		i = -1;
	    }
	} // for selection

	graph.getModel().endUpdate();
	
	Object[] selectedCells = graph.getSelectionCells();
	Collection<Object> cells = Arrays.asList(selectedCells);
	XcosUIDObject[] typedCells = new XcosUIDObject[selectedCells.length];
	cells.toArray(typedCells);
	
	return new ArrayList<XcosUIDObject>(Arrays.asList(typedCells));
    }
    
    /**
     * Re-link the parent Graph
     * 
     * @param graph
     *            The parent graph (modified)
     * @param superBlock
     *            The added superblock
     * @param breaks
     *            The broken links
     */
    private void createLinks(XcosDiagram graph, SuperBlock superBlock,
	    List<BrokenLink> breaks) {
	for (BrokenLink link : breaks) {
	    BasicPort source = null;
	    BasicPort target = null;

	    if (link.getOutGoing()) {
	    	target = (BasicPort) link.getLink().getTarget();

	    	if (link.getLink() instanceof ExplicitLink) {
	    		source = BasicBlockInfo.getAllExplicitOutputPorts(superBlock, false).get(link.getPortNumber() - 1);
	    	} else if (link.getLink() instanceof ImplicitLink) {
	    		source = BasicBlockInfo.getAllImplicitOutputPorts(superBlock, false).get(link.getPortNumber() - 1);
	    	} else if (link.getLink() instanceof CommandControlLink) {
	    		source = BasicBlockInfo.getAllCommandPorts(superBlock, false).get(link.getPortNumber() - 1);
	    	}
	    } else {
	    	source = (BasicPort) link.getLink().getSource();

	    	if (link.getLink() instanceof ExplicitLink) {
	    		target = BasicBlockInfo.getAllExplicitInputPorts(superBlock, false).get(link.getPortNumber() - 1);
	    	} else if (link.getLink() instanceof ImplicitLink) {
	    		target = BasicBlockInfo.getAllImplicitInputPorts(superBlock, false).get(link.getPortNumber() - 1);
	    	} else if (link.getLink() instanceof CommandControlLink) {
	    		target = BasicBlockInfo.getAllControlPorts(superBlock, false).get(link.getPortNumber() - 1);
	    	}
	    }

	    BasicLink newLink = BasicLink.createLinkFromPorts(source, target);
	    newLink.setGeometry(link.getLink().getGeometry());
	    newLink.setSource(source);
	    newLink.setTarget(target);

	    graph.getModel().beginUpdate();
	    graph.addCell(newLink);
	    graph.getModel().endUpdate();

	    // this method don't call CELLS_REMOVED between beginUpdate and
	    // endUpdate
	    // this function unlink source and target correctly too
	    graph.getModel().beginUpdate();
	    graph.getModel().remove(link.getLink());
	    graph.getModel().endUpdate();
	}
    }

    /**
     * Add the IN/OUT blocks and links in the child graph
     * 
     * @param diagram
     *            The child graph
     * @param breaks
     *            The broken links in the parent graph
     * @param maxValues
     *            The I/O block values previously used in the parent diagram
     *            (must be unique)
     */
    private void updateChildGraph(SuperBlockDiagram diagram,
	    List<BrokenLink> breaks, List<Integer> maxValues) {

	/*
	 * Add in/out blocks in SuperBlock (Child Graph)
	 */
	for (BrokenLink link : breaks) {
	    BasicBlock block = null;

	    if (link.getLink() instanceof ExplicitLink) {
		if (link.getOutGoing()) { // OUT_f
		    block = BlockFactory.createBlock("OUT_f");
		    ExplicitInputPort port = new ExplicitInputPort();
		    port.setDefaultValues();
		    block.addPort(port);
		    link.setPortNumber(maxValues.get(0) + 1);
		    maxValues.set(0, maxValues.get(0) + 1);
		} else { // IN_f
		    block = BlockFactory.createBlock("IN_f");
		    ExplicitOutputPort port = new ExplicitOutputPort();
		    port.setDefaultValues();
		    block.addPort(port);
		    link.setPortNumber(maxValues.get(1) + 1);
		    maxValues.set(1, maxValues.get(1) + 1);
		}
	    } else if (link.getLink() instanceof ImplicitLink) {
		if (link.getOutGoing()) { // OUTIMPL_f
		    block = BlockFactory.createBlock("OUTIMPL_f");
		    ImplicitInputPort port = new ImplicitInputPort();
		    port.setDefaultValues();
		    block.addPort(port);
		    link.setPortNumber(maxValues.get(2) + 1);
		    maxValues.set(2, maxValues.get(2) + 1);
		} else { // INIMPL_f
		    block = BlockFactory.createBlock("INIMPL_f");
		    ImplicitOutputPort port = new ImplicitOutputPort();
		    port.setDefaultValues();
		    block.addPort(port);
		    link.setPortNumber(maxValues.get(3) + 1);
		    maxValues.set(3, maxValues.get(3) + 1);
		}
	    } else if (link.getLink() instanceof CommandControlLink) {
		if (link.getOutGoing()) { // CLKOUTV_f
		    block = BlockFactory.createBlock("CLKOUTV_f");
		    ControlPort port = new ControlPort();
		    port.setDefaultValues();
		    block.addPort(port);
		    link.setPortNumber(maxValues.get(4) + 1);
		    maxValues.set(4, maxValues.get(4) + 1);
		} else { // CLKINV_f
		    block = BlockFactory.createBlock("CLKINV_f");
		    CommandPort port = new CommandPort();
		    port.setDefaultValues();
		    block.addPort(port);
		    link.setPortNumber(maxValues.get(5) + 1);
		    maxValues.set(5, maxValues.get(5) + 1);
		}
	    }

	    block.setGeometry(link.getGeometry());
	    block.setExprs(new ScilabString(Integer.toString(link
		    .getPortNumber())));
	    block.setRealParameters(new ScilabDouble());
	    block.setIntegerParameters(new ScilabDouble(link.getPortNumber()));
	    block.setObjectsParameters(new ScilabList());

	    diagram.getModel().beginUpdate();
	    diagram.addCells(new Object[] {block});
	    diagram.getModel().endUpdate();

	    /*
	     * create new link in SuperBlock
	     */
	    BasicLink newLink = null;
	    if (link.getOutGoing()) { // old -> new
		newLink = BasicLink
			.createLinkFromPorts((BasicPort) link.getLink()
				.getSource(), (BasicPort) block.getChildAt(0));
		newLink.setGeometry(link.getLink().getGeometry());
		newLink.setSource((BasicPort) link.getPort());
		newLink.setTarget((BasicPort) block.getChildAt(0));
	    } else { // new -> old
		newLink = BasicLink.createLinkFromPorts((BasicPort) block
			.getChildAt(0), (BasicPort) link.getLink().getTarget());
		newLink.setGeometry(link.getLink().getGeometry());
		newLink.setSource((BasicPort) block.getChildAt(0));
		newLink.setTarget((BasicPort) link.getPort());
	    }

	    diagram.getModel().beginUpdate();
	    diagram.addCell(newLink);
	    diagram.getModel().endUpdate();
	}

    }

	/**
     * Getting the broken links on the diagram and construct a list of these links
     * @param objs The selected cells
     * @param copiedCells The copy of the selected cells
     * @return all the broken links in the diagram
     */
    private List<BrokenLink> getBrokenLinks(List<XcosUIDObject> objs, List<XcosUIDObject> copiedCells) {
	List<BrokenLink> breaks = new ArrayList<BrokenLink>();	

	int objsLength = objs.size();
	for (int i = 0; i < objsLength; i++) {
	    if (objs.get(i) instanceof BasicBlock) {
		BasicBlock block = (BasicBlock) objs.get(i);
		for (int j = 0; j < block.getChildCount(); j++) {
		    BasicPort port = (BasicPort) block.getChildAt(j);
		    if (port.getEdgeCount() != 0) {
			BasicLink link = (BasicLink) port.getEdgeAt(0);
			if (block.getChildAt(j) instanceof InputPort
				|| block.getChildAt(j) instanceof ControlPort) {
			    BasicBlock source = (BasicBlock) (link.getSource()
				    .getParent());
			    if (!objs.contains(source)) {
				BasicPort copiedPort = (BasicPort) ((BasicBlock) copiedCells.get(i)).getChildAt(j);
				breaks.add(new BrokenLink(link, copiedPort, source.getGeometry(), false));
			    }
			} else { // OutputPort or CommandPort
			    BasicBlock target = (BasicBlock) (link.getTarget().getParent());
			    if (!objs.contains(target)) {
				BasicPort copiedPort = (BasicPort) ((BasicBlock) copiedCells.get(i)).getChildAt(j);
				breaks.add(new BrokenLink(link, copiedPort, target.getGeometry(), true));
			    }
			}
		    }
		}
	    }
	}
	return breaks;
    }

    /**
     * Check if an object is in a collection
     * @param objs collection
     * @param item the searched item
     * @return status
     */
    private boolean isInSelection(Object[] objs, Object item) {
	return Arrays.asList(objs).contains(item);
    }

    /**
     * @param breaks List of broken link in current selection
     */
    private void printBreakingLink(List<BrokenLink> breaks) {
	System.err.println("breaks count : " + breaks.size());

	for (BrokenLink brk : breaks) {
	    System.err.println("Link : " + brk.getLink());
	    System.err.println("OutGoing : " + brk.getOutGoing());
	    System.err.println("Geometry : " + brk.getGeometry());
	}
    }

    /**
     * @param blocks list of blocks
     * @return integer list of max values
     */
    private List<Integer> getMaxBlocksValues(List<XcosUIDObject> blocks) {
	List<Integer> values = new ArrayList<Integer>();
	Map<ContextUpdate.IOBlocks, List<BasicBlock>> items = new EnumMap<ContextUpdate.IOBlocks, List<BasicBlock>>(ContextUpdate.IOBlocks.class);

	// ExplicitInBlock
	for (XcosUIDObject cell : blocks) {
	    if (cell instanceof ContextUpdate) {
	    if (cell instanceof ExplicitOutBlock) {
		if (!items.containsKey(IOBlocks.ExplicitInBlock)) {
		    items.put(IOBlocks.ExplicitOutBlock, new ArrayList<BasicBlock>());
		}
		items.get(IOBlocks.ExplicitOutBlock).add((BasicBlock) cell);
	    } else if (cell instanceof ExplicitInBlock) {
		if (!items.containsKey(IOBlocks.ExplicitInBlock)) {
		    items.put(IOBlocks.ExplicitInBlock, new ArrayList<BasicBlock>());
		}
		items.get(IOBlocks.ExplicitInBlock).add((BasicBlock) cell);
	    } else if (cell instanceof ImplicitOutBlock) {
		if (!items.containsKey(IOBlocks.ImplicitOutBlock)) {
		    items.put(IOBlocks.ImplicitOutBlock, new ArrayList<BasicBlock>());
		}
		items.get(IOBlocks.ImplicitOutBlock).add((BasicBlock) cell);
	    } else if (cell instanceof ImplicitInBlock) {
		if (!items.containsKey(IOBlocks.ImplicitInBlock)) {
		    items.put(IOBlocks.ImplicitInBlock, new ArrayList<BasicBlock>());
		}
		items.get(IOBlocks.ImplicitInBlock).add((BasicBlock) cell);
	    } else if (cell instanceof EventOutBlock) {
		if (!items.containsKey(IOBlocks.EventOutBlock)) {
		    items.put(IOBlocks.EventOutBlock, new ArrayList<BasicBlock>());
		}
		items.get(IOBlocks.EventOutBlock).add((BasicBlock) cell);
	    } else if (cell instanceof EventInBlock) {
		if (!items.containsKey(IOBlocks.EventInBlock)) {
		    items.put(IOBlocks.EventInBlock, new ArrayList<BasicBlock>());
		}
		items.get(IOBlocks.EventInBlock).add((BasicBlock) cell);
	    }
	    }
	}

	for (IOBlocks klass : ContextUpdate.IOBlocks.values()) {
	    values.add(getMaxValue(items.get(klass)));
	}

	return values;
    }

    /**
     * @param blocks list of blocks
     * @return max value
     */
    private int getMaxValue(List<BasicBlock> blocks) {
	int maxValue = 0;
	if (blocks != null) {
	    for (int i = 0; i < blocks.size(); i++) {
		if (((BasicBlock) blocks.get(i)).getExprs() instanceof ScilabString) {
		    maxValue = Math.max(maxValue, Integer
			    .parseInt(((ScilabString) ((BasicBlock) blocks
				    .get(i)).getExprs()).getData()[0][0]));
		}
	    }
	}
	return maxValue;
    }
}
