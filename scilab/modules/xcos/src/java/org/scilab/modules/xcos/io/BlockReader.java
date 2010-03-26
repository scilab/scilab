/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan SIMON
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.io;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import ncsa.hdf.hdf5lib.exceptions.HDF5Exception;
import ncsa.hdf.hdf5lib.exceptions.HDF5JavaException;
import ncsa.hdf.hdf5lib.exceptions.HDF5LibraryException;

import org.scilab.modules.graph.utils.StyleMap;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.hdf5.read.H5Read;
import org.scilab.modules.hdf5.scilabTypes.ScilabBoolean;
import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;
import org.scilab.modules.hdf5.scilabTypes.ScilabList;
import org.scilab.modules.hdf5.scilabTypes.ScilabMList;
import org.scilab.modules.hdf5.scilabTypes.ScilabString;
import org.scilab.modules.hdf5.scilabTypes.ScilabTList;
import org.scilab.modules.hdf5.scilabTypes.ScilabType;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BlockFactory;
import org.scilab.modules.xcos.block.TextBlock;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.BasicPort.DataType;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.scilab.modules.xcos.port.output.OutputPort;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxGeometry;

/**
 * Utility class used to load Scicos diagram data. 
 */
public class BlockReader {

	/**
	 * Convert a Scicos MList (scs_m) to a map data.
	 * 
	 * @param data
	 *            the scs_m Scicos main structure
	 * @return the data
	 */
    public static Map<String, Object> convertMListToDiagram(ScilabMList data) {
	return convertMListToDiagram(data, true);
    }

	/**
	 * Validate the scs_m structure and convert it to a map data.
	 * 
	 * @param data
	 *            the raw Scicos structure
	 * @param checkVersion
	 *            true, if the version checking has to be performed; false
	 *            otherwise.
	 * @return the data
	 */
    public static Map<String, Object> convertMListToDiagram(ScilabMList data, boolean checkVersion) {

	try {
	    isAValidScs_mStructure(data, checkVersion);
	} catch (WrongTypeException e2) {
	    e2.printStackTrace();
	    return null;
	} catch (WrongStructureException e) {
	    e.printStackTrace();
	    return null;
	} catch (VersionMismatchException e) {
	    // UNKNOW_VERSION TRY_TO_CONTINUE
	    ScilabModalDialog.show(null, new String[] {
		    XcosMessages.UNKNOW_VERSION + e.getVersion(),
		    XcosMessages.TRY_TO_CONTINUE },
		    XcosMessages.FAIL_LOADING_DIAGRAM, IconType.ERROR_ICON);
	}

	Map<String, Object> result = new HashMap<String, Object>();
	Map<Integer, BasicBlock> indexedBlock = new HashMap<Integer, BasicBlock>();

	List<BasicBlock> blocks = new ArrayList<BasicBlock>();
	List<TextBlock> textBlocks = new ArrayList<TextBlock>();
	Map<String, Object> links = new HashMap<String, Object>();

	double minX = Double.MAX_VALUE;
	double minY = Double.MAX_VALUE;

	int nbObjs = getNbObjs(data);

	// Read diagrams properties
	Map<String, Object> properties = null;
	try {
	    properties = fillDiagrammProperties((ScilabTList) data.get(1));
	} catch (WrongStructureException e1) {
	    e1.printStackTrace();
	    return null;
	} catch (WrongTypeException e1) {
	    e1.printStackTrace();
	    return null;
	}
	result.put("Properties", properties);

	// Read all Blocks
	for (int i = 0; i < nbObjs; ++i) {
	    try {
		if (isBlock(data, i)) {
		    BasicBlock currentBlock = fillBlockStructure(getBlockAt(data, i));
		    currentBlock.setOrdering(i);
		    indexedBlock.put(i + 1, currentBlock);

		    // tips to set block direction at load
		    // "BLOCK_f;direction=east"
		    currentBlock.setStyle(currentBlock.getInterfaceFunctionName() + ";" + currentBlock.getStyle());
		    // currentBlock.setValue(currentBlock.getInterfaceFunctionName());

		    blocks.add(currentBlock);
		    minX = Math.min(minX, currentBlock.getGeometry().getX());
		    minY = Math.min(minY, currentBlock.getGeometry().getY());
		}
	    } catch (BlockReaderException e) {
		e.printStackTrace();
		return null;
	    }
	}

	List<BasicPort[]> linkPorts = new ArrayList<BasicPort[]>();
	List<double[][]> linkPoints = new ArrayList<double[][]>();
	// Read all Links
	for (int i = 0; i < nbObjs; ++i) {
	    if (isLink(data, i)) {
		try {

		    ScilabMList link = getLinkAt(data, i);
		    BasicPort startingPort = null;
		    BasicPort endingPort = null;

		    int startBlockIndex = getStartBlockIndex(link); // 6.0
		    int startPortIndex = getStartPortIndex(link); // 6.1
		    PortType startPortType = getStartPortType(link); // 5.1

		    int endBlockIndex = getEndBlockIndex(link); // 7.0
		    int endPortIndex = getEndPortIndex(link); // 7.1
		    PortType endPortType = getEndPortType(link); // 5.1

		    switch (startPortType) {
		    case INPUT:
			startingPort = BasicBlockInfo.getAllInputPorts(indexedBlock.get(startBlockIndex), false).get(startPortIndex - 1);
			break;
		    case OUTPUT:
			startingPort = BasicBlockInfo.getAllOutputPorts(indexedBlock.get(startBlockIndex), false).get(startPortIndex - 1);
			break;
		    case COMMAND:
			startingPort = BasicBlockInfo.getAllCommandPorts(indexedBlock.get(startBlockIndex), false).get(startPortIndex - 1);
			break;
		    case CONTROL:
			startingPort = BasicBlockInfo.getAllControlPorts(indexedBlock.get(startBlockIndex), false).get(startPortIndex - 1);
			break;
		    default:
			break;
		    }

		    switch (endPortType) {
		    case INPUT:
			endingPort = BasicBlockInfo.getAllInputPorts(indexedBlock.get(endBlockIndex), false).get(endPortIndex - 1);
			break;
		    case OUTPUT:
			endingPort = BasicBlockInfo.getAllOutputPorts(indexedBlock.get(endBlockIndex), false).get(endPortIndex - 1);
			break;
		    case COMMAND:
			endingPort = BasicBlockInfo.getAllCommandPorts(indexedBlock.get(endBlockIndex), false).get(endPortIndex - 1);
			break;
		    case CONTROL:
			endingPort = BasicBlockInfo.getAllControlPorts(indexedBlock.get(endBlockIndex), false).get(endPortIndex - 1);
			break;
		    default:
			break;
		    }

		    BasicPort[] startAndEnd = {startingPort, endingPort};
		    linkPorts.add(startAndEnd);

		    // First and last value -> start and end point
		    if (link.get(1).getHeight() > 2 || link.get(1).getWidth() > 2) {
			int maxDim = Math.max(link.get(1).getHeight(), link.get(1).getWidth());
			double[][] linkPoint = new double[maxDim - 2][2];
			for (int point = 0; point < maxDim - 2; point++) {
			    linkPoint[point] = getLinkPoint(link, point);
			    minX = Math.min(minX, linkPoint[point][0]);
			    minY = Math.min(minY, linkPoint[point][1]);
			}
			linkPoints.add(linkPoint);
		    } else {
			linkPoints.add(null);
		    }
		} catch (BlockReaderException e) {
		    e.printStackTrace();
		    return null;
		}
	    }
	}

	// Read all Labels
	for (int i = 0; i < nbObjs; ++i) {
	    try {
		if (isLabel(data, i)) {
		    TextBlock currentText = fillTextStructure(getBlockAt(data, i));
		    currentText.setOrdering(i);

		    // tips to set block direction at load
		    // "BLOCK_f;direction=east"
		    // currentText.setStyle(currentText.getInterfaceFunctionName()
		    // + currentText.getStyle());
		    if (isEmptyField(currentText.getRealParameters())) {
			currentText.setValue("");
		    } else {
			currentText.setValue(((ScilabString) currentText.getRealParameters()).getData()[0][0]);
		    }

		    textBlocks.add(currentText);
		    minX = Math.min(minX, currentText.getGeometry().getX());
		    minY = Math.min(minY, currentText.getGeometry().getY());
		}
	    } catch (BlockReaderException e) {
		e.printStackTrace();
		return null;
	    }

	}

	double offsetX = -minX + 20;
	double offsetY = -minY + 20;

	for (int i = 0; i < blocks.size(); ++i) {
	    blocks.get(i).getGeometry().setX(blocks.get(i).getGeometry().getX() + offsetX);
	    blocks.get(i).getGeometry().setY(blocks.get(i).getGeometry().getY() + offsetY);
	}

	for (int i = 0; i < linkPoints.size(); i++) {
	    if (linkPoints.get(i) != null) {
		for (int j = 0; j < linkPoints.get(i).length; j++) {
		    linkPoints.get(i)[j][0] += offsetX;
		    linkPoints.get(i)[j][1] += offsetY;
		}
	    }
	}

	for (int i = 0; i < textBlocks.size(); ++i) {
	    textBlocks.get(i).getGeometry().setX(textBlocks.get(i).getGeometry().getX() + offsetX);
	    textBlocks.get(i).getGeometry().setY(textBlocks.get(i).getGeometry().getY() + offsetY);
	}

	// put all data
	result.put("Blocks", blocks);
	result.put("TextBlocks", textBlocks);
	links.put("Ports", linkPorts);
	links.put("Points", linkPoints);
	result.put("Links", links);

	return result;
    }

