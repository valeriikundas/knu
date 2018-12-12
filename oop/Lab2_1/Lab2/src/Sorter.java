import Constants.Files;

import java.util.Collections;
import java.util.Comparator;
public class Sorter {

    public static final Comparator<Medication> SORT_MEDICATION_BY_NAME =
            new Comparator<Medication>() {
                @Override
                public int compare(Medication o1, Medication o2) {
                    return o1.getName().compareTo(o2.getName());
                }
            };

    public static final Comparator<Medication> SORT_MEDICATION_BY_Analogs =
            new Comparator<Medication>() {
                @Override
                public int compare(Medication o1, Medication o2) {
                    return o1.getAnalogs().compareTo(o2.getAnalogs());
                }
            };

    public static final Comparator<Medication> SORT_MEDICATION_BY_VERSIONS =
            new Comparator<Medication>() {
                @Override
                public int compare(Medication o1, Medication o2) {
                    return o1.getVersions().compareTo(o2.getVersions());
                }
            };

    public static final void sortMedicationByName(Medicine medicine) {
        Collections.sort(medicine.getMedications(), SORT_MEDICATION_BY_NAME);
    }

    public static final void sortMedicationByAnalogs(Medicine medicine) {
        Collections.sort(medicine.getMedications(), SORT_MEDICATION_BY_Analogs);
    }

    public static final void sortMedicationByVersions(Medicine medicine) {
        Collections.sort(medicine.getMedications(), SORT_MEDICATION_BY_VERSIONS);
    }

    public static void main(String[] args) throws Exception {

        DOM dom = new DOM(
                Files.VALID_XML_FILE);
        dom.parse(false);
        Medicine medicine = dom.getMedicine();

        System.out.println("====================================");
        System.out.println(medicine);
        System.out.println("====================================");

        System.out.println("====================================");
        Sorter.sortMedicationByName(medicine);
        System.out.println(medicine);
        System.out.println("====================================");

        System.out.println("====================================");
        Sorter.sortMedicationByAnalogs(medicine);
        System.out.println(medicine);

        System.out.println("====================================");
        Sorter.sortMedicationByVersions(medicine);
        System.out.println(medicine);
    }
}