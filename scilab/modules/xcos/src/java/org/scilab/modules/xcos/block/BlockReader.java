package org.scilab.modules.xcos.block;

import ncsa.hdf.hdf5lib.exceptions.HDF5LibraryException;

import org.scilab.modules.hdf5.H5ScilabConstant;
import org.scilab.modules.hdf5.read.H5Read;
import org.scilab.modules.hdf5.scilabTypes.ScilabBoolean;
import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;
import org.scilab.modules.hdf5.scilabTypes.ScilabList;
import org.scilab.modules.hdf5.scilabTypes.ScilabMList;
import org.scilab.modules.hdf5.scilabTypes.ScilabString;
import org.scilab.modules.hdf5.scilabTypes.ScilabTList;
import org.scilab.modules.hdf5.write.H5Write;
import org.scilab.modules.xcos.block.sinusoid.SinusoidBlock;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;

public class BlockReader {
	
	ScilabMList data ;

	public static void main(String[] args) {
		System.out.println("Hello World");

		ScilabMList data = new ScilabMList();
		
		


		int fileId;
		try {
			fileId = H5Read.openFile("/home/allan/sinusoid_generator.h5");
			H5Read.readDataFromFile(fileId, data);
			System.out.println(H5Read.getRootType(fileId));
			
			// check the basic strcture 
			if ( isAValidScs_mStructure(data)) {
				
				ScilabMList firstBlockFields =  getBlockAt(data, 0);
				
				BasicBlock newBlock = new BasicBlock ("temp") ;
				
				if ( fillBlockStructure(firstBlockFields , newBlock)){
					
				}
			}
					

			
			
			
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			
		}
		SinusoidBlock testSinusoidBloc = new SinusoidBlock() ;
		
		ScilabList listAllBlocksAndLinks =(ScilabList)  data.get(2);
		
		//H5ScilabConstant.SCILAB_CLASS_LIST;
		//H5Read.readDataFromFile(fileId, data);
		//Assert.assertEquals(data.isEmpty(), true);
	}
	
	public static BasicBlock read( String hdf5file ){
	ScilabMList data = new ScilabMList();
		
	BasicBlock newBlock = new BasicBlock ("temp") ;	


		int fileId;
		try {
			fileId = H5Read.openFile(hdf5file);
			H5Read.readDataFromFile(fileId, data);
			
			
			
			
			// check the basic strcture 
			if ( isAValidScs_mStructure(data)) {
				
				ScilabMList firstBlockFields =  getBlockAt(data, 0);
				//fillBlockStructure(data  , newBlock);
				
				
				if ( fillBlockStructure( firstBlockFields , newBlock)){
					
				}else 
					return null ;
			}else{
				
				if (fillBlockStructure(data , newBlock)){
					
					
				}else 
				return null ;
				
				//return null  TODO comment only for the moulinette
			}
			
			
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			
		}
		SinusoidBlock testSinusoidBloc = new SinusoidBlock() ;
		
		//ScilabList listAllBlocksAndLinks =(ScilabList)  data.get(2);
		
		//H5ScilabConstant.SCILAB_CLASS_LIST;
		//H5Read.readDataFromFile(fileId, data);
		//Assert.assertEquals(data.isEmpty(), true);
		
		return  newBlock;
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
			}else{
				return false ;
			}
			// the second field must contain list of props
			if (( data.get(1) instanceof ScilabTList)) { 
				
			}else{
				return false ;
			}
 
			
			
			// the third field must contains lists of blocks and links 
			
			if (( data.get(2) instanceof ScilabList)) { 
				
			}else{
				return false ;
			}
			
			
			// the last field must contain the scicos version used 
			
