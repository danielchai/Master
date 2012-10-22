#ifndef __mod_register_H
#define __mod_register_H

extern uint16_t UT_BITS_ADDRESS;
extern uint16_t UT_BITS_NB ;
extern uint8_t UT_BITS_TAB[];

extern uint16_t UT_INPUT_BITS_ADDRESS;
extern uint16_t UT_INPUT_BITS_NB ;
extern uint8_t UT_INPUT_BITS_TAB[];
extern uint16_t UT_REGISTERS_TAB[];

extern uint16_t UT_REGISTERS_ADDRESS;
extern uint16_t UT_REGISTERS8_ADDRESS;
extern uint16_t UT_REGISTERS9_ADDRESS;
extern uint16_t UT_REGISTERS10_ADDRESS;
extern uint16_t UT_REGISTERS12_ADDRESS;
extern uint16_t UT_REGISTERS13_ADDRESS;
extern uint16_t UT_REGISTERS20_ADDRESS;
extern uint16_t UT_REGISTERS21_ADDRESS;

extern uint16_t UT_REGISTERS_START_PRO_ADDRESS;
extern uint16_t UT_REGISTERS2_START_PRO_ADDRESS1;

extern uint16_t UT_REGISTERS_READ_SENSOR_ADDRESS;
extern uint16_t UT_REGISTERS2_READ_SENSOR_ADDRESS;
extern uint16_t UT_REGISTERS3_READ_SENSOR_ADDRESS;

extern uint16_t UT_REGISTERS_UPLOAD_TIME_ADDRESS;
extern uint16_t UT_REGISTERS2_UPLOAD_TIME_ADDRESS;
extern uint16_t UT_REGISTERS3_UPLOAD_TIME_ADDRESS;
extern uint16_t UT_REGISTERS4_UPLOAD_TIME_ADDRESS;
extern uint16_t UT_REGISTERS5_UPLOAD_TIME_ADDRESS;
extern uint16_t UT_REGISTERS6_UPLOAD_TIME_ADDRESS;

extern uint16_t UT_REGISTERS_NEXT_UPLOAD_TIME_ADDRESS;

extern uint16_t UT_REGISTERS_CH_WIFI_ADDRESS;

extern uint16_t UT_REGISTERS_CANCLE_PROFILE_ADDRESS;
extern uint16_t UT_REGISTERS_SET_RTC_TIME;
//extern uint16_t UT_REGISTERS_CANCLE_RD_SENSOR_ADDRESS = 0x1441;

extern uint16_t UT_REGISTERS_RD_DATA_BUFFSIZE_ADDRESS;
extern uint16_t UT_REGISTERS2_RD_DATA_BUFFSIZE_ADDRESS;

extern uint16_t UT_REGISTERS_RD_DATA_BUFF_ADDRESS;

//extern uint16_t UT_REGISTERS_CURR_POSITION_ADDRESS;
//extern uint16_t UT_REGISTERS_CURR_WIFI_ADDRESS;
/* Raise a manual exception when this adress is used for the first byte */
extern uint16_t UT_REGISTERS_ADDRESS_SPECIAL;
extern uint16_t UT_REGISTERS_NB;
//uint16_t UT_REGISTERS_TAB[] = { 0x022B, 0x0001, 0x0064 };
/* If the following value is used, a bad response is sent.
   It's better to test with a lower value than
   UT_REGISTERS_NB_POINTS to try to raise a segfault. */
extern uint16_t UT_REGISTERS_NB_SPECIAL;

extern uint16_t UT_INPUT_REGISTERS_ADDRESS;
extern uint16_t UT_INPUT_REGISTERS_NB ;
extern uint16_t UT_INPUT_REGISTERS_TAB[];

#endif
