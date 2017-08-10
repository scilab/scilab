/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 * Copyright (C) Scilab Enterprises - 2011 - Clement DAVID
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.palette;

import static java.util.Arrays.asList;

import java.io.File;
import java.net.MalformedURLException;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.List;
import java.util.Map;

import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabTList;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.io.scicos.AbstractElement;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongElementException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongStructureException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongTypeException;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.util.mxConstants;
import com.mxgraph.view.mxStylesheet;

/**
 * Decode a palette into a {@link mxStylesheet}.
 */
public class StyleElement extends AbstractElement<mxStylesheet> {
    private static final List<String> DATA_FIELD_NAMES = asList("palette", "name", "blockNames", "icons", "style");

    /** Mutable field to easily get the data through methods */
    private ScilabTList data;

    /**
     * Default constructor
     */
    public StyleElement() {
        super(null);
    }

    /**
     * Check if the element can be decoded by the current class.
     *
     * @param element
     *            the data
     * @return <code>false</code> if {@link #decode(ScilabType, mxStylesheet)}
     *         will always throw an exception, <code>false</code> is sometimes
     *         only.
     * @see org.scilab.modules.xcos.io.scicos.Element#canDecode(org.scilab.modules.types.ScilabType)
     */
    @Override
    public boolean canDecode(ScilabType element) {
        data = (ScilabTList) element;

        final String type = ((ScilabString) data.get(0)).getData()[0][0];
        return type.equals(DATA_FIELD_NAMES.get(0));
    }

    /**
     * Decode the current element on the into argument.
     *
     * @param element
     *            the Scilab data
     * @param into
     *            the target of the decoding (may be null)
     * @return the filled instance
     * @throws ScicosFormatException
     *             on decoding error
     * @see org.scilab.modules.xcos.io.scicos.Element#decode(org.scilab.modules.types.ScilabType,
     *      java.lang.Object)
     */
    @Override
    public mxStylesheet decode(ScilabType element, mxStylesheet into) throws ScicosFormatException {
        data = (ScilabTList) element;
        final mxStylesheet styleSheet = into;

        validate();

        if (into == null) {
            throw new NullPointerException("No place to decode data");
        }

        /*
         * get the data
         */

        int field = 2;
        String[][] blockNames = ((ScilabString) data.get(field)).getData();

        field++;
        field++;
        String[][] styles = ((ScilabString) data.get(field)).getData();

        /*
         * Configure the current styleSheet instance
         */

        for (int i = 0; i < blockNames.length; i++) {
            for (int j = 0; j < blockNames[i].length; j++) {
                /*
                 * Do not update the style if it already exists
                 */
                if (styleSheet.getStyles().containsKey(blockNames[i][j])) {
                    continue;
                }

                /*
                 * Inherit attributes from the Icon style
                 */
                final Map<String, Object> style = styleSheet.getCellStyle(styles[i][j], styleSheet.getCellStyle("Icon", styleSheet.getDefaultVertexStyle()));

                /*
                 * Translate Paths to URLs and check validity
                 */
                if (style.containsKey(mxConstants.STYLE_IMAGE)) {
                    String url = (String) style.get(mxConstants.STYLE_IMAGE);
                    url = validateURL(url);
                    style.put(mxConstants.STYLE_IMAGE, url);
                }

                styleSheet.putCellStyle(blockNames[i][j], style);
            }
        }

        return styleSheet;
    }

    /**
     * Validate the path URI.
     *
     * @param url
     *            the path to validate
     * @return the non-null valid path
     * @throws RuntimeException
     *             in case of invalid path argument
     */
    private String validateURL(final String url) {
        URL u;
        File f;
        try {
            u = new URL(url);

            try {
                f = new File(u.toURI());
            } catch (URISyntaxException e) {
                f = new File(u.getPath());
            }
        } catch (MalformedURLException e1) {
            throw new RuntimeException(e1);
        }

        if (f.exists()) {
            return u.toString();
        } else {
            throw new RuntimeException(String.format(XcosMessages.IMAGE_URL_DOESNT_EXIST, url));
        }
    }

    /**
     * Validate the current data.
     *
     * This method doesn't pass the metrics because it perform many test.
     * Therefore all these tests are trivial and the conditioned action only
     * throw an exception.
     *
     * @throws ScicosFormatException
     *             when there is a validation error.
     */
    // CSOFF: CyclomaticComplexity
    // CSOFF: NPathComplexity
    private void validate() throws ScicosFormatException {
        if (!canDecode(data)) {
            throw new WrongElementException();
        }

        int field = 0;

        // we test if the structure as enough field
        if (data.size() != DATA_FIELD_NAMES.size()) {
            throw new WrongStructureException(DATA_FIELD_NAMES);
        }

        /*
         * Checking the TList header
         */

        // Check the first field
        if (!(data.get(field) instanceof ScilabString)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }
        final String[] header = ((ScilabString) data.get(field)).getData()[0];

        // Checking for the field names
        if (header.length != DATA_FIELD_NAMES.size()) {
            throw new WrongStructureException(DATA_FIELD_NAMES);
        }
        for (int i = 0; i < header.length; i++) {
            if (!header[i].equals(DATA_FIELD_NAMES.get(i))) {
                throw new WrongStructureException(DATA_FIELD_NAMES);
            }
        }

        /*
         * Checking the data
         */

        // the second field is handled by the PreLoadedElement decoder
        field++;

        // the third field must contains the block name (row column)
        field++;
        if (!(data.get(field) instanceof ScilabString) || data.get(field).getWidth() != 1) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // the fourth field is handled by the PreLoadedElement decoder
        field++;

        // the fifth field must contains a valid style
        field++;
        if (!(data.get(field) instanceof ScilabString) || data.get(field).getWidth() != 1) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }
    }
}
