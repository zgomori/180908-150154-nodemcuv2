#include <TFT_eSPI.h>
#include <Fonts/Custom/orbitron_light_11_2.h>
#include <Fonts/Custom/orbitron_light_8.h>
  
#define CF_OL24 &Orbitron_Light_24
#define CF_OL32 &Orbitron_Light_32

#define CF_ORB &orbitron_light11pt7b
#define CF_ORB8 &orbitron_light8pt7b


#define TFT_CS   PIN_D8  // Chip select control pin D8
#define TFT_DC   PIN_D4  // Data Command control pin
#define TFT_RST  -1  // Reset pin (could connect to NodeMCU RST, see next line)
#define TOUCH_CS PIN_D2     // Chip select pin (T_CS) of touch screen


#define COLOR1 0x18C3
#define COLOR2 0x10A2

#define BUFF_SIZE 64


const  uint16_t  clock [] = {
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x2104, 0x52aa, 0x9492, 0xc638, 0xc638, 0x9492, 0x52aa, 0x2104, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1082, 0x7bcf, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x7bcf, 0x1082, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x18e3, 0xbdd7, 0xffff, 0xad75, 0x4208, 0x10a2, 0x0020, 0x0020, 0x10a2, 0x4208, 0xad75, 0xffff, 0xb5b6, 0x18e3, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0x1082, 0xbdd7, 0xffff, 0x632c, 0x0841, 0x0000, 0x0000, 0x7bef, 0x7bcf, 0x0000, 0x0000, 0x0841, 0x632c, 0xffff, 0xb5b6, 0x1082, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0x7bcf, 0xffff, 0x632c, 0x0861, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0x0861, 0x632c, 0xffff, 0x73ae, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x2104, 0xffff, 0xad75, 0x0841, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0841, 0xad75, 0xffff, 0x2104, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x52aa, 0xffff, 0x4208, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x4208, 0xffff, 0x52aa, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x9492, 0xffff, 0x10a2, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x10a2, 0xffff, 0x9492, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0xbdf7, 0xffff, 0x0020, 0x0000, 0x0020, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x7bcf, 0x0020, 0x0020, 0xffff, 0xbdf7, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0xbdf7, 0xffff, 0x0020, 0x0000, 0x0020, 0x0000, 0x0000, 0x0000, 0x7bcf, 0xffff, 0xffff, 0xffff, 0xffff, 0x7bcf, 0x0020, 0x0020, 0xffff, 0xbdf7, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x9492, 0xffff, 0x10a2, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x10a2, 0xffff, 0x8c71, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x52aa, 0xffff, 0x4208, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x4208, 0xffff, 0x52aa, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x2104, 0xffff, 0xad75, 0x0841, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0841, 0xb596, 0xffff, 0x2104, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0x7bcf, 0xffff, 0x632c, 0x0861, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0861, 0x632c, 0xffff, 0x73ae, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0x1082, 0xb5b6, 0xffff, 0x632c, 0x0841, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0841, 0x632c, 0xffff, 0xb5b6, 0x1082, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x18e3, 0xb5b6, 0xffff, 0xb596, 0x4208, 0x10a2, 0x0020, 0x0020, 0x10a2, 0x4208, 0xb596, 0xffff, 0xb5b6, 0x18e3, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1082, 0x73ae, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x73ae, 0x1082, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x2104, 0x52aa, 0x8c71, 0xc618, 0xc618, 0x8c71, 0x52aa, 0x2104, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};

