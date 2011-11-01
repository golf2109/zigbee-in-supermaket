package com.Excel;

import java.io.File;
import java.io.IOException;

import jxl.Cell;
import jxl.Sheet;
import jxl.Workbook;
import jxl.read.biff.BiffException;

public class ReadFile {
	public String read(String inputFile, int col, int row) throws IOException  {
		File inputWorkbook = new File(inputFile);
		Workbook w=null;
			try {
				w = Workbook.getWorkbook(inputWorkbook);
			} catch (BiffException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			// Get the first sheet
			Sheet sheet = w.getSheet(0);
			// Loop over first 10 column and lines
			Cell cell = sheet.getCell(col, row);
			return cell.getContents();
	}
}
