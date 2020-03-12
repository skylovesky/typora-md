package com.springboot.sky.study.test;

import java.math.BigInteger;
import java.util.*;

public class MainTest {

    public static void main(String[] args) {
        String str = "5E266335";
        getCode(str);
    }

    public static String getCode(String str){
        List<String> list = new ArrayList<>();
        String hex = "";
        for (int i = 0 ; i < str.length() ; i++){
            if (i % 2==0){
                hex = str.substring(i,i+1);
            }else {
                hex += str.substring(i,i+1);
                list.add(hex);
            }
        }
        byte[] bytes = new byte[list.size()];
        int j = 0 ;
        for (int i = list.size()-1;i >=0 ; i--){
            BigInteger bigInteger = new BigInteger(list.get(i),16);
            bytes[j] = bigInteger.byteValueExact();
            System.out.println(bytes[j]);
            j++;
        }
        int code = CRC16Util.CRC16_ccitt(bytes);
        String result = ""+Integer.toHexString(code).toUpperCase();
        if (4-result.length()>0){
            for (int i = 0 ; i< 4-result.length();i++){
                result = "0"+result;
            }
        }
        System.out.println(result);
        return result;
    }

}
