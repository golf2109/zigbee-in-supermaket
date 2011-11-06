package com;

import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import com.Excel.ReadFile;

public class ConvertDataIn {
	
	public static final String sDATE_FORMAT_NOW = "yyyy_MM_dd_HH_mm_ss";
	static String sPathDatabase ="c:/temp/Database.xls";
	public int iNumOfTypeProduct ;
	public static String[] saProductIDError =  new String[100];
	public static int iNumOfProductIDError = 0;
	
	/* 
	 * Process data which was recv from the booad through COM port
	 * @param _bDataIn : byte array data incoming from board EB through COM port
	 * return Data which was converted as String
	 */
	public String ConvertDataFromBoard(byte[] _bDataIn) {

		int i = 9;
		String _sDataIn = new String(_bDataIn);
		String _sDataOut = "";
		
		_sDataOut += _sDataIn.substring(0, 8)+ _bDataIn[8]; //Get Packet Id and Number Of Tpye Product
		
		int _iNumOfTypeProduct = _bDataIn[8];
		int _iNumOfEachProduct;
		while(i < 14*_iNumOfTypeProduct + 9){
			_sDataOut += _sDataIn.substring(i, i+13);
			_iNumOfEachProduct = _bDataIn[i+13];
			_sDataOut += _iNumOfEachProduct;
			i +=14;
		}
		return _sDataOut;
	}
	
	/* 
	 * Compare product ID with database, find price
	 * @param Path : Path to Database.xls
	 * @param _sDataIn : String Data return by ConvertDataFromBoard()
	 * return Array of String with all infomation of all product of packet
	 */
	public String[] GetDataFromDatabase(String _Path, String _sDataIn) {
		int i = 9;
		int j = 1;
		boolean _inDatabase = false;
		iNumOfTypeProduct = 0;
		String _sTemp;
		ReadFile _cReadDataBase = new ReadFile();
		String[] _saDataOut = new String[4 * ((_sDataIn.length() - 8)/14)+1];
		_saDataOut[0] = _sDataIn.substring(0, 8); // packet ID
		while(i < _sDataIn.length()) {
			_sTemp = _sDataIn.substring(i,i+13);
			i += 14;
			_inDatabase = false;
			for (j = 2; j < 12; j++) { // Check Database
				try {
					if (_cReadDataBase.ReadExcel(_Path, 1, j).equals(_sTemp)) {
						_inDatabase = true;
						break;
					}
				} catch (IOException e) {
					// TODO Auto-generated catch block
					System.out.println("Can't File " + _Path);
				}
			}
			if (_inDatabase) {
				try {
					iNumOfTypeProduct++;
					_saDataOut[iNumOfTypeProduct] = _cReadDataBase.ReadExcel(_Path, 1, j);
					iNumOfTypeProduct++;
					_saDataOut[iNumOfTypeProduct] = _cReadDataBase.ReadExcel(_Path, 2, j);
					iNumOfTypeProduct++;
					_saDataOut[iNumOfTypeProduct] = _cReadDataBase.ReadExcel(_Path, 3, j);
					iNumOfTypeProduct++;
					_saDataOut[iNumOfTypeProduct] = _sDataIn.substring(i-1,i);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			} else{ //Product ID isn't exist in database, something error
				saProductIDError[iNumOfProductIDError] = _sTemp; 
				iNumOfProductIDError++;
			}
			
		}
		return _saDataOut;
	}

	public static String now() {
		Calendar cal = Calendar.getInstance();
		SimpleDateFormat sdf = new SimpleDateFormat(sDATE_FORMAT_NOW);
		return sdf.format(cal.getTime());

	}
	public static String GerneralPathToWrite(String _PacketID) {
		return "c:/temp/Date " + now() + "__" + _PacketID + ".xls";
		
	}

//	public static void main(String args[]) {//just for test
//		byte[] byteArray = new byte[] { 69, 
//				                        55, 55, 55, 55, 55, 55, 55, 
//										2,
//										55, 55, 55, 55, 55, 55, 55, 56, 56, 56,56,55, 55, 2,
//										53, 52, 55, 55, 55, 55, 55, 56, 56, 56,56,52, 52, 2
//										};
//		ConvertDataIn _testGetData = new ConvertDataIn();
//		String _a = _testGetData.ConvertDataFromBoard(byteArray);
//		String [] _b= _testGetData.GetDataFromDatabase(sPathDatabase, _a);
//		for(int i=0; i< iNumOfTypeProduct+1;i++){
//		System.out.print(_b[i]+" ");
//		}
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
//}
