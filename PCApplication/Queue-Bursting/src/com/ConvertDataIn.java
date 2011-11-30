package com;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

import com.Excel.ReadFile;
import com.Uart.Read;
import com.Uart.Write;

public class ConvertDataIn {
	
	public static final String sDATE_FORMAT_NOW = "yyyy_MM_dd_HH_mm_ss";
	public static final String sDATE_FORMAT_NOW_ = "yyyy/MM/dd HH:mm:ss";

	static String sPathDatabase ="c:/temp/Database.xls";
	public int iNumOfTypeProduct ;
	public static String[] saProductIDError =  new String[100];
	public static int iNumOfProductIDError = 0;
	public static int[] LengthOfNum = new int[5000];
	public static int LengthOfPacketID = 8;
	public static int LengthOfProductID = 13;
	public static int ErrorID;
	
	public static int MacAddLen = 6;
	public static int ShortAddLen = 2;
	public static int ParentAddLen = 2;
	public static String MacAdd ="";
	private static boolean HasReadDatabase = false;
	static String[][] _Database = null;
	
	
	/* 
	 * Process data which was recv from the board through COM port
	 * @param _bDataIn : byte array data incoming from board EB through COM port
	 * return Data which was converted as String
	 */
	public String ConvertDataFromBoard(byte[] _bDataIn, int _bType) {

		
		String _sDataIn = new String(_bDataIn);
		String _sDataOut = "";
		int IndexLengthOfNum = 0;
		int i = 3 + MacAddLen + LengthOfPacketID +1;
		int j;
		ErrorID = -1; 
		switch (_bType){
			case 0:// normal packet
				for( j =1; j < 1 + MacAddLen; j++ ){
					
					MacAdd += My_Util.hex8Bit(My_Util.get8b(_bDataIn, j));
					if(j != MacAddLen) MacAdd += "-";
					System.out.println(MacAdd);
				}
				LengthOfPacketID = _bDataIn[1 + MacAddLen];
				LengthOfProductID = _bDataIn[2 + MacAddLen];
				_sDataOut += _sDataIn.substring(3 + MacAddLen, 3 + MacAddLen + LengthOfPacketID)+ 
				                                _bDataIn[3 + MacAddLen + LengthOfPacketID]; //Get Packet Id and Number Of Type Product
				iNumOfTypeProduct = _bDataIn[3 + MacAddLen + LengthOfPacketID];
				if (iNumOfTypeProduct>99){
					LengthOfNum[IndexLengthOfNum++] = 3;
				}else if (iNumOfTypeProduct>9){
					LengthOfNum[IndexLengthOfNum++] = 2;
				}else{
					LengthOfNum[IndexLengthOfNum++] = 1;
				}
				int _iNumOfEachProduct;
				while(i < 3 + MacAddLen + LengthOfPacketID +1 + iNumOfTypeProduct*(LengthOfProductID+1)){
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
				//delete packet from board
				String _sDeletePacket = "^";
				byte _bNum = 1;
				_sDeletePacket += _bNum + _sDataIn.substring(3 + MacAddLen, 3 + MacAddLen + LengthOfPacketID);
				if(Read.serialPort != null)
					Write.SendData(_sDeletePacket);
				break;
			case 1:// add and remove product
				_sDataOut = _sDataIn.substring(0, LengthOfProductID+1)+_bDataIn[LengthOfProductID+1];
				if ((int)_bDataIn[14]>99){
					LengthOfNum[IndexLengthOfNum] = 3;
				}else if ((int)_bDataIn[14]>9){
					LengthOfNum[IndexLengthOfNum] = 2;
				}else{
					LengthOfNum[IndexLengthOfNum] = 1;
				}
				break;
			case 2:// handle Error
				ErrorID = _bDataIn[1];
				_sDataOut = _sDataIn.substring(2, 2 + LengthOfPacketID);
				break;
			case 3:// handle Status
				for(i = 1; i < 1 + MacAddLen + ShortAddLen + ParentAddLen; i++){
					_sDataOut += My_Util.hex8Bit(My_Util.get8b(_bDataIn, i));
					
					if (i!=  MacAddLen + ShortAddLen + ParentAddLen)
						_sDataOut += "-";					
				}
				//_sDataOut = _sDataIn.substring(1, 1 + MacAddLen + ShortAddLen + ParentAddLen);
				break;
		}//End Switch
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
		
		if(!HasReadDatabase){
			try {
				switch(_iType){
				case 0:
				case 1:
					_Database = _cReadDataBase.ReadExcel(_Path, 14, 4);
					break;
				case 2:
					_Database = _cReadDataBase.ReadExcel(_Path, 14, 3);
					break;
				}
			} catch (IOException e) {
				System.out.println("Read Database error");
			}
			HasReadDatabase = true;
		}//end if
		String[] _saDataOut;
		_saDataOut = new String[4 *iNumOfTypeProduct+1];
		switch (_iType){
			case 0:// normal packet
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
				break;
			case 1:// add or remove product
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
				break;
			case 2:// status network
				_saDataOut = new String[4];
				_inDatabase = false;
				_saDataOut[0] =  _sDataIn.substring(0 , 0 + MacAddLen*3 - 1);
				_saDataOut[1] =  _sDataIn.substring(0 + MacAddLen*3 , 0 + MacAddLen*3 + ShortAddLen*2 + 1);
				_saDataOut[2] =  _sDataIn.substring(0 + MacAddLen*3 + ShortAddLen*2 + 2 );

				_sTemp = _sDataIn.substring(0 , 0 + MacAddLen*3 - 1);
				
				for (j = 2; j < 14; j++) { // Check Database
					if (_Database[j][1].equals(_sTemp)) {
						_inDatabase = true;
						break;
					}
				}
				if (_inDatabase) {
					_saDataOut[3] = _Database[j][2];
	
				}else
					_saDataOut[3] = "Not Found";
				break;
			
		}//end switch
		return _saDataOut;
	}

	public static String now() {
		Calendar cal = Calendar.getInstance();
		SimpleDateFormat sdf = new SimpleDateFormat(sDATE_FORMAT_NOW);
		return sdf.format(cal.getTime());

	}
	public static String now1() {
		Calendar cal = Calendar.getInstance();
		SimpleDateFormat sdf = new SimpleDateFormat(sDATE_FORMAT_NOW_);
		return sdf.format(cal.getTime());

	}
	public static void WriteLog(String _sInfo){
			BufferedWriter bufferedWriter = null;
		
		try {
			bufferedWriter = new BufferedWriter(new FileWriter("C:\\temp\\Error.log", true));
			synchronized (bufferedWriter) {
				bufferedWriter.write(now1()+" : " + _sInfo );
				bufferedWriter.newLine();
				try {
					// Waiting to get new time 
					Thread.sleep(1000);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			if (bufferedWriter != null) {
				try {
					bufferedWriter.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}
	public static String GerneralPathToWrite(String _PacketID) {
		return "c:/temp/Date " + now() + "__" + _PacketID + ".xls";
		
	}

}