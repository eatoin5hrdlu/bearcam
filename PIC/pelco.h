    /*
 * PELCO.H
 */

#ifdef DEBUG  // TEXT VERSIONS OF IDENTIFIERS FOR DEBUGGING

#define LEFT  'L'
#define RIGHT 'R'
#define UP    'U'
#define DOWN  'D'
#define IN    'I'
#define OUT   'O'

#else         // ACTUAL VALUES USED FOR PELCO-D PROTOCOL

#define LEFT  0x04     // PELCO COMMANDS
#define RIGHT 0x02
#define UP    0x08
#define DOWN  0x10
#define IN    0x20
#define OUT   0x40

#endif


#define CAM1  1
#define CAM2  2


#define CAM1_SW  0xFE  // ACTIVE-LOW INPUT SWITCHES
#define CAM2_SW  0xFD  

#define UP_SW    0xFB  
#define DOWN_SW  0xF7  
#define LEFT_SW  0xEF  
#define RIGHT_SW 0xDF  

#define IN_SW    0xBF  
#define OUT_SW   0x7F  


unsigned char i;
 // Pan/Tilt speed for each camera
unsigned char speed[4] = { 0x20, 0x20, 0x20, 0x20 };

unsigned char msg[5] = { CAM1, 0x00, 0x00, 0x20, 0x20 };
unsigned char stp[5] = { CAM1, 0x00, 0x00, 0x20, 0x20 };

#define MSGLENGTH   5

#define CAM        msg[0]
#define CMD        msg[2]
#define PAN_SPEED  msg[3]
#define TILT_SPEED msg[4]







