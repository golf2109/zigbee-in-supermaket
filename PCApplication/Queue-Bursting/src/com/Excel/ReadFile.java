package com.Excel;

import java.io.File;
import java.io.IOException;

import jxl.Cell;
import jxl.Sheet;
import jxl.Workbook;
import jxl.read.biff.BiffException;
/*
 * Read data from database, put it in array of array string
 * @param _InputFile path to Database.xls
 * @param _Num number of data product want to get from Database.xls
 * return array of array string contain _Num data product
 * 
 */
public class ReadFile {
	public String[][] ReadExcel(String _InputFile, int _Num) throws IOException {
		File _fInputWorkbook = new File(_InputFile);
		String[][] _sResult = new String[_Num][4];
		Cell _cCell;
		Workbook _w = null;
		try {
			_w = Workbook.getWorkbook(_fInputWorkbook);
		} catch (BiffException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		// Get the first sheet
		Sheet sheet = _w.getSheet(0);
		// Loop over first 10 column and lines
		for (int j = 2; j < _Num; j++) {
			for (int i = 1; i < 4; i++) {
				_cCell = sheet.getCell(i, j);
				_sResult[j][i] = _cCell.getContents();
			}

		}
		return _sResult;
	}
}
