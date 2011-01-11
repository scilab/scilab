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

import java.awt.Graphics2D;
import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.StringReader;
import java.nio.channels.FileChannel;
import java.util.HashMap;
import java.util.Map;

import javax.activation.MimetypesFileTypeMap;
import javax.imageio.ImageIO;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JLabel;

/**
 * Class to handle the image conversion
 * @author Calixte DENIZET
 */
public final class ImageConverter {

    private static Map<String, ExternalImageConverter> externalConverters = new HashMap();
    private static MimetypesFileTypeMap mimeMap = new MimetypesFileTypeMap();

    static {
        mimeMap.addMimeTypes("type=image/latex exts=tex,latex");
        mimeMap.addMimeTypes("type=image/mathml exts=mml,mathml");
        mimeMap.addMimeTypes("type=image/svg exts=svg");
    }

    /**
     * Register a new ExternalImageConverter
     * @param c the converter to register
     */
    public static void registerExternalImageConverter(ExternalImageConverter c) {
        if (c != null) {
            externalConverters.put(c.getMimeType(), c);
        }
    }

    /**
     * @param attrs the attribute of the image
     * @param path the current XML file which is parsed
     * @param image the filename
     * @param destDir the destination directory
     * @return the HTML code to insert the image
     */
    public static String getImageByFile(Map<String, String> attrs, String path, String image, String outputDir, String destDir) {
        File f = new File(image);
        if (!f.isAbsolute()) {
            f = new File(path + File.separator + image);
        }

        String destFile = outputDir + File.separator + destDir + File.separator + f.getName();

        ExternalImageConverter conv = externalConverters.get(mimeMap.getContentType(f));
        if (conv != null) {
            destFile += ".png";
        }
        File imageFile = new File(destFile);
        String imageName = destDir + "/" + imageFile.getName();

        if (f.lastModified() > imageFile.lastModified()) {
            if (conv != null) {
                return conv.convertToImage(f, attrs, imageFile, imageName);
            }
            copyImageFile(f, destDir);
        }

        return "<img src=\'" + imageName + "\'/>";
    }


    /**
     * @param code the code to translate
     * @param attrs the attribute of the image
     * @param mime type
     * @param imageFile the filename
     * @return the HTML code to insert the image
     */
    public static String getImageByCode(String code, Map<String, String> attrs, String mime, File imageFile, String imageName) {
        ExternalImageConverter conv = externalConverters.get(mime);
        if (conv != null) {
            return conv.convertToImage(code, attrs, imageFile, imageName);
        }
        System.err.println("Code not handled:\n" + code);

        return null;
    }

    /**
     * @param code the code to translate
     * @param img image informations
     * @param fileName the filename
     * @param attrs the attribute of the image
     * @return the HTML code to insert the image
     */
    public static String generateCode(Image img, String fileName, Map<String, String> attrs) {
        String style = (String) attrs.get("style");
        String top = "";
        boolean display = style != null && style.equals("display");

        if (!display) {
            top = "top:" + img.descent + "px;";
        }

        String alignAttr = (String) attrs.get("align");
        String align = "";
        String div = "div";
        if (alignAttr != null) {
            align = " style=\'text-align:" + alignAttr + "\'";
        } else if (display) {
            align = " style=\'text-align:center\'";
        } else {
            div = "span";
        }

        return "<" + div + align + "><img src=\'" + fileName + "\' style=\'position:relative;" + top  + "width:" + img.width + "px;height:" + img.height + "px\'/></" + div + ">";
    }

    /**
     * @param f the file to copy
     * @param destDir the destination directory
     */
    public static void copyImageFile(File f, String destDir) {
        FileChannel src = null;
        FileChannel dest = null;
        try {
            File destFile = new File(destDir + File.separator + f.getName());
            if (!destFile.exists()) {
                destFile.createNewFile();
            } else if (f.lastModified() <= destFile.lastModified()) {
                return;
            }

            src = new FileInputStream(f).getChannel();
            dest = new FileOutputStream(destFile).getChannel();
            dest.transferFrom(src, 0, src.size());
        } catch (IOException e) {
            System.err.println(e);
        }
        finally {
            try {
                if (src != null) {
                    src.close();
                }
                if (dest != null) {
                    dest.close();
                }
            } catch (IOException e) {
                System.err.println(e);
            }
        }
    }

    /**
     * @param icon the icon to convert into PNG
     * @param imageFile the destination file
     * @return true if all is ok
     */
    public static boolean convertIconToPNG(Icon icon, File imageFile) {
        BufferedImage image = new BufferedImage(icon.getIconWidth(), icon.getIconHeight(), BufferedImage.TYPE_INT_ARGB);
        Graphics2D g2d = image.createGraphics();
        icon.paintIcon(null, g2d, 0, 0);

        try {
            ImageIO.write(image, "png", imageFile.getAbsoluteFile());
        } catch (IOException ex) {
            return false;
        }

        g2d.dispose();

        return true;
    }
}
