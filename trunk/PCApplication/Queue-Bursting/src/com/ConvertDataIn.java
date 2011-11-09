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
	public static int LengthOfPacketID;
	public static int LengthOfProductID;
	
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
			
			if (iNumOfTypeProduct>9){
				LengthOfNum[IndexLengthOfNum++] = 2;
			}else if (iNumOfTypeProduct>99){
				LengthOfNum[IndexLengthOfNum++] = 3;
			}else{
				LengthOfNum[IndexLengthOfNum++] = 1;
			}
			int i = 3+ LengthOfPacketID +1;
			int _iNumOfEachProduct;
			while(i < 3+ LengthOfPacketID +1 + iNumOfTypeProduct*(LengthOfProductID+1)){
				_iNumOfEachProduct = _bDataIn[i+LengthOfProductID];
				if (_iNumOfEachProduct>9){
					LengthOfNum[IndexLengthOfNum++] = 2;
				}else if (_iNumOfEachProduct>99){
					LengthOfNum[IndexLengthOfNum++] = 3;
				}else{
					LengthOfNum[IndexLengthOfNum++] = 1;
				}
				
				_sDataOut += _sDataIn.substring(i, i+LengthOfProductID);
				_sDataOut += _iNumOfEachProduct;	
				
				i += LengthOfProductID +1;
				
			}
			
		}else{// add and remove
			_sDataOut = _sDataIn.substring(0, 14)+_bDataIn[14];
			if ((int)_bDataIn[14]>9){
				LengthOfNum[IndexLengthOfNum] = 2;
			}else if ((int)_bDataIn[14]>99){
				LengthOfNum[IndexLengthOfNum] = 3;
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
						_saDataOut[++_iNumOfTypeProduct] = _cReadDataBase.ReadExcel(_Path, 1, j);
						_saDataOut[++_iNumOfTypeProduct] = _cReadDataBase.ReadExcel(_Path, 2, j);
						_saDataOut[++_iNumOfTypeProduct] = _cReadDataBase.ReadExcel(_Path, 3, j);
						_saDataOut[++_iNumOfTypeProduct] = _sDataIn.substring(i-LengthOfNum[k],i);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
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
			_sTemp = _sDataIn.substring(1,14);
			for (j = 2; j < 14; j++) { // Check Database
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
					if(_sDataIn.substring(0,1).equals("+"))
						_saDataOut[0] = "Add more product";
					else
						_saDataOut[0] = "Remove product";
					
					_saDataOut[1] = _cReadDataBase.ReadExcel(_Path, 1, j);
					_saDataOut[2] = _cReadDataBase.ReadExcel(_Path, 2, j);
					_saDataOut[3] = _cReadDataBase.ReadExcel(_Path, 3, j);
					_saDataOut[4] = _sDataIn.substring(14,15);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}else
				_saDataOut[0] = "Not Found";
			    _saDataOut[1]=_sDataIn.substring(1,14);
			
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
