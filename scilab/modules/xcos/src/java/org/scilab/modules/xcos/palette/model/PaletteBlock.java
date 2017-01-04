/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
 * Copyright (C) 2015 - Marcos CARDINOT
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

package org.scilab.modules.xcos.palette.model;

import java.awt.Dimension;
import java.awt.Image;
import java.io.File;
import java.net.MalformedURLException;

import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;

import org.apache.batik.transcoder.TranscoderException;
import org.apache.batik.transcoder.TranscoderInput;
import org.scilab.modules.xcos.utils.ImageIconTranscoder;

/**
 * <p>
 * Representation of a function. This can be associated with an icon.
 *
 * <p>
 * The following schema fragment specifies the expected content contained within
 * this class.
 *
 * <pre>
 * &lt;complexType name="PaletteBlock">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="data" type="{}VariablePath"/>
 *         &lt;element name="icon" type="{}VariablePath"/>
 *       &lt;/sequence>
 *       &lt;attribute name="name" use="required" type="{http://www.w3.org/2001/XMLSchema}string" />
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 *
 *
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "PaletteBlock", propOrder = { "data", "icon" })
public class PaletteBlock {

    @XmlElement(required = true)
    private VariablePath data;
    @XmlElement(required = true)
    private VariablePath icon;
    @XmlAttribute(required = true)
    private String name;

    /**
     * Default constructor
     */
    public PaletteBlock() {
    }

    /**
     * Gets the value of the data property.
     *
     * @return possible object is {@link VariablePath }
     *
     */
    public VariablePath getData() {
        return data;
    }

    /**
     * Sets the value of the data property.
     *
     * @param value
     *            allowed object is {@link VariablePath }
     *
     */
    public void setData(VariablePath value) {
        this.data = value;
    }

    /**
     * Gets the value of the icon property.
     *
     * @return possible object is {@link VariablePath }
     *
     */
    public VariablePath getIcon() {
        return icon;
    }

    /**
     * Sets the value of the icon property.
     *
     * @param value
     *            allowed object is {@link VariablePath }
     *
     */
    public void setIcon(VariablePath value) {
        this.icon = value;
    }

    /**
     * Gets the value of the name property.
     *
     * @return possible object is {@link String }
     *
     */
    public String getName() {
        return name;
    }

    /**
     * Sets the value of the name property.
     *
     * @param value
     *            allowed object is {@link String }
     *
     */
    public void setName(String value) {
        this.name = value;
    }

    /**
     * Load the icon and return it.
     * @param maxWidth maximum width
     * @param maxHeight maximum height
     * @return the loaded icon
     * @see PaletteBlock#getIcon()
     */
    public Icon getLoadedIcon(int maxWidth, int maxHeight) {
        String path = getIcon().getEvaluatedPath();
        File file = new File(path);
        ImageIcon imgIcon = new ImageIcon();

        if (!file.exists()) {
            return imgIcon;
        }

        String extension = path.substring(path.lastIndexOf(".") + 1);
        if (extension.equals("svg")) {
            ImageIconTranscoder t = new ImageIconTranscoder();
            try {
                path = file.toURI().toURL().toString();
                TranscoderInput ti = new TranscoderInput(path);
                t.transcode(ti, null);
                imgIcon = t.getImageIcon();
                // resizing
                Dimension newSize = getNewSize(imgIcon, maxWidth, maxHeight);
                t.setDimensions(newSize.width, newSize.height);
                t.transcode(ti, null);
                imgIcon = t.getImageIcon();
            } catch (MalformedURLException e) {
                e.printStackTrace();
            } catch (TranscoderException e) {
                e.printStackTrace();
            }
        } else {
            imgIcon = new ImageIcon(path);
            Dimension newSize = getNewSize(imgIcon, maxWidth, maxHeight);
            imgIcon = new ImageIcon(imgIcon.getImage().getScaledInstance(
                    newSize.width, newSize.height, Image.SCALE_SMOOTH));
        }

        return imgIcon;
    }

    /**
     * Get the new icons size.
     * @param img original image
     * @param maxW maximum width
     * @param maxH maximum height
     * @return the new dimension
     */
    private Dimension getNewSize(ImageIcon img, int maxW, int maxH) {
        int iconW = img.getIconWidth();
        int iconH = img.getIconHeight();
        float scale = Math.min((float) maxW / iconW, (float) maxH / iconH);
        iconW *= scale;
        iconH *= scale;
        return new Dimension(iconW, iconH);
    }
}