const  uint8_t  wifi8 [] = {
0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 
0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 
0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 
0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 
0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x0d, 0x3d, 0x6b, 0xb7, 0xff, 0xff, 0xff, 0xff, 0xb7, 0x6b, 0x3d, 0x0d, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 
0x05, 0x05, 0x05, 0x05, 0x0d, 0x5d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x5d, 0x0d, 0x05, 0x05, 0x05, 0x05, 
0x05, 0x05, 0x05, 0x2d, 0xb7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb7, 0x2d, 0x05, 0x05, 0x05, 
0x05, 0x05, 0x4f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x4f, 0x05, 0x05, 
0x05, 0x35, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x87, 0x47, 0x25, 0x25, 0x25, 0x25, 0x47, 0x87, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x35, 0x05, 
0x05, 0x47, 0xff, 0xff, 0xff, 0xff, 0x87, 0x25, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x25, 0x87, 0xff, 0xff, 0xff, 0xff, 0x47, 0x05, 
0x05, 0x35, 0xff, 0xff, 0xff, 0x5d, 0x05, 0x05, 0x35, 0x77, 0xff, 0xff, 0xff, 0xff, 0x77, 0x35, 0x05, 0x05, 0x5d, 0xff, 0xff, 0xff, 0x35, 0x05, 
0x05, 0x05, 0x35, 0x5d, 0x3d, 0x05, 0x15, 0x87, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x87, 0x15, 0x05, 0x3d, 0x5d, 0x35, 0x05, 0x05, 
0x05, 0x05, 0x05, 0x05, 0x05, 0x0d, 0xb7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb7, 0x0d, 0x05, 0x05, 0x05, 0x05, 0x05, 
0x05, 0x05, 0x05, 0x05, 0x05, 0x25, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x25, 0x05, 0x05, 0x05, 0x05, 0x05, 
0x05, 0x05, 0x05, 0x05, 0x05, 0x1b, 0xff, 0xff, 0xff, 0xb7, 0x47, 0x25, 0x25, 0x47, 0xb7, 0xff, 0xff, 0xff, 0x1b, 0x05, 0x05, 0x05, 0x05, 0x05, 
0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x47, 0x9f, 0x6b, 0x1b, 0x05, 0x0d, 0x0d, 0x05, 0x1b, 0x6b, 0x9f, 0x47, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 
0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x47, 0xff, 0xff, 0x47, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 
0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0xff, 0xff, 0xff, 0xff, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 
0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0xff, 0xff, 0xff, 0xff, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 
0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x47, 0xff, 0xff, 0x47, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 
0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 
0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 
0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 
0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05
};

/*
const  uint8_t  wifi4 [] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x03, 0x6b, 0xff, 0xff, 0xb6, 0x30, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x05, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x50, 0x00, 0x00, 
0x00, 0x02, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0x20, 0x00, 
0x00, 0x4f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf4, 0x00, 
0x03, 0xff, 0xff, 0xff, 0x84, 0x22, 0x22, 0x48, 0xff, 0xff, 0xff, 0x30, 
0x04, 0xff, 0xff, 0x82, 0x00, 0x00, 0x00, 0x00, 0x28, 0xff, 0xff, 0x40, 
0x03, 0xff, 0xf5, 0x00, 0x37, 0xff, 0xff, 0x73, 0x00, 0x5f, 0xff, 0x30, 
0x00, 0x35, 0x30, 0x18, 0xff, 0xff, 0xff, 0xff, 0x81, 0x03, 0x53, 0x00, 
0x00, 0x00, 0x00, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xfb, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x02, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x20, 0x00, 0x00, 
0x00, 0x00, 0x01, 0xff, 0xfb, 0x42, 0x24, 0xbf, 0xff, 0x10, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x49, 0x61, 0x00, 0x00, 0x16, 0x94, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x4f, 0xf4, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x4f, 0xf4, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
*/

