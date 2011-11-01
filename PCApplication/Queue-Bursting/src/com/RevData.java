package com;

import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Calendar;

import jxl.write.WriteException;

import com.Excel.ReadFile;
import com.Excel.WriteFile;

public class RevData {
	public static final String DATE_FORMAT_NOW = "yyyy_MM_dd_HH_mm_ss";
	static String PathDatabase ="c:/temp/Database.xls";
	
	public String[] ConvertDataFromBoard(byte[] Data, int length) {
		int i = 1;
		int j = 1;
		String[] Result = new String[1 + length / 8];
		String temp = new String(Data);
		Result[0] = temp.substring(0, 1);
		while (i < length) {
			Result[j] = temp.substring(i, i + 8);
			i = i + 8;
			j++;
		}
		return Result;
	}

	public String[] GetDataFromDatabase(String Path, String[] Data, int length) {
		int i = 1;
		int j = 1;
		boolean _inDatabase = false;
		int count = 0;
		ReadFile ReadDataBase = new ReadFile();
		String[] Result = new String[3 * (length - 1) + 1];
		Result[0] = Data[0];// packet ID

		for (i = 1; i < length; i++) {
			_inDatabase = false;
			for (j = 2; j < 12; j++) {
				try {
					if (ReadDataBase.read(Path, 1, j).equals(Data[i])) {
						_inDatabase = true;
						break;
					}
				} catch (IOException e) {
					// TODO Auto-generated catch block
					System.out.println("Database.xls is non Exist");;
				}
			}
			if (_inDatabase) {
				try {
					count++;
					Result[count] = ReadDataBase.read(Path, 1, j);
					count++;
					Result[count] = ReadDataBase.read(Path, 2, j);
					count++;
					Result[count] = ReadDataBase.read(Path, 3, j);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
		return Result;
	}

	public static String now() {
		Calendar cal = Calendar.getInstance();
		SimpleDateFormat sdf = new SimpleDateFormat(DATE_FORMAT_NOW);
		return sdf.format(cal.getTime());

	}
	public static String GerneralPathToWrite(String _PacketID) {
		return "c:/temp/Date " + now() + "__" + _PacketID + ".xls";
		
	}

	public static void main(String args[]) {
		byte[] byteArray = new byte[] { 69, 57, 48, 49, 50, 51, 52, 53, 48, 
				                            57, 49,	50, 51, 52, 53, 54, 55
		// 69, 69, 69, 69, 69, 69, 69
		};
		RevData _testGetData = new RevData();
		String[] _a = _testGetData.ConvertDataFromBoard(byteArray, byteArray.length);
		
		
		String [] _b= _testGetData.GetDataFromDatabase(PathDatabase, _a, _a.length);
		for(int i=0; i< _b.length;i++){
		System.out.print(_b[i]+" ");
		}
		
		WriteFile _testWrite = new WriteFile();
		String Path = GerneralPathToWrite(_b[0]);
		try {
			_testWrite.writeExcel(Path, _b, _b.length);
		} catch (WriteException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
