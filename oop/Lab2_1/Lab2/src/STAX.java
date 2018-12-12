import java.io.IOException;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.stream.XMLEventReader;
import javax.xml.stream.XMLInputFactory;
import javax.xml.stream.XMLStreamException;
import javax.xml.stream.events.Characters;
import javax.xml.stream.events.EndElement;
import javax.xml.stream.events.StartElement;
import javax.xml.stream.events.XMLEvent;
import javax.xml.transform.stream.StreamSource;

import Constants.Files;
import Constants.XML;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

public class STAX extends DefaultHandler {

    private String xmlFileName;

    private Medicine medicine;

    public Medicine getMedicine() {
        return medicine;
    }

    public STAX(String xmlFileName) {
        this.xmlFileName = xmlFileName;
    }

    public void parse() throws ParserConfigurationException, SAXException,
            IOException, XMLStreamException {

        Medication medication = null;

        String currentElement = null;

        XMLInputFactory factory = XMLInputFactory.newInstance();

        factory.setProperty(XMLInputFactory.IS_NAMESPACE_AWARE, true);

        XMLEventReader reader = factory.createXMLEventReader(
                new StreamSource(xmlFileName));

        while (reader.hasNext()) {
            XMLEvent event = reader.nextEvent();

            if (event.isCharacters() && event.asCharacters().isWhiteSpace()) {
                continue;
            }

            if (event.isStartElement()) {
                StartElement startElement = event.asStartElement();
                currentElement = startElement.getName().getLocalPart();

                if (XML.MEDICINE.equalsTo(currentElement)) {
                    medicine = new Medicine();
                    continue;
                }

                if (XML.MEDICATION.equalsTo(currentElement)) {
                    medication = new Medication();
                    continue;
                }

            }

            if (event.isCharacters()) {
                Characters characters = event.asCharacters();

                if (XML.NAME.equalsTo(currentElement)) {
                    medication.setName(characters.getData());
                    continue;
                }

                if (XML.PHARM.equalsTo(currentElement)) {
                    medication.setPharm(characters.getData());
                    continue;
                }

                if (XML.GROUP.equalsTo(currentElement)) {
                    medication.setGroup(characters.getData());
                    continue;
                }

                if (XML.ANALOGS.equalsTo(currentElement)) {
                    medication.setAnalogs(characters.getData());
                    continue;
                }

                if (XML.VERSIONS.equalsTo(currentElement)) {
                    medication.setVersions(characters.getData());
                    continue;
                }

                if (XML.CERTIFICATE.equalsTo(currentElement)) {
                    medication.setCertificate(characters.getData());
                    continue;
                }

                if (XML.PACKAGE.equalsTo(currentElement)) {
                    medication.setPackagemy(characters.getData());
                    continue;
                }

                if (XML.DOSAGE.equalsTo(currentElement)) {
                    medication.setDosage(characters.getData());
                    continue;
                }
            }

            if (event.isEndElement()) {
                EndElement endElement = event.asEndElement();
                String localName = endElement.getName().getLocalPart();

                if (XML.MEDICATION.equalsTo(localName)) {
                    medicine.getMedications().add(medication);
                    continue;
                }

            }
        }
        reader.close();
    }

    public static void start() throws Exception {

        STAX staxContr = new STAX(Files.VALID_XML_FILE);
        staxContr.parse();

        Medicine medicine = staxContr.getMedicine();

        System.out.println("====================================");
        System.out.println("Here is the medicine medications: \n" + medicine);
        System.out.println("====================================");
    }
}