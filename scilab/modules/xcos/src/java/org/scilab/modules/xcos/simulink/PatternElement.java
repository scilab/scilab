/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Jerzy Zagorski
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.simulink;
import java.io.File;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.List;
import java.util.ListIterator;

import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBElement;
import javax.xml.bind.JAXBException;
import javax.xml.bind.UnmarshalException;
import javax.xml.bind.Unmarshaller;
import javax.xml.transform.stream.StreamSource;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.jvm.utils.ScilabConstants;
import org.scilab.modules.types.scilabTypes.ScilabDouble;
import org.scilab.modules.types.scilabTypes.ScilabInteger;
import org.scilab.modules.types.scilabTypes.ScilabList;
import org.scilab.modules.types.scilabTypes.ScilabString;
import org.scilab.modules.types.scilabTypes.ScilabType;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.simulink.patterns.Block;
import org.scilab.modules.xcos.simulink.patterns.BlockPalette;
import org.scilab.modules.xcos.simulink.patterns.Int2StrParameters;
import org.scilab.modules.xcos.simulink.patterns.IntegerParameters;
import org.scilab.modules.xcos.simulink.patterns.IntegerValueMap;
import org.scilab.modules.xcos.simulink.patterns.RealParameters;
import org.scilab.modules.xcos.simulink.patterns.RealValueMap;
import org.scilab.modules.xcos.simulink.patterns.SimpleParameter;
import org.scilab.modules.xcos.simulink.patterns.Str2IntParameters;
import org.scilab.modules.xcos.simulink.patterns.Str2IntValueMap;
import org.scilab.modules.xcos.simulink.patterns.StringParameters;
import org.scilab.modules.xcos.utils.FileUtils;

import edu.tum.cs.simulink.model.SimulinkBlock;

public class PatternElement {
	private static final Log LOG = LogFactory.getLog(PatternElement.class);
	private JAXBContext jc;
	private Unmarshaller u;
	private BlockPalette blocks;
	
	final String file = "simulinkImportBlocks.xml";
	final String homePath = ScilabConstants.SCIHOME.getAbsolutePath();
	final File userPatternSheet = new File(homePath + '/' + file);
	/**
	 * General version of patternElement initialization
	 */
	public PatternElement(){
	/*
	 * Copy the base patternsheet into the user dir when it doesn't exist.
	 */
		if (!userPatternSheet.exists()) {
			final String sciPath = ScilabConstants.SCI.getAbsolutePath();
		
			File baseStyleSheet = new File(sciPath + "/modules/xcos/etc/" + file);
			FileUtils.forceCopy(baseStyleSheet, userPatternSheet);
		}
		bindPatterns();
	}
	/*
	 * block that compatibility patterns are being processed for
	 */
	private Block currentBlock = null;
	/*
	 * block containing all general parameters
	 */
	private Block generalParameters = null;
	/**
	 * specific block patternElement initialization
	 * @param simulinkBlockName
	 */
	public PatternElement(String simulinkBlockName){
		/*
		 * Copy the base patternsheet into the user dir when it doesn't exist.
		 */
		if (!userPatternSheet.exists()) {
			final String sciPath = ScilabConstants.SCI.getAbsolutePath();

			File baseStyleSheet = new File(sciPath + "/modules/xcos/etc/" + file);
			FileUtils.forceCopy(baseStyleSheet, userPatternSheet);
		}
		bindPatterns();
		/*
		 * Initialize patterns for specific block
		 */
		Iterator<Block> blockIter = blocks.getBlock().iterator();
		while (blockIter.hasNext()){
			Block block = blockIter.next();
			if(block.getSim().contentEquals(simulinkBlockName)){
				currentBlock = block;
			}
			if(block.getSim().contentEquals("GeneralParameters")){
				generalParameters = block;
			}
		}
		if(LOG.isTraceEnabled() && currentBlock != null){
			LOG.trace(currentBlock.getSim());
			LOG.trace(currentBlock.getXcos());
		}
		else if(LOG.isTraceEnabled()) {
			LOG.trace("unknown block");
		}
	}
	
	private void bindPatterns(){
		try{
			JAXBContext jc = JAXBContext.newInstance( "org.scilab.modules.xcos.simulink.patterns" );
			Unmarshaller u = jc.createUnmarshaller();
			//FIXME: now you have to manually copy compatibility pattern to SCIHOME
			JAXBElement<BlockPalette> element = u.unmarshal(
					new StreamSource(ScilabConstants.SCIHOME + "/simulinkImportBlocks.xml"),
					BlockPalette.class );
			blocks = element.getValue();
		} catch( UnmarshalException ue ) {
			LogFactory.getLog(PatternElement.class).error(ue);
		} catch( JAXBException je ) {
			LogFactory.getLog(PatternElement.class).error(je);
		}
	}
	

