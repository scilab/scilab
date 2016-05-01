package org.scilab.modules.helptools;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;

import javax.xml.transform.Result;
import javax.xml.transform.Source;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.sax.SAXResult;
import javax.xml.transform.stream.StreamSource;

import org.apache.fop.apps.FOPException;
import org.apache.fop.apps.Fop;
import org.apache.fop.apps.FopFactory;
import org.apache.fop.apps.FormattingResults;
import org.apache.fop.apps.MimeConstants;
import org.scilab.forge.jlatexmath.fop.JLaTeXMathElementMapping;
import org.scilab.forge.jlatexmath.fop.JLaTeXMathXMLHandler;
import org.scilab.modules.commons.xml.ScilabTransformerFactory;
import org.xml.sax.SAXException;

public class FopConverter extends ContainerConverter {

    final Backend format;

    public FopConverter(SciDocMain sciDocMain) {
        super(sciDocMain.getOutputDirectory(), sciDocMain.getLanguage());
        this.format = sciDocMain.getFormat();
    }

    @Override
    public void convert() throws SAXException, IOException {
        String baseName = Helpers.getBaseName(language);
        /* the following '..' is used because we are in the current working
           directory with all the tmp stuff in it */
        String fileName = outputDirectory + "/" + baseName + "." + format.name().toLowerCase();

        try {
            final File configuration = new File(System.getenv("SCI") + "/modules/helptools/etc/fopconf.xml");
            FopFactory fopFactory = FopFactory.newInstance(configuration);
            fopFactory.addElementMapping(new JLaTeXMathElementMapping());
            fopFactory.getXMLHandlerRegistry().addXMLHandler(new JLaTeXMathXMLHandler());

            // Step 3: Construct fop with desired output format
            OutputStream out = new BufferedOutputStream(new FileOutputStream(fileName));
            final Fop fop;
            switch (format) {
                case PS:
                    fop = fopFactory.newFop(MimeConstants.MIME_POSTSCRIPT, out);
                    break;
                case PDF:
                    fop = fopFactory.newFop(MimeConstants.MIME_POSTSCRIPT, out);
                    break;

                default:
                    out.close();
                    throw new IOException(String.format("%s is not a supported format.\n", format));
            }

            // Step 4: Setup JAXP using identity transformer
            String factoryName = ScilabTransformerFactory.useDefaultTransformerFactoryImpl();
            TransformerFactory factory = TransformerFactory.newInstance();
            Transformer transformer = factory.newTransformer(); // identity transformer
            // Step 5: Setup input and output for XSLT transformation
            // Setup input stream
            Source src = new StreamSource(new File(Helpers.getTemporaryNameFo(outputDirectory)));

            // Resulting SAX events (the generated FO) must be piped through to FOP
            Result res = new SAXResult(fop.getDefaultHandler());

            // Step 6: Start XSLT transformation and FOP processing
            transformer.transform(src, res);
            ScilabTransformerFactory.restoreTransformerFactoryImpl(factoryName);

            FormattingResults foResults = fop.getResults();
            System.out.println("Generated " + foResults.getPageCount() + " pages in total.");

            //Clean-up
            out.close();
        } catch (FOPException e) {
            System.out.println(e.getLocalizedMessage());
        } catch (FileNotFoundException e) {
            System.out.println(e.getLocalizedMessage());
        } catch (TransformerConfigurationException e) {
            System.out.println(e.getLocalizedMessage());
        } catch (TransformerException e) {
            System.out.println(e.getLocalizedMessage());
        } catch (IOException e) {
            System.out.println(e.getLocalizedMessage());
        } catch (SAXException e) {
            System.out.println(e.getLocalizedMessage());
        }
    }

    @Override
    public void install() throws IOException {
    }
}
