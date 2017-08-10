/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.gui.editor;

import java.awt.datatransfer.*;
import java.awt.Toolkit;
import java.awt.Image;
import java.io.File;
import java.io.IOException;
import java.awt.image.BufferedImage;
import javax.imageio.ImageIO;
import java.util.*;
import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.implementation.jogl.JoGLCanvas;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;

/**
 * implements the system clipboard, where a figure of scilab goes to the clipboard as an image
 *
 * @author Caio Souza <caioc2bolado@gmail.com>
 * @author Pedro Souza <bygrandao@gmail.com>
 *
 * @since 2012-06-01
 */

public class SystemClipboard {

    static SystemClipboard instance = null;
    static String name = "png";

    public static SystemClipboard getInstance() {
        if (instance == null) {
            instance = new SystemClipboard();
        }
        return instance;
    }

    /**
     * Given a figureID it make a buffered image and copy it to system clipboard
     *
     * @param uid The figure to copy
     */
    public static void copyToSysClipboard(Integer uid) {
        BufferedImage image = bufferFromUID(uid);
        ImageTransfer img = new ImageTransfer((Image)image);
        Toolkit.getDefaultToolkit().getSystemClipboard().setContents(img, null);
    }

    /**
     * Structure to retain an buffered image in the clipboard
     */
    public static class ImageTransfer implements Transferable {

        private Image image;

        public ImageTransfer(Image image) {
            this.image = image;
        }

        /**
         * Default structure for transferable flavors(type of flavor)
         *
         */
        public DataFlavor[] getTransferDataFlavors() {
            return new DataFlavor[] { DataFlavor.imageFlavor };
        }

        /**
         * Default structure for transferable flavor( supported / not suported )
         *
         * @param flavor
         * @return <ReturnValue>
         */
        public boolean isDataFlavorSupported(DataFlavor flavor) {
            return DataFlavor.imageFlavor.equals(flavor);
        }


        /**
         * Default structure for transferable flavor, returns the image or throw an excepction
         *
         * @param flavor
         * @return <ReturnValue>
         */
        public Object getTransferData(DataFlavor flavor) throws UnsupportedFlavorException, IOException {
            if (!DataFlavor.imageFlavor.equals(flavor)) {
                throw new UnsupportedFlavorException(flavor);
            }
            return image;
        }
    }

    private static BufferedImage bufferFromUID(Integer uid) {
        JoGLCanvas joglCanvas = null;

        DrawerVisitor visitor = DrawerVisitor.getVisitor(uid);
        Canvas canvas = visitor.getCanvas();
        if (canvas instanceof JoGLCanvas) {
            joglCanvas = (JoGLCanvas)canvas;
        }
        if (joglCanvas != null) {
            BufferedImage image = joglCanvas.getImage();
            //joglCanvas.destroy();
            return image;
        }
        return null;
    }
}