    /**
     * Read diagram from a file.
     * 
     * @param hdf5File hdf5 file
     * @return diagram structure
     */
    public static Map<String, Object> readDiagramFromFile(String hdf5File) {
	ScilabMList data = new ScilabMList();

	try {
	    int fileId = H5Read.openFile(hdf5File);
	    if (fileId == -1) {
		throw new WrongStructureException();
	    }
	    H5Read.readDataFromFile(fileId, data);
	    H5Read.closeFile(fileId);

	    return convertMListToDiagram(data);
	} catch (HDF5LibraryException e) {
	    System.err.println("An error occurred while working with the HDF5 library " + e.getLocalizedMessage());
	} catch (WrongStructureException e) {
	    System.err.println("A HDF5 loading error occurred: Wrong Structure: " + e.getLocalizedMessage());
	} catch (HDF5JavaException e) {
	    System.err.println("A HDF5 loading error occurred: Error: " + e.getLocalizedMessage());
	} catch (HDF5Exception e) {
	    System.err.println("A HDF5 loading error occurred: Error: " + e.getLocalizedMessage());
	}
	return null;
    }

    /**
     * The first index of the block list.
     * 
     * @param link link structure
     * @return block index
     * @throws WrongTypeException wrong type
     */
    private static int getStartBlockIndex(ScilabMList link)
	    throws WrongTypeException {
	if (!(link.get(6) instanceof ScilabDouble)) {
	    throw new WrongTypeException();
	}

	return (int) ((ScilabDouble) link.get(6)).getRealPart()[0][0];
    }

    /**
     * Get the link point at the current index.
     * 
     * @param link link structure
     * @param index the current index
     * @return Java native link point descriptor
     * @throws WrongTypeException 
     */
    private static double[] getLinkPoint(ScilabMList link, int index)
	    throws WrongTypeException {
	if (!(link.get(1) instanceof ScilabDouble)) {
	    throw new WrongTypeException();
	}

	if (index < link.get(1).getHeight() - 2) {
	    double[] ret = new double[2];
	    ret[0] = ((ScilabDouble) link.get(1)).getRealPart()[index + 1][0];
	    ret[1] = -((ScilabDouble) link.get(2)).getRealPart()[index + 1][0];
	    return ret;
	} else if (index < link.get(1).getWidth() - 2) {
	    double[] ret = new double[2];
	    ret[0] = ((ScilabDouble) link.get(1)).getRealPart()[0][index + 1];
	    ret[1] = -((ScilabDouble) link.get(2)).getRealPart()[0][index + 1];
	    return ret;
	}
	return null;
    }

    /**
     * The first index of the port list.
     * 
     * @param link link structure
     * @return port index
     * @throws WrongTypeException wrong type
     */
    private static int getStartPortIndex(ScilabMList link)
	    throws WrongTypeException {
	if (!(link.get(6) instanceof ScilabDouble)) {
	    throw new WrongTypeException();
	}

	return (int) ((ScilabDouble) link.get(6)).getRealPart()[0][1];
    }

    /**
     * The port type on the link
     * 
     * @param link link structure
     * @return port type
     * @throws WrongTypeException wrong type
     */
    private static PortType getStartPortType(ScilabMList link)
	    throws WrongTypeException {
	if (!(link.get(5) instanceof ScilabDouble)) {
	    throw new WrongTypeException();
	}
	if (!(link.get(6) instanceof ScilabDouble)) {
	    throw new WrongTypeException();
	}

	// ct = [color, type] 1 : data , -1 event
	int type = (int) ((ScilabDouble) link.get(5)).getRealPart()[0][1];
	// from = [ blockId, portNumber, I/O] 0 : Output , 1 : Input
	
	int io = 0;
	//implicit way
	if(((ScilabDouble) link.get(6)).getRealPart()[0].length == 2) {
	    io = 0;
	} else {
	    io = (int) ((ScilabDouble) link.get(6)).getRealPart()[0][2];
	}

	return getPortType(type, io);
    }

    /**
     * Get the last block index
     * 
     * @param link link structure
     * @return the last index.
     * @throws WrongTypeException wrong type
     */
    private static int getEndBlockIndex(ScilabMList link)
	    throws WrongTypeException {
	if (!(link.get(7) instanceof ScilabDouble)) {
	    throw new WrongTypeException();
	}

	return (int) ((ScilabDouble) link.get(7)).getRealPart()[0][0];
    }

    /**
     * Get the last port index
     * 
     * @param link link structure
     * @return the last index.
     * @throws WrongTypeException wrong type
     */
    private static int getEndPortIndex(ScilabMList link)
	    throws WrongTypeException {
	if (!(link.get(7) instanceof ScilabDouble)) {
	    throw new WrongTypeException();
	}

	return (int) ((ScilabDouble) link.get(7)).getRealPart()[0][1];
    }

    /**
     * Get the last port type
     * 
     * @param link link structure
     * @return the last type.
     * @throws WrongTypeException wrong type
     */
    private static PortType getEndPortType(ScilabMList link)
	    throws WrongTypeException {
	if (!(link.get(5) instanceof ScilabDouble)) {
	    throw new WrongTypeException();
	}
	if (!(link.get(7) instanceof ScilabDouble)) {
	    throw new WrongTypeException();
	}

	// ct = [color, type] 1 : data , -1 event
	int type = (int) ((ScilabDouble) link.get(5)).getRealPart()[0][1];
	// from = [ blockId, portNumber, I/O] 0 : Output , 1 : Input
	int io = 0;
	
	//implicit way
	if(((ScilabDouble) link.get(7)).getRealPart()[0].length == 2) {
	    io = 1;
	} else {
	    io = (int) ((ScilabDouble) link.get(7)).getRealPart()[0][2];
	}

	return getPortType(type, io);
    }

    private static PortType getPortType(int type, int io)
	    throws WrongTypeException {
	if (type == 1 && io == 0) {
	    return PortType.OUTPUT;
	}
	if (type == 1 && io == 1) {
	    return PortType.INPUT;
	}
	if (type == 2 && io == 0) {
	    return PortType.OUTPUT;
	}
	if (type == 2 && io == 1) {
	    return PortType.INPUT;
	}
	if (type == -1 && io == 0) {
	    return PortType.COMMAND;
	}
	if (type == -1 && io == 1) {
	    return PortType.CONTROL;
	}

	throw new WrongTypeException();
    }