const  uint8_t  wifi4 [] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x24, 0x57, 0x75, 0x42, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x02, 0x6c, 0xff, 0xff, 0xff, 0xff, 0xc6, 0x20, 0x00, 0x00, 
0x00, 0x00, 0x6f, 0xff, 0xb6, 0x54, 0x45, 0x6b, 0xff, 0xf6, 0x00, 0x00, 
0x00, 0x3c, 0xff, 0x62, 0x00, 0x00, 0x00, 0x00, 0x26, 0xff, 0xc3, 0x00, 
0x03, 0xff, 0x81, 0x00, 0x00, 0x12, 0x21, 0x00, 0x00, 0x18, 0xff, 0x30, 
0x4f, 0xf5, 0x00, 0x02, 0x7e, 0xff, 0xff, 0xe7, 0x20, 0x00, 0x5f, 0xf4, 
0xaf, 0x50, 0x01, 0x8f, 0xff, 0xfb, 0xbf, 0xff, 0xf8, 0x10, 0x05, 0xfa, 
0x12, 0x00, 0x2d, 0xfe, 0x52, 0x10, 0x01, 0x25, 0xef, 0xd2, 0x00, 0x21, 
0x00, 0x02, 0xef, 0xa1, 0x00, 0x00, 0x00, 0x00, 0x1a, 0xfe, 0x20, 0x00, 
0x00, 0x04, 0xf7, 0x00, 0x03, 0x6a, 0xa6, 0x30, 0x00, 0x7f, 0x40, 0x00, 
0x00, 0x00, 0x10, 0x00, 0x8f, 0xff, 0xff, 0xf8, 0x00, 0x01, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x0b, 0xfb, 0x42, 0x24, 0xbf, 0xb0, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x0d, 0x81, 0x00, 0x00, 0x18, 0xd0, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xf7, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x8f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x51, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const  uint8_t  clock4 [] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x25, 0x9c, 0xc9, 0x52, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x17, 0xff, 0xff, 0xff, 0xff, 0x71, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x01, 0xbf, 0xa4, 0x10, 0x01, 0x4a, 0xfb, 0x10, 0x00, 0x00, 
0x00, 0x00, 0x1b, 0xf6, 0x00, 0x07, 0x70, 0x00, 0x6f, 0xb1, 0x00, 0x00, 
0x00, 0x00, 0x7f, 0x60, 0x00, 0x0f, 0xf0, 0x00, 0x06, 0xf7, 0x00, 0x00, 
0x00, 0x02, 0xfa, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0xaf, 0x20, 0x00, 
0x00, 0x05, 0xf4, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x4f, 0x50, 0x00, 
0x00, 0x09, 0xf1, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x1f, 0x90, 0x00, 
0x00, 0x0b, 0xf0, 0x00, 0x00, 0x0f, 0xff, 0xff, 0x70, 0x0f, 0xb0, 0x00, 
0x00, 0x0b, 0xf0, 0x00, 0x00, 0x07, 0xff, 0xff, 0x70, 0x0f, 0xb0, 0x00, 
0x00, 0x09, 0xf1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 
0x00, 0x05, 0xf4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4f, 0x50, 0x00, 
0x00, 0x02, 0xfa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbf, 0x20, 0x00, 
0x00, 0x00, 0x7f, 0x60, 0x00, 0x00, 0x00, 0x00, 0x06, 0xf7, 0x00, 0x00, 
0x00, 0x00, 0x1b, 0xf6, 0x00, 0x00, 0x00, 0x00, 0x6f, 0xb1, 0x00, 0x00, 
0x00, 0x00, 0x01, 0xbf, 0xb4, 0x10, 0x01, 0x4b, 0xfb, 0x10, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x17, 0xff, 0xff, 0xff, 0xff, 0x71, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x25, 0x8c, 0xc8, 0x52, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const  uint8_t  thermo4 [] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0xc8, 0x20, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x02, 0xff, 0xff, 0xf2, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x08, 0xf3, 0x04, 0xf8, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x0d, 0xf0, 0x00, 0xfd, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x90, 0xff, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0xf0, 0xff, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0xf0, 0xff, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0xf0, 0xff, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0xf0, 0xff, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x7f, 0x70, 0xf0, 0x7f, 0x70, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x01, 0xfa, 0x06, 0xf6, 0x0a, 0xf1, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x03, 0xf5, 0x3f, 0xff, 0x35, 0xf3, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x03, 0xf5, 0x3f, 0xff, 0x35, 0xf3, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x01, 0xfa, 0x06, 0xc6, 0x0a, 0xf1, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x8f, 0x81, 0x01, 0x8f, 0x80, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x19, 0xff, 0xff, 0xf9, 0x10, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x49, 0xd9, 0x40, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const  uint8_t  cloudUp4 [] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x02, 0x6b, 0xc8, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x6f, 0xff, 0xff, 0xfc, 0x20, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x07, 0xff, 0x51, 0x01, 0x5e, 0xd2, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x3f, 0xf3, 0x00, 0x00, 0x03, 0xfa, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x9f, 0xe9, 0x40, 0x00, 0x00, 
0x00, 0x00, 0xbf, 0x40, 0x00, 0x00, 0x00, 0x4f, 0xff, 0xf9, 0x10, 0x00, 
0x00, 0x04, 0xff, 0x40, 0x00, 0x06, 0x00, 0x11, 0x01, 0x8f, 0x80, 0x00, 
0x00, 0x6f, 0xff, 0x70, 0x00, 0xff, 0xf0, 0x00, 0x00, 0x0a, 0xf1, 0x00, 
0x02, 0xfc, 0x20, 0x00, 0x0f, 0xff, 0xff, 0x00, 0x00, 0x05, 0xf3, 0x00, 
0x05, 0xf5, 0x00, 0x00, 0xff, 0x0f, 0x0f, 0xf0, 0x00, 0x05, 0xf3, 0x00, 
0x06, 0xf5, 0x00, 0x00, 0x60, 0x0f, 0x00, 0x60, 0x00, 0x0a, 0xf1, 0x00, 
0x03, 0xfd, 0x20, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x01, 0x8f, 0x80, 0x00, 
0x00, 0x6f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0x10, 0x00, 
0x00, 0x03, 0x9e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe9, 0x40, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const  uint8_t  cloudDown4 [] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x02, 0x6b, 0xc8, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x6f, 0xff, 0xff, 0xfc, 0x20, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x07, 0xff, 0x51, 0x01, 0x5e, 0xd2, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x3f, 0xf3, 0x00, 0x00, 0x03, 0xfa, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x7f, 0x80, 0x00, 0x06, 0x00, 0x9f, 0xe9, 0x40, 0x00, 0x00, 
0x00, 0x00, 0xbf, 0x40, 0x00, 0x0f, 0x00, 0x4f, 0xff, 0xf9, 0x10, 0x00, 
0x00, 0x04, 0xff, 0x40, 0x00, 0x0f, 0x00, 0x11, 0x01, 0x8f, 0x80, 0x00, 
0x00, 0x6f, 0xff, 0x70, 0x60, 0x0f, 0x00, 0x60, 0x00, 0x0a, 0xf1, 0x00, 
0x02, 0xfc, 0x20, 0x00, 0xff, 0x0f, 0x0f, 0xf0, 0x00, 0x05, 0xf3, 0x00, 
0x05, 0xf5, 0x00, 0x00, 0x0f, 0xff, 0xff, 0x00, 0x00, 0x05, 0xf3, 0x00, 
0x06, 0xf5, 0x00, 0x00, 0x00, 0xff, 0xf0, 0x00, 0x00, 0x0a, 0xf1, 0x00, 
0x03, 0xfd, 0x20, 0x00, 0x00, 0x06, 0x00, 0x00, 0x01, 0x8f, 0x80, 0x00, 
0x00, 0x6f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0x10, 0x00, 
0x00, 0x03, 0x9e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe9, 0x40, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


