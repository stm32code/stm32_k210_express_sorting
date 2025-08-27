#include "git.h"

Data_TypeDef Data_init;						  // 设备数据结构体
Threshold_Value_TypeDef threshold_value_init; // 设备阈值设置结构体
Device_Satte_Typedef device_state_init;		  // 设备状态

extern U8 MG996R_TEST; // 变速测试
// 获取数据参数
mySta Read_Data(Data_TypeDef *Device_Data)
{


	return MY_SUCCESSFUL;
}
// 初始化
mySta Reset_Threshole_Value(Threshold_Value_TypeDef *Value, Device_Satte_Typedef *device_state)
{

	device_state->open = 1;
  // 写
  //W_Test();
	// 读
	R_Test();
	// 状态重置
	device_state->check_device = 0;
	return MY_SUCCESSFUL;
}
// 更新OLED显示屏中内容
mySta Update_oled_massage()
{
#if OLED // 是否打开
	char str[50];

	sprintf(str, "编码   ");
	OLED_ShowCH(0, 0, (unsigned char *)str);
	sprintf(str, "%04d-%02d-%02d ",device_state_init.expressage_city,
	device_state_init.expressage_area,device_state_init.expressage_id);
	OLED_ShowCH(0, 2, (unsigned char *)str);

	if(Data_init.color==0)
	{
		sprintf(str, "状态: 红色   ");
	}else if(Data_init.color==1)
	{
		sprintf(str, "状态: 绿色   ");
	}else if(Data_init.color==2)
	{
		sprintf(str, "状态: 蓝色   ");
	}
	OLED_ShowCH(0, 4, (unsigned char *)str);


#endif

	return MY_SUCCESSFUL;
}