    /**
     * Read a block data from a file
     * @param hdf5file the file
     * @return the read block instance
     */
    public static BasicBlock readBlockFromFile(String hdf5file) {
	ScilabMList data = new ScilabMList();
	BasicBlock newBlock;

	try {
	    int fileId = H5Read.openFile(hdf5file);
	    // if (fileId == -1) { throw new WrongStructureException(); }
	    H5Read.readDataFromFile(fileId, data);
	    H5Read.closeFile(fileId);
	    newBlock = fillBlockStructure(data);
	    StyleMap style = new StyleMap(newBlock.getStyle());
	    style.put(newBlock.getInterfaceFunctionName(), null);
	    newBlock.setStyle(style.toString());
	    newBlock.setGeometry(new mxGeometry(newBlock.getGeometry().getX(),
		    newBlock.getGeometry().getY(), newBlock.getGeometry()
			    .getWidth(), newBlock.getGeometry().getHeight()));
	} catch (Exception e) {
	    e.printStackTrace();
	    newBlock = null;
	}
	return newBlock;
    }

    /**
     * Get the number of objects stored on the data.
     * @param data the data
     * @return the number of objects
     */
    public static int getNbObjs(ScilabMList data) {
	return ((ScilabList) data.get(2)).size();
    }

    private static boolean isBlock(ScilabMList data, int index) {
	ScilabMList object = (ScilabMList) ((ScilabList) data.get(2))
		.get(index);

	if (object.get(0) instanceof ScilabString
		&& ((ScilabString) object.get(0)).getData()[0][0]
			.equals("Block")) {
	    return true;
	}
	return false;
    }

    /**
     * Check if the block at the index is a label.
     * @param data the data
     * @param index the index
     * @return true, if data(index) is a label; false otherwise. 
     */
    public static boolean isLabel(ScilabMList data, int index) {
	ScilabMList object = (ScilabMList) ((ScilabList) data.get(2))
		.get(index);
	if (object.get(0) instanceof ScilabString
		&& ((ScilabString) object.get(0)).getData()[0][0]
			.equals("Text")) {

	    return true;
	} else {
	    return false;
	}

    }

    private static boolean isLink(ScilabMList data, int index) {
	ScilabMList object = (ScilabMList) ((ScilabList) data.get(2))
		.get(index);

	if (object.get(0) instanceof ScilabString
		&& ((ScilabString) object.get(0)).getData()[0][0]
			.equals("Link")) {
	    return true;
	}
	return false;
    }

    /**
     * Get the block at the index.
     * @param data the data
     * @param index the index
     * @return virtually data(index).
     */
    public static ScilabMList getBlockAt(ScilabMList data, int index) {
	return (ScilabMList) ((ScilabList) data.get(2)).get(index);
    }

    /**
     * Get the link at the index.
     * @param data the data
     * @param index the index
     * @return virtually data(index).
     */
    public static ScilabMList getLinkAt(ScilabMList data, int index) {
	return (ScilabMList) ((ScilabList) data.get(2)).get(index);
    }

    /**
     * Get the name field array
     * @param structure the data.
     * @return all the fields name
     */
    public static String[] getNameOfFieldsInStructure(ScilabMList structure) {
	return ((ScilabString) structure.get(0)).getData()[0];
    }

    private static String getBlockInterfaceName(ScilabMList blockFields) {
	return ((ScilabString) blockFields.get(3)).getData()[0][0];
    }

    private static String getBlockSimulationFunctionName(ScilabMList blockFields) {
	return ((ScilabString) blockFields.get(1)).getData()[0][0];
    }

    private static void isAValidScs_mStructure(ScilabMList data, boolean checkVersion)
	    throws WrongTypeException, VersionMismatchException,
	    WrongStructureException {

	int numberOfFieldInScs_m = 4;
	String[] realNameOfScs_mFields = new String[] { "diagram", "props",
		"objs", "version" };
	String realScicosVersion = new String("scicos4.2");

	// we test if the structure as enough field
	if (data.size() < numberOfFieldInScs_m) {
	    throw new WrongStructureException();
	}

	// the first field is a list of string containing the name of the other
	// fields
	if (!(data.get(0) instanceof ScilabString)) {
	    throw new WrongTypeException();
	}

	String[] nameOfScs_mFields = getNameOfFieldsInStructure(data);
	// check if all the expecting field's name are present
	if (nameOfScs_mFields.length < numberOfFieldInScs_m) {
	    throw new WrongStructureException();
	}
	for (int i = 0; i < numberOfFieldInScs_m; i++) {
	    if (!nameOfScs_mFields[i].equals(realNameOfScs_mFields[i])) {
		throw new WrongStructureException();
	    }
	}

	// the second field must contain list of props
	if (!(data.get(1) instanceof ScilabTList)) {
	    throw new WrongTypeException();
	}

	// the third field must contains lists of blocks and links
	if (!(data.get(2) instanceof ScilabList)) {
	    throw new WrongTypeException();
	}

	// the last field must contain the scicos version used
	if (!(data.get(3) instanceof ScilabString)) {
	    throw new WrongTypeException();
	}

	if(checkVersion) {
	    String scicosVersion = ((ScilabString) data.get(3)).getData()[0][0];
	    if (!scicosVersion.equals(realScicosVersion)) {
		throw new VersionMismatchException(scicosVersion);
	    }
	}
    }

    private static boolean isEmptyField(ScilabType object) {
	if (object instanceof ScilabDouble && ((ScilabDouble) object).isEmpty()) {
	    return true;
	}
	if (object instanceof ScilabList && ((ScilabList) object).isEmpty()) {
	    return true;
	}
	return false;
    }

    private static Map<String, Object> fillDiagrammProperties(
	    ScilabTList params) throws WrongStructureException,
	    WrongTypeException {
	Map<String, Object> diagramProperties = new HashMap<String, Object>();

	String[] propsFields = { "params", "wpar", "title", "tol", "tf",
		"context", "void1", "options", "void2", "void3", "doc" };
	// we test if the structure as enough field
	if (params.size() != propsFields.length) {
	    throw new WrongStructureException();
	}

	// check if all fields are present
	if (!(params.get(0) instanceof ScilabString)) {
	    throw new WrongStructureException();
	}
	String[] nameOfScs_mFields = ((ScilabString) params.get(0)).getData()[0];

	// check if all the expecting field's name are present
	if (nameOfScs_mFields.length != propsFields.length) {
	    throw new WrongStructureException();
	}
	for (int i = 0; i < propsFields.length; i++) {
	    if (!nameOfScs_mFields[i].equals(propsFields[i])) {
		throw new WrongStructureException();
	    }
	}

	// wpar
	if (!(params.get(1) instanceof ScilabDouble)) {
	    throw new WrongTypeException();
	}

	// title
	if (!(params.get(2) instanceof ScilabString)) {
	    throw new WrongTypeException();
	}
	diagramProperties.put("title",
		((ScilabString) params.get(2)).getData()[0][0]);

	// tol
	// !! WARNING !! parameters can be stored row or column wise
	if (!(params.get(3) instanceof ScilabDouble)) {
	    throw new WrongTypeException();
	}
	if (params.get(3).getHeight() >= params.get(3).getWidth()) {
	    diagramProperties.put("integratorAbsoluteTolerance",
		    ((ScilabDouble) params.get(3)).getRealPart()[0][0]);
	    diagramProperties.put("integratorRelativeTolerance",
		    ((ScilabDouble) params.get(3)).getRealPart()[1][0]);
	    diagramProperties.put("toleranceOnTime", ((ScilabDouble) params
		    .get(3)).getRealPart()[2][0]);
	    diagramProperties.put("maxIntegrationTimeinterval",
		    ((ScilabDouble) params.get(3)).getRealPart()[3][0]);
	    diagramProperties.put("realTimeScaling", ((ScilabDouble) params
		    .get(3)).getRealPart()[4][0]);
	    diagramProperties.put("solver", ((ScilabDouble) params.get(3))
		    .getRealPart()[5][0]);
	    if (params.get(3).getHeight() >= 7) {
		diagramProperties.put("maximumStepSize", ((ScilabDouble) params
			.get(3)).getRealPart()[6][0]);
	    } else {
		diagramProperties.put("maximumStepSize", 0.0);
	    }
	} else {
	    diagramProperties.put("integratorAbsoluteTolerance",
		    ((ScilabDouble) params.get(3)).getRealPart()[0][0]);
	    diagramProperties.put("integratorRelativeTolerance",
		    ((ScilabDouble) params.get(3)).getRealPart()[0][1]);
	    diagramProperties.put("toleranceOnTime", ((ScilabDouble) params
		    .get(3)).getRealPart()[0][2]);
	    diagramProperties.put("maxIntegrationTimeinterval",
		    ((ScilabDouble) params.get(3)).getRealPart()[0][3]);
	    diagramProperties.put("realTimeScaling", ((ScilabDouble) params
		    .get(3)).getRealPart()[0][4]);
	    diagramProperties.put("solver", ((ScilabDouble) params.get(3))
		    .getRealPart()[0][5]);
	    if (params.get(3).getWidth() >= 7) {
		diagramProperties.put("maximumStepSize", ((ScilabDouble) params
			.get(3)).getRealPart()[0][6]);
	    } else {
		diagramProperties.put("maximumStepSize", 0.0);
	    }
	}

	// tf
	if (!(params.get(4) instanceof ScilabDouble)) {
	    throw new WrongTypeException();
	}
	diagramProperties.put("finalIntegrationTime", ((ScilabDouble) params
		.get(4)).getRealPart()[0][0]);

	// context
	if (!(params.get(5) instanceof ScilabString)
		&& !isEmptyField(params.get(5))) {
	    throw new WrongTypeException();
	}
	ArrayList<String> context = new ArrayList<String>();
	if (params.get(5).getHeight() >= params.get(5).getWidth()) {
	    for (int i = 0; i < params.get(5).getHeight(); i++) {
		context.add(((ScilabString) params.get(5)).getData()[i][0]);
	    }
	} else {
	    for (int i = 0; i < params.get(5).getWidth(); i++) {
		context.add(((ScilabString) params.get(5)).getData()[0][i]);
	    }

	}
	diagramProperties.put("context", context.toArray(new String[context
		.size()]));

	// void1
	if (!isEmptyField(params.get(6))) {
	    throw new WrongTypeException();
	}

	// options
	if (!(params.get(7) instanceof ScilabTList)) {
	    throw new WrongTypeException();
	}

	// void2
	if (!isEmptyField(params.get(8))) {
	    throw new WrongTypeException();
	}

	// void3
	if (!isEmptyField(params.get(9))) {
	    throw new WrongTypeException();
	}

	// doc
	if (!(params.get(10) instanceof ScilabList)
		&& !isEmptyField(params.get(10))) {
	    throw new WrongTypeException();
	}

	return diagramProperties;
    }