			if (( data.get(3) instanceof ScilabString)) { 
				String scicosVersion = ((ScilabString)data.get(3)).getData()[0][0];
				if(! scicosVersion.equals(realScicosVersion) ){
					return false ;
				}
				
				
			}else{
				return false ;
			}
			
			
		}else{
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
						
						if ( !nameOfScs_mFields[i].equals(realNameOfBlockFields[i]))
								return false ;
						
					}
				}
			}else{
				return false ;
			}
			// the second field must contain list of all graphic property (how the block will be displayed )
			if (( blockFields.get(1) instanceof ScilabMList)) { 
				fillGraphicsStructure( blockFields, newBlock);
			}else{
				return false ;
			}
 
			
			
			// the third field must contains all the informations needed to compile the block
			
			if ((blockFields.get(2) instanceof ScilabMList)) { 
				fillModelStructure(blockFields, newBlock);
			}else{
				return false ;
			}
			
			
			// the fourth field must contain the name of the interface function
			
			if (( blockFields.get(3) instanceof ScilabString)) { 
				newBlock.setValue(getBlockInterfaceName (blockFields));
				newBlock.setInterfaceFunctionName( getBlockInterfaceName (blockFields) );
			}else{
				return false ;
			}
			
			// the last field must contain a list of nothing aka scicos doc
			if ((blockFields.get(4) instanceof ScilabList)) { 
				
			}else{
				return false ;
			}
			
			
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
						
						if ( !nameOfStructureFields[i].equals(realNameOfStructureFields[i]))
								return false ;
						
					}
				}
			}else{
				return false ;
			}
			


			
			
			if ((graphicsStructure.get(1) instanceof ScilabDouble)&&// orig : must contain the coord of the block
				(graphicsStructure.get(2) instanceof ScilabDouble)&&// sz : must contains the size of the block
				(graphicsStructure.get(3) instanceof ScilabBoolean )&&// flip
				(graphicsStructure.get(4) instanceof ScilabDouble)// theta
				) { 
			}else{
				return false ;
			}
			
		
			
			// exprs 
			if ((graphicsStructure.get(5) instanceof ScilabString)) { 
				
				int size = graphicsStructure.get(5).getHeight();
				
				for (int i = 0 ; i < size ; i++){
					
					newBlock.getExprs().add(((ScilabString)graphicsStructure.get(5)).getData()[i][0]);
					
				}
				
				
			}else{
				return false ;
			}
			
			// pin
			if ((graphicsStructure.get(6) instanceof ScilabDouble)) { 
				
				//
			}else{
				return false ;
			}
			
			// pout
			if ((graphicsStructure.get(7) instanceof ScilabDouble)) { 
				//
			}else{
				return false ;
			}
			
			// pein
			if ((graphicsStructure.get(8) instanceof ScilabDouble)) { 
				//
			}else{
				return false ;
			}
			
			// peout
			if ((graphicsStructure.get(9) instanceof ScilabDouble)) { 
				//
			}else{
				return false ;
			}
			
			// gr_i
			if ((graphicsStructure.get(10) instanceof ScilabList)) { 
				//newBlock.setStyle(  )
				
			}else{
				return false ;
			}
			
			// id
			if ((graphicsStructure.get(11) instanceof ScilabString)) { 
			}else{
				return false ;
			}
			
			// in_implicit
			if ((graphicsStructure.get(12) instanceof ScilabString)) { 
				String[] implicitExplicitInArray = ((ScilabString)graphicsStructure.get(12)).getData()[0];
				int size =  implicitExplicitInArray.length ;

				
				
			}else if(graphicsStructure.get(12) instanceof ScilabDouble) {
			}else{
				return false ;
			}
			
			
			
			// out_implicit
			if ((graphicsStructure.get(13) instanceof ScilabString)) { 
				
				
			}else{
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
						
						if ( !nameOfScs_mFields[i].equals(realNameOfStructureFields[i]))
								return false ;
						
					}
				}
			}else{
				return false ;
			}
				
			// sim
			if (( modelFields.get(1) instanceof ScilabString)) { 
				
				newBlock.setSimulationFunctionName(getBlockSimulationFunctionName(modelFields) );
				
			}else if (( modelFields.get(1) instanceof ScilabList)){
				
				newBlock.setSimulationFunctionName( ((ScilabString)((ScilabList) modelFields.get(1)).get(0)).getData()[0][0] + " "+ ((ScilabDouble)((ScilabList) modelFields.get(1)).get(1)).getRealPart()[0][0]);   
;
				
			}else{
				return false ;
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
				
					int nbControlPort = (int) dataNbControlPort.getRealPart()[0][0];
					
					for (int i = 0 ; i < nbControlPort ; i++){
						
						newBlock.addPort(new ControlPort());
					}
				
				}
				
				if (dataNbCommandPort.getRealPart() != null ){ 
					
					int nbCommandPort = (int) dataNbCommandPort.getRealPart()[0][0];
					
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



