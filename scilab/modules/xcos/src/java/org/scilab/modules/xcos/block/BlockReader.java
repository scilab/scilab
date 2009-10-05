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

package org.scilab.modules.xcos.block;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import org.scilab.modules.hdf5.read.H5Read;
import org.scilab.modules.hdf5.scilabTypes.ScilabBoolean;
import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;
import org.scilab.modules.hdf5.scilabTypes.ScilabList;
import org.scilab.modules.hdf5.scilabTypes.ScilabMList;
import org.scilab.modules.hdf5.scilabTypes.ScilabString;
import org.scilab.modules.hdf5.scilabTypes.ScilabTList;
import org.scilab.modules.hdf5.scilabTypes.ScilabType;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.scilab.modules.xcos.port.output.OutputPort;

import com.mxgraph.model.mxGeometry;

public class BlockReader {


    private static void INFO(String msg) {
	//System.err.println("[INFO] BlockReader : "+msg);
    }

    private static void WARNING(String msg) {
	System.err.println("[WARNING] BlockReader : "+msg);
    }


    private static void DEBUG(String msg) {
	System.err.println("[DEBUG] BlockReader : "+msg);
    }

    public static HashMap<String, List> readDiagramFromFile(String hdf5File) {
	ScilabMList data = new ScilabMList();

	HashMap<String, List> result = new HashMap<String, List>();
	HashMap<Integer, BasicBlock> indexedBlock = new HashMap<Integer, BasicBlock>(); 

	List<BasicBlock> blocks = new ArrayList<BasicBlock>();
	List<BasicPort[]> links = new ArrayList<BasicPort[]>();
	double minX = Double.MAX_VALUE;
	double minY = Double.MAX_VALUE;

	try {
	    int fileId = H5Read.openFile(hdf5File);
	    if (fileId == -1) { throw new WrongStructureException(); }
	    H5Read.readDataFromFile(fileId, data);
	    H5Read.closeFile(fileId);
	    if(!isAValidScs_mStructure(data)) { throw new WrongStructureException(); }
	    int nbObjs = getNbObjs(data);

	    // First read all Block
	    for (int i = 0 ; i < nbObjs ; ++i) {
		try {
		    if(isBlock(data, i)) {
			INFO("Reading Block "+ i);
			BasicBlock currentBlock = fillBlockStructure(getBlockAt(data, i));
			currentBlock.setOrdering(i);
			indexedBlock.put(i + 1, currentBlock);
			blocks.add(currentBlock);
			INFO("Block geometry "+currentBlock.getGeometry().getX()+" , "+currentBlock.getGeometry().getY());
			minX = Math.min(minX, currentBlock.getGeometry().getX());
			minY = Math.min(minY, currentBlock.getGeometry().getY());
		    }
		}
		catch (BlockReaderException e) {
		    WARNING(" Fail reading Block " + (i + 1));
		    e.printStackTrace();
		}
	    }
	    for (int i = 0 ; i < blocks.size() ; ++i) {
		blocks.get(i).getGeometry().setX(blocks.get(i).getGeometry().getX() + Math.abs(minX));
		blocks.get(i).getGeometry().setY(blocks.get(i).getGeometry().getY() + Math.abs(minY));
	    }
	    result.put("Blocks", blocks);

	    // Then read all Link
	    for (int i = 0 ; i < nbObjs ; ++i) {
		if(isLink(data, i)) {
		    try {
			INFO("Reading Link " + (i + 1));
			ScilabMList link = getLinkAt(data, i);
			BasicPort startingPort = null;
			BasicPort endingPort = null;
			
			int startBlockIndex = getStartBlockIndex(link);
			int startPortIndex = getStartPortIndex(link);
			PortType startPortType = getStartPortType(link);
			int endBlockIndex = getEndBlockIndex(link);
			int endPortIndex = getEndPortIndex(link);
			PortType endPortType = getEndPortType(link);

			DEBUG("Start : ["+startBlockIndex+", "+startPortIndex+", "+startPortType+"]");
			DEBUG("End : ["+endBlockIndex+", "+endPortIndex+", "+endPortType+"]");
			switch (startPortType) {
			case INPUT:
			    startingPort = indexedBlock.get(startBlockIndex).getAllInputPorts().get(startPortIndex - 1);
			    break;
			case OUTPUT:
			    startingPort = indexedBlock.get(startBlockIndex).getAllOutputPorts().get(startPortIndex - 1);
			    break;
			case COMMAND:
			    startingPort = indexedBlock.get(startBlockIndex).getAllCommandPorts().get(startPortIndex - 1);
			    break;
			case CONTROL:
			    startingPort = indexedBlock.get(startBlockIndex).getAllControlPorts().get(startPortIndex - 1);
			    break;
			}
			
			switch (endPortType) {
			case INPUT:
			    endingPort = indexedBlock.get(endBlockIndex).getAllInputPorts().get(endPortIndex - 1);
			    break;
			case OUTPUT:
			    endingPort = indexedBlock.get(endBlockIndex).getAllOutputPorts().get(endPortIndex - 1);
			    break;
			case COMMAND:
			    endingPort = indexedBlock.get(endBlockIndex).getAllCommandPorts().get(endPortIndex - 1);
			    break;
			case CONTROL:
			    endingPort = indexedBlock.get(endBlockIndex).getAllControlPorts().get(endPortIndex - 1);
			    break;
			}
			BasicPort[] startAndEnd = {startingPort, endingPort};
			links.add(startAndEnd);
		    }
		    catch (BlockReaderException e) {
			WARNING(" Fail reading Link "+(i+1));
			DEBUG(e.getStackTrace().toString());
		    }
		}
	    }
	    result.put("Links", links);

	    return result;
	}
	catch (Exception e) {
	    e.printStackTrace();
	}
	finally {
	    result.put("Blocks", blocks);

	    return result;
	}
    }

