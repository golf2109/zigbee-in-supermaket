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

public class WriteFile {
	private WritableCellFormat timesBoldUnderline;
	private WritableCellFormat times;
	private String inputFile;
	String Label1[] = { "IDProduct", "NameProduct", "PRICE" };
	String Label2[] = { "IDCustommer", "IDProduct" };

	public void writeExcel(String Path, int column, int type)
			throws IOException, WriteException {
		File file = new File(Path);
		// Workbook workbook = application.createWorkbook("Custom title");
		WorkbookSettings wbSettings = new WorkbookSettings();

		wbSettings.setLocale(new Locale("en", "EN"));

		WritableWorkbook workbook = Workbook.createWorkbook(file, wbSettings);
		workbook.createSheet("List", 0);// first create sheet
		WritableSheet excelSheet = workbook.getSheet(0);
		createLabel(excelSheet, column, type);
		createContent(excelSheet, column, type);

		workbook.write();
		workbook.close();
	}

	private void createLabel(WritableSheet sheet, int column, int type)
			throws WriteException {
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
		int i;
		String label[] = new String[10];
		switch (type) {
		case 1:// list of product
			for (i = 0; i < Label1.length; i++) {
				label[i] = Label1[i];
			}

		case 2:// list of custommer
			for (i = 0; i < Label2.length; i++) {
				label[i] = Label2[i];
			}
		case 3:// list of product of each custommer
		}

		for (i = 0; i < column; i++) {
			addCaption(sheet, i, 0, label[i]);
		}
	}

	private void createContent(WritableSheet sheet, int column, int type)
			throws WriteException, RowsExceededException {
		String name = "Product ";

		switch (type) {
		case 1:// list of product
			for (int i = 1; i < 10; i++) {
				// First column
				addNumber(sheet, 0, i, i + 100);
				// Second column
				addLabel(sheet, 1, i, name);
				// Thrid column
				addNumber(sheet, 2, i, i + 10);
			}
			// //

		case 2:// list of custommer
			for (int i = 1; i < 10; i++) {
				// First column
				addNumber(sheet, 0, i, i + 100);
				// Second column
				addNumber(sheet, 1, i, i);
			}
			// //
		case 3:// list of product of each custommer
		}

		// StringBuffer buf = new StringBuffer();
		// buf.append("SUM(A2:A10)");
		// Formula f = new Formula(0, 10, buf.toString());
		// sheet.addCell(f);
		// buf = new StringBuffer();
		// buf.append("SUM(B2:B10)");
		// f = new Formula(1, 10, buf.toString());
		// sheet.addCell(f);
		//
		// // Now a bit of text
		// for (int i = 12; i < 20; i++) {
		// // First column
		// addLabel(sheet, 0, i, "Boring text " + i);
		// // Second column
		// addLabel(sheet, 1, i, "Another text");
		// }
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
