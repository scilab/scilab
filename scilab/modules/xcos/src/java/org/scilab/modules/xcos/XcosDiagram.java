/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos;

import java.awt.Color;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;

import javax.swing.JFileChooser;
import javax.swing.JOptionPane;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.gui.filechooser.FileChooser;
import org.scilab.modules.gui.filechooser.ScilabFileChooser;
import org.scilab.modules.gui.messagebox.MessageBox;
import org.scilab.modules.gui.messagebox.ScilabMessageBox;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.utils.UIElementMapper;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.hdf5.scilabTypes.ScilabMList;
import org.scilab.modules.xcos.actions.XcosShortCut;
import org.scilab.modules.xcos.block.AfficheBlock;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BlockReader;
import org.scilab.modules.xcos.block.BlockWriter;
import org.scilab.modules.xcos.block.TextBlock;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.link.commandcontrol.CommandControlLink;
import org.scilab.modules.xcos.link.explicit.ExplicitLink;
import org.scilab.modules.xcos.link.implicit.ImplicitLink;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.PortCheck;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.scilab.modules.xcos.utils.Signal;
import org.scilab.modules.xcos.utils.XcosMessages;
import org.w3c.dom.Document;

import com.mxgraph.io.mxCodec;
import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGeometry;
import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.util.mxUtils;
import com.mxgraph.view.mxMultiplicity;

public class XcosDiagram extends ScilabGraph {

	// Default values : SCI/modules/scicos/macros/scicos_scicos/scicos_params.sci
	private double[][] wpar = {{600,450,0,0,600,450}}; 
	private String title = XcosMessages.UNTITLED;
	private double finalIntegrationTime = 100000;
	private double integratorAbsoluteTolerance = 1e-4;
	private double integratorRelativeTolerance = 1e-6;
	private double toleranceOnTime = 1e-10;
	private double maxIntegrationTimeinterval = finalIntegrationTime + 1;
	private double realTimeScaling = 0;
	private double solver = 0;
	private double maximumStepSize = 0;
	private String context = "";
	private List doc = null;
	private String version = "scicos4.2";

	private Tab parentTab;
	//private Window palette;
	private Tab viewPort;

	private CheckBoxMenuItem viewPortMenu;

	public Object addEdge(Object edge, Object parent, Object source,
			Object target, Integer index)
	{	
		System.err.println("[DEBUG] AddEdge :");
		System.err.println("[DEBUG] AddEdge : source "+source.toString());
		System.err.println("[DEBUG] AddEdge : target "+target.toString());
		// Command -> Control
		if(source instanceof CommandPort) {
			if (target instanceof ControlPort) {
				return super.addEdge(new CommandControlLink(), parent, source, target, index);
			}
		}

		// Control -> Command
		// Switch source and target !
		if(target instanceof CommandPort) {
			if (source instanceof ControlPort) {
				return super.addEdge(new CommandControlLink(), parent, target, source, index);
			}
		}

		// ExplicitOutput -> ExplicitInput
		if (source instanceof ExplicitOutputPort) {
			if(target instanceof ExplicitInputPort) {
				return super.addEdge(new ExplicitLink(), parent, source, target, index);
			}
		}
		// ExplicitInput -> ExplicitOutput
		// Switch source and target !
		if(target instanceof ExplicitOutputPort) {
			if (source instanceof ExplicitInputPort) {
				return super.addEdge(new ExplicitLink(), parent, target, source, index);
			}
		}

		// ImplicitOutput -> ImplicitInput
		if (source instanceof ImplicitOutputPort) {
			if(target instanceof ImplicitInputPort) {
				return super.addEdge(new ImplicitLink(), parent, source, target, index);
			}
		}
		// ImplicitInput -> ImplicitOutput
		// Switch source and target !
		if(target instanceof ImplicitOutputPort) {
			if (source instanceof ImplicitInputPort) {
				return super.addEdge(new ImplicitLink(), parent, target, source, index);
			}
		}

		return null;
	}