const  uint8_t  radio4 [] = {
0x00, 0x00, 0x1a, 0x20, 0x00, 0x00, 0x00, 0x00, 0x02, 0xa1, 0x00, 0x00, 
0x00, 0x00, 0x9f, 0x10, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf9, 0x00, 0x00, 
0x00, 0x04, 0xf5, 0x02, 0x20, 0x00, 0x00, 0x02, 0x20, 0x5f, 0x40, 0x00, 
0x00, 0x1e, 0x90, 0x1e, 0x70, 0x00, 0x00, 0x07, 0xe1, 0x09, 0xe1, 0x00, 
0x00, 0x3f, 0x20, 0x8f, 0x10, 0x00, 0x00, 0x02, 0xf8, 0x03, 0xf3, 0x00, 
0x00, 0x5f, 0x11, 0xf5, 0x00, 0x27, 0x71, 0x00, 0x6f, 0x11, 0xf5, 0x00, 
0x00, 0x8c, 0x03, 0xf3, 0x00, 0xbf, 0xfa, 0x00, 0x3f, 0x30, 0xc8, 0x00, 
0x00, 0xa9, 0x04, 0xf2, 0x01, 0xff, 0xff, 0x10, 0x2f, 0x40, 0x9a, 0x00, 
0x00, 0x7e, 0x02, 0xf3, 0x00, 0x7f, 0xf7, 0x00, 0x3f, 0x20, 0xf7, 0x00, 
0x00, 0x4f, 0x11, 0xf8, 0x00, 0x03, 0x30, 0x00, 0x8f, 0x01, 0xf4, 0x00, 
0x00, 0x2f, 0x40, 0x5f, 0x30, 0x03, 0x30, 0x03, 0xf5, 0x04, 0xf2, 0x00, 
0x00, 0x0a, 0xd1, 0x0a, 0x70, 0x0d, 0xd0, 0x07, 0xa0, 0x1c, 0xa0, 0x00, 
0x00, 0x02, 0xf8, 0x00, 0x00, 0x3f, 0xf3, 0x00, 0x00, 0x8f, 0x20, 0x00, 
0x00, 0x00, 0x5f, 0x20, 0x00, 0x6f, 0xf6, 0x00, 0x02, 0xf6, 0x00, 0x00, 
0x00, 0x00, 0x04, 0x00, 0x00, 0xd9, 0x9d, 0x00, 0x00, 0x40, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x02, 0xf4, 0x4f, 0x20, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x06, 0xf7, 0x7f, 0x60, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x0c, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x2f, 0x50, 0x05, 0xf2, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x5f, 0x10, 0x01, 0xf5, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0xbf, 0x99, 0x99, 0xfb, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x02, 0xfb, 0xaa, 0xaa, 0xbf, 0x20, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x05, 0xf1, 0x00, 0x00, 0x1f, 0x50, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x04, 0x60, 0x00, 0x00, 0x07, 0x40, 0x00, 0x00, 0x00
};

