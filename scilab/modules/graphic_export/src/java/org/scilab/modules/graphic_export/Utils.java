/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre Koumar
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.graphic_export;

import java.io.File;
import java.io.IOException;

public class Utils {

	/**
	 * Check if we have the permission to export on this file
	 * @param file exported file
	 * @return permission status
	 */
	public static int checkWritePermission(File file) {
		try {
			file.createNewFile();			
			if (!file.canWrite()) {
				return ExportRenderer.INVALID_FILE;
			}
			return ExportRenderer.SUCCESS;
		} catch (IOException e1) {
			return ExportRenderer.IOEXCEPTION_ERROR;
		} catch (SecurityException e2) {
			return ExportRenderer.INVALID_FILE;
		}
	}
	
}