	/**
	 * For now I'm using simple parameter for translating FunctionName and FunctionType
	 * @param data
	 * @return
	 */
	public String decodeFunctionName(SimulinkBlock data) {
		/*
		 * Check if current block is well initialized for corresponding SimulinkBlock
		 */
		if(currentBlock!=null){
			Iterator<SimpleParameter> simpleParamIter = currentBlock.getSimple().iterator();
			while (simpleParamIter.hasNext()){
				SimpleParameter simpleParam = simpleParamIter.next();
				if(simpleParam.getSim().contentEquals("FunctionName")){
					if(LOG.isTraceEnabled()){
						LOG.trace(currentBlock.getXcos() + "FunctionName:");
						LOG.trace(simpleParam.getXcos());
					}
					return simpleParam.getXcos();
				}
			}
		}
		else {
			LOG.trace("FunctionName: wrong block!");
			BlockSpecificElement.print(data);
			/*
			 * If compatibility pattern doesn't exist, name the block with it's simulink name. It makes easier to replace it with a propr xcos block by user.
			 */
			return data.getParameter("BlockType");
		}
		return "xcos_simulate";
	}
	
	public boolean haveFunctionType(SimulinkBlock data){
		if(currentBlock!=null){
			Iterator<SimpleParameter> simpleParamIter = currentBlock.getSimple().iterator();
			while (simpleParamIter.hasNext()){
				SimpleParameter simpleParam = simpleParamIter.next();
				if(simpleParam.getSim().contentEquals("FunctionType")){
					if(simpleParam.getXcos().equals("null")){
						return false;
					} else {
						return true;
					}
						
				}
			}
		}
		else {
			LOG.trace("FunctionType: wrong block!");
		}
		/*
		 * return default function type
		 */
		return false;
	}
	/**
	 * As the Simulink blocks don't have explicit function types as xcos ones,
	 *  this function resolves function type from additional info from compatibility pattern
	 * @param data
	 * @return
	 */
	public int decodeFunctionType(SimulinkBlock data) {
		/*
		 * Check if current block is well initialized for coresponding SimulinkBlock
		 */
		if(currentBlock!=null){
			Iterator<SimpleParameter> simpleParamIter = currentBlock.getSimple().iterator();
			while (simpleParamIter.hasNext()){
				SimpleParameter simpleParam = simpleParamIter.next();
				if(simpleParam.getSim().contentEquals("FunctionType")){
					try {
					if(LOG.isTraceEnabled()){
						LOG.trace(currentBlock.getXcos() + "FunctionType:");
						LOG.trace(simpleParam.getXcos());
					}
					return Integer.parseInt(simpleParam.getXcos());
					} catch(NumberFormatException ne) {
						LogFactory.getLog(PatternElement.class).error(ne);
					}
				}
			}
		}
		else {
			LOG.trace("FunctionType: wrong block!");
		}
		/*
		 * return default function type
		 */
		return 0;
	}

	public ScilabType decodeState(SimulinkBlock data) {
		double[][] stateData;
		/*
		 * Check if current block is well initialized for coresponding SimulinkBlock
		 */
		if(currentBlock!=null){
			Iterator<RealParameters> realParamIter = currentBlock.getReal().iterator();
			while (realParamIter.hasNext()){
				RealParameters realParam = realParamIter.next();
				/*
				 * Check if state
				 */
				if(realParam.getXcos().contentEquals("state")){
					Iterator<RealValueMap> valueMapIter = realParam.getMap().iterator();
					stateData=new double[realParam.getMap().size()][1]; //FIXME: add count to realParameters
					ArrayList<double[]> parameters = new ArrayList<double[]>();
					while (valueMapIter.hasNext()){
						RealValueMap valueMap = valueMapIter.next();
						/**
						 * Position parameter read by parser is string and looks like this "55,43" 
						 * \\W is used to strip string from non-word characters 
						 * \\s+ to split string around whitespaces
						 */
						String[] position = valueMap.getIndex().replaceAll("\\W", " ").trim().split("\\s+");
						int x = Integer.parseInt(position[0]);
						int y = Integer.parseInt(position[1]);
						if(data.getParameter(valueMap.getSimName())!= null){
							parameters.add(MatrixElement.decode(data.getParameter(valueMap.getSimName())));
						} else {
							parameters.add(MatrixElement.decode(valueMap.getSimName()));
						}
						if(LOG.isTraceEnabled()){
							LOG.trace(currentBlock.getXcos() + "state:");
						}
					}
					return MatrixElement.concatenate(parameters);
				}
			}
		}
		else {
			LOG.trace("state: wrong block!");
		}
		return new ScilabDouble();
	}