    private static int getStartBlockIndex(ScilabMList link) throws WrongTypeException {
	if (!(link.get(6) instanceof ScilabDouble)) { throw new WrongTypeException(); }

	return (int) ((ScilabDouble) link.get(6)).getRealPart()[0][0];
    }

    private static int getStartPortIndex(ScilabMList link) throws WrongTypeException {
	if (!(link.get(6) instanceof ScilabDouble)) { throw new WrongTypeException(); }

	return (int) ((ScilabDouble) link.get(6)).getRealPart()[0][1];
    }

    private static PortType getStartPortType(ScilabMList link) throws WrongTypeException {
	if (!(link.get(5) instanceof ScilabDouble)) { throw new WrongTypeException(); }
	if (!(link.get(6) instanceof ScilabDouble)) { throw new WrongTypeException(); }

	// ct  = [color, type] 1 : data , -1 event
	int type = (int) ((ScilabDouble) link.get(5)).getRealPart()[0][1];
	// from = [ blockId, portNumber, I/O] 0 : Output , 1 : Input
	int io = (int) ((ScilabDouble) link.get(6)).getRealPart()[0][2];

	return getPortType(type, io);
    }

    private static int getEndBlockIndex(ScilabMList link) throws WrongTypeException {
	if (!(link.get(7) instanceof ScilabDouble)) { throw new WrongTypeException(); }

	return (int) ((ScilabDouble) link.get(7)).getRealPart()[0][0];
    }

    private static int getEndPortIndex(ScilabMList link) throws WrongTypeException {
	if (!(link.get(7) instanceof ScilabDouble)) { throw new WrongTypeException(); }

	return (int) ((ScilabDouble) link.get(7)).getRealPart()[0][1];
    }

    private static PortType getEndPortType(ScilabMList link) throws WrongTypeException {
	if (!(link.get(5) instanceof ScilabDouble)) { throw new WrongTypeException(); }
	if (!(link.get(7) instanceof ScilabDouble)) { throw new WrongTypeException(); }

	// ct  = [color, type] 1 : data , -1 event
	int type = (int) ((ScilabDouble) link.get(5)).getRealPart()[0][1];
	// from = [ blockId, portNumber, I/O] 0 : Output , 1 : Input
	int io = (int) ((ScilabDouble) link.get(7)).getRealPart()[0][2];

	return getPortType(type, io);
    }

