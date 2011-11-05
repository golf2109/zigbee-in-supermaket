package com;

public class DataStruct {
	
		byte[] begin;
		byte[] PacketID;
		byte[] NumOfTypeProduct;
		byte[][] ProductID;
		public DataStruct(){
			begin =new byte[1];
			PacketID=new byte[7];
			NumOfTypeProduct= new byte[1];
			ProductID=new byte[1][14];
		}
		
}
