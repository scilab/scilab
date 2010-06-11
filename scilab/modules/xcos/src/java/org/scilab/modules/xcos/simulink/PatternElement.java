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
import java.util.Iterator;

import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBElement;
import javax.xml.bind.JAXBException;
import javax.xml.bind.UnmarshalException;
import javax.xml.bind.Unmarshaller;
import javax.xml.transform.stream.StreamSource;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.jvm.utils.ScilabConstants;
import org.scilab.modules.xcos.simulink.patterns.Block;
import org.scilab.modules.xcos.simulink.patterns.BlockPalette;
import org.scilab.modules.xcos.simulink.patterns.Int2StrParameters;
import org.scilab.modules.xcos.simulink.patterns.IntegerParameters;
import org.scilab.modules.xcos.simulink.patterns.SimpleParameter;
import org.scilab.modules.xcos.simulink.patterns.Str2IntParameters;
import org.scilab.modules.xcos.simulink.patterns.Str2IntValueMap;
import org.scilab.modules.xcos.simulink.patterns.StringParameters;

public class PatternElement {
	private static final Log LOG = LogFactory.getLog(PatternElement.class);
	private JAXBContext jc;
	private Unmarshaller u;
	private BlockPalette blocks;
	
	/**
	 * Function reading compatibility patterns to BlockPalette object
	 */
	public PatternElement(){
	try{
		/*
		 * Creating Java JAXBContext from compiled xsd schema of compatibility patterns
		 */
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
	 * Function without target functionality yet
	 * @param String paramName;
	 * @return respondend Xcos parameterName
	 */
	public String decode(String simulinkBlockName, String paramName, String simulinkValue){
		try{
			Iterator<Block> blockIter = blocks.getBlock().iterator();
			while (blockIter.hasNext()){
				Block block = blockIter.next();
				if(block.getSim().contentEquals(simulinkBlockName)){
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
							return integerParam.getXcos();
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
							Iterator<Str2IntValueMap> valueMapIter = str2intParam.getParMap().iterator();
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
