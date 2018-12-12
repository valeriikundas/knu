import java.util.ArrayList;
import java.util.List;

public class Medicine {

    private List<Medication> medications;

    public List<Medication> getMedications() {
        if (medications == null) {
            medications = new ArrayList<Medication>();
        }
        return medications;
    }

    @Override
    public String toString() {
        if (medications == null || medications.size() == 0) {
            return "Medicine contains no medications";
        }
        StringBuilder result = new StringBuilder("");
        for (Medication medication : medications) {
            result.append(medication).append("\n\n");
        }
        return result.delete(result.length()-2, result.length()).toString();
    }

}