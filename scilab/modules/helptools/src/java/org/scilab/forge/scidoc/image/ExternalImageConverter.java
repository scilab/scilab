/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.forge.scidoc.image;

import java.io.File;
import java.util.Map;

public interface ExternalImageConverter {

    public String getMimeType();

    public String convertToImage(File f, Map<String, String> attributes, File imageFile, String imageName);

    public String convertToImage(String str, Map<String, String> attributes, File imageFile, String imageName);
}
