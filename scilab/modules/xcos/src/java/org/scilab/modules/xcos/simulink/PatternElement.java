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
import java.io.FileInputStream;
import java.io.IOException;
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
import org.scilab.modules.xcos.simulink.patterns.*;
import org.scilab.modules.xcos.utils.XcosConstants;

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
	 * Functions printPattern is created for debugging compatibility patterns
	 */
	public void printPattern() {
		if (LOG.isTraceEnabled()) {
			try{
				Iterator<Block> blockIter = blocks.getBlock().iterator();
				while (blockIter.hasNext()){
					Block block = blockIter.next();
					if(LOG.isTraceEnabled()){
						LOG.trace("From: " + block.getSim() + " to: " + block.getXcos());
					}
				}
			} catch(Exception e1) {
				LogFactory.getLog(PatternElement.class).error(e1);
			}
		}
	}
}
