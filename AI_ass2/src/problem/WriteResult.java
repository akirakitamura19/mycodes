package problem;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;

public class WriteResult {
    public static void writeResult(){
        String string = "save me";
        
        try {
            BufferedWriter writer = 
                    new BufferedWriter (new FileWriter(".\\text.txt"));
            writer.write(string);
            
            writer.close();
            System.out.println(string);

        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