	public ScilabType decodeDState(SimulinkBlock data) {
		double[][] stateData;
		/*
		 * Check if current block is well initialized for coresponding SimulinkBlock
		 */
		if(currentBlock!=null){
			Iterator<RealParameters> realParamIter = currentBlock.getReal().iterator();
			while (realParamIter.hasNext()){
				RealParameters realParam = realParamIter.next();
				/*
				 * Check if state
				 */
				if(realParam.getXcos().contentEquals("dstate")){
					Iterator<RealValueMap> valueMapIter = realParam.getMap().iterator();
					stateData=new double[realParam.getMap().size()][1]; //FIXME: add count to realParameters
					while (valueMapIter.hasNext()){
						RealValueMap valueMap = valueMapIter.next();
						/**
						 * Position parameter read by parser is string and looks like this "55,43" 
						 * \\W is used to strip string from non-word characters 
						 * \\s+ to split string around whitespaces
						 */
						String[] position = valueMap.getIndex().replaceAll("\\W", " ").trim().split("\\s+");
						int x = Integer.parseInt(position[0]);
						int y = Integer.parseInt(position[1]);
						if(data.getParameter(valueMap.getSimName())!= null){
							stateData[x][y] = Double.parseDouble(data.getParameter(valueMap.getSimName()));
						} else {
							stateData[x][y] =  Double.parseDouble(valueMap.getSimName());
						}
						if(LOG.isTraceEnabled()){
							LOG.trace(currentBlock.getXcos() + "dstate:");
							LOG.trace(stateData[x][y]);
						}
					}
					return new ScilabDouble(stateData);
				}
			}
		}
		else {
			LOG.trace("dstate: wrong block!");
		}
		return new ScilabDouble();
	}

	public ScilabType decodeODState(SimulinkBlock data) {
		// TODO Auto-generated method stub
		return new ScilabList();
	}

	public String decodeBlockType(SimulinkBlock data) {
		/*
		 * Check if current block is well initialized for coresponding SimulinkBlock
		 */
		if(currentBlock!=null){
			Iterator<SimpleParameter> simpleParamIter = currentBlock.getSimple().iterator();
			while (simpleParamIter.hasNext()){
				SimpleParameter simpleParam = simpleParamIter.next();
				if(simpleParam.getSim().contentEquals("BlockType")){
					if(LOG.isTraceEnabled()){
						LOG.trace(currentBlock.getXcos() + "blocktype:");
						LOG.trace(simpleParam.getXcos());
					}
					return simpleParam.getXcos();
				}
			}
		}
		else {
			LOG.trace("blocktype: wrong block!");
		}
		return "c";
	}

	public ScilabType decodeNbZerosCrossing(SimulinkBlock data) {
		/*
		 * Check if current block is well initialized for coresponding SimulinkBlock
		 */
		if(currentBlock!=null){
			Iterator<Str2IntParameters> str2intParamIter = currentBlock.getStr2Int().iterator();
			while (str2intParamIter.hasNext()){
				Str2IntParameters str2intParam = str2intParamIter.next();
				/*
				 * Check if nzcross enabled, and set if available
				 */
				if(str2intParam.getXcos().contentEquals("nzcross")){
					String nzcross = data.getParameter(str2intParam.getSim());
					Iterator<Str2IntValueMap> valueMapIter = str2intParam.getMap().iterator();
					while (valueMapIter.hasNext()){
						Str2IntValueMap valueMap = valueMapIter.next();
						if(valueMap.getSimVal().contentEquals("nzcross")){
							//TODO: set if available
							//TODO: check how simulink stores info about number of zero crossing surfaces
							return new ScilabDouble(Double.parseDouble(valueMap.getXcosVal().toString()));
						}
					}
				}
			}
		}
		else {
			LOG.trace("nzcross: wrong block!");
		}
		return new ScilabDouble(0);
	}

