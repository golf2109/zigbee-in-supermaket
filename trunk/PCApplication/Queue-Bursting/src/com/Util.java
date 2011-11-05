package com;

public class Util {
   

    /**
     * Convert the byte array to an int starting from the given offset.
     *
     * @param b The byte array
     * @param offset The array offset
     * @return The integer
     */
    public static int byteArrayToInt(byte[] b,int length, int offset) {
        int value = 0;
        for (int i = 0; i < length; i++) {
            int shift = (length - 1 - i) * 8;
            value += (b[i + offset] & 0x000000FF) << shift;
        }
        return value;
    }
    public static void main(String arg[]){
    	byte[] byteArray = new byte[] {1,1};
    	System.out.println(byteArrayToInt(byteArray,2,0));
    	
    }
}
