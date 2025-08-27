#include "git.h"

Data_TypeDef Data_init;						  // �豸���ݽṹ��
Threshold_Value_TypeDef threshold_value_init; // �豸��ֵ���ýṹ��
Device_Satte_Typedef device_state_init;		  // �豸״̬

extern U8 MG996R_TEST; // ���ٲ���
// ��ȡ���ݲ���
mySta Read_Data(Data_TypeDef *Device_Data)
{


	return MY_SUCCESSFUL;
}
// ��ʼ��
mySta Reset_Threshole_Value(Threshold_Value_TypeDef *Value, Device_Satte_Typedef *device_state)
{

	device_state->open = 1;
  // д
  //W_Test();
	// ��
	R_Test();
	// ״̬����
	device_state->check_device = 0;
	return MY_SUCCESSFUL;
}
// ����OLED��ʾ��������
mySta Update_oled_massage()
{
#if OLED // �Ƿ��
	char str[50];

	sprintf(str, "����   ");
	OLED_ShowCH(0, 0, (unsigned char *)str);
	sprintf(str, "%04d-%02d-%02d ",device_state_init.expressage_city,
	device_state_init.expressage_area,device_state_init.expressage_id);
	OLED_ShowCH(0, 2, (unsigned char *)str);

	if(Data_init.color==0)
	{
		sprintf(str, "״̬: ��ɫ   ");
	}else if(Data_init.color==1)
	{
		sprintf(str, "״̬: ��ɫ   ");
	}else if(Data_init.color==2)
	{
		sprintf(str, "״̬: ��ɫ   ");
	}
	OLED_ShowCH(0, 4, (unsigned char *)str);


#endif

	return MY_SUCCESSFUL;
}

