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
	public static int NumOfTypeProduct ;
	
	public String ConvertDataFromBoard(byte[] Data) {

//		ObjectInputStream in;
//		DataStruct message = null;
//		ByteArrayInputStream _ByteInputStream =new ByteArrayInputStream(Data);
//		try {
//			in = new ObjectInputStream(_ByteInputStream);
//			try {
//				message = (DataStruct) in.readObject();
//			} catch (ClassNotFoundException e) {
//				// TODO Auto-generated catch block
//				e.printStackTrace();
//			}
//			in.close();
//		} catch (IOException e) {
//			// TODO Auto-generated catch block
//			e.printStackTrace();
//		}
		int i = 9;
		String temp1 = new String(Data);
		String result = "";
		result += temp1.substring(0, 8)+Data[8];
		int length = Data[8];
		while(i < 14*length+9){
		
				result += temp1.substring(i, i+13);
				int NumOfEachProduct = Data[i+13];
				result += NumOfEachProduct;
				i +=14;
		}

//		ProductID = new byte[a.byteArrayToInt(NumOfTypeProduct,1,0)][14];
		return result;
	}

	public String[] GetDataFromDatabase(String Path,String Data,int length) {
		int i = 9;
		int j = 1;
		boolean _inDatabase = false;
		NumOfTypeProduct = 0;
		String temp;
		ReadFile ReadDataBase = new ReadFile();
		String[] Result = new String[4 * ((length - 8)/14)+1];
		Result[0] = Data.substring(0, 8);// packet ID
		while(i < length) {
			
			temp = Data.substring(i,i+13);
			
			i += 14;
			_inDatabase = false;
			for (j = 2; j < 12; j++) {
				try {
					if (ReadDataBase.read(Path, 1, j).equals(temp)) {
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
					NumOfTypeProduct++;
					Result[NumOfTypeProduct] = ReadDataBase.read(Path, 1, j);
					NumOfTypeProduct++;
					Result[NumOfTypeProduct] = ReadDataBase.read(Path, 2, j);
					NumOfTypeProduct++;
					Result[NumOfTypeProduct] = ReadDataBase.read(Path, 3, j);
					NumOfTypeProduct++;
					Result[NumOfTypeProduct] = Data.substring(i-1,i);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
			
		}
//		System.out.println(Result.length);
//		System.out.println(NumOfTypeProduct);
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
		byte[] byteArray = new byte[] { 69, 
				                        55, 55, 55, 55, 55, 55, 55, 
										2,
										55, 55, 55, 55, 55, 55, 55, 56, 56, 56,56,55, 55, 2,
										53, 52, 55, 55, 55, 55, 55, 56, 56, 56,56,52, 52, 2
										};
		RevData _testGetData = new RevData();
		String _a = _testGetData.ConvertDataFromBoard(byteArray);
		String [] _b= _testGetData.GetDataFromDatabase(PathDatabase, _a, _a.length());
		for(int i=0; i< NumOfTypeProduct+1;i++){
		System.out.print(_b[i]+" ");
		}
//		
//		WriteFile _testWrite = new WriteFile();
//		String Path = GerneralPathToWrite(_b[0]);
//		try {
//			_testWrite.writeExcel(Path, _b, NumOfTypeProduct+1,true);
//		} catch (WriteException e) {
//			// TODO Auto-generated catch block
//			e.printStackTrace();
//		} catch (IOException e) {
//			// TODO Auto-generated catch block
//			e.printStackTrace();
//		}
	}
}