	public ScilabType decodeNmode(SimulinkBlock data) {
		// TODO Auto-generated method stub
		return new ScilabDouble(0);
	}
	public ScilabType decodeRealParameters(SimulinkBlock data) {
		/*
		 * Check if current block is well initialized for coresponding SimulinkBlock
		 */
		if(currentBlock!=null){
			Iterator<RealParameters> realParamIter = currentBlock.getReal().iterator();
			while (realParamIter.hasNext()){
				RealParameters realParam = realParamIter.next();
				/*
				 * Check if rpar
				 */
				if(realParam.getXcos().contentEquals("rpar")){
					Iterator<RealValueMap> valueMapIter = realParam.getMap().iterator();
					ArrayList<double[]> parameters = new ArrayList<double[]>();
					while (valueMapIter.hasNext()){
						RealValueMap valueMap = valueMapIter.next();
						/**
						 * Position parameter read by parser is string and looks like this "55,43" 
						 * \\W is used to strip string from non-word characters 
						 * \\s+ to split string around whitespaces
						 */
						String[] position = valueMap.getIndex().replaceAll("\\W", " ").trim().split("\\s+");
						int x = Integer.parseInt(position[0]);
						int y = Integer.parseInt(position[1]);
						if(data.getParameter(valueMap.getSimName())!= null){
							parameters.add(MatrixElement.decode(data.getParameter(valueMap.getSimName())));
						} else {
							parameters.add(MatrixElement.decode(valueMap.getSimName()));
						}
						if(LOG.isTraceEnabled()){
							LOG.trace(currentBlock.getXcos() + "rpar:");
						}
					}
					return MatrixElement.concatenate(parameters);
				}
			}
		}
		else {
			LOG.trace("rpar: wrong block!");
		}
		return new ScilabDouble(0);
	}

	public ScilabType decodeIntegerParameters(SimulinkBlock data) {
		/*
		 * Check if current block is well initialized for coresponding SimulinkBlock
		 */
		if(currentBlock!=null){
			Iterator<RealParameters> realParamIter = currentBlock.getReal().iterator();
			while (realParamIter.hasNext()){
				RealParameters realParam = realParamIter.next();
				/*
				 * Check if ipar
				 */
				if(realParam.getXcos().contentEquals("ipar")){
					Iterator<RealValueMap> valueMapIter = realParam.getMap().iterator();
					ArrayList<double[]> parameters = new ArrayList<double[]>();
					while (valueMapIter.hasNext()){
						RealValueMap valueMap = valueMapIter.next();
						/**
						 * Position parameter read by parser is string and looks like this "55,43" 
						 * \\W is used to strip string from non-word characters 
						 * \\s+ to split string around whitespaces
						 */
						String[] position = valueMap.getIndex().replaceAll("\\W", " ").trim().split("\\s+");
						int x = Integer.parseInt(position[0]);
						int y = Integer.parseInt(position[1]);
						if(data.getParameter(valueMap.getSimName())!= null){
							parameters.add(MatrixElement.decode(data.getParameter(valueMap.getSimName())));
						} else {
							parameters.add(MatrixElement.decode(valueMap.getSimName()));
						}
						if(LOG.isTraceEnabled()){
							LOG.trace(currentBlock.getXcos() + "ipar:");
						}
					}
					return MatrixElement.concatenate(parameters);
				}
			}
		}
		else {
			LOG.trace("rpar: wrong block!");
		}
		return new ScilabDouble(0);
	}

	public ScilabType decodeObjectsParameters(SimulinkBlock data) {
		// TODO Auto-generated method stub
		return new ScilabList();
	}

	public ScilabType decodeEquations(SimulinkBlock data) {
		// TODO Auto-generated method stub
		return new ScilabList();
	}

	public String decodeInterfaceFunctionName(SimulinkBlock data) {
		if(currentBlock!=null){
			return currentBlock.getXcos();
		}
		else {
			LOG.trace("interface: wrong block!");
		}
		return "xcos_block";
	}

	public boolean decodeDependsOnT(SimulinkBlock data) {
		// TODO Auto-generated method stub
		return true;
	}

