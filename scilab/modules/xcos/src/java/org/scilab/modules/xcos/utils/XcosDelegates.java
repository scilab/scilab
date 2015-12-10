/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Clement David
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.utils;

import java.awt.image.BufferedImage;
import java.io.File;

import javax.imageio.ImageIO;

import org.scilab.modules.helptools.image.XcosImageConverter;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.XcosFileType;

import com.mxgraph.swing.mxGraphComponent;
import com.mxgraph.util.mxCellRenderer;

/**
 * Class which contains all Xcos dependant function used by other modules.
 */
public class XcosDelegates {

    private XcosDelegates() {
        // static singleton
    }

    /**
     * Delegate method to avoid a strong dependency on helptools.
     *
     * @param helpID
     * @param xcosFile
     * @param imageFile
     * @param imageName
     * @throws Exception
     *
     * @see {@link XcosImageConverter}
     */
    public static final void convertToPNG(final String helpID, final String xcosFile, final File imageFile, final String imageName) throws Exception {
        JavaController controller = new JavaController();

        final XcosDiagram diag = new XcosDiagram(controller.createObject(Kind.DIAGRAM), Kind.DIAGRAM);
        diag.installListeners();

        final XcosFileType filetype = XcosFileType.findFileType(xcosFile);
        filetype.load(xcosFile, diag);
        final mxGraphComponent graphComponent = diag.getAsComponent();
        final BufferedImage image = mxCellRenderer.createBufferedImage(diag, null, 1, null, graphComponent.isAntiAlias(), null, graphComponent.getCanvas());
        ImageIO.write(image, "png", imageFile);

        controller.deleteObject(diag.getUID());
    }
}
