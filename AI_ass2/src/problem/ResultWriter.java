package problem;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;

public class ResultWriter {
	
		public static void write(String filename,ArrayList<String> content){
			try{
				File file = new File(filename);
				if (!file.exists()) {
					file.createNewFile();
				}
				FileWriter fw = new FileWriter(file.getAbsoluteFile());
				BufferedWriter bw = new BufferedWriter(fw);
				for(int i =0;i<content.size();i++){
					bw.write(content.get(i));
					//System.out.println(content.get(i));
					bw.newLine();
				}
				bw.close();
			} catch(IOException e){
				e.printStackTrace();
			}
			
		}
}