	public ScilabType decodeExprs(SimulinkBlock data) {
		// TODO Auto-generated method stub
		
		String[][] exprsData;
		/*
		 * Check if current block is well initialized for coresponding SimulinkBlock
		 */
		if(currentBlock!=null){
			Iterator<RealParameters> realParamIter = currentBlock.getReal().iterator();
			while (realParamIter.hasNext()){
				RealParameters realParam = realParamIter.next();
				/*
				 * Check if ipar
				 */
				if(realParam.getXcos().contentEquals("exprs")){
					Iterator<RealValueMap> valueMapIter = realParam.getMap().iterator();
					exprsData=new String[realParam.getMap().size()][1]; //FIXME: add count to realParameters
					while (valueMapIter.hasNext()){
						RealValueMap valueMap = valueMapIter.next();
						/**
						 * Position parameter read by parser is string and looks like this "55,43" 
						 * \\W is used to strip string from non-word characters 
						 * \\s+ to split string around whitespace
						 */
						String[] position = valueMap.getIndex().replaceAll("\\W", " ").trim().split("\\s+");
						int x = Integer.parseInt(position[0]);
						int y = Integer.parseInt(position[1]);
						if(data.getParameter(valueMap.getSimName())!= null){
							exprsData[x][y] = data.getParameter(valueMap.getSimName());
						} else {
							exprsData[x][y] = valueMap.getSimName();
						}
						if(LOG.isTraceEnabled()){
							LOG.trace(currentBlock.getXcos() + "exprs:");
							LOG.trace(exprsData[x][y]);
						}
					}
					return new ScilabString(exprsData);
				}
			}
		}
		else {
			LOG.trace("ipar: wrong block!");
		}
		return new ScilabDouble();
	}

	public int decodeControlPorts(SimulinkBlock from) {
		/*
		 * Check if current block is well initialized for coresponding SimulinkBlock
		 */
		if(currentBlock!=null){
			Iterator<SimpleParameter> simpleParamIter = currentBlock.getSimple().iterator();
			while (simpleParamIter.hasNext()){
				SimpleParameter simpleParam = simpleParamIter.next();
				if(simpleParam.getSim().contentEquals("ControlPort")){
					if(LOG.isTraceEnabled()){
						LOG.trace(currentBlock.getXcos() + "ControlPort:");
						LOG.trace(simpleParam.getXcos());
					}
					return Integer.parseInt(simpleParam.getXcos());
				}
			}
		}
		else {
			LOG.trace("FunctionName: wrong block!");
		}
		return 0;
	}

	public int decodeCommandPorts(SimulinkBlock from) {
		/*
		 * Check if current block is well initialized for coresponding SimulinkBlock
		 */
		if(currentBlock!=null){
			Iterator<SimpleParameter> simpleParamIter = currentBlock.getSimple().iterator();
			while (simpleParamIter.hasNext()){
				SimpleParameter simpleParam = simpleParamIter.next();
				if(simpleParam.getSim().contentEquals("CommandPort")){
					if(LOG.isTraceEnabled()){
						LOG.trace(currentBlock.getXcos() + "CommandPort:");
						LOG.trace(simpleParam.getXcos());
					}
					return Integer.parseInt(simpleParam.getXcos());
				}
			}
		}
		else {
			LOG.trace("FunctionName: wrong block!");
		}
		return 0;
	}
	