	public XcosDiagram() {
		super();
		keyboardHandler = new XcosShortCut(this);
		mxCodec codec = new mxCodec();
		Document doc = mxUtils.loadDocument(System.getenv("SCI")+"/modules/xcos/etc/Xcos-style.xml");
		codec.decode(doc.getDocumentElement(), getStylesheet());

		getAsComponent().setToolTips(true);

		// Forbid disconnecting cells once it is connected.
		//setCellsDisconnectable(false);

		// Forbid pending edges.
		setAllowDanglingEdges(false);

		// Cannot connect port to itself.
		setAllowLoops(false);

		// Override isCellResizable to filter what the user can resize
		setCellsResizable(true);

		// force auto resize cell
		setAutoSizeCells(true);

		// Override isCellEditable to filter what the user can edit
		setCellsEditable(true);
		// This enable stop editing cells when pressing Enter.
		getAsComponent().setEnterStopsCellEditing(false);

		setConnectableEdges(false);

		getAsComponent().getViewport().setOpaque(false);
		getAsComponent().setBackground(Color.WHITE);

		mxMultiplicity[] multiplicities = new mxMultiplicity[1];

		// Command Port as source can only go to Control Port
		multiplicities[0] = new PortCheck(true, "commandPort", "type", "Command", 1,
				"1", Arrays.asList(new Object[] { "Control" }),
				"Command Port Must Have 1 Control Port",
				"Control Port Must Connect to Command Port", true);
		// Control Port must be connected !
		//	multiplicities[1] = new mxMultiplicity(false, "controlPort", null, null, 1,
		//			"n", Arrays.asList(new Object[] {"commandPort"}),
		//			"Block Should be controled",
		//			"Command port should connect to Control", true);


		//	// Source nodes needs 1..2 connected Targets
		//	multiplicities[0] = new mxMultiplicity(true, "Source", null, null, 1,
		//			"2", Arrays.asList(new Object[] { "Target" }),
		//			"Source Must Have 1 or 2 Targets",
		//			"Source Must Connect to Target", true);
		//
		//	// Output Port as source can only go to Input Port
		//	multiplicities[1] = new mxMultiplicity(true, "outputPort", "PortType", "input", 1,
		//			"n", Arrays.asList(new Object[] { "inputPort" }),
		//			"Input Port Must Have 1 Output Port",
		//			"Output Port Must Connect to Input Port", true);
		//	
		//	// Command Port as source can only go to Control Port
		//	multiplicities[3] = new mxMultiplicity(true, "commandPort", null, null, 1,
		//			"1", Arrays.asList(new Object[] { "controlPort" }),
		//			"Command Port Must Have 1 Control Port",
		//			"Control Port Must Connect to Command Port", true);
		//	// Control Port must be connected !
		//	multiplicities[4] = new mxMultiplicity(false, "controlPort", null, null, 1,
		//			"1", Arrays.asList(new Object[] {"commandPort"}),
		//			"Block Should be controled",
		//			"Command port should connect to Control", true);
		//	// Source node does not want any incoming connections
		//	multiplicities[2] = new mxMultiplicity(false, "Source", null, null, 0,
		//			"0", null, "Source Must Have No Incoming Edge", null, true);
		setMultiplicities(multiplicities);
		getModel().addListener(mxEvent.CHANGE, new mxIEventListener()
		{
			public void invoke(Object sender, mxEventObject evt)
			{
				getAsComponent().validateGraph();
			}
		});

		getAsComponent().getGraphControl().addMouseListener(new MouseListener() {
			public void mouseClicked(MouseEvent arg0) {
				Object cell = getAsComponent().getCellAt(arg0.getX(), arg0.getY());

				// Double Click within empty diagram Area
				if (arg0.getClickCount() >= 2 && arg0.getButton() == MouseEvent.BUTTON1 && cell == null) {
					TextBlock textBlock = new TextBlock("Edit me !!!");
					textBlock.getGeometry().setX(arg0.getX() - textBlock.getGeometry().getWidth() / 2.0);
					textBlock.getGeometry().setY(arg0.getY() - textBlock.getGeometry().getWidth() / 2.0);
					addCell(textBlock);
					return;
				}

				// Double Click within some component
				if (arg0.getClickCount() >= 2 && arg0.getButton() == MouseEvent.BUTTON1 && cell != null)
				{
					getModel().beginUpdate();
					if (cell instanceof BasicBlock && !(cell instanceof TextBlock)) {
						BasicBlock block = (BasicBlock) cell;
						block.openBlockSettings(getContext());
						getAsComponent().doLayout();
					}
					if (cell instanceof BasicLink) {
						((BasicLink) cell).insertPoint(arg0.getX(), arg0.getY());
					}
					getModel().endUpdate();
					refresh();
				}

				// Ctrl + Shift + Right Double Click : for debug !!
				if (arg0.getClickCount() >= 2 && arg0.getButton() == MouseEvent.BUTTON3
						&& arg0.isShiftDown() && arg0.isControlDown())
				{
					System.err.println("[DEBUG] Click at position : "+arg0.getX()+" , "+arg0.getY());
					System.err.println("[DEBUG] Click on : "+cell);
					if(cell != null) {
						System.err.println("[DEBUG] NbEdges : "+((mxCell) cell).getEdgeCount());
						System.err.println("[DEBUG] NbChildren : "+((mxCell) cell).getChildCount());
					}
				}
			}

			public void mouseEntered(MouseEvent arg0) {
				// TODO Auto-generated method stub
			}

			public void mouseExited(MouseEvent arg0) {
				// TODO Auto-generated method stub
			}

			public void mousePressed(MouseEvent arg0) {
				// TODO Auto-generated method stub

			}

			public void mouseReleased(MouseEvent arg0) {
				// TODO Auto-generated method stub

			}

		});
	}

