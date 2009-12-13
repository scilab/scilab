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

import java.util.ArrayList;
import java.util.Arrays;
import java.util.EnumMap;
import java.util.List;
import java.util.Map;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;
import org.scilab.modules.hdf5.scilabTypes.ScilabList;
import org.scilab.modules.hdf5.scilabTypes.ScilabString;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.ContextUpdate;
import org.scilab.modules.xcos.block.ContextUpdate.IOBlocks;
import org.scilab.modules.xcos.block.EventInBlock;
import org.scilab.modules.xcos.block.EventOutBlock;
import org.scilab.modules.xcos.block.ExplicitInBlock;
import org.scilab.modules.xcos.block.ExplicitOutBlock;
import org.scilab.modules.xcos.block.ImplicitInBlock;
import org.scilab.modules.xcos.block.ImplicitOutBlock;
import org.scilab.modules.xcos.block.SplitBlock;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.graph.SuperBlockDiagram;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.BasicBlockInfo;
import org.scilab.modules.xcos.io.BlockWriter;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.link.commandcontrol.CommandControlLink;
import org.scilab.modules.xcos.link.explicit.ExplicitLink;
import org.scilab.modules.xcos.link.implicit.ImplicitLink;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.BasicPort.DataType;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGeometry;


public class RegionToSuperblockAction extends DefaultAction {

    private class BrokenLink {
	private BasicLink link;
	private BasicPort edge;
	private mxGeometry geom;
	private boolean outGoing;
	private int portNumber;

	public BrokenLink(BasicLink link, BasicPort edge, mxGeometry geom, boolean outGoing) {
	    this.link = link;
	    this.edge = edge;
	    this.outGoing = outGoing;
	    this.geom = geom;
	}

	public boolean getOutGoing() {
	    return outGoing;
	}

	public BasicLink getLink() {
	    return link;
	}
	
	public BasicPort getCopiedEdge() {
	    return edge;
	}

	public mxGeometry getGeometry() {
	    return geom;
	}

	public void setPortNumber(int portNumber) {
	    this.portNumber = portNumber;
	}

	public int getPortNumber() {
	    return portNumber;
	}
    }

    private RegionToSuperblockAction(ScilabGraph scilabGraph) {
	super(XcosMessages.REGION_TO_SUPERBLOCK, scilabGraph);
    }

    public static MenuItem createMenu(ScilabGraph scilabGraph) {
	return createMenu(XcosMessages.REGION_TO_SUPERBLOCK, null,
		new RegionToSuperblockAction(scilabGraph), null);
    }

    public void doAction() {

	XcosDiagram graph = (XcosDiagram) getGraph(null);
	graph.info(XcosMessages.GENERATE_SUPERBLOCK);
	graph.getModel().beginUpdate();

	/*
	 * Update selection
	 */
	updateForNotSelectedLinks(graph);
	
	/*
	 * Clone cells and generate a translation matrix between old and new mxCells
	 */
	Object[] cellsCopy = getGraph(null).cloneCells(getGraph(null).getSelectionCells());
	Object[] translationMatrix = new Object[cellsCopy.length]; 
	for (int i = 0; i < translationMatrix.length; i++) {
	    translationMatrix[i] = getGraph(null).getSelectionCells()[i];
	}
	
	/*
	 * Getting selection rectangle
	 */
	double minX = Double.MAX_VALUE;
	double minY = Double.MAX_VALUE;
	double maxX = Double.MIN_VALUE;
	double maxY = Double.MIN_VALUE;

	for (int i = 0; i < cellsCopy.length; ++i) {
	    mxCell current = (mxCell) cellsCopy[i];
	    if (current instanceof BasicBlock) {
		minX = Math.min(minX, current.getGeometry().getX());
		minY = Math.min(minY, current.getGeometry().getY());
		maxX = Math.max(maxX, current.getGeometry().getX());
		maxY = Math.max(maxY, current.getGeometry().getY());
	    }
	}

	/*
	 * Creating the superblock
	 */
	SuperBlock superBlock = (SuperBlock) BasicBlock.createBlock("SUPER_f");
	superBlock.setStyle("SUPER_f");
	superBlock.getGeometry().setX((maxX + minX) / 2.0);
	superBlock.getGeometry().setY((maxY + minY) / 2.0);


	/*
	 * Creating the child graph
	 */
	SuperBlockDiagram diagram = new SuperBlockDiagram(superBlock);

	diagram.getModel().beginUpdate();
	diagram.addCells(cellsCopy);
	diagram.getModel().endUpdate();

	/*
	 * Find broken links, to insert input/output blocks And update the child
	 * graph
	 */
	List<BrokenLink> breaks = getBrokenLinks(graph.getSelectionCells(), cellsCopy);
	List<Integer> maxValues = getMaxBlocksValues(graph.getSelectionCells());
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
	superBlock.updateExportedPort();

	// change source or target of old link
	createLinks(graph, superBlock, breaks);
	superBlock.closeBlockSettings();

	graph.getModel().endUpdate();
	graph.refresh();
	diagram.refresh();
	graph.info(XcosMessages.EMPTY_INFO);
    }

