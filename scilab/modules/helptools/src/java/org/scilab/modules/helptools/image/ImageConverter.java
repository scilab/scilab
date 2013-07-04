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

package org.scilab.modules.helptools.image;

import java.awt.Graphics2D;
import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.OutputStream;
import java.io.StringReader;
import java.net.URI;
import java.net.URISyntaxException;
import java.nio.channels.FileChannel;
import java.util.HashMap;
import java.util.Map;
import java.util.TreeMap;

import javax.activation.MimetypesFileTypeMap;
import javax.imageio.ImageIO;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JLabel;

import org.scilab.modules.commons.ScilabCommonsUtils;

/**
 * Class to handle the image conversion
 * @author Calixte DENIZET
 */
public final class ImageConverter {

    private static final String MD5_FILE = "images_md5.txt";

    private static Map<String, ExternalImageConverter> externalConverters = new HashMap<String, ExternalImageConverter>();
    private static MimetypesFileTypeMap mimeMap = new MimetypesFileTypeMap();
    private static Map<String, String> md5s;

    static {
        mimeMap.addMimeTypes("type=image/latex exts=tex,latex");
        mimeMap.addMimeTypes("type=image/mathml exts=mml,mathml");
        mimeMap.addMimeTypes("type=image/svg exts=svg");
        mimeMap.addMimeTypes("type=image/scilab exts=sce");
        mimeMap.addMimeTypes("type=image/scilab-xcos exts=xcos,zcos");
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
     * Load file containing md5s
     * @param dir the directory where to find the file
     */
    public static void loadMD5s(String dir) {
        md5s = getMD5s(dir);
    }

    /**
     * Save file containing md5s
     * @param dir the directory where to find the file
     */
    public static void saveMD5s(String dir) {
        writeMD5s(dir, md5s);
        md5s = null;
    }

    /**
     * Compare md5
     * @param code the code to compare
     * @param file the file name of the future image
     */
    public static boolean compareMD5(String code, String file) {
        if (md5s != null && code != null && file != null && !file.isEmpty()) {
            code = code.trim().replaceAll("[ \t]*[\n]+[ \t]*", "");
            String md5 = ScilabCommonsUtils.getMD5(code);
            String oldmd5 = md5s.get(file);
            if (oldmd5 != null && md5.equals(oldmd5)) {
                return true;
            }
            md5s.put(file, md5);
        }

        return false;
    }

    /**
     * Get the generated md5s
     * @param dir the dir where to find the files
     * @return a map filename -&gt; md5 of the code
     */
    private static Map<String, String> getMD5s(String dir) {
        File f = new File(dir + File.separator + MD5_FILE);
        Map<String, String> map = new HashMap<String, String>();
        if (f.exists() && f.canRead()) {
            BufferedReader reader = null;
            try {
                reader = new BufferedReader(new FileReader(f));
                String line = reader.readLine();
                while (line != null) {
                    String[] arr = line.split("=");
                    map.put(arr[0], arr[1]);
                    line = reader.readLine();
                }
            } catch (IOException e) {
                System.err.println(e);
            } finally {
                if (reader != null) {
                    try {
                        reader.close();
                    } catch (IOException e) {

                    }
                }
            }
        }

        return map;
    }

    /**
     * Write the generated md5s
     * @param dir the dir where to find the files
     * @param a map filename -&gt; md5 of the code
     */
    private static void writeMD5s(String dir, Map<String, String> map) {
        File f = new File(dir + File.separator + MD5_FILE);
        if (!f.exists()) {
            try {
                f.createNewFile();
            } catch (IOException e) {
                System.err.println(e);
                return;
            }
        }

        if (f.canWrite()) {
            BufferedWriter writer = null;
            try {
                writer = new BufferedWriter(new FileWriter(f));
                Map<String, String> tree = new TreeMap<String, String>(map);
                for (Map.Entry<String, String> entry : tree.entrySet()) {
                    String s = entry.getKey() + "=" + entry.getValue();
                    writer.write(s, 0, s.length());
                    writer.newLine();
                }
                writer.flush();
            } catch (IOException e) {
                System.err.println(e);
            } finally {
                if (writer != null) {
                    try {
                        writer.close();
                    } catch (IOException e) {

                    }
                }
            }
        }
    }

    /**
     * @param attrs the attribute of the image
     * @param path the current XML file which is parsed
     * @param image the filename
     * @param destDir the destination directory
     * @return the HTML code to insert the image
     */
    public static String getImageByFile(Map<String, String> attrs, String path, String image, String outputDir, String destDir, String baseImagePath) {
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
            copyImageFile(f, outputDir + File.separator + destDir);
        }

        return "<img src=\'" + baseImagePath + imageName + "\'/>";
    }

    /**
     * @param code the code to translate
     * @param attrs the attribute of the image
     * @param mime type
     * @param imageFile the filename
     * @return the HTML code to insert the image
     */
    public static String getImageByCode(String currentFile, String code, Map<String, String> attrs, String mime, File imageFile, String imageName, String baseImagePath, int lineNumber, String language, boolean isLocalized) {
        ExternalImageConverter conv = externalConverters.get(mime);
        if (conv == null) {
            System.err.println("In file " + currentFile + " at line " + lineNumber + ": invalid code:\n" + code);
            return null;
        }

        if (!imageFile.exists() && md5s != null) {
            md5s.remove(imageFile.getName());
        }

        if (!compareMD5(code, imageFile.getName())) {
            File current = new File(currentFile);
            if (isLocalized || language.equals("en_US")) {
                System.err.println("Info: Create image " + imageFile.getName() + " from line " + lineNumber + " in " + current.getName());
            } else if (!language.equals("en_US") && imageFile.exists()) {
                System.err.println("Warning: Overwrite image " + imageFile.getName() + " from line " + lineNumber + " in " + current.getName() + ". Check the code or use scilab:localized=\"true\" attribute.");
            }

            return conv.convertToImage(currentFile, code, attrs, imageFile, imageName);
        }

        return "<img src=\'" + baseImagePath + imageName + "\'/>";
    }

    /**
     * @param code the code to translate
     * @param img image information
     * @param fileName the filename
     * @param attrs the attribute of the image
     * @return the HTML code to insert the image
     */
    public static String generateCode(Image img, String fileName, Map<String, String> attrs) {
        String style = attrs.get("style");
        String top = "";
        boolean display = style != null && style.equals("display");

        if (!display) {
            top = "top:" + img.descent + "px;";
        }

        String alignAttr = attrs.get("align");
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
     * Test if an image file exists.
     * @param path of the parsed file
     * @param image the image name
     * @return null if the image exists, the expected file path otherwise.
     */
    public static File imageExists(String path, String image) {
        File f = new File(image);
        if (!f.isAbsolute()) {
            f = new File(path + File.separator + image);
        }

        if (f.exists()) {
            return null;
        } else {
            return f;
        }
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
        } finally {
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