	/*
	 * Manage Group to be CellFoldable i.e with a (-) to reduce
	 * and a (+) to expand them.
	 * Only non-Block / non-Port Cell are foldable. 
	 * 
	 * (non-Javadoc)
	 * @see com.mxgraph.view.mxGraph#isCellFoldable(java.lang.Object, boolean)
	 */
	public boolean isCellFoldable(Object cell, boolean collapse) {
		return !(cell instanceof BasicBlock) && super.isCellFoldable(cell, collapse);
	}

	public boolean isCellMovable(Object cell) {
		return !(cell instanceof BasicLink) && !(cell instanceof BasicPort) && super.isCellMovable(cell);
	}

	public boolean isCellResizable(Object cell) {
		return (cell instanceof BasicBlock) && super.isCellResizable(cell);
	}

	public boolean isCellDeletable(Object cell) {
		return !(cell instanceof BasicPort)
		&& super.isCellDeletable(cell);
	}

	public boolean isCellEditable(Object cell) {
		return (cell instanceof TextBlock) && super.isCellDeletable(cell);
	}

	public boolean isCellConnectable(Object cell)
	{
		return !(cell instanceof BasicBlock) && super.isCellConnectable(cell);
	}

	public boolean isAutoSizeCell(Object cell){
		return (cell instanceof AfficheBlock) || super.isAutoSizeCell(cell);
	}

	public void dumpToHdf5File(String fileName) {
		if (fileName == null) {
			FileChooser fc = ScilabFileChooser.createFileChooser();
			fc.setMultipleSelection(false);
			fc.displayAndWait();

			if (fc.getSelection() == null || fc.getSelection().length == 0 || fc.getSelection()[0].equals("")) {
				return;
			}
			fileName = fc.getSelection()[0];
			System.out.println("Saving to file : {"+fileName+"}");
		}

		BlockWriter.writeDiagramToFile(fileName, this);
	}

	public double getFinalIntegrationTime() {
		return finalIntegrationTime;
	}

	public void setFinalIntegrationTime(double finalIntegrationTime) {
		this.finalIntegrationTime = finalIntegrationTime;
	}

	public double getIntegratorAbsoluteTolerance() {
		return integratorAbsoluteTolerance;
	}

	public void setIntegratorAbsoluteTolerance(double integratorAbsoluteTolerance) {
		this.integratorAbsoluteTolerance = integratorAbsoluteTolerance;
	}