// �����豸״̬
mySta Update_device_massage()
{


	// ��⵽���嵽��
	if( device_state_init.transport_state == 1){

		device_state_init.expressage_state = 2; //���ɹ�
		if (Data_init.App == 0) {
				Data_init.App = 1;
		}
		device_state_init.transport_time = 3; //�ӳ�5S
	}

	if (Data_init.App)
	{
		switch (Data_init.App)
		{
		case 1:
			SendMqtt(1); // �������ݵ�APP
			break;
		case 2:
			SendData(); //  ���͵���ƽ̨
			break;
		}
		Data_init.App = 0;
	}
	return MY_SUCCESSFUL;
}
// ���״̬
void clear(void)
{
		// device_state_init.expressage_id = 0;
		device_state_init.expressage_city = 0;
		device_state_init.expressage_area = 0;
		device_state_init.expressage_state = 0;
		device_state_init.expressage_app=0;
}
// ��ʱ��
void Automation_Close(void)
{
	if(device_state_init.transport_time > 0)
	{
		device_state_init.transport_time--;
	}
	if( device_state_init.expressage_app == 1){
		BeepNo(100);
		// �Զ����
		if(Data_init.color == 0 ){
				device_state_init.expressage_id ++;
				device_state_init.expressage_city = 3601;
				device_state_init.expressage_area = 1;
				device_state_init.expressage_state = 2;
				SendMqtt(1); // �������ݵ�APP
				area_1();
				clear();
		}else if(Data_init.color == 1){
				device_state_init.expressage_id ++;
				device_state_init.expressage_city = 3601;
				device_state_init.expressage_area = 2;
				device_state_init.expressage_state = 2;
				SendMqtt(1); // �������ݵ�APP
				area_2();
				clear();
		}else if(Data_init.color == 2){
				area_3();
				device_state_init.expressage_id ++;
				device_state_init.expressage_city = 3601;
				device_state_init.expressage_area = 3;
				device_state_init.expressage_state = 2;
				SendMqtt(1); // �������ݵ�APP
				clear();
		}
	}

}
// ��ⰴ���Ƿ���
static U8 num_on = 0;
static U8 key_old = 0;
void Check_Key_ON_OFF()
{
	U8 key;
	key = KEY_Scan(1);
	// ����һ�εļ�ֵ�Ƚ� �������ȣ������м�ֵ�ı仯����ʼ��ʱ
	if (key != 0 && num_on == 0)
	{
		key_old = key;
		num_on = 1;
	}
	if (key != 0 && num_on >= 1 && num_on <= Key_Scan_Time) // 25*10ms
	{
		num_on++; // ʱ���¼��
	}
	if (key == 0 && num_on > 0 && num_on < Key_Scan_Time) // �̰�
	{
		switch (key_old)
		{
		case KEY1_PRES:
			
//			Sg90_turn(1); //��ת	
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
//			Sg90_turn(2); //��ת	
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
//			Sg90_turn(3); //��ת
			break;

		default:
			break;
		}
		num_on = 0;
	}
	else if (key == 0 && num_on >= Key_Scan_Time) // ����
	{
		switch (key_old)
		{
		case KEY1_PRES:
			// ����һ
			area_1();
			break;
		case KEY2_PRES:
			// �����
			area_2();

			break;
		case KEY3_PRES:
			// ������
			area_3();

			break;
		default:
			break;
		}
		num_on = 0;
	}
}
// ����json����
mySta massage_parse_json(char *message)
{

	cJSON *cjson_test = NULL; // ���json��ʽ
	// cJSON *cjson_data = NULL; // ����
	const char *massage;
	// ������������
	u8 cjson_cmd; // ָ��,����

	/* ��������JSO���� */
	cjson_test = cJSON_Parse(message);
	if (cjson_test == NULL)
	{
		// ����ʧ��
		printf("parse fail.\n");
		return MY_FAIL;
	}

	/* ���θ���������ȡJSON���ݣ���ֵ�ԣ� */
	cjson_cmd = cJSON_GetObjectItem(cjson_test, "cmd")->valueint;
	/* ����Ƕ��json���� */
	// cjson_data = cJSON_GetObjectItem(cjson_test, "data");

	switch (cjson_cmd)
	{
	case 0x01: // ��Ϣ��
//		device_state_init.expressage_city = cJSON_GetObjectItem(cjson_test, "cityid")->valueint;
//		device_state_init.expressage_area = cJSON_GetObjectItem(cjson_test, "area")->valueint;
//		device_state_init.expressage_id = cJSON_GetObjectItem(cjson_test, "id")->valueint;
//		device_state_init.expressage_state = cJSON_GetObjectItem(cjson_test, "state")->valueint;
//		device_state_init.expressage_app = device_state_init.expressage_state;
//	  if (Data_init.App == 0) {
//        Data_init.App = 1;
//    }
		break;
	case 0x02: // ��Ϣ��
		// ��ѯ
//		device_state_init.expressage_city = cJSON_GetObjectItem(cjson_test, "city")->valueint;
//		device_state_init.expressage_area = cJSON_GetObjectItem(cjson_test, "area")->valueint;
//		device_state_init.expressage_id = cJSON_GetObjectItem(cjson_test, "id")->valueint;
//    if (Data_init.App == 0) {
//        Data_init.App = 1;
//    }
		device_state_init.expressage_state = 0;
		break;
	case 0x03: // ���ݰ�


		break;
	case 0x04: // ���ݰ�

		break;
	default:
		break;
	}

	/* ���JSON����(��������)���������� */
	cJSON_Delete(cjson_test);

	return MY_SUCCESSFUL;
}
// ��������
mySta massage_k210(char *message)
{

	char *dataPtr = NULL;

	char numBuf[10];
	int num = 0;

	dataPtr = strchr(message, ':'); // ����':'

	if (dataPtr != NULL) // ����ҵ���
	{
		dataPtr++;
		while (*dataPtr >= '0' && *dataPtr <= '9') // �ж��Ƿ����·��������������
		{
			numBuf[num++] = *dataPtr++;
		}
		numBuf[num] = 0;
		num = atoi((const char *)numBuf); // תΪ��ֵ��ʽ
		if (strstr((char *)message, "color")) // ����"fruits"
		{
			Data_init.color = num;
			
		}

	}
	// printf("%s\n", message);
	return MY_SUCCESSFUL;
}


