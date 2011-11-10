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
	public static int[] LengthOfNum = new int[5000];
	public static int LengthOfPacketID = 8;
	public static int LengthOfProductID = 13;
	
	/* 
	 * Process data which was recv from the board through COM port
	 * @param _bDataIn : byte array data incoming from board EB through COM port
	 * return Data which was converted as String
	 */
	public String ConvertDataFromBoard(byte[] _bDataIn, int _bType) {

		
		String _sDataIn = new String(_bDataIn);
		String _sDataOut = "";
		int IndexLengthOfNum = 0;
		if (_bType == 0){
			LengthOfPacketID = _bDataIn[1];
			LengthOfProductID = _bDataIn[2];
			_sDataOut += _sDataIn.substring(3, 3+ LengthOfPacketID)+ _bDataIn[3+LengthOfPacketID]; //Get Packet Id and Number Of Type Product
			iNumOfTypeProduct = _bDataIn[3+ LengthOfPacketID];
			if (iNumOfTypeProduct>99){
				LengthOfNum[IndexLengthOfNum++] = 3;
			}else if (iNumOfTypeProduct>9){
				LengthOfNum[IndexLengthOfNum++] = 2;
			}else{
				LengthOfNum[IndexLengthOfNum++] = 1;
			}
			int i = 3+ LengthOfPacketID +1;
			int _iNumOfEachProduct;
			while(i < 3+ LengthOfPacketID +1 + iNumOfTypeProduct*(LengthOfProductID+1)){
				_iNumOfEachProduct = _bDataIn[i+LengthOfProductID];
				if (_iNumOfEachProduct>99){
					LengthOfNum[IndexLengthOfNum++] = 3;
				}else if (_iNumOfEachProduct>9){
					LengthOfNum[IndexLengthOfNum++] = 2;
				}else{
					LengthOfNum[IndexLengthOfNum++] = 1;
				}
				
				_sDataOut += _sDataIn.substring(i, i+LengthOfProductID);
				_sDataOut += _iNumOfEachProduct;	
				
				i += LengthOfProductID +1;
				
			}
			
		}else{// add and remove
			_sDataOut = _sDataIn.substring(0, LengthOfProductID+1)+_bDataIn[LengthOfProductID+1];
			if ((int)_bDataIn[14]>99){
				LengthOfNum[IndexLengthOfNum] = 3;
			}else if ((int)_bDataIn[14]>9){
				LengthOfNum[IndexLengthOfNum] = 2;
			}else{
				LengthOfNum[IndexLengthOfNum] = 1;
			}
		}
		return _sDataOut;
	}
	
	/* 
	 * Compare product ID with database, find price
	 * @param Path : Path to Database.xls
	 * @param _sDataIn : String Data return by ConvertDataFromBoard()
	 * return Array of String with all infomation of all product of packet
	 */
	public String[] GetDataFromDatabase(String _Path, String _sDataIn, int _iType) {
		int k=0;
		int i = LengthOfPacketID + LengthOfNum[k];
		int j = 2;
		boolean _inDatabase = false;
		int _iNumOfTypeProduct = 0;
		String _sTemp;
		ReadFile _cReadDataBase = new ReadFile();
		String[][] _Database = null;
		try {
			_Database = _cReadDataBase.ReadExcel(_Path, 14);
		} catch (IOException e) {
			System.out.println("Read Database error");
		}
		String[] _saDataOut;
		_saDataOut = new String[4 *iNumOfTypeProduct+1];
		if(_iType == 0){
			_saDataOut[0] = _sDataIn.substring(0, i); // packet ID
			while(i < _sDataIn.length()) {
				k++;
				_sTemp = _sDataIn.substring(i, i+ LengthOfProductID);
				i += LengthOfProductID+ LengthOfNum[k];
				_inDatabase = false;
				
					for (j = 2; j < 14; j++) { // Check Database
						if (_Database[j][1].equals(_sTemp)) {
							_inDatabase = true;
							break;
						}
					}
				
				if (_inDatabase) {
					_saDataOut[++_iNumOfTypeProduct] = _Database[j][1];
					_saDataOut[++_iNumOfTypeProduct] = _Database[j][2];
					_saDataOut[++_iNumOfTypeProduct] = _Database[j][3];
					_saDataOut[++_iNumOfTypeProduct] = _sDataIn.substring(i-LengthOfNum[k],i);
				} else{ //Product ID isn't exist in database, something error
					_saDataOut[++_iNumOfTypeProduct]=_sTemp;
					_saDataOut[++_iNumOfTypeProduct]="Not found ";
					_saDataOut[++_iNumOfTypeProduct]="0";
					_saDataOut[++_iNumOfTypeProduct] = _sDataIn.substring(i-LengthOfNum[k],i);;				
					
				}					
				}
			}else{////////////////////////////////////////////////
			_saDataOut = new String[5];
			_inDatabase = false;
			_sTemp = _sDataIn.substring(1,LengthOfProductID+1);
			
			for (j = 2; j < 14; j++) { // Check Database
				if (_Database[j][1].equals(_sTemp)) {
					_inDatabase = true;
					break;
				}
			}
			if (_inDatabase) {
				if(_sDataIn.substring(0,1).equals("+"))
					_saDataOut[0] = "Add more product";
				else
					_saDataOut[0] = "Remove product";
				
				_saDataOut[1] = _Database[j][1];
				_saDataOut[2] = _Database[j][2];
				_saDataOut[3] = _Database[j][3];
				_saDataOut[4] = _sDataIn.substring(LengthOfProductID+ 1 ,LengthOfProductID+ 1+ LengthOfNum[0]);

			}else
				_saDataOut[0] = "Not Found";
			    _saDataOut[1]=_sDataIn.substring(1,LengthOfProductID+1);
			
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

}