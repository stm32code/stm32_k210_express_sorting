package com.example.expresssortingsystemdesign.utils;

import com.itfitness.mqttlibrary.MQTTHelper;

public class Common {
    //    public static String Port = "6002";
//    public static String Sever = "tcp://183.230.40.39" + ":" + Port;
    public static String Port = "1883";
    public static String Sever = "tcp://iot-06z00axdhgfk24n.mqtt.iothub.aliyuncs.com" + ":" + Port;

    public static String ReceiveTopic = "/broadcast/h9sjEGr1om8/test2";
    public static String PushTopic = "/broadcast/h9sjEGr1om8/test1";
    public static String DriveID = "h9sjEGr1om8.smartapp|securemode=2,signmethod=hmacsha256,timestamp=1714300879748|";
    public static String DriveName = "smartapp&h9sjEGr1om8";
    public static String DrivePassword = "30d2a7e6a81fcbaa83876a0fd7f9e761857949e4137d82e16d5f49703e74072d";
    public static String Drive2ID = "1181073144";
    public static String api_key = "l=yQf7V3jwBh10rp7rjt=5GUQfU=";
    public static boolean DeviceOnline = false;
    public static String LatestOnlineDate = "离线";
    public static MQTTHelper mqttHelper = null;
}
