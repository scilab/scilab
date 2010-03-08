/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graph.utils;

import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Perform useful conversions between a style string and key/value based map.
 */
public final class StyleMap extends HashMap<String, String> {
	
	private static final Pattern P = Pattern.compile("(\\w+)(=((\\w|#)+))?($|;)");
	private static final int KEY_GROUP = 1;
	private static final int VALUE_GROUP = 3;
	
	/**
	 * Create a Map from a style string
	 * @param style The string which contains key=value list 
	 */
	public StyleMap(String style) {
		super();
		Matcher m = P.matcher(style);

		while (m.find()) {
			String key = m.group(KEY_GROUP);
			String value = m.group(VALUE_GROUP);
			put(key, value);
		}
	}
	
	/**
	 * Export to a key=value; string
	 * @return formatted string
	 */
	@Override
	public String toString() {
		StringBuilder str = new StringBuilder();
		String valueRef = null;
		
		for (Map.Entry<String, String> entry : entrySet()) {
			str.append(entry.getKey());
			
			valueRef = entry.getValue();
			if (valueRef != null && valueRef.length() > 0) {
				str.append("=");
				str.append(valueRef);
			}
			str.append(";");
		}
		
		return str.toString();
	}
}