    private static TextBlock fillTextStructure(ScilabMList blockFields)
	    throws WrongStructureException, WrongTypeException {
	String[] realNameOfTextFields = { "Text", "graphics", "model", "void",
		"gui" };

	TextBlock newBlock = (TextBlock) BlockFactory.createBlock("TEXT_f");
	// we test if the structure as enough field
	if (blockFields.size() != realNameOfTextFields.length) {
	    throw new WrongStructureException();
	}

	// check if all fields are present
	if (!(blockFields.get(0) instanceof ScilabString)) {
	    throw new WrongStructureException();
	}
	String[] nameOfScs_mFields = ((ScilabString) blockFields.get(0))
		.getData()[0];

	// check if all the expecting field's name are present
	if (nameOfScs_mFields.length != realNameOfTextFields.length) {
	    throw new WrongStructureException();
	}
	for (int i = 0; i < realNameOfTextFields.length; i++) {
	    if (!nameOfScs_mFields[i].equals(realNameOfTextFields[i])) {
		throw new WrongStructureException();
	    }
	}

	// the second field must contain list of all graphic property (how the
	// block will be displayed )
	if (!(blockFields.get(1) instanceof ScilabMList)) {
	    throw new WrongTypeException();
	}
	fillGraphicsStructure(blockFields, newBlock);

	// the third field must contains all the informations needed to compile
	// the block
	if (!(blockFields.get(2) instanceof ScilabMList)) {
	    throw new WrongTypeException();
	}
	fillTextModelStructure(blockFields, newBlock);

	return newBlock;

    }

    private static BasicBlock fillBlockStructure(ScilabMList blockFields)
	    throws WrongStructureException, WrongTypeException {
	String[] realNameOfBlockFields = { "Block", "graphics", "model", "gui",
		"doc" };

	// we test if the structure as enough field
	if (blockFields.size() != realNameOfBlockFields.length) {
	    throw new WrongStructureException();
	}

	// check if all fields are present
	if (!(blockFields.get(0) instanceof ScilabString)) {
	    throw new WrongStructureException();
	}
	String[] nameOfScs_mFields = ((ScilabString) blockFields.get(0))
		.getData()[0];

	// check if all the expecting field's name are present
	if (nameOfScs_mFields.length != realNameOfBlockFields.length) {
	    throw new WrongStructureException();
	}
	for (int i = 0; i < realNameOfBlockFields.length; i++) {
	    if (!nameOfScs_mFields[i].equals(realNameOfBlockFields[i])) {
		throw new WrongStructureException();
	    }
	}

	// !! WARNING !! create block with different type depending on the
	// interfaceFunction name.
	// the fourth field must contain the name of the interface function
	if (!(blockFields.get(3) instanceof ScilabString)) {
	    throw new WrongTypeException();
	}
	BasicBlock newBlock = BlockFactory.createBlock(getBlockInterfaceName(blockFields));
	// newBlock.setValue(getBlockInterfaceName (blockFields));
	newBlock.setInterfaceFunctionName(getBlockInterfaceName(blockFields));

	// the second field must contain list of all graphic property (how the
	// block will be displayed )
	if (!(blockFields.get(1) instanceof ScilabMList)) {
	    throw new WrongTypeException();
	}
	fillGraphicsStructure(blockFields, newBlock);

	// the third field must contains all the informations needed to compile
	// the block
	if (!(blockFields.get(2) instanceof ScilabMList)) {
	    throw new WrongTypeException();
	}
	fillModelStructure(blockFields, newBlock);

	// the last field must contain a list of nothing aka scicos doc
	if (!(blockFields.get(4) instanceof ScilabList)
		&& !isEmptyField(blockFields.get(4))) {
	    throw new WrongTypeException();
	}

	if (blockFields.get(4) instanceof ScilabList) {
	    ScilabList list = (ScilabList) blockFields.get(4);
	    if (list.size() > 0) {
		if (list.get(0) instanceof ScilabString) {
		    ScilabString uid = (ScilabString) list.get(0);
		    newBlock.setId(uid.getData()[0][0]);
		}
	    } else {
		newBlock.generateId();
	    }
	}
	return newBlock;
    }

