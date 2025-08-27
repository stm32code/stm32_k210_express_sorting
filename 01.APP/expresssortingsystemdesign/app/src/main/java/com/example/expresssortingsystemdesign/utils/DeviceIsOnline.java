package com.example.expresssortingsystemdesign.utils;

import static com.example.expresssortingsystemdesign.utils.Common.Drive2ID;
import static com.example.expresssortingsystemdesign.utils.Common.api_key;

import android.util.Log;

import org.json.JSONObject;

import java.io.IOException;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

/***
 * 通过平台获取硬件设备在线信息
 */
public class DeviceIsOnline {
    public static void getOnline() {
        OkHttpClient okHttp = new OkHttpClient();
        Request request = new Request.Builder()
                .url("https://api.heclouds.com/devices/"+Drive2ID)
                .addHeader("api-key", api_key)
                .build();
        Call call = okHttp.newCall(request);
        call.enqueue(new Callback() {
            @Override
            public void onResponse(Call call, Response response) throws IOException {
                try {
                    String responseData = response.body().string();
                    Log.e("getOnline()", responseData);
                    JSONObject root = new JSONObject(responseData);
                    JSONObject data = root.getJSONObject("data");
                    Common.DeviceOnline = data.getBoolean("online");
                } catch (Exception e) {
                    e.printStackTrace();
                    Common.DeviceOnline = false;
                }
            }

            @Override
            public void onFailure(Call call, IOException e) {
                Common.DeviceOnline = false;
            }
        });
    }
}
