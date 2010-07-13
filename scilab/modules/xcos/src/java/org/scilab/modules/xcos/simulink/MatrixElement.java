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

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.types.scilabTypes.ScilabDouble;

public class MatrixElement {
	private static final Log LOG = LogFactory.getLog(MatrixElement.class);
	/**
	 * Function decoding Matrix parameters
	 * @param from - string with parameter from simulink parser
	 * @return
	 */
	public static double[] decode(String from){
		int width, height;
		String[] rows = from.split(";");
		width = rows.length;
		String[] cells = rows[0].replaceAll("[\\[\\];,]", " ").trim().split("\\s+");
		height = cells.length;
		
		double [] into = new double[height*width];
		
		for(int i = 0 ; i < rows.length ; i++ ) {
			cells = rows[i].replaceAll("[\\[\\];,]", " ").trim().split("\\s+");
			for(int j = 0 ; j < cells.length ; j ++){
				if(cells[j].equals("inf")){
					into[i*cells.length + j] = Double.MAX_VALUE;
				}
				else if(cells[j].equals("-inf")){
					into[i*cells.length + j] = Double.MIN_VALUE;
				} else {
					/*
					 * should be able to handle expressions like sqrt(1/2)
					 */
					try {
						into[i*cells.length + j] = Double.parseDouble(cells[j].replaceAll("\\w", ""));
					} catch (NumberFormatException fe) {
						into[i*cells.length + j] = 0;
					}
				}
				if(LOG.isTraceEnabled()){
					LOG.trace(into[i*cells.length + j]);
				}
			}
		}
		return into;
	}
	/**
	 * Function concatenating multiple double[] parameters into one ScilabDouble parameter ready to be assigned to rpar, state etc
	 * @param from
	 * @return
	 */
	public static ScilabDouble concatenate(ArrayList<double[]> from) {
		
		int length = 0;
		Iterator<double[]> parts = from.iterator();
		
		while(parts.hasNext()) {
			length += parts.next().length;
		}
		double[][] into = new double[length][1];
		if(LOG.isTraceEnabled()){
			LOG.trace(length);
		}
		parts = from.iterator();
		int i = 0;
		while(parts.hasNext()) {
			double[] current = parts.next();
			for(int j = 0 ; j < current.length ; j++ ) {
				if(LOG.isTraceEnabled()){
					LOG.trace("i:" + i + "j:" + j);
					LOG.trace(current[j]);
				}
				into[i][0] = current[j];
				if(LOG.isTraceEnabled()){
					LOG.trace(into[i][0]);
				}
				i++;
			}
		}
		if(LOG.isTraceEnabled()){
			LOG.trace(into);
		}
		return new ScilabDouble(into);
	}
}