    /**
     * Customize the block with the graphics fields
     * 
     * @param blockFields the raw data
     * @param newBlock the block to customize
     * @throws WrongTypeException when any type is not valid
     * @throws WrongStructureException when the structure is not valid
     */
    public static void fillGraphicsStructure(ScilabMList blockFields,
	    BasicBlock newBlock) throws WrongTypeException,
	    WrongStructureException {

	ScilabMList graphicsStructure = (ScilabMList) blockFields.get(1);

	String[] realNameOfStructureFields = new String[] { "graphics", "orig",
		"sz", "flip", "theta", "exprs", "pin", "pout", "pein", "peout",
		"gr_i", "id", "in_implicit", "out_implicit" };

	int numberOfFieldInStructure = realNameOfStructureFields.length;

	// we test if the structure as enough field
	if (graphicsStructure.size() != numberOfFieldInStructure) {
	    throw new WrongStructureException();
	}

	// the first field is a list of string containing the name of the other
	// fields
	if (!(graphicsStructure.get(0) instanceof ScilabString)) {
	    throw new WrongTypeException();
	}
	String[] nameOfStructureFields = getNameOfFieldsInStructure(graphicsStructure);

	// check if all the expecting field's name are present
	if (nameOfStructureFields.length == numberOfFieldInStructure) {
	    for (int i = 0; i < numberOfFieldInStructure; i++) {
		if (!nameOfStructureFields[i]
			.equals(realNameOfStructureFields[i])) {
		    throw new WrongStructureException();
		}
	    }
	}

	// orig : must contain the coord of the block
	if (!(graphicsStructure.get(1) instanceof ScilabDouble)) {
	    throw new WrongTypeException();
	}
	// !!! WARNING !!!
	// scicos can store [x,y] or [x;y] watch out !!!!
	double x = 0;
	double y = 0;
	if (graphicsStructure.get(1).getHeight() == 1
		&& graphicsStructure.get(1).getWidth() == 2) {
	    x = ((ScilabDouble) graphicsStructure.get(1)).getRealPart()[0][0];
	    y = ((ScilabDouble) graphicsStructure.get(1)).getRealPart()[0][1];
	} else {
	    if (graphicsStructure.get(1).getHeight() == 2
		    && graphicsStructure.get(1).getWidth() == 1) {
		x = ((ScilabDouble) graphicsStructure.get(1)).getRealPart()[0][0];
		y = ((ScilabDouble) graphicsStructure.get(1)).getRealPart()[1][0];
	    }
	}
	// Multiply size by 40 for better visualisation
	int sizeFactor = 1;
	newBlock.getGeometry().setX(sizeFactor * x);
	newBlock.getGeometry().setY(-sizeFactor * y);

	// sz : must contains the size of the block
	if (!(graphicsStructure.get(2) instanceof ScilabDouble)) {
	    throw new WrongTypeException();
	}
	// !!! WARNING !!!
	// scicos can store [width, height] or [width; height] watch out !!!!
	// sz = [width, height]
	double width = 0;
	double height = 0;
	if (graphicsStructure.get(2).getHeight() == 1
		&& graphicsStructure.get(2).getWidth() == 2) {
	    width = ((ScilabDouble) graphicsStructure.get(2)).getRealPart()[0][0];
	    height = ((ScilabDouble) graphicsStructure.get(2)).getRealPart()[0][1];
	} else {
	    if (graphicsStructure.get(2).getHeight() == 2
		    && graphicsStructure.get(2).getWidth() == 1) {
		width = ((ScilabDouble) graphicsStructure.get(2)).getRealPart()[0][0];
		height = ((ScilabDouble) graphicsStructure.get(2))
			.getRealPart()[1][0];
	    }
	}

	// Multiply size by 2 and fix 20 as minimal size so I can see "hidden"
	// blocks
	mxGeometry geom = newBlock.getGeometry();
	geom.setWidth(Math.max(sizeFactor * width, 40));
	geom.setHeight(Math.max(sizeFactor * height, 40));
	newBlock.setGeometry(geom);

	// Adjust block cause Scilab(0,0) is bottom left
	newBlock.getGeometry().setY(
		newBlock.getGeometry().getY()
			- newBlock.getGeometry().getHeight());

	if (!(graphicsStructure.get(3) instanceof ScilabBoolean)) {// flip
	    throw new WrongTypeException();
	}

	if (((ScilabBoolean) graphicsStructure.get(3)).getData()[0][0] == false) {
	    newBlock.setFlip(true);
	} else {
	    newBlock.setFlip(false);
	}

	if (!(graphicsStructure.get(4) instanceof ScilabDouble)) {// theta
	    throw new WrongTypeException();
	}

	int theta = (int) ((ScilabDouble) graphicsStructure.get(4))
		.getRealPart()[0][0];

	// convert negative value
	theta += 360;
	theta %= 360;

	if (theta > 315 || theta <= 45) {
	    newBlock.setAngle(0);
	} else if (theta > 45 && theta <= 135) {
	    newBlock.setAngle(90);
	} else if (theta > 135 && theta <= 225) {
	    newBlock.setAngle(180);
	} else if (theta > 225 && theta <= 315) {
	    newBlock.setAngle(270);
	}

	// exprs
	if (!(graphicsStructure.get(5) instanceof ScilabString)
		&& !(graphicsStructure.get(5) instanceof ScilabList)
		&& !(graphicsStructure.get(5) instanceof ScilabTList)
		&& !isEmptyField(graphicsStructure.get(5))) {
	    throw new WrongTypeException();
	}
	// exprs = ["a", "b", "c"]
	// if (graphicsStructure.get(5) instanceof ScilabString) {
	// for (int i = 0 ; i < graphicsStructure.get(5).getHeight() ; i++){
	// newBlock.getExprs().add(((ScilabString)graphicsStructure.get(5)).getData()[i][0]);
	// }
	newBlock.setExprs(graphicsStructure.get(5));
	// }
	// else {
	// TODO : See how to store it properly;
	// WARNING("exprs defined as Scilab List : Not managed !!!");
	// }

	// pin
	if (!(graphicsStructure.get(6) instanceof ScilabDouble)) {
	    throw new WrongTypeException();
	}

	// pout
	if (!(graphicsStructure.get(7) instanceof ScilabDouble)) {
	    throw new WrongTypeException();
	}

	// pein
	if (!(graphicsStructure.get(8) instanceof ScilabDouble)) {
	    throw new WrongTypeException();
	}

	// peout
	if (!(graphicsStructure.get(9) instanceof ScilabDouble)) {
	    throw new WrongTypeException();
	}

	// gr_i
	// !! WARNING !! we do not care about gr_i because there are only
	// block look related.
	// if (!(graphicsStructure.get(10) instanceof ScilabList)
	// && !(graphicsStructure.get(10) instanceof ScilabString)) {
	// throw new WrongTypeException();
	// }

	// id
	if (!(graphicsStructure.get(11) instanceof ScilabString)) {
	    throw new WrongTypeException();
	}

	// in_implicit
	if (!(graphicsStructure.get(12) instanceof ScilabString)
		&& !isEmptyField(graphicsStructure.get(12))) {
	    throw new WrongTypeException();
	}

	// out_implicit
	if (!(graphicsStructure.get(13) instanceof ScilabString)
		&& !isEmptyField(graphicsStructure.get(13))) {
	    throw new WrongTypeException();
	}
    }