TFT_eSPI tft = TFT_eSPI();  

void setup() {
  Serial.begin(115200);

  uint8_t bx;
  uint8_t by;


  uint8_t bw = 120;
  uint8_t bh = 98;
  
  uint8_t b1x = 0;
  uint8_t b1y = 124;

  uint8_t b2x = b1x+bw;
  uint8_t b2y = b1y;

  uint8_t b3x = b1x;
  uint8_t b3y = b1y+bh;

  uint8_t b4x = b1x+bw;
  uint8_t b4y = b1y+bh;  
  


  tft.begin();
  tft.setRotation(0);

  tft.fillScreen(TFT_BLACK);

//  tft.setTextColor(TFT_DARKGREY,TFT_BLACK);



  tft.fillRect(0,  0, 240, 24, COLOR1);

//tft.pushImage(10, 50, 24, 24, wifi );

//drawIcon(clock, 10, 40, 24, 24);
//drawIcon4(clock4, 10, 65, 24, 24, TFT_WHITE, TFT_BLACK);

int iconX = 4;

drawIcon4(radio4, iconX, 0, 24, 24, TFT_DARKGREY, COLOR1);  
iconX = iconX + 25;
drawIcon4(wifi4, iconX, 0, 24, 24, TFT_DARKGREY, COLOR1);
iconX = iconX + 25;
yield();
drawIcon4(thermo4, iconX, 0, 24, 24, TFT_DARKGREY, COLOR1);
iconX = iconX + 25;
yield();
drawIcon4(clock4, iconX, 0, 24, 24, TFT_DARKGREY, COLOR1);
iconX = iconX + 25;
yield();
drawIcon4(cloudUp4, iconX, 0, 24, 24, TFT_DARKGREY, COLOR1);
iconX = iconX + 25;
yield();
drawIcon4(cloudDown4, iconX, 0, 24, 24, TFT_DARKGREY, COLOR1);
iconX = iconX + 25;
yield();


  tft.fillRect(b1x,  b1y, bw, bh, COLOR1);
  tft.fillRect(b2x,  b2y, bw, bh, COLOR2);
  tft.fillRect(b3x,  b3y, bw, bh, COLOR2);
  tft.fillRect(b4x,  b4y, bw, bh, COLOR1);

  tft.setTextColor(TFT_DARKGREY,COLOR1);
  tft.setTextDatum(MR_DATUM); 
  tft.drawString("4.41V", 237, 11, 2);


  // clock
  tft.setTextColor(TFT_GREEN,TFT_BLACK);
  tft.setTextDatum(MC_DATUM);  
  tft.drawString("15:30", 120, 60, 7);

  tft.setFreeFont(CF_ORB);
//  tft.drawString("2018.10.29", 120, 100, 1);
  


  tft.setTextColor(TFT_DARKGREY,TFT_BLACK);
  tft.setTextDatum(ML_DATUM);
  tft.drawString("09.10", 4, 100, 1);
  tft.setTextDatum(MR_DATUM);
  tft.drawString("sun", 237, 100, 1);

/*B1********************************************************/  
  bx = b1x;
  by = b1y;
  
  tft.setTextColor(TFT_DARKGREY,COLOR1);
  tft.setTextDatum(ML_DATUM);
  tft.drawString("livingroom", bx+4, by+10, 2);
  
  tft.setTextDatum(MC_DATUM);  
  tft.setTextColor(TFT_GREEN,COLOR1);
  tft.setFreeFont(CF_OL32);
  tft.drawString("20.1", bx+60, by+35, 1);
 // tft.setFreeFont(CF_OL24);
  tft.setFreeFont(CF_ORB);

  tft.drawString("37.0", bx+60, by+70, 1);

/*B2********************************************************/  
  bx = b2x;
  by = b2y;

  tft.setTextColor(TFT_DARKGREY,COLOR2);
  tft.setTextDatum(ML_DATUM);
  tft.drawString("child's room", bx+4, by+10, 2);
  
  tft.setTextDatum(MC_DATUM);  
  tft.setTextColor(TFT_GREEN,COLOR2);  
  tft.setFreeFont(CF_OL32);
  tft.drawString("21.1", bx+60, by+35, 1);
  //tft.setFreeFont(CF_OL24);
  tft.setFreeFont(CF_ORB);

  tft.drawString("56.8", bx+60, by+70, 1);

/*B3********************************************************/  
  bx = b3x;
  by = b3y;

  tft.setTextColor(TFT_DARKGREY,COLOR2);
  tft.setTextDatum(ML_DATUM);
  tft.drawString("outdoor", bx+4, by+10, 2);
  
  tft.setTextDatum(MC_DATUM);  
  tft.setTextColor(TFT_GREEN,COLOR2);
  tft.setFreeFont(CF_OL32);
  tft.drawString("-5.5", bx+60, by+35, 1);
  tft.setFreeFont(CF_OL24);
  tft.drawString("13.8", bx+60, by+70, 1);

/*B4********************************************************/ 
  bx = b4x;
  by = b4y;

  tft.setTextColor(TFT_DARKGREY,COLOR1);
  tft.setTextDatum(ML_DATUM);
  tft.drawString("atm.pressure", bx+4, by+10, 2);
  
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_GREEN,COLOR1);
  tft.setFreeFont(CF_OL24);
  tft.drawString("1015", bx+60, by+34, 1);

  int xd = 144;
  int dBottom =  310;
  int dh = 30;
  tft.fillRect(xd,dBottom-dh,4,dh,TFT_DARKGREY);
  xd += 8;
  dh = 30;
  tft.fillRect(xd,dBottom-dh,4,dh,TFT_DARKGREY);
  xd += 8;
  dh = 28;
  tft.fillRect(xd,dBottom-dh,4,dh,TFT_DARKGREY);
  xd += 8;
  dh = 26;  
  tft.fillRect(xd,dBottom-dh,4,dh,TFT_DARKGREY);
  xd += 8;
  dh = 22;  
  tft.fillRect(xd,dBottom-dh,4,dh,TFT_DARKGREY);
  xd += 8;
  dh = 12;  
  tft.fillRect(xd,dBottom-dh,4,dh,TFT_DARKGREY);
  xd += 8;
  dh = 4;  
  tft.fillRect(xd,dBottom-dh,4,dh,TFT_DARKGREY);
  xd += 8;
  dh = 10;  
  tft.fillRect(xd,dBottom-dh,4,dh,TFT_DARKGREY);
  xd += 8;
  dh = 18;  
  tft.fillRect(xd,dBottom-dh,4,dh,TFT_DARKGREY);
  xd += 8;
  dh = 24;  
  tft.fillRect(xd,dBottom-dh,4,dh,TFT_DARKGREY);

}

