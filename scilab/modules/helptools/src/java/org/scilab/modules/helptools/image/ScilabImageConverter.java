/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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

package org.scilab.modules.helptools.image;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.util.Map;

import org.scilab.modules.commons.ScilabCommons;
import org.scilab.modules.helptools.DocbookTagConverter;

/**
 * Scilab code to PNG converter
 */
public class ScilabImageConverter implements ExternalImageConverter {

    private static final byte[] BOM = { (byte) 0xEF, (byte) 0xBB, (byte) 0xBF };
    private final StringBuilder buffer;
    private final DocbookTagConverter conv;

    public ScilabImageConverter(DocbookTagConverter conv) {
        buffer = new StringBuilder(8192);
        this.conv = conv;
    }

    public String getMimeType() {
        return "image/scilab";
    }

    /**
     * {@inheritDoc}
     */
    public boolean mustRegenerate() {
        return false;
    }

    public String getFileWithScilabCode() {
        if (buffer.length() != 0) {
            try {
                File f = File.createTempFile("help-", ".sce", new File(ScilabCommons.getTMPDIR()));
                FileOutputStream fos = new FileOutputStream(f);
                Writer out = new BufferedWriter(new OutputStreamWriter(fos, "UTF-8"));
                fos.write(BOM);
                out.write(buffer.toString());
                out.flush();

                out.close();
                fos.close();

                return f.getAbsolutePath();
            } catch (Exception e) {
                System.err.println("Cannot generate the file with Scilab code to execute:\n" + e);
            }
        }

        return null;
    }

    /**
     * {@inheritDoc}
     */
    public String convertToImage(String currentFile, String code, Map<String, String> attributes, File imageFile, String imageName) {
        return convertToPNG(currentFile, code, attributes, imageFile, imageName);
    }

    /**
     * {@inheritDoc}
     */
    public String convertToImage(File code, Map<String, String> attributes, File imageFile, String imageName) {
        try {
            BufferedReader in = new BufferedReader(new FileReader(code));
            StringBuilder buffer = new StringBuilder(8192);
            String line;

            while ((line = in.readLine()) != null) {
                buffer.append(line).append("\n");
            }

            in.close();

            return convertToPNG(code.getName(), buffer.toString(), attributes, imageFile, imageName);
        } catch (Exception e) {
            System.err.println("Problem when exporting Scilab code to " + imageFile + "!\n" + e.toString());
        }

        return null;
    }


    private final String convertToPNG(String currentFile, String code, Map<String, String> attributes, File imageFile, String imageName) {

        buffer.append("function _generate_image_from_doc\n");

        buffer.append("__olddrv__=driver();\n");
        buffer.append("mprintf(\"Generate image " + imageName + " from Scilab code from file " + new File(currentFile).getName() + "\\n\");");
        buffer.append("driver(\"png\");\n");
        buffer.append("xinit(\"").append(imageFile.getAbsolutePath()).append("\");\n");
        buffer.append("clf();\n");
        buffer.append(code).append("\n");
        buffer.append("___f___=gcf();___f___.anti_aliasing=\"2x\";clear(\"___f___\");\n");
        buffer.append("xend();\n");
        buffer.append("driver(__olddrv__);\n");

        buffer.append("endfunction\n");
        buffer.append("_generate_image_from_doc();\n");
        buffer.append("clear _generate_image_from_doc;\n");

        return conv.generateImageCode(code, conv.getBaseImagePath() + imageName, attributes);
    }
}