    /**
     * Customize the block with the text model fields
     * 
     * @param blockFields the raw data
     * @param newBlock the block to customize
     * @throws WrongTypeException when any type is not valid
     * @throws WrongStructureException when the structure is not valid
     */
    public static void fillTextModelStructure(ScilabMList blockFields,
	    BasicBlock newBlock) throws WrongTypeException,
	    WrongStructureException {

	// graphicsStructure because some infromations about the port are stored
	// there too
	ScilabMList graphicsStructure = (ScilabMList) blockFields.get(1);
	ScilabMList modelFields = (ScilabMList) blockFields.get(2);

	String[] realNameOfStructureFields = new String[] { "model", "sim",
		"in", "in2", "intyp", "out", "out2", "outtyp", "evtin",
		"evtout", "state", "dstate", "rpar", "ipar", "blocktype",
		"firing", "dep_ut", "label", "nzcross", "nmode", "equations" };
	int numberOfFieldInStructure = realNameOfStructureFields.length;

	// we test if the structure as enough field
	if (modelFields.size() != numberOfFieldInStructure) {
	    String[] realNameOfBlockStructureFields = new String[] { "model",
		    "sim", "in", "in2", "intyp", "out", "out2", "outtyp",
		    "evtin", "evtout", "state", "dstate", "odstate", "rpar",
		    "ipar", "opar", "blocktype", "firing", "dep_ut", "label",
		    "nzcross", "nmode", "equations" };
	    int numberOfFieldInBlockStructure = realNameOfBlockStructureFields.length;
	    if (modelFields.size() != numberOfFieldInBlockStructure) {

		throw new WrongStructureException();
	    } else {
		fillModelStructure(blockFields, newBlock);
		return;
	    }
	}

	// the first field is a list of string containing the name of the other
	// fields
	if (!(modelFields.get(0) instanceof ScilabString)) {
	    throw new WrongTypeException();
	}

	String[] nameOfScs_mFields = ((ScilabString) modelFields.get(0))
		.getData()[0];

	// check if all the expecting field's name are present
	if (nameOfScs_mFields.length != numberOfFieldInStructure) {
	    throw new WrongStructureException();
	}
	for (int i = 0; i < numberOfFieldInStructure; i++) {
	    if (!nameOfScs_mFields[i].equals(realNameOfStructureFields[i])) {
		throw new WrongTypeException();
	    }
	}

	// sim : String or list(String, int)
	if (!(modelFields.get(1) instanceof ScilabDouble)
		&& !(modelFields.get(1) instanceof ScilabString)
		&& !(modelFields.get(1) instanceof ScilabList)) {
	    throw new WrongTypeException();
	}

	if (modelFields.get(1) instanceof ScilabString) {
	    newBlock
		    .setSimulationFunctionName(getBlockSimulationFunctionName(modelFields));
	}
	if ((modelFields.get(1) instanceof ScilabList)) {
	    newBlock
		    .setSimulationFunctionName(((ScilabString) ((ScilabList) modelFields
			    .get(1)).get(0)).getData()[0][0]);
	    newBlock
		    .setSimulationFunctionType((int) ((ScilabDouble) ((ScilabList) modelFields
			    .get(1)).get(1)).getRealPart()[0][0]);
	}

	// fill inputPort (in , in2 , intyp)
	if (!(modelFields.get(2) instanceof ScilabDouble)
		&& !(modelFields.get(3) instanceof ScilabDouble)
		&& !(modelFields.get(4) instanceof ScilabDouble)) {
	    throw new WrongTypeException();
	}

	int size = modelFields.get(2).getHeight();

	// in_implicit = []
	// by defaults Ports are Explicit
	if (isEmptyField(graphicsStructure.get(12))) {
	    for (int i = 0; i < size; i++) {
		ExplicitInputPort tempInputPort = new ExplicitInputPort();
		ScilabDouble dataLines = (ScilabDouble) modelFields.get(2);
		ScilabDouble dataColumns = (ScilabDouble) modelFields.get(3);

		if (dataLines.getRealPart() != null) {
		    int nbLines = (int) dataLines.getRealPart()[i][0];
		    tempInputPort.setDataLines(nbLines);
		}
		if (dataColumns.getRealPart() != null) {
		    int nbColumns = (int) dataColumns.getRealPart()[i][0];
		    tempInputPort.setDataColumns(nbColumns);
		}
		newBlock.addPort(tempInputPort);
	    }
	} else {
	    String[][] implicitExplicitInArray = ((ScilabString) graphicsStructure
		    .get(12)).getData();

	    for (int i = 0; i < size; i++) {
		InputPort tempInputPort = null;
		// "E" -> Explicit
		if (graphicsStructure.get(12).getHeight() > graphicsStructure
			.get(12).getWidth()) {
		    if (implicitExplicitInArray[i][0].equals("E")) {
			tempInputPort = new ExplicitInputPort();
		    }
		    if (implicitExplicitInArray[i][0].equals("I")) {
			tempInputPort = new ImplicitInputPort();
		    }
		} else {
		    if (implicitExplicitInArray[0][i].equals("E")) {
			tempInputPort = new ExplicitInputPort();
		    }
		    if (implicitExplicitInArray[0][i].equals("I")) {
			tempInputPort = new ImplicitInputPort();
		    }
		}
		ScilabDouble dataLines = (ScilabDouble) modelFields.get(2);
		ScilabDouble dataColumns = (ScilabDouble) modelFields.get(3);

		if (dataLines.getRealPart() != null) {
		    int nbLines = (int) dataLines.getRealPart()[i][0];
		    tempInputPort.setDataLines(nbLines);
		}
		if (dataColumns.getRealPart() != null) {
		    int nbColumns = (int) dataColumns.getRealPart()[i][0];
		    tempInputPort.setDataColumns(nbColumns);
		}
		newBlock.addPort(tempInputPort);
	    }
	}

	// fill outputPort (out , out2 , outtyp)
	if (!(modelFields.get(5) instanceof ScilabDouble)
		&& !(modelFields.get(6) instanceof ScilabDouble)
		&& !(modelFields.get(7) instanceof ScilabDouble)) {
	    throw new WrongTypeException();
	}

	size = modelFields.get(5).getHeight();

	// out_implicit = []
	// by defaults Ports are Explicit
	if (isEmptyField(graphicsStructure.get(13))) {
	    for (int i = 0; i < size; i++) {
		ExplicitOutputPort tempOutputPort = new ExplicitOutputPort();
		ScilabDouble dataLines = (ScilabDouble) modelFields.get(5);
		ScilabDouble dataColumns = (ScilabDouble) modelFields.get(6);

		if (dataLines.getRealPart() != null) {
		    int nbLines = (int) dataLines.getRealPart()[i][0];
		    tempOutputPort.setDataLines(nbLines);
		}
		if (dataColumns.getRealPart() != null) {
		    int nbColumns = (int) dataColumns.getRealPart()[i][0];
		    tempOutputPort.setDataColumns(nbColumns);
		}
		newBlock.addPort(tempOutputPort);
	    }
	} else {
	    String[][] implicitExplicitInArray = ((ScilabString) graphicsStructure
		    .get(13)).getData();

	    for (int i = 0; i < size; i++) {
		OutputPort tempOutputPort = null;
		if (graphicsStructure.get(13).getHeight() > graphicsStructure
			.get(13).getWidth()) {
		    if (implicitExplicitInArray[i][0].equals("E")) {
			tempOutputPort = new ExplicitOutputPort();
		    }
		    if (implicitExplicitInArray[i][0].equals("I")) {
			tempOutputPort = new ImplicitOutputPort();
		    }
		} else {
		    if (implicitExplicitInArray[0][i].equals("E")) {
			tempOutputPort = new ExplicitOutputPort();
		    }
		    if (implicitExplicitInArray[0][i].equals("I")) {
			tempOutputPort = new ImplicitOutputPort();
		    }
		}

		ScilabDouble dataLines = (ScilabDouble) modelFields.get(5);
		ScilabDouble dataColumns = (ScilabDouble) modelFields.get(6);

		if (dataLines.getRealPart() != null) {
		    int nbLines = (int) dataLines.getRealPart()[i][0];
		    tempOutputPort.setDataLines(nbLines);
		}
		if (dataColumns.getRealPart() != null) {
		    int nbColumns = (int) dataColumns.getRealPart()[i][0];
		    tempOutputPort.setDataColumns(nbColumns);
		}
		newBlock.addPort(tempOutputPort);
	    }
	}

	if (!(modelFields.get(8) instanceof ScilabDouble) && // evtin
		!(modelFields.get(9) instanceof ScilabDouble)) { // evtout
	    throw new WrongTypeException();
	}

	ScilabDouble dataNbControlPort = (ScilabDouble) modelFields.get(8);
	ScilabDouble dataNbCommandPort = (ScilabDouble) modelFields.get(9);

	if (dataNbControlPort.getRealPart() != null) {
	    int nbControlPort = dataNbControlPort.getHeight();
	    for (int i = 0; i < nbControlPort; i++) {
		newBlock.addPort(new ControlPort());
	    }
	}

	if (dataNbCommandPort.getRealPart() != null) {
	    int nbCommandPort = dataNbCommandPort.getHeight();
	    for (int i = 0; i < nbCommandPort; i++) {
		newBlock.addPort(new CommandPort());
	    }
	}

	if (!(modelFields.get(10) instanceof ScilabDouble) && // state
		!(modelFields.get(11) instanceof ScilabDouble)) { // dstate
	    // !(modelFields.get(12) instanceof ScilabList)) { //odstate
	    throw new WrongTypeException();
	}
	newBlock.setState(modelFields.get(10));
	newBlock.setDState(modelFields.get(11));
	// newBlock.setODState(modelFields.get(12));

	// rpar
	// SuperBlocks store all "included" data in rpar field.
	if (!(modelFields.get(12) instanceof ScilabDouble)
		&& !(modelFields.get(12) instanceof ScilabString)) {
	    throw new WrongTypeException();
	}

	newBlock.setRealParameters(modelFields.get(12));

	// ipar
	// !! WARNING !! scifunc_block_m ipar = list(...)
	if (!(modelFields.get(13) instanceof ScilabDouble)
		&& !(modelFields.get(13) instanceof ScilabList)) {
	    throw new WrongTypeException();
	}

	newBlock.setIntegerParameters(modelFields.get(13));

	// blocktype
	if (!(modelFields.get(14) instanceof ScilabString)) {
	    throw new WrongTypeException();
	}
	newBlock
		.setBlockType(((ScilabString) modelFields.get(14)).getData()[0][0]);

	// firing
	if (!(modelFields.get(15) instanceof ScilabDouble)
		&& !(modelFields.get(15) instanceof ScilabBoolean)) {
	    throw new WrongTypeException();
	}
	if (modelFields.get(15) instanceof ScilabDouble
		&& !isEmptyField(modelFields.get(15))) {
	    List<CommandPort> allCommandPorts = BasicBlockInfo
		    .getAllCommandPorts(newBlock, false);
	    if (modelFields.get(15).getHeight() >= modelFields.get(15)
		    .getWidth()) {
		for (int i = 0; i < allCommandPorts.size(); ++i) {
		    allCommandPorts.get(i)
			    .setInitialState(
				    ((ScilabDouble) modelFields.get(15))
					    .getRealPart()[i][0]);
		}
	    } else {
		for (int i = 0; i < allCommandPorts.size(); ++i) {
		    allCommandPorts.get(i)
			    .setInitialState(
				    ((ScilabDouble) modelFields.get(15))
					    .getRealPart()[0][i]);
		}
	    }
	}

	// dep-ut
	if (!(modelFields.get(16) instanceof ScilabBoolean)) {
	    throw new WrongTypeException();
	}
	newBlock
		.setDependsOnU(((ScilabBoolean) modelFields.get(16)).getData()[0][0]);
	newBlock
		.setDependsOnT(((ScilabBoolean) modelFields.get(16)).getData()[0][1]);

	// label
	if (!(modelFields.get(17) instanceof ScilabString)) {
	    throw new WrongTypeException();
	}

	// nzcross
	if (!(modelFields.get(18) instanceof ScilabDouble)) {
	    throw new WrongTypeException();
	}
	newBlock.setNbZerosCrossing(modelFields.get(18));

	// nmode
	if (!(modelFields.get(19) instanceof ScilabDouble)) {
	    throw new WrongTypeException();
	}
	newBlock.setNmode(modelFields.get(19));

	// equations
	if (!(modelFields.get(20) instanceof ScilabTList)
		&& !isEmptyField(modelFields.get(20))) {
	    throw new WrongTypeException();
	}
	newBlock.setEquations(modelFields.get(20));
    }

