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

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.types.ScilabDouble;

public class MatrixElement {
	private static final Log LOG = LogFactory.getLog(MatrixElement.class);
	/**
	 * Function decoding Matrix parameters
	 * @param from - string with parameter from simulink parser
	 * @return
	 */
	public static double[] decode(String from, String toTrace , TraceElement trace){
		int width, height;
/*		if(LOG.isTraceEnabled()){
			LOG.trace("Decoding parameter:" + from + ".");
		}*/
		String[] rows = from.split(";");
		width = rows.length;
		String[] cells = rows[0].replaceAll("[\\[\\];,]", " ").trim().split("\\s+");
		height = cells.length;
		
		double [] into = new double[height*width];
		
		for(int i = 0 ; i < rows.length ; i++ ) {
			/*
			 * handling of *+ and -/ 
			 */
			int k = 0;
			if(rows[i].endsWith("+") || rows[i].endsWith("*") || rows[i].endsWith("-") || rows[i].endsWith("/") || rows[i].endsWith("|")){
				char[] signArray = rows[i].toCharArray();
				double [] intoSumProd = new double[signArray.length];
				for(int j = 0 ; j < signArray.length; j++) {
					if(signArray[j] == '+' || signArray[j] == '*'){
						intoSumProd[k++] = 1;
					} else if(signArray[j] == '-' || signArray[j] == '/'){
						intoSumProd[k++] = -1;
					}
				}
				return intoSumProd;
			} 
			
			cells = rows[i].replaceAll("[\\[\\];,]", " ").trim().split("\\s+");
			for(int j = 0 ; j < cells.length ; j ++){
				/*
				 * handling of inf and -inf
				 */
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
						into[i*cells.length + j] = Double.parseDouble(cells[j].replaceAll("\\D", " ").trim());
					} catch (NumberFormatException fe) {
						into[i*cells.length + j] = 0;
						LOG.error("NumberFormaException with:" + cells[j].replaceAll("\\D", " ").trim());
						trace.addFaultParameters(toTrace + "NumberFormaException from:" + from + " with: " + cells[j].replaceAll("\\D", " ").trim());
					}
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
		parts = from.iterator();
		int i = 0;
		while(parts.hasNext()) {
			double[] current = parts.next();
			for(int j = 0 ; j < current.length ; j++ ) {
				into[i][0] = current[j];
				i++;
			}
		}
		return new ScilabDouble(into);
	}
}
