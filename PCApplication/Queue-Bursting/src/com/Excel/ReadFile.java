package com.Excel;

import java.io.File;
import java.io.IOException;

import com.ConvertDataIn;
import com.Gui.MainGui;

import jxl.Cell;
import jxl.Sheet;
import jxl.Workbook;
import jxl.read.biff.BiffException;
/*
 * Read data from database, put it in array of array string
 * @param _InputFile path to Database.xls or MacTable.xls
 * @param _iRow number of row want to get from Database.xls or MacTable.xls
 * @param _iCol number of collumn want to get from Database.xls or MacTable.xls
 * return array of array string contain
 * 
 */
public class ReadFile {
	public String[][] ReadExcel(String _InputFile, int _iRow, int _iCol) throws IOException {
		File _fInputWorkbook = new File(_InputFile);
		
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
		if(_InputFile.equals(MainGui.PathDatabase))
		   ConvertDataIn.MAXROWDATABASE = sheet.getRows();
		else
		   ConvertDataIn.MAXROWNETWORK = sheet.getRows();
		String[][] _sResult = new String[sheet.getRows()][_iCol];
		for (int j = 2; j < sheet.getRows(); j++) {
			for (int i = 1; i < _iCol; i++) {
				_cCell = sheet.getCell(i, j);
				_sResult[j][i] = _cCell.getContents();
			}

		}
		return _sResult;
	}
}
