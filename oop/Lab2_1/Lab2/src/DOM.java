import java.io.File;
import java.io.IOException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import Constants.Files;
import Constants.XML;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;
import org.xml.sax.SAXParseException;
import org.xml.sax.helpers.DefaultHandler;

public class DOM {

    private String xmlFileName;
    private Medicine medicine;

    public DOM(String xmlFileName) {
        this.xmlFileName = xmlFileName;
    }

    public Medicine getMedicine() {
        return medicine;
    }

    public void parse(boolean validate)
            throws ParserConfigurationException, SAXException, IOException {

        DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
        dbf.setNamespaceAware(true);

        if (validate) {
            dbf.setFeature(Files.FEATURE_TURN_VALIDATION_ON, true);
            dbf.setFeature(Files.FEATURE_TURN_SCHEMA_VALIDATION_ON, true);
        }

        DocumentBuilder db = dbf.newDocumentBuilder();

        db.setErrorHandler(new DefaultHandler() {
            @Override
            public void error(SAXParseException e) throws SAXException {
                throw e;
            }
        });

        Document document = db.parse(xmlFileName);
        Element root = document.getDocumentElement();
        medicine = new Medicine();

        NodeList medicationNodes = root
                .getElementsByTagName(XML.MEDICATION.value());

        for (int j = 0; j < medicationNodes.getLength(); j++) {
            Medication medication = getMedication(medicationNodes.item(j));
            medicine.getMedications().add(medication);
        }
    }

    private Medication getMedication(Node dNode) {
        Medication medication = new Medication();
        NodeList nodeList = ((Element) dNode).getElementsByTagName("*");
        medication.setName(nodeList.item(0).getTextContent());
        medication.setPharm(nodeList.item(1).getTextContent());
        medication.setGroup(nodeList.item(2).getTextContent());
        medication.setAnalogs(nodeList.item(3).getTextContent());
        medication.setVersions(nodeList.item(4).getTextContent());
        medication.setCertificate(nodeList.item(5).getTextContent());
        medication.setPackagemy(nodeList.item(6).getTextContent());
        medication.setDosage(nodeList.item(7).getTextContent());
        return medication;
    }

    public static Document getDocument(Medicine medicine) throws ParserConfigurationException {

        DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
        dbf.setNamespaceAware(true);

        DocumentBuilder db = dbf.newDocumentBuilder();
        Document document = db.newDocument();

        Element bElement = document.createElement(XML.MEDICINE.value());

        document.appendChild(bElement);

        for (Medication medication : medicine.getMedications()) {

            Element dElement = document.createElement(XML.MEDICATION.value());
            bElement.appendChild(dElement);

            Element dpElement =
                    document.createElement(XML.NAME.value());
            dpElement.setTextContent(medication.getName());
            dElement.appendChild(dpElement);

            dpElement =
                    document.createElement(XML.PHARM.value());
            dpElement.setTextContent(medication.getPharm());
            dElement.appendChild(dpElement);

            dpElement =
                    document.createElement(XML.GROUP.value());
            dpElement.setTextContent(medication.getGroup());
            dElement.appendChild(dpElement);

            dpElement =
                    document.createElement(XML.ANALOGS.value());
            dpElement.setTextContent(medication.getAnalogs());
            dElement.appendChild(dpElement);

            dpElement =
                    document.createElement(XML.VERSIONS.value());
            dpElement.setTextContent(medication.getVersions());
            dElement.appendChild(dpElement);

            dpElement =
                    document.createElement(XML.CERTIFICATE.value());
            dpElement.setTextContent(medication.getCertificate());
            dElement.appendChild(dpElement);

            dpElement =
                    document.createElement(XML.PACKAGE.value());
            dpElement.setTextContent(medication.getPackagemy());
            dElement.appendChild(dpElement);

            dpElement =
                    document.createElement(XML.DOSAGE.value());
            dpElement.setTextContent(medication.getDosage());
            dElement.appendChild(dpElement);
        }
        return document;
    }

    public static void saveToXML(Medicine medicine, String xmlFileName)
            throws ParserConfigurationException, TransformerException {
        saveToXML(getDocument(medicine), xmlFileName);
    }

    public static void saveToXML(Document document, String xmlFileName)
            throws TransformerException {

        StreamResult result = new StreamResult(new File(xmlFileName));
        TransformerFactory tf = TransformerFactory.newInstance();
        javax.xml.transform.Transformer t = tf.newTransformer();
        t.setOutputProperty(OutputKeys.INDENT, "yes");
        t.transform(new DOMSource(document), result);
    }

    public static void start() throws Exception {

        DOM domContr = new DOM(Files.INVALID_XML_FILE);
        try {
            domContr.parse(true);
        } catch (SAXException ex) {
            System.err.println("====================================");
            System.err.println("Constants.XML not valid");
            System.err.println("Medicine object --> " + domContr.getMedicine());
            System.err.println("====================================");
        }

        domContr.parse(false);
        System.out.println("====================================");
        System.out.println("Here is the medicine medications: \n" + domContr.getMedicine());
        System.out.println("====================================");

        Medicine medicine = domContr.getMedicine();
        DOM.saveToXML(medicine, Files.INVALID_XML_FILE + ".dom-result.xml");
    }
}
