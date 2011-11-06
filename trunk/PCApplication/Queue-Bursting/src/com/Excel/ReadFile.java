package com.Excel;

import java.io.File;
import java.io.IOException;

import jxl.Cell;
import jxl.Sheet;
import jxl.Workbook;
import jxl.read.biff.BiffException;

public class ReadFile {
	public String ReadExcel(String _InputFile, int _Col, int _Row) throws IOException  {
		File _fInputWorkbook = new File(_InputFile);
		Workbook _w=null;
			try {
				_w = Workbook.getWorkbook(_fInputWorkbook);
			} catch (BiffException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			// Get the first sheet
			Sheet sheet = _w.getSheet(0);
			// Loop over first 10 column and lines
			Cell _cCell = sheet.getCell(_Col, _Row);
			return _cCell.getContents();
	}
}
