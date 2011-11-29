package com;


public class My_Util {

	static private final String[] Digits = { "0", "1", "2", "3", "4", "5",
			"6", "7", "8", "9", "A", "B", "C", "D", "E", "F" };

	static public final String hex8Bit(int _val) {
		return (Digits[(_val >>> 4) & 0x0f] + 
				Digits[_val & 0x0f]);
	}

		
	static public final int get8b(byte _body[], int _off) {
		return ((int) (_body[_off]));
	}

} 