	/**
	 * Function without target functionality yet
	 * @param String paramName;
	 * @return responded Xcos parameterName
	 */
	@Deprecated
	public String decode(String simulinkBlockName, String paramName, String simulinkValue){
		try{
			Iterator<Block> blockIter = blocks.getBlock().iterator();
			while (blockIter.hasNext()){
				Block block = blockIter.next();
				if(block.getSim().contentEquals(simulinkBlockName)){
					//TODO: check also in generals section
					/*
					 * checking simple parameters
					 */
					Iterator<SimpleParameter> simpleParamIter = block.getSimple().iterator();
					while (simpleParamIter.hasNext()){
						SimpleParameter simpleParam = simpleParamIter.next();
						if(simpleParam.getSim().contentEquals(paramName)){
							return simpleParam.getXcos();
						}
					}
					/*
					 * checking integer parameters
					 */
					Iterator<IntegerParameters> integerParamIter = block.getInteger().iterator();
					while (integerParamIter.hasNext()){
						IntegerParameters integerParam = integerParamIter.next();
						if(integerParam.getSim().contentEquals(paramName)){
							String returnValue = integerParam.getXcos() + ": ";
							Iterator<IntegerValueMap> valueMapIter = integerParam.getMap().iterator();
							//while (valueMapIter.hasNext()){
								returnValue += simulinkValue;
							//}
							//TODO: enable compatibility patterns
							return returnValue;
							}
					}
					/*
					 * checking string parameters
					 */
					Iterator<StringParameters> stringParamIter = block.getString().iterator();
					while (stringParamIter.hasNext()){
						StringParameters stringParam = stringParamIter.next();
						if(stringParam.getSim().contentEquals(paramName)){
							return stringParam.getXcos();
						}
					}
					/*
					 * checking str2int parameters
					 */
					Iterator<Str2IntParameters> str2intParamIter = block.getStr2Int().iterator();
					while (str2intParamIter.hasNext()){
						Str2IntParameters str2intParam = str2intParamIter.next();
						if(str2intParam.getSim().contentEquals(paramName)){
							String returnValue = str2intParam.getXcos() + ": ";
							Iterator<Str2IntValueMap> valueMapIter = str2intParam.getMap().iterator();
							while (valueMapIter.hasNext()){
								Str2IntValueMap valueMap = valueMapIter.next();
								if(valueMap.getSimVal().contentEquals(simulinkValue)){
									returnValue +=  valueMap.getXcosVal();
								}
							}
							return returnValue;
						}
					}
					/*
					 * checking int2str parameters
					 */
					Iterator<Int2StrParameters> int2strParamIter = block.getInt2Str().iterator();
					while (int2strParamIter.hasNext()){
						Int2StrParameters int2strParam = int2strParamIter.next();
						if(int2strParam.getSim().contentEquals(paramName)){
							return int2strParam.getXcos();
						}
					}
					return ".";
				}
			}
		} catch(Exception e1) {
			LogFactory.getLog(PatternElement.class).error(e1);
		}
		return "";
	}
	
	/**
	 * Functions printPattern is created for debugging compatibility patterns
	 */
	@Deprecated
	public void printPattern() {
		if (LOG.isTraceEnabled()) {
			try{
				Iterator<Block> blockIter = blocks.getBlock().iterator();
				while (blockIter.hasNext()){
					Block block = blockIter.next();
					if(LOG.isTraceEnabled()){
						LOG.trace("From: " + block.getSim() + 
								" to: " + block.getXcos() + 
								" with: " + block.getDescription());
					}
					/*
					 * printing simple parameters
					 */
					Iterator<SimpleParameter> simpleParamIter = block.getSimple().iterator();
					while (simpleParamIter.hasNext()){
						SimpleParameter simpleParam = simpleParamIter.next();
						if(LOG.isTraceEnabled()){
							LOG.trace("  From: " + simpleParam.getSim() + 
									" to: " + simpleParam.getXcos() + 
									" with: " + simpleParam.getDescription());
						}
					}
					/*
					 * printing integer parameters
					 */
					Iterator<IntegerParameters> integerParamIter = block.getInteger().iterator();
					while (integerParamIter.hasNext()){
						IntegerParameters integerParam = integerParamIter.next();
						if(LOG.isTraceEnabled()){
							LOG.trace("  From: " + integerParam.getSim() + 
									" to: " + integerParam.getXcos() + 
									" with: " + integerParam.getDescription());
						}
					}
					/*
					 * printing string parameters
					 */
					Iterator<StringParameters> stringParamIter = block.getString().iterator();
					while (stringParamIter.hasNext()){
						StringParameters stringParam = stringParamIter.next();
						if(LOG.isTraceEnabled()){
							LOG.trace("  From: " + stringParam.getSim() + 
									" to: " + stringParam.getXcos() + 
									" with: " + stringParam.getDescription());
						}
					}
					/*
					 * printing str2int parameters
					 */
					Iterator<Str2IntParameters> str2intParamIter = block.getStr2Int().iterator();
					while (str2intParamIter.hasNext()){
						Str2IntParameters str2intParam = str2intParamIter.next();
						if(LOG.isTraceEnabled()){
							LOG.trace("  From: " + str2intParam.getSim() + 
									" to: " + str2intParam.getXcos() + 
									" with: " + str2intParam.getDescription());
						}
					}
					/*
					 * printing int2str parameters
					 */
					Iterator<Int2StrParameters> int2strParamIter = block.getInt2Str().iterator();
					while (int2strParamIter.hasNext()){
						Int2StrParameters int2strParam = int2strParamIter.next();
						if(LOG.isTraceEnabled()){
							LOG.trace("  From: " + int2strParam.getSim() + 
									" to: " + int2strParam.getXcos() + 
									" with: " + int2strParam.getDescription());
						}
					}
				}
			} catch(Exception e1) {
				LogFactory.getLog(PatternElement.class).error(e1);
			}
		}
	}
}
