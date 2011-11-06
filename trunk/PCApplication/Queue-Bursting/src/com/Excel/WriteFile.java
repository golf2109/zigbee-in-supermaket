package com.Excel;

import java.io.File;
import java.io.IOException;
import java.util.Locale;

import jxl.CellView;
import jxl.Workbook;
import jxl.WorkbookSettings;
import jxl.format.UnderlineStyle;
import jxl.write.Label;
import jxl.write.Number;
import jxl.write.WritableCellFormat;
import jxl.write.WritableFont;
import jxl.write.WritableSheet;
import jxl.write.WritableWorkbook;
import jxl.write.WriteException;
import jxl.write.biff.RowsExceededException;

import com.Gui.MainGui;



public class WriteFile {
	private WritableCellFormat timesBoldUnderline;
	private WritableCellFormat times;
	String Label1[] = { "Index", "IDProduct", "NameProduct", "PRICE", "NumOfProduct" };
	int row = 0;
	int total = 0;
	int num =0;
	boolean done=false; 
	int i=0;
	public void  WriteExcel(String _sPath)
			throws IOException, WriteException {
			File file = new File(_sPath);
			WorkbookSettings wbSettings = new WorkbookSettings();
			wbSettings.setLocale(new Locale("en", "EN"));
			WritableWorkbook workbook = Workbook.createWorkbook(file, wbSettings);
			workbook.createSheet("ListProduct", 0);// first create sheet
			WritableSheet excelSheet = workbook.getSheet("ListProduct");
			for(i = 0; i < MainGui.numofpacket; i++){
				if(i==0) createLabel(_sPath,excelSheet,true);
				else  createLabel(_sPath,excelSheet,false);
				createContent(excelSheet, MainGui._d[i], MainGui.NumOfNumProduct[i]+1);
			}
			workbook.write();
			workbook.close();
	}

	private void createLabel(String Path,WritableSheet sheet, boolean first) throws WriteException {
		// Lets create a times font
		WritableFont times10pt = new WritableFont(WritableFont.TIMES, 10);
		// Define the cell format
		times = new WritableCellFormat(times10pt);
		// Lets automatically wrap the cells
		times.setWrap(true);

		// Create create a bold font with unterlines
		WritableFont times18ptBoldUnderline = new WritableFont(
				WritableFont.TIMES, 16, WritableFont.BOLD, false,
				UnderlineStyle.NO_UNDERLINE);
		timesBoldUnderline = new WritableCellFormat(times18ptBoldUnderline);
		// Lets automatically wrap the cells
		timesBoldUnderline.setWrap(true);

		CellView cv = new CellView();
		cv.setFormat(times);
		cv.setFormat(timesBoldUnderline);
		cv.setAutosize(true);
		// Write a few headers
		
		addCaption(sheet, 0, row++, MainGui._d[i][0]); // Write Packet ID
		if(first)
		for (int i = 0; i < 5; i++) {
			addCaption(sheet, i, row, Label1[i]);
		}
		row++;
	}
	private void createContent(WritableSheet sheet, String[] Data, int length)
			throws WriteException, RowsExceededException {
		int col;
		int i1=1;
		int t=1;
		int _total =0;
		while (i1 < length-1) {
			addNumber(sheet, 0, row,t++ ); //index
			for (col = 1; col <= 4; col++) {
				addLabel(sheet, col, row, Data[i1]);
				if(col==3&&Data[i1]!=null)
				_total += Integer.parseInt(Data[i1])*Integer.parseInt(Data[i1+1]);
				
				i1++;
			}
			row++;
			col = 0;
		}	
		total += _total;
		addLabel(sheet, 1, row, "Total");
		addLabel(sheet, 2, row, t-1 +" Product");
		addNumber(sheet, 3, row, _total);
		num +=t-1;
		row++;
		if(i == MainGui.numofpacket-1){
			addLabel(sheet, 1, row+1, "Total");
			addLabel(sheet, 2, row+1, num +" Product");
			addNumber(sheet, 3, row+1, total);
		}
}

	private void addCaption(WritableSheet sheet, int column, int row, String s)
			throws RowsExceededException, WriteException {
		Label label;
		label = new Label(column, row, s, timesBoldUnderline);
		sheet.addCell(label);
	}

	private void addNumber(WritableSheet sheet, int column, int row,
			Integer integer) throws WriteException, RowsExceededException {
		Number number;
		number = new Number(column, row, integer, times);
		sheet.addCell(number);
	}

	private void addLabel(WritableSheet sheet, int column, int row, String s)
			throws WriteException, RowsExceededException {
		Label label;
		label = new Label(column, row, s, times);
		sheet.addCell(label);
	}
}