    /**
     * Check for missing links or selected ports, to add or exclude them.
     */
    private void updateForNotSelectedLinks(XcosDiagram graph) {

	graph.getModel().beginUpdate();

	for (int i = 0; i < graph.getSelectionCells().length; i++) {
	    mxCell current = (mxCell) graph.getSelectionCells()[i];
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

				if (isInSelection(graph.getSelectionCells(),
					otherSide)) {
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
		    block = BasicBlock.createBlock("OUT_f");
		    ExplicitInputPort port = new ExplicitInputPort();
		    port.setDataLines(-1);
		    port.setDataColumns(-2);
		    port.setDataType(DataType.UNKNOW_TYPE);
		    block.addPort(port);
		    link.setPortNumber(maxValues.get(0) + 1);
		    maxValues.set(0, maxValues.get(0) + 1);
		} else { // IN_f
		    block = BasicBlock.createBlock("IN_f");
		    ExplicitOutputPort port = new ExplicitOutputPort();
		    port.setDataLines(-1);
		    port.setDataColumns(-2);
		    port.setDataType(DataType.UNKNOW_TYPE);
		    block.addPort(port);
		    link.setPortNumber(maxValues.get(1) + 1);
		    maxValues.set(1, maxValues.get(1) + 1);
		}
	    } else if (link.getLink() instanceof ImplicitLink) {
		if (link.getOutGoing()) { // OUTIMPL_f
		    block = BasicBlock.createBlock("OUTIMPL_f");
		    ImplicitInputPort port = new ImplicitInputPort();
		    port.setDataLines(-1);
		    port.setDataColumns(-2);
		    port.setDataType(DataType.UNKNOW_TYPE);
		    block.addPort(port);
		    link.setPortNumber(maxValues.get(2) + 1);
		    maxValues.set(2, maxValues.get(2) + 1);
		} else { // INIMPL_f
		    block = BasicBlock.createBlock("INIMPL_f");
		    ImplicitOutputPort port = new ImplicitOutputPort();
		    port.setDataLines(-1);
		    port.setDataColumns(-2);
		    port.setDataType(DataType.UNKNOW_TYPE);
		    block.addPort(port);
		    link.setPortNumber(maxValues.get(3) + 1);
		    maxValues.set(3, maxValues.get(3) + 1);
		}
	    } else if (link.getLink() instanceof CommandControlLink) {
		if (link.getOutGoing()) { // CLKOUTV_f
		    block = BasicBlock.createBlock("CLKOUTV_f");
		    ControlPort port = new ControlPort();
		    port.setDataLines(-1);
		    port.setDataColumns(-2);
		    port.setDataType(DataType.UNKNOW_TYPE);
		    block.addPort(port);
		    link.setPortNumber(maxValues.get(4) + 1);
		    maxValues.set(4, maxValues.get(4) + 1);
		} else { // CLKINV_f
		    block = BasicBlock.createBlock("CLKINV_f");
		    CommandPort port = new CommandPort();
		    port.setDataLines(-1);
		    port.setDataColumns(-2);
		    port.setDataType(DataType.UNKNOW_TYPE);
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
	    diagram.addCells(new Object[] { block });
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
		newLink.setSource((BasicPort) link.getCopiedEdge());
		newLink.setTarget((BasicPort) block.getChildAt(0));
	    } else { // new -> old
		newLink = BasicLink.createLinkFromPorts((BasicPort) block
			.getChildAt(0), (BasicPort) link.getLink().getTarget());
		newLink.setGeometry(link.getLink().getGeometry());
		newLink.setSource((BasicPort) block.getChildAt(0));
		newLink.setTarget((BasicPort) link.getCopiedEdge());
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
    private List<BrokenLink> getBrokenLinks(Object[] objs, Object[] copiedCells) {
	List<BrokenLink> breaks = new ArrayList<BrokenLink>();	

	for (int i = 0; i < objs.length; i++) {
	    if (objs[i] instanceof BasicBlock) {
		BasicBlock block = (BasicBlock) objs[i];
		for (int j = 0; j < block.getChildCount(); j++) {
		    BasicPort port = (BasicPort) block.getChildAt(j);
		    if (port.getEdgeCount() != 0) {
			BasicLink link = (BasicLink) port.getEdgeAt(0);
			if (block.getChildAt(j) instanceof InputPort
				|| block.getChildAt(j) instanceof ControlPort) {
			    BasicBlock source = (BasicBlock) (link.getSource()
				    .getParent());
			    if (!isInSelection(objs, source)) {
				BasicPort copiedPort = (BasicPort) ((BasicBlock)copiedCells[i]).getChildAt(j);
				breaks.add(new BrokenLink(link, copiedPort, source.getGeometry(), false));
			    }
			} else { // OutputPort or CommandPort
			    BasicBlock target = (BasicBlock) (link.getTarget().getParent());
			    if (!isInSelection(objs, target)) {
				BasicPort copiedPort = (BasicPort) ((BasicBlock)copiedCells[i]).getChildAt(j);
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
     * @return 
     */
    private boolean isInSelection(Object[] objs, Object item) {
	return Arrays.asList(objs).contains(item);
    }

    private void printBreakingLink(List<BrokenLink> breaks) {
	System.err.println("breaks count : " + breaks.size());

	for (BrokenLink brk : breaks) {
	    System.err.println("Link : " + brk.getLink());
	    System.err.println("OutGoing : " + brk.getOutGoing());
	    System.err.println("Geometry : " + brk.getGeometry());
	}
    }

    private List<Integer> getMaxBlocksValues(Object[] blocks) {
	List<Integer> values = new ArrayList<Integer>();
	Map<ContextUpdate.IOBlocks, List<BasicBlock>> items = new EnumMap<ContextUpdate.IOBlocks, List<BasicBlock>>(ContextUpdate.IOBlocks.class);

	// ExplicitInBlock
	for (int i = 0; i < blocks.length; i++) {
	    if (blocks[i] instanceof ContextUpdate) {
	    if (blocks[i] instanceof ExplicitOutBlock) {
		if (!items.containsKey(IOBlocks.ExplicitInBlock)) {
		    items.put(IOBlocks.ExplicitOutBlock, new ArrayList<BasicBlock>());
		}
		items.get(IOBlocks.ExplicitOutBlock).add((BasicBlock) blocks[i]);
	    } else if (blocks[i] instanceof ExplicitInBlock) {
		if (!items.containsKey(IOBlocks.ExplicitInBlock)) {
		    items.put(IOBlocks.ExplicitInBlock, new ArrayList<BasicBlock>());
		}
		items.get(IOBlocks.ExplicitInBlock).add((BasicBlock) blocks[i]);
	    } else if (blocks[i] instanceof ImplicitOutBlock) {
		if (!items.containsKey(IOBlocks.ImplicitOutBlock)) {
		    items.put(IOBlocks.ImplicitOutBlock, new ArrayList<BasicBlock>());
		}
		items.get(IOBlocks.ImplicitOutBlock).add((BasicBlock) blocks[i]);
	    } else if (blocks[i] instanceof ImplicitInBlock) {
		if (!items.containsKey(IOBlocks.ImplicitInBlock)) {
		    items.put(IOBlocks.ImplicitInBlock, new ArrayList<BasicBlock>());
		}
		items.get(IOBlocks.ImplicitInBlock).add((BasicBlock) blocks[i]);
	    } else if (blocks[i] instanceof EventOutBlock) {
		if (!items.containsKey(IOBlocks.EventOutBlock)) {
		    items.put(IOBlocks.EventOutBlock, new ArrayList<BasicBlock>());
		}
		items.get(IOBlocks.EventOutBlock).add((BasicBlock) blocks[i]);
	    } else if (blocks[i] instanceof EventInBlock) {
		if (!items.containsKey(IOBlocks.EventInBlock)) {
		    items.put(IOBlocks.EventInBlock, new ArrayList<BasicBlock>());
		}
		items.get(IOBlocks.EventInBlock).add((BasicBlock) blocks[i]);
	    }
	    }
	}

	for (IOBlocks klass : ContextUpdate.IOBlocks.values()) {
	    values.add(getMaxValue(items.get(klass)));
	}

	return values;
    }

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
	// System.err.println("maxValue : " + maxValue);
	return maxValue;
    }
}