void loop() {
  // put your main code here, to run repeatedly:

}


void drawIcon(const unsigned short* icon, int16_t x, int16_t y, uint16_t width, uint16_t height) {

  uint16_t  pix_buffer[BUFF_SIZE];   // Pixel buffer (16 bits per pixel)

  // Set up a window the right size to stream pixels into
  tft.setWindow(x, y, x + width - 1, y + height - 1);

  // Work out the number whole buffers to send
  uint16_t nb = ((uint16_t)height * width) / BUFF_SIZE;

  // Fill and send "nb" buffers to TFT
  for (int i = 0; i < nb; i++) {
    for (int j = 0; j < BUFF_SIZE; j++) {
      pix_buffer[j] = pgm_read_word(&icon[i * BUFF_SIZE + j]);
    }
    tft.pushColors(pix_buffer, BUFF_SIZE);
  }

  // Work out number of pixels not yet sent
  uint16_t np = ((uint16_t)height * width) % BUFF_SIZE;

  // Send any partial buffer left over
  if (np) {
    for (int i = 0; i < np; i++) pix_buffer[i] = pgm_read_word(&icon[nb * BUFF_SIZE + i]);
    tft.pushColors(pix_buffer, np);
  }
}


void drawIcon8(const uint8_t icon[], int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t fgColor, uint16_t bgColor ) {
  unsigned long mill = millis();
  tft.setWindow(x, y, x + width - 1, y + height - 1);
  uint16_t arraySize = ((uint16_t)width * height);
  for (int i = 0; i < arraySize; i++) {
    tft.pushColor(alphaBlend(icon[i], fgColor, bgColor));
  }
  Serial.println(millis()-mill);
}


