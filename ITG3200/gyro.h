

/* WIRING GUIDE
 * 10 kOhm pull-ups on I2C lines.
 * SCL -> SCL
 * SDA -> SDA
 * CLK -> GND
 * INT -- not connected
 * GND -> GND
 * VIO -> 3.3V
 * VDD -> 3.3V
 */


#define W_ADDR 0xD2 //7-bit address
#define R_ADDR 0xD3 //7-bit address


//-----------
// Gyro Registers
//-----------

#define WHO_AM_I_REG    	0x00
#define SMPLRT_DIV_REG  	0x15
#define DLPF_FS_REG     	0x16
#define INT_CFG_REG     	0x17
#define INT_STATUS      	0x1A
#define TEMP_OUT_H_REG  	0x1B
#define TEMP_OUT_L_REG  	0x1C
#define GYRO_XOUT_H_REG 	0x1D
#define GYRO_XOUT_L_REG 	0x1E
#define GYRO_YOUT_H_REG 	0x1F
#define GYRO_YOUT_L_REG 	0x20
#define GYRO_ZOUT_H_REG 	0x21
#define GYRO_ZOUT_L_REG 	0x22
#define PWR_MGM_REG     	0x3E

//----------------------------
// Low Pass Filter Bandwidths
//----------------------------
#define LPFBW_256HZ 	0x00
#define LPFBW_188HZ 	0x01
#define LPFBW_98HZ  	0x02
#define LPFBW_42HZ  	0x03
#define LPFBW_20HZ  	0x04
#define LPFBW_10HZ  	0x05
#define LPFBW_5HZ   	0x06


/******************************** FUNCTIONS **************************************/

void Gyro_read_X(void);
void Gyro_read_Y(void);
void Gyro_read_Z(void);
void Gyro_Init(void);
void Temp_read(void);
void Gyro_set_default(void);
void send_check(void);
void rec_check(void);

/******************************** EOF *******************************************/
