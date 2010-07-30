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

import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.Map.Entry;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Perform useful conversions between a style string and key/value based map.
 */
public final class StyleMap extends LinkedHashMap<String, String> {
	
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
		
		for (Iterator<Entry<String, String>> iterator = entrySet().iterator();
		     iterator.hasNext();) {
			Entry<String, String> entry = iterator.next();
			
			str.append(entry.getKey());
			
			valueRef = entry.getValue();
			if (valueRef != null && valueRef.length() > 0) {
				str.append("=");
				str.append(valueRef);
			}
			
			if (iterator.hasNext()) {
				str.append(";");
			}
			
		}
		
		return str.toString();
	}
}
