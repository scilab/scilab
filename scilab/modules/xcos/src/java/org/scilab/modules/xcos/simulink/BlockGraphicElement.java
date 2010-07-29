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

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.TextBlock;
import org.scilab.modules.xcos.utils.BlockPositioning;

import edu.tum.cs.simulink.model.SimulinkAnnotation;
import edu.tum.cs.simulink.model.SimulinkBlock;

public class BlockGraphicElement{
	
	private static final Log LOG = LogFactory.getLog(BlockGraphicElement.class);
	
	public BasicBlock decode(SimulinkBlock from, BasicBlock into) {
		
		/**
		 * mutable field used to share decoded BasicBlock with submethods
		 */
		BasicBlock base = into;
		validate();
		//LOG.debug("Setting up graphics of:" + into.getInterfaceFunctionName());
		/*
		 * without this some blocks style weren't set properly
		 */
		if(base.getStyle().isEmpty()){
			base.setStyle(into.getInterfaceFunctionName());
		}
		/*
		 * fill the data
		 */
		fillDimension(from, base);
		fillOrigin(from, base);
		fillFlipAndRotation(from, base);
		fillColors(from, base);
		BlockPositioning.updateBlockView(into);
		
		return base;
	}
	/**
	 * Function filling strokeColor, fillColor and textColor parameters
	 * @param from imported SimulinkBlock
	 * @param base 
	 */
	private void fillColors(SimulinkBlock from, BasicBlock base) {
		// TODO Auto-generated method stub
		if(from.getParameter("ForegroundColor") != null) {
			//LOG.debug(base.getStyle());
			base.setStyle(base.getStyle() + "; strokeColor=" + from.getParameter("ForegroundColor"));
		}
		if(from.getParameter("BackgroundColor") != null) {
			//LOG.debug(base.getStyle());
			base.setStyle(base.getStyle() + "; fillColor=" + from.getParameter("BackgroundColor"));
		}
		if(from.getParameter("TextColor") != null) {
			//LOG.debug(base.getStyle());
			base.setStyle(base.getStyle() + "; textColor=" + from.getParameter("BackgroundColor"));
		}
	}

	private void fillFlipAndRotation(SimulinkBlock from, BasicBlock into) {
		// TODO: Check if compatibility pattern needed
		// TODO: Add Flip handling
		if(from.getParameter("Orientation") != null) {
			/*
			 * left, up, down, (else - right)
			 */
			if(from.getParameter("Orientation").equals("left")){
				into.setAngle(180);
				//LOG.debug("true " + into.getFlip());
			} else if(from.getParameter("Orientation").equals("up")) {
				into.setAngle(90);
				//LOG.debug("false " + into.getAngle());
			} else if(from.getParameter("Orientation").equals("down")) {
				into.setAngle(240);
				//LOG.debug("false " + into.getAngle());
			} else {
				into.setAngle(0);
				into.setFlip(false);
			}
			
			if(from.getParameter("BlockMirror") != null) {
				if(from.getParameter("BlockMirror").equals("on")){
					into.setFlip(true);
					//LOG.debug("true " + into.getFlip());
				} else {
					into.setFlip(false);
					//LOG.debug("false " + into.getFlip());
				}
			}
		}
		//into.setMirror(false);
		
		String rotation = from.getParameter("BlockRotation");
		if(rotation != null){
			int theta = Integer.parseInt(rotation);
			theta = BlockPositioning.roundAngle(theta);
			into.setAngle(theta);
		}
	}

	private void fillOrigin(SimulinkBlock from, BasicBlock into) {
		// TODO: Check if compatibility pattern needed
		/**
		 * Position parameter read by parser is string and looks like this [55, 43, 54, 42] 
		 * \\W is used to strip string from non-word characters 
		 * \\s+ to split string around whitespaces
		 */
		String[] position = from.getParameter("Position").replaceAll("\\W", " ").trim().split("\\s+");
		double x = Double.parseDouble(position[0]);
		double y = Double.parseDouble(position[1]);
		
		into.getGeometry().setX(x);
		into.getGeometry().setY(y);
	}

	private void fillDimension(SimulinkBlock from, BasicBlock into) {
		// TODO: Check if compatibility pattern needed
		/**
		 * Position parameter read by parser is string and looks like this [55, 43, 54, 42] 
		 * \\W is used to strip string from non-word characters 
		 * \\s+ to split string around whitespaces
		 */
		String[] position = from.getParameter("Position").replaceAll("\\W", " ").trim().split("\\s+");
		double w = Double.parseDouble(position[2]) - Double.parseDouble(position[0]);
		double h = Double.parseDouble(position[3]) - Double.parseDouble(position[1]);
		
		into.getGeometry().setWidth(w);
		into.getGeometry().setHeight(h);
	}

	private void validate() {
		// TODO Auto-generated method stub
		
	}

	/*
	 * Overloaded methods for TextBlocks
	 */
	
	public void decode(SimulinkAnnotation from, TextBlock annotation) {
		// TODO Auto-generated method stub
		
		TextBlock base = annotation;
		/*
		 * fill the data
		 */
		//fillDimension(from, base);
		fillOrigin(from, base);
		fillFlipAndRotation(from, base);
		fillColors(from, base);
	}
	
	private void fillFlipAndRotation(SimulinkAnnotation from, TextBlock into) {
		// TODO: Check if compatibility pattern needed
		// TODO: Add Flip handling
		
		String rotation = from.getParameter("BlockRotation");
		if(rotation != null){
		int theta = Integer.parseInt(rotation);
		theta = BlockPositioning.roundAngle(theta);
		
		into.setAngle(theta);
		}
	}

	private void fillOrigin(SimulinkAnnotation from, TextBlock into) {
		// TODO: Check if compatibility pattern needed
		/**
		 * Position parameter read by parser is string and looks like this [55, 43, 54, 42] 
		 * \\W is used to strip string from non-word characters 
		 * \\s+ to split string around whitespaces
		 */
		String[] position = from.getParameter("Position").replaceAll("\\W", " ").trim().split("\\s+");
		double x = Double.parseDouble(position[0]);
		double y = Double.parseDouble(position[1]);
		
		into.getGeometry().setX(x);
		into.getGeometry().setY(y);
	}
	
	private void fillColors(SimulinkAnnotation from, TextBlock base) {
		// TODO Auto-generated method stub
		if(from.getParameter("ForegroundColor") != null) {
			//LOG.debug(base.getStyle());
			base.setStyle(base.getStyle() + "; strokeColor=" + from.getParameter("ForegroundColor"));
		}
		if(from.getParameter("BackgroundColor") != null) {
			//LOG.debug(base.getStyle());
			base.setStyle(base.getStyle() + "; fillColor=" + from.getParameter("BackgroundColor"));
		}
		if(from.getParameter("TextColor") != null) {
			//LOG.debug(base.getStyle());
			base.setStyle(base.getStyle() + "; textColor=" + from.getParameter("BackgroundColor"));
		}
	}
}