	public double getIntegratorRelativeTolerance() {
		return integratorRelativeTolerance;
	}

	public void setIntegratorRelativeTolerance(double integratorRelativeTolerance) {
		this.integratorRelativeTolerance = integratorRelativeTolerance;
	}

	public double getMaximumStepSize() {
		return maximumStepSize;
	}

	public void setMaximumStepSize(double maximumStepSize) {
		this.maximumStepSize = maximumStepSize;
	}

	public double getMaxIntegrationTimeinterval() {
		return maxIntegrationTimeinterval;
	}

	public void setMaxIntegrationTimeinterval(double maxIntegrationTimeinterval) {
		this.maxIntegrationTimeinterval = maxIntegrationTimeinterval;
	}

	public double getRealTimeScaling() {
		return realTimeScaling;
	}

	public void setRealTimeScaling(double realTimeScaling) {
		this.realTimeScaling = realTimeScaling;
	}

	public double getSolver() {
		return solver;
	}

	public void setSolver(double solver) {
		this.solver = solver;
	}

	public double getToleranceOnTime() {
		return toleranceOnTime;
	}

	public void setToleranceOnTime(double toleranceOnTime) {
		this.toleranceOnTime = toleranceOnTime;
	}

	public Tab getParentTab() {
		return parentTab;
	}

	public void setParentTab(Tab parentTab) {
		this.parentTab = parentTab;
	}

	/**
	 * Set the associated ViewPort
	 * @param viewPort the Viewport
	 */
	public void setViewPort(Tab viewPort) {
		this.viewPort = viewPort;
	}

	/**
	 * Get the associated ViewPort
	 * @return the Viewport
	 */
	public Tab getViewPort() {
		return viewPort;
	}

	/**
	 * Manage the visibility of the associated viewport
	 * @param status new status
	 */
	public void setViewPortVisible(boolean status) {
		// Hide/Show parent window if the viewport is the only tab
		if (viewPort.getParentWindow().getNbDockedObjects() == 1) {
			viewPort.getParentWindow().setVisible(status);
		}
		// Hide/Show viewport tab
		viewPort.setVisible(status);

		// (Un)Check the corresponding menu
		viewPortMenu.setChecked(status);
	}

	/**
	 * Set menu used to manage Viewport visibility
	 * @param menu the menu
	 */
	public void setViewPortMenuItem(CheckBoxMenuItem menu) {
		this.viewPortMenu = menu;
	}
	/**
	 * Close Xcos instance including all tabs
	 */
	public void closeDiagram() {

		boolean wantToClose = true;

		if (this.undoManager.canUndo()) {
			// The diagram has been modified
			// Ask the user want he want to do !
			int choice = JOptionPane.showConfirmDialog(getAsComponent(), XcosMessages.DIAGRAM_MODIFIED);
			if (choice == 0) {
				// Save the diagram
				saveDiagram();
			} else if (choice == 1) {
				// The user selects no !
			} else if (choice == 2) {
				// The user cancels
				wantToClose = false;
			}
		}

		if (wantToClose) {
			ScilabWindow xcosWindow = (ScilabWindow) UIElementMapper.getCorrespondingUIElement(parentTab.getParentWindowId());
			xcosWindow.removeTab(parentTab);
			//palette.getAsSimpleWindow().close();
			viewPort.close();
			Xcos.closeDiagram(this);
		}
	}

	public boolean saveDiagram() {

		if (title.equals(XcosMessages.UNTITLED)) {
			return saveDiagramAs();
		} else {
			return BlockWriter.writeDiagramToFile(title, this);
		}
	}

	public boolean saveDiagramAs() {

		boolean isSuccess = false;
		String fileName;

		// Choose a filename
		FileChooser fc = ScilabFileChooser.createFileChooser();
		fc.setTitle(XcosMessages.SAVE_AS);
		fc.setUiDialogType(JFileChooser.SAVE_DIALOG);
		fc.setMultipleSelection(false);
		fc.displayAndWait();

		if (fc.getSelection() == null || fc.getSelection().length == 0 || fc.getSelection()[0].equals("")) {
			return isSuccess;
		}
		fileName = fc.getSelection()[0];
		System.out.println("Saving to file : {" + fileName + "}");

		isSuccess = BlockWriter.writeDiagramToFile(fileName, this);

		if (isSuccess) {
			this.setTitle(fileName);
			this.getParentTab().setName(fileName);
		}

		return isSuccess;
	}