    private static PortType getPortType(int type, int io) throws WrongTypeException {
	if (type == 1 && io == 0) { return PortType.OUTPUT; }
	if (type == 1 && io == 1) { return PortType.INPUT; }
	if (type == -1 && io == 0) { return PortType.COMMAND; }
	if (type == -1 && io == 1) { return PortType.CONTROL; }

	throw new WrongTypeException();
    }


    public static BasicBlock readBlockFromFile( String hdf5file ) {
	ScilabMList data = new ScilabMList();
	BasicBlock newBlock = new BasicBlock("FAILED !!");

	try {
	    int fileId = H5Read.openFile(hdf5file);
	    //if (fileId == -1) { throw new WrongStructureException(); }
	    H5Read.readDataFromFile(fileId, data);
	    H5Read.closeFile(fileId);
	    newBlock = fillBlockStructure(data);
	    newBlock.setGeometry(new mxGeometry(newBlock.getGeometry().getX(),
		    newBlock.getGeometry().getY(),
		    newBlock.getGeometry().getWidth() * 2,
		    newBlock.getGeometry().getHeight() * 2));
	}
	catch (Exception e) {
	    // TODO Auto-generated catch block
	    DEBUG("FAIL importing "+hdf5file);
	    e.printStackTrace();
	}
	finally {
	    return  newBlock;
	}
    }

    public static int getNbObjs(ScilabMList data) {
	return ((ScilabList) data.get(2)).size();
    }

    private static boolean isBlock(ScilabMList data , int index) {
	ScilabMList object = (ScilabMList) ((ScilabList) data.get(2)).get(index);

	if(object.get(0) instanceof ScilabString && 
		((ScilabString) object.get(0)).getData()[0][0].equals("Block")) {
	    return true;
	}
	return false;
    }

    private static boolean isLink(ScilabMList data , int index) {
	ScilabMList object = (ScilabMList) ((ScilabList) data.get(2)).get(index);

	if(object.get(0) instanceof ScilabString && 
		((ScilabString) object.get(0)).getData()[0][0].equals("Link")) {
	    return true;
	}
	return false;
    }

    public static ScilabMList getBlockAt (ScilabMList data , int index ){
	return (ScilabMList)((ScilabList)data.get(2)).get(index);
    }

    public static ScilabMList getLinkAt (ScilabMList data , int index ){
	return (ScilabMList)((ScilabList)data.get(2)).get(index);
    }

    public static String[] getNameOfFieldsInStructure (ScilabMList structure ){
	return ((ScilabString)structure.get(0)).getData()[0];
    }

    public static String getBlockInterfaceName (ScilabMList blockFields ){
	return ((ScilabString)blockFields.get(3)).getData()[0][0];
    }

    public static String getBlockSimulationFunctionName(ScilabMList blockFields) {
	return ((ScilabString)blockFields.get(1)).getData()[0][0];
    }