void drawIcon4(const uint8_t icon[], int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t fgColor, uint16_t bgColor ) {
  unsigned long mill = millis();  
  tft.setWindow(x, y, x + width - 1, y + height - 1);

  uint16_t arraySize = width * height / 2;
  uint8_t alpha;
  
  for (int i = 0; i < arraySize; i++) {
    alpha = icon[i] & 0xF0;  
    tft.pushColor(alphaBlend(alpha, fgColor, bgColor));
    alpha = icon[i]<<4;  
    tft.pushColor(alphaBlend(alpha, fgColor, bgColor));
  }
  Serial.println(millis()-mill);  
}


uint16_t alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc)
{
  // For speed use fixed point maths and rounding to permit a power of 2 division
  uint16_t fgR = ((fgc >> 10) & 0x3E) + 1;
  uint16_t fgG = ((fgc >>  4) & 0x7E) + 1;
  uint16_t fgB = ((fgc <<  1) & 0x3E) + 1;

  uint16_t bgR = ((bgc >> 10) & 0x3E) + 1;
  uint16_t bgG = ((bgc >>  4) & 0x7E) + 1;
  uint16_t bgB = ((bgc <<  1) & 0x3E) + 1;

  // Shift right 1 to drop rounding bit and shift right 8 to divide by 256
  uint16_t r = (((fgR * alpha) + (bgR * (255 - alpha))) >> 9);
  uint16_t g = (((fgG * alpha) + (bgG * (255 - alpha))) >> 9);
  uint16_t b = (((fgB * alpha) + (bgB * (255 - alpha))) >> 9);

  // Combine RGB565 colours into 16 bits
  //return ((r&0x18) << 11) | ((g&0x30) << 5) | ((b&0x18) << 0); // 2 bit greyscale
  //return ((r&0x1E) << 11) | ((g&0x3C) << 5) | ((b&0x1E) << 0); // 4 bit greyscale
  return (r << 11) | (g << 5) | (b << 0);
}