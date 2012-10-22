#ifndef __Defs_H
#define __Defs_H

//#define  LOW               0
//#define  HIGH              1
#define  P_OFFSET           1
#define  AVR_ADDR          0x07 
#define  SLEEP             0x08
#define  TEMP_ADDR         0x49 
#define  Time_Value        "1"
#define  Flowrate_Value    "2"
#define  Calibrate_Value   "3"
#define  Accelerate_Value  "4"
#define  Decelerate_Value  "5"
#define  Purge_Value       "6"
#define  Records_Size      40
#define  checkpin          130
#define  steppin           147
#define  direction         146   
#define  database          "/home/sampler/lr.sl3"
#define  loginfile         "/home/sampler/log.txt"
#define  winch_database    "/home/sampler/steps.sl3"
#define  sleeptime         860000  //For the usleep() function which is used for count profile
#define  max_position 		20
#define  DepthID            1
#define  SpacingID          2
#define  DwellID            3
#define  IntervalID         4
#define Error_rec_factor   750
#define Max_Records         32
#define  SERVER_ID          1
#define  SDL_Par            60

#endif
