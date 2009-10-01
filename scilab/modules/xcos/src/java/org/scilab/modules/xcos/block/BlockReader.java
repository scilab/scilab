/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan SIMON
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
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;

public class BlockReader {

	public static HashMap<String, List> readDiagramFromFile(String hdf5File) {
		ScilabMList data = new ScilabMList();
		HashMap<String, List> result = new HashMap<String, List>();
		
		double minX = Double.MAX_VALUE;
		double minY = Double.MAX_VALUE;
		
		try {
			int fileId = H5Read.openFile(hdf5File);
			H5Read.readDataFromFile(fileId, data);
			
			if(isAValidScs_mStructure(data)) {
				int nbBlocks = getNbBlocks(data);
				List<BasicBlock> blocks = new ArrayList<BasicBlock>();
				
				for (int i = 0 ; i < nbBlocks ; ++i) {
					BasicBlock currentBlock = new BasicBlock("temp");
					System.err.println("Reading Block "+ i);
					fillBlockStructure(getBlockAt(data, i), currentBlock);
					blocks.add(currentBlock);
					System.err.println("Block geometry "+currentBlock.getGeometry().getX()+" , "+currentBlock.getGeometry().getY());
					minX = Math.min(minX, currentBlock.getGeometry().getX());
					minY = Math.min(minY, currentBlock.getGeometry().getY());
				}
				
				for (int i = 0 ; i < blocks.size() ; ++i) {
				    blocks.get(i).getGeometry().setX(blocks.get(i).getGeometry().getX() + Math.abs(minX));
				    blocks.get(i).getGeometry().setY(blocks.get(i).getGeometry().getY() + Math.abs(minY));
				}
				result.put("Blocks", blocks);
				
				return result;
			}
		}
		catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return  null;
	}
	
	
	public static BasicBlock readBlockFromFile( String hdf5file ) {
		ScilabMList data = new ScilabMList();
		BasicBlock newBlock = new BasicBlock("temp");
		
		try {
			int fileId = H5Read.openFile(hdf5file);
			H5Read.readDataFromFile(fileId, data);

			if (!fillBlockStructure(data , newBlock)){
				return null ;
			}
		}
		catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return  newBlock;
	}

	public static int getNbBlocks(ScilabMList data) {
		ScilabList allBlockAndLink = getListOfBlockAndLink(data);
//		int j = 0;
//		while (j < allBlockAndLink.size()) {
//			if (((ScilabString) ((ScilabMList) allBlockAndLink.get(j)).get(0)).getData()[0][0].equals("Block")) {
//				++j;
//			}
//			else {
//				break;
//			}
//		}
		return allBlockAndLink.size();
	}
	
	public static ScilabList getListOfBlockAndLink (ScilabMList data){
		return (ScilabList)data.get(2);		
	}
	
