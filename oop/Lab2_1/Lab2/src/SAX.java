import java.io.IOException;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import Constants.Files;
import Constants.XML;
import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

public class SAX extends DefaultHandler {

    private String xmlFileName;

    private String currentElement;

    private Medicine medicine;

    private Medication medication;

    public SAX(String xmlFileName) {
        this.xmlFileName = xmlFileName;
    }

    public void parse(boolean validate)
            throws ParserConfigurationException, SAXException, IOException {

        SAXParserFactory factory = SAXParserFactory.newInstance();

        factory.setNamespaceAware(true);

        if (validate) {
            factory.setFeature(Files.FEATURE_TURN_VALIDATION_ON, true);
            factory.setFeature(Files.FEATURE_TURN_SCHEMA_VALIDATION_ON, true);
        }

        SAXParser parser = factory.newSAXParser();
        parser.parse(xmlFileName, this);
    }

    @Override
    public void error(org.xml.sax.SAXParseException e) throws SAXException {
        throw e;
    };

    public Medicine getMedicine() {
        return medicine;
    }

    @Override
    public void startElement(String uri, String localName, String qName,
                             Attributes attributes) throws SAXException {

        currentElement = localName;

        if (XML.MEDICINE.equalsTo(currentElement)) {
            medicine = new Medicine();
            return;
        }

        if (XML.MEDICATION.equalsTo(currentElement)) {
            medication = new Medication();
            return;
        }
    }

    @Override
    public void characters(char[] ch, int start, int length)
            throws SAXException {

        String elementText = new String(ch, start, length).trim();

        if (elementText.isEmpty()) return;

        if (XML.NAME.equalsTo(currentElement)) {
            medication.setName(elementText);
            return;
        }

        if (XML.PHARM.equalsTo(currentElement)) {
            medication.setPharm(elementText);
            return;
        }

        if (XML.GROUP.equalsTo(currentElement)) {
            medication.setGroup(elementText);
            return;
        }

        if (XML.ANALOGS.equalsTo(currentElement)) {
            medication.setAnalogs(elementText);
            return;
        }

        if (XML.VERSIONS.equalsTo(currentElement)) {
            medication.setVersions(elementText);
            return;
        }

        if (XML.CERTIFICATE.equalsTo(currentElement)) {
            medication.setCertificate(elementText);
            return;
        }

        if (XML.PACKAGE.equalsTo(currentElement)) {
            medication.setPackagemy(elementText);
            return;
        }

        if (XML.DOSAGE.equalsTo(currentElement)) {
            medication.setDosage(elementText);
            return;
        }

    }

    @Override
    public void endElement(String uri, String localName, String qName)
            throws SAXException {

        if (XML.MEDICATION.equalsTo(localName)) {
            medicine.getMedications().add(medication);
            return;
        }
    }

    public static void start() throws Exception {

        SAX saxContr = new SAX(Files.VALID_XML_FILE);

        saxContr.parse(true);

        Medicine medicine = saxContr.getMedicine();

        System.out.println("====================================");
        System.out.println("Here is the medicine medications: \n" + medicine);
        System.out.println("====================================");

        saxContr = new SAX(Files.INVALID_XML_FILE);
        try {
            saxContr.parse(true);
        } catch (Exception ex) {
            System.err.println("====================================");
            System.err.println("Validation is failed:\n" + ex.getMessage());
            System.err
                    .println("Try to print medicine object:\n" + saxContr.getMedicine());
            System.err.println("====================================");
        }

        saxContr.parse(false);

        System.out.println("====================================");
        System.out.println("Here is the medicine medications: \n" + saxContr.getMedicine());
        System.out.println("====================================");
    }
}