    public static boolean isAValidScs_mStructure (ScilabMList data){

	int numberOfFieldInScs_m = 4 ;
	String[] realNameOfScs_mFields = new String[]{"diagram", "props" , "objs" , "version"};
	String realScicosVersion = new String ("scicos4.2");

	// we test if the structure as enough field
	if (data.size() == numberOfFieldInScs_m ){

	    // the first field is a list of string containing the name of the other fields
	    if ( data.get(0) instanceof ScilabString ) {
		String[] nameOfScs_mFields = getNameOfFieldsInStructure(data);

		// check if all the expecting field's name are present
		if ( nameOfScs_mFields.length == numberOfFieldInScs_m ){
		    for (int i = 0 ; i < numberOfFieldInScs_m ; i++){

			if ( !nameOfScs_mFields[i].equals(realNameOfScs_mFields[i]))
			    return false ;

		    }
		}
	    }
	    else{
		return false ;
	    }
	    // the second field must contain list of props
	    if (!( data.get(1) instanceof ScilabTList)) { 
		return false ;
	    }



	    // the third field must contains lists of blocks and links 

	    if (!( data.get(2) instanceof ScilabList)) { 
		return false ;
	    }


	    // the last field must contain the scicos version used 

	    if (( data.get(3) instanceof ScilabString)) { 
		String scicosVersion = ((ScilabString)data.get(3)).getData()[0][0];
		if(! scicosVersion.equals(realScicosVersion) ){
		    return false ;
		}
	    }
	    else{
		return false ;
	    }
	}
	else{
	    return false ;
	}

	return true ;
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

    private static BasicBlock  fillBlockStructure (ScilabMList blockFields) throws WrongStructureException, WrongTypeException {
	String[] realNameOfBlockFields = {"Block", "graphics" , "model" , "gui" , "doc"};

	// we test if the structure as enough field
	if (blockFields.size() != realNameOfBlockFields.length ) { throw new WrongStructureException(); }

	// check if all fields are present
	if (!(blockFields.get(0) instanceof ScilabString)) { throw new WrongStructureException(); }
	String[] nameOfScs_mFields = ((ScilabString)blockFields.get(0)).getData()[0];

	// check if all the expecting field's name are present
	if (nameOfScs_mFields.length != realNameOfBlockFields.length ) { throw new WrongStructureException(); }
	for (int i = 0 ; i < realNameOfBlockFields.length ; i++){
	    if (!nameOfScs_mFields[i].equals(realNameOfBlockFields[i])) {
		throw new WrongStructureException();
	    }
	}

	
	// !! WARNING !! create block with different type depending on the interfaceFunction name.
	// the fourth field must contain the name of the interface function
	if (!(blockFields.get(3) instanceof ScilabString)) { throw new WrongTypeException(); } 
	BasicBlock newBlock = BasicBlock.createBlock(getBlockInterfaceName (blockFields));
	newBlock.setValue(getBlockInterfaceName (blockFields));
	newBlock.setInterfaceFunctionName( getBlockInterfaceName (blockFields) );
	
	// the second field must contain list of all graphic property (how the block will be displayed )
	if (!(blockFields.get(1) instanceof ScilabMList)) { throw new WrongTypeException(); }
	fillGraphicsStructure( blockFields, newBlock);


	// the third field must contains all the informations needed to compile the block
	if (!(blockFields.get(2) instanceof ScilabMList)) { throw new WrongTypeException(); }
	fillModelStructure(blockFields, newBlock);

	// the last field must contain a list of nothing aka scicos doc
	if (!(blockFields.get(4) instanceof ScilabList) && !isEmptyField(blockFields.get(4))) { throw new WrongTypeException(); }
	
	return newBlock;
    }

    public static void fillGraphicsStructure (ScilabMList blockFields ,BasicBlock newBlock ) throws WrongTypeException, WrongStructureException{

	ScilabMList graphicsStructure =(ScilabMList)blockFields.get(1);

	String[] realNameOfStructureFields = new String[]{"graphics"	,"orig"	,"sz"	,"flip","theta","exprs","pin","pout","pein","peout","gr_i","id","in_implicit","out_implicit"};

	int numberOfFieldInStructure = realNameOfStructureFields.length;

	// we test if the structure as enough field
	if (graphicsStructure.size() != numberOfFieldInStructure ) { throw new WrongStructureException(); }

	// the first field is a list of string containing the name of the other fields
	if (!(graphicsStructure.get(0) instanceof ScilabString)) { throw new WrongTypeException(); }
	String[] nameOfStructureFields = getNameOfFieldsInStructure(graphicsStructure);

	// check if all the expecting field's name are present
	if (nameOfStructureFields.length == numberOfFieldInStructure ){
	    for (int i = 0 ; i < numberOfFieldInStructure ; i++){
		if ( !nameOfStructureFields[i].equals(realNameOfStructureFields[i])) {
		    throw new WrongStructureException();
		}
	    }
	}

	// orig : must contain the coord of the block
	if (!(graphicsStructure.get(1) instanceof ScilabDouble)) { throw new WrongTypeException(); }
	// !!! WARNING !!!
	// scicos can store [x,y] or [x;y] watch out !!!!
	double x = 0;
	double y = 0;
	if (graphicsStructure.get(1).getHeight() == 1 &&  graphicsStructure.get(1).getWidth() == 2) {
	    x = ((ScilabDouble) graphicsStructure.get(1)).getRealPart()[0][0];
	    y = ((ScilabDouble) graphicsStructure.get(1)).getRealPart()[0][1];
	}
	else {
	    if (graphicsStructure.get(1).getHeight() == 2 &&  graphicsStructure.get(1).getWidth() == 1) {
		x = ((ScilabDouble) graphicsStructure.get(1)).getRealPart()[0][0];
		y = ((ScilabDouble) graphicsStructure.get(1)).getRealPart()[1][0]; 
	    }
	}
	// Multiply size by 40 for better visualisation
	int sizeFactor = 1;
	newBlock.getGeometry().setX(sizeFactor * x);
	newBlock.getGeometry().setY(-sizeFactor * y);

	// sz : must contains the size of the block
	if (!(graphicsStructure.get(2) instanceof ScilabDouble)) { throw new WrongTypeException(); }
	// sz = [width, height]
	double width = ((ScilabDouble) graphicsStructure.get(2)).getRealPart()[0][0];
	double height = ((ScilabDouble) graphicsStructure.get(2)).getRealPart()[0][1];

	// Multiply size by 2 and fix 20 as minimal size so I can see "hidden" blocks
	newBlock.getGeometry().setWidth(Math.max(sizeFactor * width, 20));
	newBlock.getGeometry().setHeight(Math.max(sizeFactor * height, 20));

	// Adjust block cause Scilab(0,0) is bottom left
	newBlock.getGeometry().setY(newBlock.getGeometry().getY() - newBlock.getGeometry().getHeight()); 
	
	if (!((graphicsStructure.get(3) instanceof ScilabBoolean )&&// flip
		(graphicsStructure.get(4) instanceof ScilabDouble))) {// theta 
	    throw new WrongTypeException();
	}

	// exprs 
	if (!(graphicsStructure.get(5) instanceof ScilabString) 
		&& !(graphicsStructure.get(5) instanceof ScilabList)
		&& !(graphicsStructure.get(5) instanceof ScilabTList)
		&& !isEmptyField(graphicsStructure.get(5))) {
	    throw new WrongTypeException(); 
	}
	// exprs = ["a", "b", "c"]
	//if (graphicsStructure.get(5) instanceof ScilabString) {
	    //	    for (int i = 0 ; i < graphicsStructure.get(5).getHeight() ; i++){
	    //		newBlock.getExprs().add(((ScilabString)graphicsStructure.get(5)).getData()[i][0]);
	    //	    }
	    newBlock.setExprs(graphicsStructure.get(5));
	//}
	//else {
	    // TODO : See how to store it properly;
	    //WARNING("exprs defined as Scilab List : Not managed !!!");
	//}

	// pin
	if (!(graphicsStructure.get(6) instanceof ScilabDouble)) { throw new WrongTypeException(); }

	// pout
	if (!(graphicsStructure.get(7) instanceof ScilabDouble)) { throw new WrongTypeException(); }

	// pein
	if (!(graphicsStructure.get(8) instanceof ScilabDouble)) { throw new WrongTypeException(); }

	// peout
	if (!(graphicsStructure.get(9) instanceof ScilabDouble)) { throw new WrongTypeException(); }

	// gr_i
	// !! WARNING !! we do not care about gr_i because there are only
	// block look related.
	//	if (!(graphicsStructure.get(10) instanceof ScilabList)
//		&& !(graphicsStructure.get(10) instanceof ScilabString)) {
//	    throw new WrongTypeException(); 
//	}

	// id
	if (!(graphicsStructure.get(11) instanceof ScilabString))  { throw new WrongTypeException(); }

	// in_implicit
	if (!(graphicsStructure.get(12) instanceof ScilabString) && !isEmptyField(graphicsStructure.get(12))) { 
	    throw new WrongTypeException();
	}

	// out_implicit
	if (!(graphicsStructure.get(13) instanceof ScilabString) && !isEmptyField(graphicsStructure.get(13))) { 
	    throw new WrongTypeException();
	}
    }


    public static void fillModelStructure ( ScilabMList blockFields,BasicBlock newBlock ) throws WrongTypeException, WrongStructureException {

	// graphicsStructure because some infromations about the port are stored there too 
	ScilabMList graphicsStructure =(ScilabMList)blockFields.get(1);
	ScilabMList modelFields =(ScilabMList)blockFields.get(2);

	String[] realNameOfStructureFields = new String[]{"model", "sim", "in", "in2", "intyp", "out", "out2", "outtyp", "evtin", "evtout",
		"state", "dstate", "odstate", "rpar", "ipar", "opar", "blocktype", "firing", "dep_ut", "label",
		"nzcross", "nmode", "equations"};
	int numberOfFieldInStructure = realNameOfStructureFields.length;

	// we test if the structure as enough field
	if (modelFields.size() != numberOfFieldInStructure ) { throw new WrongStructureException(); }

	// the first field is a list of string containing the name of the other fields
	if (!(modelFields.get(0) instanceof ScilabString)) { throw new WrongTypeException(); }

	String[] nameOfScs_mFields = ((ScilabString)modelFields.get(0)).getData()[0];

	// check if all the expecting field's name are present
	if (nameOfScs_mFields.length != numberOfFieldInStructure ) { throw new WrongStructureException(); }
	for (int i = 0 ; i < numberOfFieldInStructure ; i++) {
	    if ( !nameOfScs_mFields[i].equals(realNameOfStructureFields[i])) {
		throw new WrongTypeException();
	    }
	}

	// sim : String or list(String, int)
	if (!(modelFields.get(1) instanceof ScilabString) && !(modelFields.get(1) instanceof ScilabList)) { throw new WrongTypeException(); }

	if (modelFields.get(1) instanceof ScilabString) { 
	    newBlock.setSimulationFunctionName(getBlockSimulationFunctionName(modelFields) );
	}
	if (( modelFields.get(1) instanceof ScilabList)){
	    newBlock.setSimulationFunctionName(((ScilabString)((ScilabList) modelFields.get(1)).get(0)).getData()[0][0]);   
	    newBlock.setSimulationFunctionType((int) ((ScilabDouble)((ScilabList) modelFields.get(1)).get(1)).getRealPart()[0][0]);
	}

	// fill inputPort (in , in2 , intyp)
	if (!(modelFields.get(2) instanceof ScilabDouble) &&
		!(modelFields.get(3) instanceof ScilabDouble) &&
		!(modelFields.get(4) instanceof ScilabDouble)) { throw new WrongTypeException(); }  


	int size = modelFields.get(2).getHeight();

	INFO("inputs = "+size);

	// in_implicit = []
	// by defaults Ports are Explicit
	if (isEmptyField(graphicsStructure.get(12))) {
	    for (int i = 0 ; i < size ; i++) {
		ExplicitInputPort tempInputPort =  new ExplicitInputPort();
		ScilabDouble dataLines = (ScilabDouble)modelFields.get(2);
		ScilabDouble dataColumns = (ScilabDouble)modelFields.get(3);

		if ( dataLines.getRealPart() != null ){
		    int nbLines = (int)dataLines.getRealPart()[i][0];
		    tempInputPort.setDataLines(nbLines);
		}
		if ( dataColumns.getRealPart() != null ){
		    int nbColumns = (int)dataColumns.getRealPart()[i][0];
		    tempInputPort.setDataColumns(nbColumns);
		}
		newBlock.addPort(tempInputPort);
	    }
	}
	else {
	    String[][] implicitExplicitInArray = ((ScilabString)graphicsStructure.get(12)).getData();

	    for (int i = 0 ; i < size ; i++) {
		InputPort tempInputPort = null;
		// "E" -> Explicit
		if (implicitExplicitInArray[i][0].equals("E")) {
		    tempInputPort =  new ExplicitInputPort();
		}
		if(implicitExplicitInArray[i][0].equals("I")) {
		    tempInputPort =  new ImplicitInputPort();
		}
		ScilabDouble dataLines = (ScilabDouble)modelFields.get(2);
		ScilabDouble dataColumns = (ScilabDouble)modelFields.get(3);

		if ( dataLines.getRealPart() != null ){
		    int nbLines = (int)dataLines.getRealPart()[i][0];
		    tempInputPort.setDataLines(nbLines);
		}
		if ( dataColumns.getRealPart() != null ){
		    int nbColumns = (int)dataColumns.getRealPart()[i][0];
		    tempInputPort.setDataColumns(nbColumns);
		}
		newBlock.addPort(tempInputPort);
	    }
	}

	// fill outputPort (out , out2 , outtyp)
	if (!(modelFields.get(5) instanceof ScilabDouble) &&
		!(modelFields.get(6) instanceof ScilabDouble) &&
		!(modelFields.get(7) instanceof ScilabDouble)) { throw new WrongTypeException(); }

	size = modelFields.get(5).getHeight();

	INFO("outputs = "+size);

	// out_implicit = []
	// by defaults Ports are Explicit
	if (isEmptyField(graphicsStructure.get(13))) {
	    for (int i = 0 ; i < size ; i++) {
		ExplicitOutputPort tempOutputPort =  new ExplicitOutputPort();
		ScilabDouble dataLines = (ScilabDouble)modelFields.get(5);
		ScilabDouble dataColumns = (ScilabDouble)modelFields.get(6);


		if ( dataLines.getRealPart() != null ){
		    int nbLines = (int)dataLines.getRealPart()[i][0];
		    tempOutputPort.setDataLines(nbLines);
		}
		if ( dataColumns.getRealPart() != null ){
		    int nbColumns = (int)dataColumns.getRealPart()[i][0];
		    tempOutputPort.setDataColumns(nbColumns);
		}
		newBlock.addPort(tempOutputPort);
	    }
	}
	else {
	    String[][] implicitExplicitInArray = ((ScilabString)graphicsStructure.get(13)).getData();

	    for (int i = 0 ; i < size ; i++){
		OutputPort tempOutputPort = null;
		if (implicitExplicitInArray[i][0].equals("E")) {
		    tempOutputPort =  new ExplicitOutputPort();
		}
		if(implicitExplicitInArray[i][0].equals("I")){
		    tempOutputPort =  new ImplicitOutputPort();
		}
		ScilabDouble dataLines = (ScilabDouble)modelFields.get(5);
		ScilabDouble dataColumns = (ScilabDouble)modelFields.get(6);


		if ( dataLines.getRealPart() != null ){
		    int nbLines = (int)dataLines.getRealPart()[i][0];
		    tempOutputPort.setDataLines(nbLines);
		}
		if ( dataColumns.getRealPart() != null ){
		    int nbColumns = (int)dataColumns.getRealPart()[i][0];
		    tempOutputPort.setDataColumns(nbColumns);
		}
		newBlock.addPort(tempOutputPort);
	    }
	}



	if (!(modelFields.get(8) instanceof ScilabDouble) && //evtin
		!(modelFields.get(9) instanceof ScilabDouble)) { // evtout
	    throw new WrongTypeException();
	}
	
	ScilabDouble dataNbControlPort = (ScilabDouble)modelFields.get(8);
	ScilabDouble dataNbCommandPort = (ScilabDouble)modelFields.get(9);

	if (dataNbControlPort.getRealPart() != null ) { 
	    int nbControlPort = dataNbControlPort.getHeight();
	    for (int i = 0 ; i < nbControlPort ; i++) {
		newBlock.addPort(new ControlPort());
	    }
	}

	if (dataNbCommandPort.getRealPart() != null ) { 
	    int nbCommandPort = dataNbCommandPort.getHeight();
	    for (int i = 0 ; i < nbCommandPort ; i++) {
		newBlock.addPort(new CommandPort());
	    }
	}

	if (!(modelFields.get(10) instanceof ScilabDouble) && // state
		!(modelFields.get(11) instanceof ScilabDouble) && // dstate
		!(modelFields.get(12) instanceof ScilabList)) { //odstate
	    throw new WrongTypeException();
	}
	newBlock.setState(modelFields.get(10));
	newBlock.setDState(modelFields.get(11));
	newBlock.setODState(modelFields.get(12));

	// rpar
	// SuperBlocks store all "included" data in rpar field.
	if (!(modelFields.get(13) instanceof ScilabDouble) && !(modelFields.get(13) instanceof ScilabMList)) { throw new WrongTypeException(); }
//	if (modelFields.get(13) instanceof ScilabDouble) {
//	    for (int i = 0; i < ((ScilabDouble)modelFields.get(13)).getHeight() ; i++){
//		newBlock.getRealParameters().add(((ScilabDouble)modelFields.get(13)).getRealPart()[i][0]); 
//	    }
//	}
//	if (modelFields.get(13) instanceof ScilabList) {
//
//	}
	newBlock.setRealParameters(modelFields.get(13));
	

	// ipar
	if (!(modelFields.get(14) instanceof ScilabDouble)) { throw new WrongTypeException(); }
	//	for (int i = 0; i < ((ScilabDouble)modelFields.get(14)).getHeight() ; i++){
	//	    newBlock.getIntegerParameters().add((int) ((ScilabDouble)modelFields.get(14)).getRealPart()[i][0]); 
	//	}
	newBlock.setIntegerParameters(modelFields.get(14));

	// opar
	if (!(modelFields.get(15) instanceof ScilabList)) { throw new WrongTypeException(); }
	newBlock.setObjectsParameters(modelFields.get(15));
	
	//blocktype
	if (!(modelFields.get(16) instanceof ScilabString)) { throw new WrongTypeException(); }
	newBlock.setBlockType(((ScilabString) modelFields.get(16)).getData()[0][0]);

	//firing
	if (!(modelFields.get(17) instanceof ScilabDouble)
		&& !(modelFields.get(17) instanceof ScilabBoolean)) { 
	    throw new WrongTypeException(); 
	}
	
	
	// dep-ut
	if (!(modelFields.get(18) instanceof ScilabBoolean)){ throw new WrongTypeException(); }
	newBlock.setDependsOnU( ((ScilabBoolean)modelFields.get(18)).getData()[0][0]);
	newBlock.setDependsOnT( ((ScilabBoolean)modelFields.get(18)).getData()[0][1]);

	// label
	if (!(modelFields.get(19) instanceof ScilabString)) { throw new WrongTypeException(); }

	// nzcross
	if (!(modelFields.get(20) instanceof ScilabDouble)) { throw new WrongTypeException(); }
	newBlock.setNbZerosCrossing(modelFields.get(20));

	//nmode 
	if (!(modelFields.get(21) instanceof ScilabDouble)) { throw new WrongTypeException(); }
	newBlock.setNmode(modelFields.get(21));
	
	// equations
	if (!(modelFields.get(22) instanceof ScilabTList)
		&& !isEmptyField(modelFields.get(22))) 
	{ 
	    throw new WrongTypeException(); 
	}
    }

    private static class BlockReaderException extends Exception { };
    private static class WrongTypeException extends BlockReaderException { };
    private static class WrongStructureException extends BlockReaderException { };

    private static enum PortType {
	INPUT,
	OUTPUT,
	CONTROL,
	COMMAND
    }

}