	public static ScilabMList getBlockAt (ScilabMList data , int index ){
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

	public static boolean fillBlockStructure ( ScilabMList blockFields , BasicBlock newBlock ) {
		
		int numberOfFieldInBlock = 5 ;
		String[] realNameOfBlockFields = new String[]{"Block", "graphics" , "model" , "gui" , "doc"};

		
		// we test if the structure as enough field
		if (blockFields.size() == numberOfFieldInBlock ){
			
			// the first field is a list of string containing the name of the other fields
			if (blockFields.get(0) instanceof ScilabString ) {
				String[] nameOfScs_mFields = ((ScilabString)blockFields.get(0)).getData()[0];
				
				// check if all the expecting field's name are present
				if ( nameOfScs_mFields.length == numberOfFieldInBlock ){
					for (int i = 0 ; i < numberOfFieldInBlock ; i++){
						if ( !nameOfScs_mFields[i].equals(realNameOfBlockFields[i])) {
								return false ;
						}
					}
				}
			}
			else{
				return false ;
			}
			// the second field must contain list of all graphic property (how the block will be displayed )
			if (( blockFields.get(1) instanceof ScilabMList)) { 
				fillGraphicsStructure( blockFields, newBlock);
			}
			else{
				return false ;
			}
 
			// the third field must contains all the informations needed to compile the block
			
			if ((blockFields.get(2) instanceof ScilabMList)) { 
				fillModelStructure(blockFields, newBlock);
			}
			else{
				return false ;
			}
			
			
			// the fourth field must contain the name of the interface function
			
			if (( blockFields.get(3) instanceof ScilabString)) { 
				newBlock.setValue(getBlockInterfaceName (blockFields));
				newBlock.setInterfaceFunctionName( getBlockInterfaceName (blockFields) );
			}
			else{
				return false ;
			}
			
			// the last field must contain a list of nothing aka scicos doc
			if (!(blockFields.get(4) instanceof ScilabList)) { 
				return false ;
			}
		}
		else {
		    return false;
		}
		
		return true ;
	}
	
	public static boolean fillGraphicsStructure (ScilabMList blockFields ,BasicBlock newBlock ){

		ScilabMList graphicsStructure =(ScilabMList)blockFields.get(1);
		
		String[] realNameOfStructureFields = new String[]{"graphics"	,"orig"	,"sz"	,"flip","theta","exprs","pin","pout","pein","peout","gr_i","id","in_implicit","out_implicit"};
		
		int numberOfFieldInStructure = realNameOfStructureFields.length;
		
		// we test if the structure as enough field
		if (graphicsStructure.size() == numberOfFieldInStructure ){
			
			// the first field is a list of string containing the name of the other fields
			if (graphicsStructure.get(0) instanceof ScilabString ) {
				String[] nameOfStructureFields = getNameOfFieldsInStructure(graphicsStructure);
				
				// check if all the expecting field's name are present
				if (nameOfStructureFields.length == numberOfFieldInStructure ){
					for (int i = 0 ; i < numberOfFieldInStructure ; i++){
						
						if ( !nameOfStructureFields[i].equals(realNameOfStructureFields[i])) {
								return false ;
						}
					}
				}
			}
			else{
				return false ;
			}
			
			if (graphicsStructure.get(1) instanceof ScilabDouble) {// orig : must contain the coord of the block
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
			    
			    System.err.println("[x , y] == ["+x+" , "+y+"]");
			    
			    
			    newBlock.getGeometry().setX(x);
			    newBlock.getGeometry().setY(-y);
			    
			}
			else {
			    return false ;
			}
			if (!((graphicsStructure.get(2) instanceof ScilabDouble)&&// sz : must contains the size of the block
				(graphicsStructure.get(3) instanceof ScilabBoolean )&&// flip
				(graphicsStructure.get(4) instanceof ScilabDouble)// theta
				)) { 
				return false ;
			}
			
			// exprs 
			if ((graphicsStructure.get(5) instanceof ScilabString)) { 
				
				int size = graphicsStructure.get(5).getHeight();
				
				for (int i = 0 ; i < size ; i++){
					newBlock.getExprs().add(((ScilabString)graphicsStructure.get(5)).getData()[i][0]);
				}
			}
			else{
				return false ;
			}
			
			// pin
			if (!(graphicsStructure.get(6) instanceof ScilabDouble)) { 
				return false ;
			}
			
			// pout
			if (!(graphicsStructure.get(7) instanceof ScilabDouble)) { 
				return false ;
			}
			
			// pein
			if (!(graphicsStructure.get(8) instanceof ScilabDouble)) { 
				return false ;
			}
			
			// peout
			if (!(graphicsStructure.get(9) instanceof ScilabDouble)) { 
				return false ;
			}
			
			// gr_i
			if (!(graphicsStructure.get(10) instanceof ScilabList)) { 
				return false ;
			}
			
			// id
			if (!(graphicsStructure.get(11) instanceof ScilabString)) { 
				return false ;
			}
			
			// in_implicit
			if ((graphicsStructure.get(12) instanceof ScilabString)) { 
				String[] implicitExplicitInArray = ((ScilabString)graphicsStructure.get(12)).getData()[0];
				int size =  implicitExplicitInArray.length ;
				
			}
			else {
				if(!(graphicsStructure.get(12) instanceof ScilabDouble)) {
					return false ;
				}
			}
			
			// out_implicit
			if (!(graphicsStructure.get(13) instanceof ScilabString)) { 
				return false ;
			}
		}
		
		return true ;
	}
	
	
	public static boolean fillModelStructure ( ScilabMList blockFields,BasicBlock newBlock ){

		// graphicsStructure because some infromations about the port are stored there too 
		ScilabMList graphicsStructure =(ScilabMList)blockFields.get(1);
		ScilabMList modelFields =(ScilabMList)blockFields.get(2);
		
		
		String[] realNameOfStructureFields = new String[]{"model", "sim", "in", "in2", "intyp", "out", "out2", "outtyp", "evtin", "evtout",
				"state", "dstate", "odstate", "rpar", "ipar", "opar", "blocktype", "firing", "dep_ut", "label",
				"nzcross", "nmode", "equations"};
		int numberOfFieldInStructure = realNameOfStructureFields.length;
		
		// we test if the structure as enough field
		if (modelFields.size() == numberOfFieldInStructure ){
			
			// the first field is a list of string containing the name of the other fields
			if (modelFields.get(0) instanceof ScilabString ) {
				String[] nameOfScs_mFields = ((ScilabString)modelFields.get(0)).getData()[0];
				
				// check if all the expecting field's name are present
				if ( nameOfScs_mFields.length == numberOfFieldInStructure ){
					for (int i = 0 ; i < numberOfFieldInStructure ; i++){
						
						if ( !nameOfScs_mFields[i].equals(realNameOfStructureFields[i])) {
								return false ;
						}
					}
				}
			}
			else{
				return false ;
			}
				
			// sim
			if (( modelFields.get(1) instanceof ScilabString)) { 
				
				newBlock.setSimulationFunctionName(getBlockSimulationFunctionName(modelFields) );
				
			}
			else {
				if (( modelFields.get(1) instanceof ScilabList)){
					newBlock.setSimulationFunctionName(((ScilabString)((ScilabList) modelFields.get(1)).get(0)).getData()[0][0]);   
					newBlock.setSimulationFunctionType((int) ((ScilabDouble)((ScilabList) modelFields.get(1)).get(1)).getRealPart()[0][0]);
				}
				else{
					return false ;
				}
			}
 
			// fill inputPort (in , in2 , intyp)
			if ((modelFields.get(2) instanceof ScilabDouble) &&
				(modelFields.get(3) instanceof ScilabDouble) &&
				(modelFields.get(4) instanceof ScilabDouble)) { 
				
				
				int size = graphicsStructure.get(12).getHeight();
				
				// if size == 0  graphicsStructure.get(12) is considered as ScilabDouble
				if (size != 0 ){ 
				
					String[][] implicitExplicitInArray = ((ScilabString)graphicsStructure.get(12)).getData();
					
					for (int i = 0 ; i < size ; i++){
						if (implicitExplicitInArray[i][0].equals("E")){
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
							
							//tempExpInputPort.setDataType( ((ScilabDouble)modelFields.get(3)).getRealPart()[0][i]);
							newBlock.addPort(tempInputPort);
						}else if(implicitExplicitInArray[i][0].equals("I")){
							
							ImplicitInputPort tempInputPort =  new ImplicitInputPort();
						
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
							
							//tempExpInputPort.setDataType( ((ScilabDouble)modelFields.get(3)).getRealPart()[0][i]);
							newBlock.addPort(tempInputPort);
							
						}else{
							return false ;
						}
						
					}
				}
			}else{
				return false ;
			}
			
			
			
			// fill outputPort (out , out2 , outtyp)
			if ((modelFields.get(5) instanceof ScilabDouble) &&
				(modelFields.get(6) instanceof ScilabDouble) &&
				(modelFields.get(7) instanceof ScilabDouble)) {
				
				
				int size = graphicsStructure.get(13).getHeight();
				
				// if size == 0  graphicsStructure.get(13) is considered as ScilabDouble
				if (size != 0 ){ 
				
					String[][] implicitExplicitInArray = ((ScilabString)graphicsStructure.get(13)).getData();
					
					
					for (int i = 0 ; i < size ; i++){
						if (implicitExplicitInArray[i][0].equals("E")){
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
							
							
							
							//tempExpInputPort.setDataType( ((ScilabDouble)modelFields.get(7)).getRealPart()[0][i]);
							newBlock.addPort(tempOutputPort);
						}else if(implicitExplicitInArray[i][0].equals("I")){
							
							ImplicitOutputPort tempOutputPort =  new ImplicitOutputPort();
						
							ScilabDouble dataLines = (ScilabDouble)modelFields.get(5);
							ScilabDouble dataColumns = (ScilabDouble)modelFields.get(6);

							
							if ( dataLines.getRealPart() != null ){
								int nbLines = (int)dataLines.getRealPart()[i][0];
								tempOutputPort.setDataColumns(nbLines);
							}
							if ( dataColumns.getRealPart() != null ){
								int nbColumns = (int)dataColumns.getRealPart()[i][0];
								tempOutputPort.setDataColumns(nbColumns);
							}
							//tempExpInputPort.setDataType( ((ScilabDouble)modelFields.get(7)).getRealPart()[0][i]);
							newBlock.addPort(tempOutputPort);
							
						}else{
							return false ;
						}
						
					}
				}
			}else{
				return false ;
			}
			
			if ((modelFields.get(8) instanceof ScilabDouble) && //evtin
				(modelFields.get(9) instanceof ScilabDouble) && // evtout
				(modelFields.get(10) instanceof ScilabDouble) && // state
				(modelFields.get(11) instanceof ScilabDouble) && // dstate
				(modelFields.get(12) instanceof ScilabList)) { //odstate
				
				ScilabDouble dataNbControlPort = (ScilabDouble)modelFields.get(8);
				ScilabDouble dataNbCommandPort = (ScilabDouble)modelFields.get(9);
				
				if (dataNbControlPort.getRealPart() != null ){ 
				
					int nbControlPort = dataNbControlPort.getHeight();
					
					for (int i = 0 ; i < nbControlPort ; i++){
						
						newBlock.addPort(new ControlPort());
					}
				
				}
				
				if (dataNbCommandPort.getRealPart() != null ){ 
					
					int nbCommandPort = dataNbCommandPort.getHeight();
					
					for (int i = 0 ; i < nbCommandPort ; i++){
						
						newBlock.addPort(new CommandPort());
					}
				
				}
				
				
				
			}else{
				return false ;
			}
				
			
			// rpar
			if (modelFields.get(13) instanceof ScilabDouble) { 
				
				int size = ((ScilabDouble)modelFields.get(13)).getHeight() ;
				
				for (int i = 0; i < size ; i++){
					newBlock.getRealParameters().add(((ScilabDouble)modelFields.get(13)).getRealPart()[i][0]); 
				}
			
			}else{
				return false ;
			}
			
			// ipar
			if ((modelFields.get(14) instanceof ScilabDouble)) { 
			    int size = ((ScilabDouble)modelFields.get(14)).getHeight() ;
				
				for (int i = 0; i < size ; i++){
					newBlock.getIntegerParameters().add((int) ((ScilabDouble)modelFields.get(14)).getRealPart()[i][0]); 
				}
			}else{
				return false ;
			}
			
			// opar
			if ((modelFields.get(15) instanceof ScilabList)) { 
				
			}else{
				return false ;
			}
			
			//blocktype
			if ((modelFields.get(16) instanceof ScilabString)) { 
				newBlock.setBlockType(((ScilabString) modelFields.get(16)).getData()[0][0]);
			}else{
				return false ;
			}
			
			//firing
			if ((modelFields.get(17) instanceof ScilabDouble)) { 
				
			}else{
				return false ;
			}
			
			// dep-ut
			if ((modelFields.get(18) instanceof ScilabBoolean)){
				
				newBlock.setDependsOnU( ((ScilabBoolean)modelFields.get(18)).getData()[0][0]);
				newBlock.setDependsOnT( ((ScilabBoolean)modelFields.get(18)).getData()[0][1]);
				
				
			}else{
				return false ;
			}
			// label
			if ((modelFields.get(19) instanceof ScilabString)) { 
				
			}else{
				return false ;
			}
			
			// nzcross
			if ((modelFields.get(20) instanceof ScilabDouble)) { 
				
			}else{
				return false ;
			}
			//nmode 
			if ((modelFields.get(21) instanceof ScilabDouble)) { 
				
			}else{
				return false ;
			}
			
			// equations
			if ((modelFields.get(22) instanceof ScilabList)) { 
				
			}else{
				return false ;
			}
			
		}
		
		
		return true ;
	}
	
}



