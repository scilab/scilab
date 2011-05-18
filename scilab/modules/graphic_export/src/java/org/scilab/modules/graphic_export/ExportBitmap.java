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

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.Iterator;

import javax.imageio.IIOImage;
import javax.imageio.ImageIO;
import javax.imageio.ImageWriteParam;
import javax.imageio.ImageWriter;
import javax.imageio.stream.FileImageOutputStream;
import javax.media.opengl.GLContext;
import javax.media.opengl.GLException;

import com.sun.opengl.impl.GLContextImpl;
import com.sun.opengl.util.ImageUtil;
import com.sun.opengl.util.Screenshot;

/**
 * Class which allows to export screen-shots in format Bitmap (BMP, GIF, JPG, PNG and PPM)
 * @author Sylvestre Koumar
 *
 */
public class ExportBitmap extends ExportToFile {

    /** File which contains the screen-shot */
    private File file;
    private BufferedImage dump;
    private String fileExtension;
    private float jpegCompressionQuality = -1;

    /**
     * Default Constructor
     * @param filename name of the exported file
     * @param filetype type of the exported file
     */
    public ExportBitmap(String filename, int filetype) {
        super(filename, filetype);
    }

    /**
     * Default Constructor
     * @param filename name of the exported file
     * @param filetype type of the exported file
     * @param jpegCompressionQuality the jpeg compression rate between 0 and 1
     */
    public ExportBitmap(String filename, int filetype, float jpegCompressionQuality) {
        super(filename, filetype);
        this.jpegCompressionQuality = jpegCompressionQuality;
    }

    /**
     * Create a bitmap file which is the screen-shot of the figure
     * @return a int which is a type of error
     */
    public int exportToBitmap() {
        String suffix = ExportRenderer.getFileExtension();

        if (suffix.length() == 0) {
            switch (getFiletype()) {
            case ExportRenderer.BMP_EXPORT:
                suffix = "bmp";
                break;
            case ExportRenderer.GIF_EXPORT:
                suffix = "gif";
                break;
            case ExportRenderer.JPG_EXPORT:
                suffix = "jpg";
                break;
            case ExportRenderer.PNG_EXPORT:
                suffix = "png";
                break;
            default:
                return ExportRenderer.INVALID_FILE;
            }
        }

        /** Select the screen-shot format */
        switch (getFiletype()) {
        case ExportRenderer.BMP_EXPORT:
        case ExportRenderer.GIF_EXPORT:
        case ExportRenderer.JPG_EXPORT:
        case ExportRenderer.PNG_EXPORT:
            file = new File(getFilename() + "." + suffix);
            break;

        default: return ExportRenderer.INVALID_FILE;
        }

        try {
            /** Generate the screen-shot */
            //Check if we have the permission to export
            if (Utils.checkWritePermission(file) == ExportRenderer.SUCCESS) {
                dump = Screenshot.readToBufferedImage(getWidth(), getHeight());
                //flip the screen-shot if the dump is mirrored
                dumpFlip();
            } else {
                return ExportRenderer.INVALID_FILE;
            }
        } catch (GLException ex1) {
            return ExportRenderer.UNKNOWN_GLEXCEPTION_ERROR;
        }

        return ExportRenderer.SUCCESS;
    }

    /**
     * flip the screen-shot if it's mirrored
     * @return result of dumpFlip (success or fail)
     */
    private int dumpFlip() {
        // check if it was the case
        boolean needFlip;
        try {
            // raises an exception if hardware acceleration is on
            needFlip = !((GLContextImpl) GLContext.getCurrent()).offscreenImageNeedsVerticalFlip();
        } catch (GLException e) {
            // hardware acceleration is on
            needFlip = false;
        }
        if (needFlip) {
            // flip it back
            ImageUtil.flipImageVertically(dump);
        }
        try {
            if (jpegCompressionQuality != -1) {
                if (!writeJPEG(dump, jpegCompressionQuality, file)) {
                    return ExportRenderer.IOEXCEPTION_ERROR;
                }
            } else {
                ImageIO.write(dump, getFileExtension(), file);
            }
        } catch (IOException e) {
            return ExportRenderer.IOEXCEPTION_ERROR;
        }
        return ExportRenderer.SUCCESS;
    }

    /**
     * Gives the extension of a file name
     * @return the extension of the file
     */
    private String getFileExtension() {
        String fileName = file.getName().toLowerCase();
        fileExtension = fileName.substring(fileName.lastIndexOf('.') + 1, fileName.length());
        return fileExtension;
    }

    /**
     * @param image the image to compress
     * @param compressionQuality the compression rate
     * @param file the output file
     */
    private boolean writeJPEG(BufferedImage image, float compressionQuality, File file) throws IOException {
        Iterator iter = ImageIO.getImageWritersByFormatName("jpeg");
        ImageWriter writer;
        if (iter.hasNext()) {
            writer = (ImageWriter) iter.next();
        } else {
            return false;
        }
        ImageWriteParam param = writer.getDefaultWriteParam();
        param.setCompressionMode(ImageWriteParam.MODE_EXPLICIT);
        param.setCompressionQuality(compressionQuality);
        FileImageOutputStream output = new FileImageOutputStream(file);
        writer.setOutput(output);
        writer.write(null, new IIOImage(image, null, null), param);
        writer.dispose();
        return true;
    }
}
