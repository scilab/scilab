/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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


    public static void copyToSysClipboard(String uid) {
        BufferedImage image = bufferFromUID(uid);
        ImageTransfer img = new ImageTransfer((Image)image);
        Toolkit.getDefaultToolkit().getSystemClipboard().setContents(img, null);
        System.out.println("Image copied to clipboard!");
    }

    public static class ImageTransfer implements Transferable {

        private Image image;

        public ImageTransfer(Image image) {
            this.image = image;
        }

        public DataFlavor[] getTransferDataFlavors() {
            return new DataFlavor[] { DataFlavor.imageFlavor };
        }

        public boolean isDataFlavorSupported(DataFlavor flavor) {
            return DataFlavor.imageFlavor.equals(flavor);
        }

        public Object getTransferData(DataFlavor flavor) throws UnsupportedFlavorException, IOException {
            if (!DataFlavor.imageFlavor.equals(flavor)) {
                throw new UnsupportedFlavorException(flavor);
            }
            return image;
        }
    }

    private static BufferedImage bufferFromUID(String uid) {
        JoGLCanvas joglCanvas = null;

        DrawerVisitor visitor = DrawerVisitor.getVisitor(uid);
        Canvas canvas = visitor.getCanvas();
        if (canvas instanceof JoGLCanvas) {
            joglCanvas = (JoGLCanvas)canvas;
        }
        if (joglCanvas != null) {
            BufferedImage image = joglCanvas.getImage();
            joglCanvas.destroy();
            return image;
        }
        return null;
    }


    public static void copyToSysClipboardB(String uid) {

        List<File> image = new ArrayList<File>();
        image.add(fileFromUID(uid));
        FileTransfer file = new FileTransfer(image);
        Toolkit.getDefaultToolkit().getSystemClipboard().setContents(file, null);
        System.out.println("Image copied to clipboard!");
    }

    public static class FileTransfer implements Transferable {

        private List<File> image;

        public FileTransfer(List<File> image) {
            this.image = image;
        }

        public DataFlavor[] getTransferDataFlavors() {
            return new DataFlavor[] { DataFlavor.javaFileListFlavor  };
        }

        public boolean isDataFlavorSupported(DataFlavor flavor) {
            return DataFlavor.javaFileListFlavor .equals(flavor);
        }

        public Object getTransferData(DataFlavor flavor) throws UnsupportedFlavorException, IOException {
            if (!DataFlavor.javaFileListFlavor .equals(flavor)) {
                throw new UnsupportedFlavorException(flavor);
            }
            return image;
        }
    }

    private static File fileFromUID(String uid) {

        JoGLCanvas joglCanvas = null;
        DrawerVisitor visitor = DrawerVisitor.getVisitor(uid);
        Canvas canvas = visitor.getCanvas();
        if (canvas instanceof JoGLCanvas) {
            joglCanvas = (JoGLCanvas)canvas;
        }
        if (joglCanvas != null) {
            try {
                BufferedImage image = joglCanvas.getImage();
                File f = new File("temp.png").getAbsoluteFile();
                joglCanvas.destroy();
                ImageIO.write(image, name, f);
                return f;
            } catch (IOException e) {
            }
        }
        return null;
    }

}

