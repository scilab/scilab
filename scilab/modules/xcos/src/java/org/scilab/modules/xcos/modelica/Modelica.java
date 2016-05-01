/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2010 - DIGITEO - Clement DAVID <clement.david@scilab.org>
 * Copyright (C) 2011-2011 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.modelica;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.StringWriter;
import java.nio.charset.Charset;
import java.util.logging.Logger;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.SwingUtilities;
import javax.xml.XMLConstants;
import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBElement;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Marshaller;
import javax.xml.bind.Unmarshaller;
import javax.xml.stream.FactoryConfigurationError;
import javax.xml.validation.Schema;
import javax.xml.validation.SchemaFactory;

import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.graph.utils.ScilabExported;
import org.scilab.modules.xcos.modelica.model.Model;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.xml.sax.SAXException;

/**
 * Main class for modelica manipulation.
 */
public final class Modelica {
    private static final String LATIN1_ENCODING = "ISO-8859-1";
    private static final String MODEL_CLASS_PACKAGE = "org.scilab.modules.xcos.modelica.model";
    private static final String SCHEMA_FILENAME = "/Modelica.xsd";

    private static Modelica instance;

    private Marshaller marshaller;
    private Unmarshaller unmarshaller;

    /**
     * Default constructor.
     */
    private Modelica() {
        final String schemaPath = ScilabConstants.SCI.getAbsolutePath() + XcosConstants.XCOS_ETC + SCHEMA_FILENAME;

        JAXBContext jaxbContext;
        try {
            jaxbContext = JAXBContext.newInstance(MODEL_CLASS_PACKAGE);
            marshaller = jaxbContext.createMarshaller();
            unmarshaller = jaxbContext.createUnmarshaller();

            Schema schema = SchemaFactory.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI).newSchema(new File(schemaPath));

            marshaller.setSchema(schema);
            unmarshaller.setSchema(schema);

            /*
             * Customize the file to be handled by the xml2modelica and
             * modelicat tool.
             */
            marshaller.setProperty(Marshaller.JAXB_ENCODING, LATIN1_ENCODING);

            marshaller.setProperty(Marshaller.JAXB_FRAGMENT, Boolean.TRUE);
        } catch (JAXBException e) {
            throw new RuntimeException(e);
        } catch (SAXException e) {
            Logger.getLogger(Modelica.class.getName()).severe(e.toString());
        }
    }

    /**
     * @return the instance
     */
    public static Modelica getInstance() {
        if (instance == null) {
            instance = new Modelica();
        }
        return instance;
    }

    /**
     * Setup a new modelica settings UI
     *
     * @param init
     *            the initialisation file.
     * @param relation
     *            the relation file
     */
    @ScilabExported(module = "xcos", filename = "Modelica.giws.xml")
    public static void load(final String init, final String relation) {
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                ModelicaController.showDialog(new File(init), new File(relation));
            }
        });
    }

    /**
     * Load a file
     *
     * @param file
     *            the file to load
     * @return the model element
     * @throws JAXBException
     *             on error
     */
    @SuppressWarnings("unchecked")
    public Model load(File file) throws JAXBException {
        InputStreamReader reader;
        try {
            reader = new InputStreamReader(new FileInputStream(file), Charset.forName(LATIN1_ENCODING));
        } catch (FileNotFoundException e) {
            Logger.getLogger(Modelica.class.getName()).severe(e.toString());
            return null;
        }
        return ((JAXBElement<Model>) unmarshaller.unmarshal(reader)).getValue();
    }

    /**
     * Save the model into the file
     *
     * @param root
     *            the root of the model to save
     * @param file
     *            the file to save
     * @throws JAXBException
     *             on error
     */
    public void save(Model root, File file) throws JAXBException {
        try {
            final StringWriter strw = new StringWriter();
            marshaller.marshal(new org.scilab.modules.xcos.modelica.model.ObjectFactory().createModel(unmerge(root)), strw);

            /*
             * Customize the file to be handled by the xml2modelica tool
             */
            final StringBuffer buffer = strw.getBuffer();
            final String newline = System.getProperty("line.separator");

            Pattern pat = Pattern.compile("(/\\w*>)(<[\\w/])");
            Matcher m = pat.matcher(buffer);
            while (m.find()) {
                final int index = m.end(1);
                buffer.insert(index, newline);
                m.reset();
            }
            buffer.append(newline);

            new FileOutputStream(file).write(strw.toString().getBytes());
        } catch (FactoryConfigurationError e) {
            Logger.getLogger(Modelica.class.getName()).severe(e.toString());
        } catch (IOException e) {
            Logger.getLogger(Modelica.class.getName()).severe(e.toString());
        }
    }

    /**
     * Merge the models into a single shared model
     *
     * @param initModel
     *            the initialization model
     * @param relationModel
     *            the relation model
     * @return a new model
     */
    public Model merge(Model initModel, Model relationModel) {
        // merge the relation model into the init model

        initModel.setIdentifiers(relationModel.getIdentifiers());
        initModel.setModelInfo(relationModel.getModelInfo());

        initModel.setExplicitRelations(relationModel.getExplicitRelations());
        initModel.setImplicitRelations(relationModel.getImplicitRelations());

        initModel.setOutputs(relationModel.getOutputs());
        return initModel;
    }

    /**
     * Unmerge the model
     *
     * @param tree
     *            the model
     * @return the unmerged model
     */
    public Model unmerge(Model initModel) {
        final Model model = new Model();

        /*
         * From xMLTree.ml
         */
        model.setName(initModel.getName());

        model.setElements(initModel.getElements());
        model.setEquations(initModel.getEquations());
        model.setWhenClauses(initModel.getWhenClauses());

        return model;
    }
}