// 更新设备状态
mySta Update_device_massage()
{


	// 监测到物体到达
	if( device_state_init.transport_state == 1){

		device_state_init.expressage_state = 2; //入库成功
		if (Data_init.App == 0) {
				Data_init.App = 1;
		}
		device_state_init.transport_time = 3; //延迟5S
	}

	if (Data_init.App)
	{
		switch (Data_init.App)
		{
		case 1:
			SendMqtt(1); // 发送数据到APP
			break;
		case 2:
			SendData(); //  发送到云平台
			break;
		}
		Data_init.App = 0;
	}
	return MY_SUCCESSFUL;
}
// 清除状态
void clear(void)
{
		// device_state_init.expressage_id = 0;
		device_state_init.expressage_city = 0;
		device_state_init.expressage_area = 0;
		device_state_init.expressage_state = 0;
		device_state_init.expressage_app=0;
}
// 定时器
void Automation_Close(void)
{
	if(device_state_init.transport_time > 0)
	{
		device_state_init.transport_time--;
	}
	if( device_state_init.expressage_app == 1){
		BeepNo(100);
		// 自动入库
		if(Data_init.color == 0 ){
				device_state_init.expressage_id ++;
				device_state_init.expressage_city = 3601;
				device_state_init.expressage_area = 1;
				device_state_init.expressage_state = 2;
				SendMqtt(1); // 发送数据到APP
				area_1();
				clear();
		}else if(Data_init.color == 1){
				device_state_init.expressage_id ++;
				device_state_init.expressage_city = 3601;
				device_state_init.expressage_area = 2;
				device_state_init.expressage_state = 2;
				SendMqtt(1); // 发送数据到APP
				area_2();
				clear();
		}else if(Data_init.color == 2){
				area_3();
				device_state_init.expressage_id ++;
				device_state_init.expressage_city = 3601;
				device_state_init.expressage_area = 3;
				device_state_init.expressage_state = 2;
				SendMqtt(1); // 发送数据到APP
				clear();
		}
	}

}
// 检测按键是否按下
static U8 num_on = 0;
static U8 key_old = 0;
void Check_Key_ON_OFF()
{
	U8 key;
	key = KEY_Scan(1);
	// 与上一次的键值比较 如果不相等，表明有键值的变化，开始计时
	if (key != 0 && num_on == 0)
	{
		key_old = key;
		num_on = 1;
	}
	if (key != 0 && num_on >= 1 && num_on <= Key_Scan_Time) // 25*10ms
	{
		num_on++; // 时间记录器
	}
	if (key == 0 && num_on > 0 && num_on < Key_Scan_Time) // 短按
	{
		switch (key_old)
		{
		case KEY1_PRES:
			
//			Sg90_turn(1); //旋转	
			break;
		case KEY2_PRES:
//			if(device_state_init.expressage_state == 0)
//			{
//				Sg90_hight(0);
//				device_state_init.expressage_state = 1;
//			}else{
//				Sg90_hight(1);
//				device_state_init.expressage_state = 0;
//			}
//			Sg90_turn(2); //旋转	
			break;
		case KEY3_PRES:
			device_state_init.expressage_app =1;
//			if(device_state_init.expressage_state == 0)
//			{
//				Sg90_run(0);
//				device_state_init.expressage_state = 1;
//			}else{
//				Sg90_run(1);
//				device_state_init.expressage_state = 0;
//			}
//			Sg90_turn(3); //旋转
			break;

		default:
			break;
		}
		num_on = 0;
	}
	else if (key == 0 && num_on >= Key_Scan_Time) // 长按
	{
		switch (key_old)
		{
		case KEY1_PRES:
			// 区域一
			area_1();
			break;
		case KEY2_PRES:
			// 区域二
			area_2();

			break;
		case KEY3_PRES:
			// 区域三
			area_3();

			break;
		default:
			break;
		}
		num_on = 0;
	}
}
// 解析json数据
mySta massage_parse_json(char *message)
{

	cJSON *cjson_test = NULL; // 检测json格式
	// cJSON *cjson_data = NULL; // 数据
	const char *massage;
	// 定义数据类型
	u8 cjson_cmd; // 指令,方向

	/* 解析整段JSO数据 */
	cjson_test = cJSON_Parse(message);
	if (cjson_test == NULL)
	{
		// 解析失败
		printf("parse fail.\n");
		return MY_FAIL;
	}

	/* 依次根据名称提取JSON数据（键值对） */
	cjson_cmd = cJSON_GetObjectItem(cjson_test, "cmd")->valueint;
	/* 解析嵌套json数据 */
	// cjson_data = cJSON_GetObjectItem(cjson_test, "data");

	switch (cjson_cmd)
	{
	case 0x01: // 消息包
//		device_state_init.expressage_city = cJSON_GetObjectItem(cjson_test, "cityid")->valueint;
//		device_state_init.expressage_area = cJSON_GetObjectItem(cjson_test, "area")->valueint;
//		device_state_init.expressage_id = cJSON_GetObjectItem(cjson_test, "id")->valueint;
//		device_state_init.expressage_state = cJSON_GetObjectItem(cjson_test, "state")->valueint;
//		device_state_init.expressage_app = device_state_init.expressage_state;
//	  if (Data_init.App == 0) {
//        Data_init.App = 1;
//    }
		break;
	case 0x02: // 消息包
		// 查询
//		device_state_init.expressage_city = cJSON_GetObjectItem(cjson_test, "city")->valueint;
//		device_state_init.expressage_area = cJSON_GetObjectItem(cjson_test, "area")->valueint;
//		device_state_init.expressage_id = cJSON_GetObjectItem(cjson_test, "id")->valueint;
//    if (Data_init.App == 0) {
//        Data_init.App = 1;
//    }
		device_state_init.expressage_state = 0;
		break;
	case 0x03: // 数据包


		break;
	case 0x04: // 数据包

		break;
	default:
		break;
	}

	/* 清空JSON对象(整条链表)的所有数据 */
	cJSON_Delete(cjson_test);

	return MY_SUCCESSFUL;
}
// 解析数据
mySta massage_k210(char *message)
{

	char *dataPtr = NULL;

	char numBuf[10];
	int num = 0;

	dataPtr = strchr(message, ':'); // 搜索':'

	if (dataPtr != NULL) // 如果找到了
	{
		dataPtr++;
		while (*dataPtr >= '0' && *dataPtr <= '9') // 判断是否是下发的命令控制数据
		{
			numBuf[num++] = *dataPtr++;
		}
		numBuf[num] = 0;
		num = atoi((const char *)numBuf); // 转为数值形式
		if (strstr((char *)message, "color")) // 搜索"fruits"
		{
			Data_init.color = num;
			
		}

	}
	// printf("%s\n", message);
	return MY_SUCCESSFUL;
}