	public void setTitle(String title) {
		this.title = title;
	}

	public String getTitle() {
		return title;
	}

	public double[][] getWpar() {
		return wpar;
	}

	public void setContext(String context){
		this.context = context;
	}

	public String getContext() {
		return context;
	}

	public String getVersion() {
		return version;
	}

	private BasicLink createLinkFromPorts(BasicPort from, BasicPort to) {
		if (from instanceof ExplicitOutputPort && to instanceof ExplicitInputPort) {
			return new ExplicitLink();
		}
		if (from instanceof ImplicitOutputPort && to instanceof ImplicitInputPort) {
			return new ImplicitLink();
		}
		if (from instanceof CommandPort && to instanceof ControlPort) {
			return new CommandControlLink();
		}
		if (to instanceof ExplicitOutputPort && from instanceof ExplicitInputPort) {
			return new ExplicitLink();
		}
		if (to instanceof ImplicitOutputPort && from instanceof ImplicitInputPort) {
			return new ImplicitLink();
		}
		if (to instanceof CommandPort && from instanceof ControlPort) {
			return new CommandControlLink();
		}
		return new ExplicitLink();
	}

	/**
	 * Open a Diagram :
	 * If current Diagram is empty, open within it
	 * else open a new window.
	 * 
	 * @param diagramm
	 */
	public void openDiagram(HashMap<String, Object> diagramm) {
		if (diagramm != null) {
			if (getModel().getChildCount(getDefaultParent()) == 0) {
				loadDiagram(diagramm);
			}
			else {
				XcosDiagram xcosDiagram = Xcos.CreateAndShowGui();
				xcosDiagram.loadDiagram(diagramm);
			}
		}
		else {
			MessageBox messageBox = ScilabMessageBox.createMessageBox();
			messageBox.setTitle(XcosMessages.FAIL_LOADING_DIAGRAM);
			String[] message = {"An error Occured while loading diagram", "Aborting..."};
			messageBox.setMessage(message);
			messageBox.displayAndWait();
		}
	}

	/**
	 * Load a Diagramm structure into current window.
	 * 
	 * @param diagramm
	 */
	public void loadDiagram(HashMap<String, Object> diagramm) {
		List<BasicBlock> allBlocks = (List) diagramm.get("Blocks");
		List<BasicPort[]> allLinks = (List) diagramm.get("Links");
		HashMap<String, Object> properties = (HashMap<String, Object>) diagramm.get("Properties");

		setFinalIntegrationTime((Double) properties.get("finalIntegrationTime"));
		setIntegratorAbsoluteTolerance((Double) properties.get("integratorAbsoluteTolerance"));
		setIntegratorRelativeTolerance((Double) properties.get("integratorRelativeTolerance"));
		setToleranceOnTime((Double) properties.get("toleranceOnTime"));
		setMaxIntegrationTimeinterval((Double) properties.get("maxIntegrationTimeinterval"));
		setRealTimeScaling((Double) properties.get("realTimeScaling"));
		setSolver((Double) properties.get("solver"));
		setMaximumStepSize((Double) properties.get("maximumStepSize"));
		setContext((String) properties.get("context"));

		getModel().beginUpdate();
		for (int i = 0; i < allBlocks.size(); ++i) {
			addCell(allBlocks.get(i));
		}

		for (int i = 0; i < allLinks.size(); ++i) {
			BasicLink link = createLinkFromPorts(allLinks.get(i)[0], allLinks.get(i)[1]);
			link.setGeometry(new mxGeometry(0,0,80,80));
			link.setSource(allLinks.get(i)[0]);
			link.setTarget(allLinks.get(i)[1]);
			addCell(link);
		}
		getModel().endUpdate();

		//this.setTitle(fileToLoad);
		//this.getParentTab().setName(fileToLoad);

		setTitle((String) properties.get("title"));
		getParentTab().setName((String) properties.get("title"));

		// Clear all undo events in Undo Manager
		undoManager.reset();
	}