    /**
     * Customize the block with the model fields
     * 
     * @param blockFields the raw data
     * @param newBlock the block to customize
     * @throws WrongTypeException when any type is not valid
     * @throws WrongStructureException when the structure is not valid
     */
    public static void fillModelStructure(ScilabMList blockFields,
	    BasicBlock newBlock) throws WrongTypeException,
	    WrongStructureException {

	// graphicsStructure because some infromations about the port are stored
	// there too
	ScilabMList graphicsStructure = (ScilabMList) blockFields.get(1);
	ScilabMList modelFields = (ScilabMList) blockFields.get(2);

	String[] realNameOfStructureFields = new String[] { "model", "sim",
		"in", "in2", "intyp", "out", "out2", "outtyp", "evtin",
		"evtout", "state", "dstate", "odstate", "rpar", "ipar", "opar",
		"blocktype", "firing", "dep_ut", "label", "nzcross", "nmode",
		"equations" };
	int numberOfFieldInStructure = realNameOfStructureFields.length;

	// we test if the structure as enough field
	if (modelFields.size() != numberOfFieldInStructure) {
	    throw new WrongStructureException();
	}

	// the first field is a list of string containing the name of the other
	// fields
	if (!(modelFields.get(0) instanceof ScilabString)) {
	    throw new WrongTypeException();
	}

	String[] nameOfScs_mFields = ((ScilabString) modelFields.get(0))
		.getData()[0];

	// check if all the expecting field's name are present
	if (nameOfScs_mFields.length != numberOfFieldInStructure) {
	    throw new WrongStructureException();
	}
	for (int i = 0; i < numberOfFieldInStructure; i++) {
	    if (!nameOfScs_mFields[i].equals(realNameOfStructureFields[i])) {
		throw new WrongTypeException();
	    }
	}

	// sim : String or list(String, int)
	if (!(modelFields.get(1) instanceof ScilabString)
		&& !(modelFields.get(1) instanceof ScilabList)) {
	    throw new WrongTypeException();
	}

	if (modelFields.get(1) instanceof ScilabString) {
	    newBlock.setSimulationFunctionName(getBlockSimulationFunctionName(modelFields));
	}

	if ((modelFields.get(1) instanceof ScilabList)) {
	    newBlock.setSimulationFunctionName(((ScilabString) ((ScilabList) modelFields.get(1)).get(0)).getData()[0][0]);
	    newBlock.setSimulationFunctionType((int) ((ScilabDouble) ((ScilabList) modelFields.get(1)).get(1)).getRealPart()[0][0]);
	}

	// fill inputPort (in , in2 , intyp)
	if (!(modelFields.get(2) instanceof ScilabDouble)
		&& !(modelFields.get(3) instanceof ScilabDouble)
		&& !(modelFields.get(4) instanceof ScilabDouble)) {
	    throw new WrongTypeException();
	}

	int size = modelFields.get(2).getHeight();

	// in_implicit = []
	// by defaults Ports are Explicit
	if (isEmptyField(graphicsStructure.get(12))) {
	    for (int i = 0; i < size; i++) {
		ExplicitInputPort tempInputPort = new ExplicitInputPort();
		ScilabDouble dataLines = (ScilabDouble) modelFields.get(2);
		ScilabDouble dataColumns = (ScilabDouble) modelFields.get(3);
		ScilabDouble dataType = (ScilabDouble) modelFields.get(4);

		if (dataLines.getRealPart() != null) {
		    int nbLines = (int) dataLines.getRealPart()[i][0];
		    tempInputPort.setDataLines(nbLines);
		}
		if (dataColumns.getRealPart() != null) {
		    int nbColumns = (int) dataColumns.getRealPart()[i][0];
		    tempInputPort.setDataColumns(nbColumns);
		}
		if (dataType.getRealPart() != null) {
		    int type = (int) dataType.getRealPart()[0][0];
		    tempInputPort.setDataType(DataType.convertScilabValue(type));
		}
		newBlock.addPort(tempInputPort);
	    }
	} else {
	    String[][] implicitExplicitInArray = ((ScilabString) graphicsStructure.get(12)).getData();

	    for (int i = 0; i < size; i++) {
		InputPort tempInputPort = null;
		// "E" -> Explicit
		if (graphicsStructure.get(12).getHeight() > graphicsStructure.get(12).getWidth()
				&& i < graphicsStructure.get(12).getHeight()) {
		    if (implicitExplicitInArray[i][0].equals("E")) {
			tempInputPort = new ExplicitInputPort();
		    }
		    if (implicitExplicitInArray[i][0].equals("I")) {
			tempInputPort = new ImplicitInputPort();
		    }
		} else if(i < graphicsStructure.get(12).getWidth()){
		    if (implicitExplicitInArray[0][i].equals("E")) {
			tempInputPort = new ExplicitInputPort();
		    }
		    if (implicitExplicitInArray[0][i].equals("I")) {
			tempInputPort = new ImplicitInputPort();
		    }
		} else {
		    //we have more declared ports than definition
		    tempInputPort = new ExplicitInputPort();
		}

		ScilabDouble dataLines = (ScilabDouble) modelFields.get(2);
		ScilabDouble dataColumns = (ScilabDouble) modelFields.get(3);
		ScilabDouble dataType = (ScilabDouble) modelFields.get(4);

		if (dataLines.getRealPart() != null) {
		    int nbLines = (int) dataLines.getRealPart()[i][0];
		    tempInputPort.setDataLines(nbLines);
		}
		if (dataColumns.getRealPart() != null) {
		    int nbColumns = (int) dataColumns.getRealPart()[i][0];
		    tempInputPort.setDataColumns(nbColumns);
		}
		if (dataType.getRealPart() != null) {
		    int type = (int) dataType.getRealPart()[0][0];
		    tempInputPort.setDataType(DataType.convertScilabValue(type));
		}
		newBlock.addPort(tempInputPort);
	    }
	}

	// fill outputPort (out , out2 , outtyp)
	if (!(modelFields.get(5) instanceof ScilabDouble)
		&& !(modelFields.get(6) instanceof ScilabDouble)
		&& !(modelFields.get(7) instanceof ScilabDouble)) {
	    throw new WrongTypeException();
	}

	size = modelFields.get(5).getHeight();

	// out_implicit = []
	// by defaults Ports are Explicit
	if (isEmptyField(graphicsStructure.get(13))) {
	    for (int i = 0; i < size; i++) {
		ExplicitOutputPort tempOutputPort = new ExplicitOutputPort();
		ScilabDouble dataLines = (ScilabDouble) modelFields.get(5);
		ScilabDouble dataColumns = (ScilabDouble) modelFields.get(6);

		if (dataLines.getRealPart() != null) {
		    int nbLines = (int) dataLines.getRealPart()[i][0];
		    tempOutputPort.setDataLines(nbLines);
		}
		if (dataColumns.getRealPart() != null) {
		    int nbColumns = (int) dataColumns.getRealPart()[i][0];
		    tempOutputPort.setDataColumns(nbColumns);
		}
		newBlock.addPort(tempOutputPort);
	    }
	} else {
	    String[][] implicitExplicitOutArray = ((ScilabString) graphicsStructure.get(13)).getData();
	    
	    for (int i = 0; i < size; i++) {
		OutputPort tempOutputPort = null;
		if (graphicsStructure.get(13).getHeight() > graphicsStructure.get(13).getWidth() 
			&& i < graphicsStructure.get(13).getHeight()) {
		    if (implicitExplicitOutArray[i][0].equals("E")) {
			tempOutputPort = new ExplicitOutputPort();
		    }
		    if (implicitExplicitOutArray[i][0].equals("I")) {
			tempOutputPort = new ImplicitOutputPort();
		    }
		} else if(i < graphicsStructure.get(13).getWidth()){
		    if (implicitExplicitOutArray[0][i].equals("E")) {
			tempOutputPort = new ExplicitOutputPort();
		    }
		    if (implicitExplicitOutArray[0][i].equals("I")) {
			tempOutputPort = new ImplicitOutputPort();
		    }
		} else {
		    //we have more declared ports than definition
		    tempOutputPort = new ExplicitOutputPort();
		}

		ScilabDouble dataLines = (ScilabDouble) modelFields.get(5);
		ScilabDouble dataColumns = (ScilabDouble) modelFields.get(6);

		if (dataLines.getRealPart() != null) {
		    int nbLines = (int) dataLines.getRealPart()[i][0];
		    tempOutputPort.setDataLines(nbLines);
		}
		if (dataColumns.getRealPart() != null) {
		    int nbColumns = (int) dataColumns.getRealPart()[i][0];
		    tempOutputPort.setDataColumns(nbColumns);
		}
		newBlock.addPort(tempOutputPort);
	    }
	}

	if (!(modelFields.get(8) instanceof ScilabDouble) && // evtin
		!(modelFields.get(9) instanceof ScilabDouble)) { // evtout
	    throw new WrongTypeException();
	}

	ScilabDouble dataNbControlPort = (ScilabDouble) modelFields.get(8);
	ScilabDouble dataNbCommandPort = (ScilabDouble) modelFields.get(9);

	if (dataNbControlPort.getRealPart() != null) {
	    int nbControlPort = dataNbControlPort.getHeight();
	    for (int i = 0; i < nbControlPort; i++) {
		newBlock.addPort(new ControlPort());
	    }
	}

	if (dataNbCommandPort.getRealPart() != null) {
	    int nbCommandPort = dataNbCommandPort.getHeight();
	    for (int i = 0; i < nbCommandPort; i++) {
		newBlock.addPort(new CommandPort());
	    }
	}

	if (!(modelFields.get(10) instanceof ScilabDouble) && // state
		!(modelFields.get(11) instanceof ScilabDouble) && // dstate
		!(modelFields.get(12) instanceof ScilabList)) { // odstate
	    throw new WrongTypeException();
	}
	newBlock.setState(modelFields.get(10));
	newBlock.setDState(modelFields.get(11));
	newBlock.setODState(modelFields.get(12));

	// rpar
	// SuperBlocks store all "included" data in rpar field.
	if (!(modelFields.get(13) instanceof ScilabDouble)
		&& !(modelFields.get(13) instanceof ScilabMList)
		&& !(modelFields.get(13) instanceof ScilabString)) {
	    throw new WrongTypeException();
	}
	// if (modelFields.get(13) instanceof ScilabDouble) {
	// for (int i = 0; i < ((ScilabDouble)modelFields.get(13)).getHeight() ;
	// i++){
	// newBlock.getRealParameters().add(((ScilabDouble)modelFields.get(13)).getRealPart()[i][0]);
	// }
	// }
	// if (modelFields.get(13) instanceof ScilabList) {
	//
	// }
	newBlock.setRealParameters(modelFields.get(13));

	// ipar
	// !! WARNING !! scifunc_block_m ipar = list(...)
	if (!(modelFields.get(14) instanceof ScilabDouble)
		&& !(modelFields.get(14) instanceof ScilabList)) {
	    throw new WrongTypeException();
	}
	// for (int i = 0; i < ((ScilabDouble)modelFields.get(14)).getHeight() ;
	// i++){
	// newBlock.getIntegerParameters().add((int)
	// ((ScilabDouble)modelFields.get(14)).getRealPart()[i][0]);
	// }
	newBlock.setIntegerParameters(modelFields.get(14));

	// opar
	if (!(modelFields.get(15) instanceof ScilabList)) {
	    throw new WrongTypeException();
	}
	newBlock.setObjectsParameters(modelFields.get(15));

	// blocktype
	if (!(modelFields.get(16) instanceof ScilabString)) {
	    throw new WrongTypeException();
	}
	newBlock
		.setBlockType(((ScilabString) modelFields.get(16)).getData()[0][0]);

	// firing
	if (!(modelFields.get(17) instanceof ScilabDouble)
		&& !(modelFields.get(17) instanceof ScilabBoolean)) {
	    throw new WrongTypeException();
	}
	if (modelFields.get(17) instanceof ScilabDouble
		&& !isEmptyField(modelFields.get(17))) {
	    List<CommandPort> allCommandPorts = BasicBlockInfo
		    .getAllCommandPorts(newBlock, false);
	    if (modelFields.get(17).getHeight() >= modelFields.get(17)
		    .getWidth()) {
		for (int i = 0; i < allCommandPorts.size(); ++i) {
		    allCommandPorts.get(i)
			    .setInitialState(
				    ((ScilabDouble) modelFields.get(17))
					    .getRealPart()[i][0]);
		}
	    } else {
		for (int i = 0; i < allCommandPorts.size(); ++i) {
		    allCommandPorts.get(i)
			    .setInitialState(
				    ((ScilabDouble) modelFields.get(17))
					    .getRealPart()[0][i]);
		}
	    }
	}

	// dep-ut
	if (!(modelFields.get(18) instanceof ScilabBoolean)) {
	    throw new WrongTypeException();
	}
	newBlock
		.setDependsOnU(((ScilabBoolean) modelFields.get(18)).getData()[0][0]);
	newBlock
		.setDependsOnT(((ScilabBoolean) modelFields.get(18)).getData()[0][1]);

	// label
	if (!(modelFields.get(19) instanceof ScilabString)) {
	    throw new WrongTypeException();
	}

	// nzcross
	if (!(modelFields.get(20) instanceof ScilabDouble)) {
	    throw new WrongTypeException();
	}
	newBlock.setNbZerosCrossing(modelFields.get(20));

	// nmode
	if (!(modelFields.get(21) instanceof ScilabDouble)) {
	    throw new WrongTypeException();
	}
	newBlock.setNmode(modelFields.get(21));

	// equations
	if (!(modelFields.get(22) instanceof ScilabTList)
		&& !isEmptyField(modelFields.get(22))) {
	    throw new WrongTypeException();
	}
	newBlock.setEquations(modelFields.get(22));
    }

    
    private static class BlockReaderException extends Exception {
    };

    
    private static class WrongTypeException extends BlockReaderException {
    };

    
    private static class WrongStructureException extends BlockReaderException {
    };

    
    private static class VersionMismatchException extends BlockReaderException {
	private String version;

	public VersionMismatchException(String version) {
	    this.version = version;
	}

	public String getVersion() {
	    return version;
	}
    };

    private static enum PortType {
	INPUT, OUTPUT, CONTROL, COMMAND
    }

}