	/**
	 * Read a diagram from an HDF5 file (ask for creation if the file does not exist) 
	 * @param diagramFileName file to open
	 */
	public void openDiagramFromFile(String diagramFileName) {

		File theFile = new File(diagramFileName);

		if (theFile.exists()) {

			String fileToLoad = diagramFileName;
			// Try to find file type
			int dotPos = theFile.getName().lastIndexOf('.');
			String extension = "";
			if (dotPos > 0 && dotPos <= theFile.getName().length() - 2) {
				extension = theFile.getName().substring(dotPos + 1);
			}

			if (extension.equals("cosf")) {
				final File tempOutput;
				try {
					tempOutput = File.createTempFile("xcos",".hdf5");
					String cmd = "exec(\"" + theFile.getAbsolutePath() + "\", -1);";
					cmd += "export_to_hdf5(\"" + tempOutput.getAbsolutePath() + "\", \"scs_m\");";
					cmd += "xcosNotify(\"" +tempOutput.getAbsolutePath()+ "\");";
					InterpreterManagement.requestScilabExec(cmd);
					Thread launchMe = new Thread() {
						public void run() {
							Signal.wait(tempOutput.getAbsolutePath());
							openDiagramFromFile(tempOutput.getAbsolutePath());
						}
					};
					launchMe.start();
					fileToLoad = tempOutput.getAbsolutePath();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			} else if (extension.equals("cos")) {
				final File tempOutput;
				try {
					tempOutput = File.createTempFile("xcos",".hdf5");
					String cmd = "load(\"" + theFile.getAbsolutePath() + "\");";
					cmd += "export_to_hdf5(\"" + tempOutput.getAbsolutePath() + "\", \"scs_m\");";
					cmd += "xcosNotify(\"" +tempOutput.getAbsolutePath()+ "\");";
					InterpreterManagement.requestScilabExec(cmd);
					Thread launchMe = new Thread() {
						public void run() {
							Signal.wait(tempOutput.getAbsolutePath());
							openDiagramFromFile(tempOutput.getAbsolutePath());
						}
					};
					launchMe.start();
					fileToLoad = tempOutput.getAbsolutePath();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			} else {
				openDiagram(BlockReader.readDiagramFromFile(fileToLoad));
			}

		} else {
			int choice = JOptionPane.showConfirmDialog(this.getAsComponent(), XcosMessages.FILE_DOESNT_EXIST);
			if (choice  == 0) {
				try {
					FileWriter writer = new FileWriter(diagramFileName);
					writer.write("");
					writer.flush();
					writer.close();

					openDiagramFromFile(diagramFileName);
				} catch (IOException ioexc) {
					JOptionPane.showMessageDialog(this.getAsComponent() , ioexc);
				}
			}	

		}
	}
	/**
	 * Returns the tooltip to be used for the given cell.
	 */
	public String getToolTipForCell(Object cell)
	{
		if (cell instanceof BasicBlock) {
			return ((BasicBlock) cell).getToolTipText();
		}
		return "";
	}

	public static void setBlockTextValue(int blockID, String[] blockValue, int iRows, int iCols){
		System.err.println("block = " + blockID);
		System.err.println("blockValue = " + blockValue[0]);
		System.err.println("Row = " + iRows);
		System.err.println("Col = " + iCols);
		AfficheBlock block = Xcos.getAfficheBlocks().get(blockID);
		
		if(block == null){
			System.err.println("block == null");
			return;
		}
		
		block.setValue(blockValue[0]);
//		mxCell currentObject = (mxCell)getModel().getChildAt(getDefaultParent(), blockID);
//		if(currentObject != null){
//			String value = "";
//			for(int i = 0 ; i < iRows ; i++){
//				for(int j = 0 ; j < iCols ; j++){
//					//blockValue[i]
//				}
//			}
			//TODO format value string with itemS
//		}
//	currentObject.setValue(blockValue[0]);
	}
}

