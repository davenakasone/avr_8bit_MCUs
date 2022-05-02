#ifndef H_ICM20948
#define H_ICM20948

//#define DEBUG_CLI 1776 // turn on or off
#define DEBUG_UART 1917 // turn on or off

// extras
#define F_CPU 16000000UL
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) -1)
#define PAUSE_RESET_MS                 200
#define PAUSE_PROG_MS                  1000
#define BUF_SIZ                        128
#define UINT8_BUF                      20
#define MOV_AVG                        10 // see "free_candy_2.c", c_autoOffsets()
#define GET_NAME(var) #var

// library
#include "i2cmaster.h"
#include <stdbool.h>
#include <math.h> // M_PI, sin, cos, etc
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/twi.h>
#include <avr/io.h>
#include <util/delay.h>

// other functions
void USART_init(unsigned int ubrr);
void USART_tx_string(char * data);
//void old_init();
//void ICM_write(uint8_t reg, uint8_t data);
//void ICM20948_verify_whoami(void);
//void get_raw_data();
void f_writeRegister8(uint8_t bank, uint8_t reg, uint8_t val);
void f_switchBank(uint8_t newBank);


// very important
#define ICM20948_ADDRESS	           0x68     // (1101001 << 1) I2C slave address when AD0=0 [gnd], no shift yet
#define AK09916_ADDRESS                0x0C
#define AK09916_WHO_AM_I_1             0x4809
#define AK09916_WHO_AM_I_2             0x0948
#define ICM20948_WHO_AM_I_CONTENT      0xEA
#define ICM20948_ROOM_TEMP_OFFSET      0.0f
#define ICM20948_T_SENSITIVITY         333.87f
#define AK09916_MAG_LSB                0.1495f
#define ICM20948_REG_BANK_SEL          0x7F    // Registers ICM20948 ALL BANKS 

// Registers ICM20948 USER BANK 0
#define ICM20948_WHO_AM_I              0x00
#define ICM20948_USER_CTRL             0x03
#define ICM20948_LP_CONFIG             0x05
#define ICM20948_PWR_MGMT_1            0x06
#define ICM20948_PWR_MGMT_2            0x07
#define ICM20948_INT_PIN_CFG           0x0F
#define ICM20948_INT_ENABLE            0x10
#define ICM20948_INT_ENABLE_1          0x11
#define ICM20948_INT_ENABLE_2          0x12
#define ICM20948_INT_ENABLE_3          0x13
#define ICM20948_I2C_MST_STATUS        0x17
#define ICM20948_INT_STATUS            0x19
#define ICM20948_INT_STATUS_1          0x1A
#define ICM20948_INT_STATUS_2          0x1B
#define ICM20948_INT_STATUS_3          0x1C
#define ICM20948_DELAY_TIME_H          0x28
#define ICM20948_DELAY_TIME_L          0x29
#define ICM20948_ACCEL_OUT             0x2D // accel data registers begin
#define ICM20948_GYRO_OUT              0x33 // gyro data registers begin
#define ICM20948_TEMP_OUT              0x39 
#define ICM20948_EXT_SLV_SENS_DATA_00  0x3B
#define ICM20948_EXT_SLV_SENS_DATA_01  0x3C
#define ICM20948_FIFO_EN_1             0x66
#define ICM20948_FIFO_EN_2             0x67
#define ICM20948_FIFO_RST              0x68
#define ICM20948_FIFO_MODE             0x69
#define ICM20948_FIFO_COUNT            0x70
#define ICM20948_FIFO_R_W              0x72
#define ICM20948_DATA_RDY_STATUS       0x74
#define ICM20948_FIFO_CFG              0x76

// Registers ICM20948 USER BANK 1
#define ICM20948_SELF_TEST_X_GYRO      0x02
#define ICM20948_SELF_TEST_Y_GYRO      0x03
#define ICM20948_SELF_TEST_Z_GYRO      0x04
#define ICM20948_SELF_TEST_X_ACCEL     0x0E
#define ICM20948_SELF_TEST_Y_ACCEL     0x0F
#define ICM20948_SELF_TEST_Z_ACCEL     0x10
#define ICM20948_XA_OFFS_H             0x14
#define ICM20948_XA_OFFS_L             0x15
#define ICM20948_YA_OFFS_H             0x17
#define ICM20948_YA_OFFS_L             0x18
#define ICM20948_ZA_OFFS_H             0x1A
#define ICM20948_ZA_OFFS_L             0x1B
#define ICM20948_TIMEBASE_CORR_PLL     0x28

// Registers ICM20948 USER BANK 2
#define ICM20948_GYRO_SMPLRT_DIV       0x00
#define ICM20948_GYRO_CONFIG_1         0x01
#define ICM20948_GYRO_CONFIG_2         0x02
#define ICM20948_XG_OFFS_USRH          0x03
#define ICM20948_XG_OFFS_USRL          0x04
#define ICM20948_YG_OFFS_USRH          0x05
#define ICM20948_YG_OFFS_USRL          0x06
#define ICM20948_ZG_OFFS_USRH          0x07
#define ICM20948_ZG_OFFS_USRL          0x08
#define ICM20948_ODR_ALIGN_EN          0x09
#define ICM20948_ACCEL_SMPLRT_DIV_1    0x10
#define ICM20948_ACCEL_SMPLRT_DIV_2    0x11
#define ICM20948_ACCEL_INTEL_CTRL      0x12
#define ICM20948_ACCEL_WOM_THR         0x13
#define ICM20948_ACCEL_CONFIG          0x14
#define ICM20948_ACCEL_CONFIG_2        0x15
#define ICM20948_FSYNC_CONFIG          0x52
#define ICM20948_TEMP_CONFIG           0x53
#define ICM20948_MOD_CTRL_USR          0x54

// Registers ICM20948 USER BANK 3
#define ICM20948_I2C_MST_ODR_CFG       0x00
#define ICM20948_I2C_MST_CTRL          0x01
#define ICM20948_I2C_MST_DELAY_CTRL    0x02
#define ICM20948_I2C_SLV0_ADDR         0x03
#define ICM20948_I2C_SLV0_REG          0x04
#define ICM20948_I2C_SLV0_CTRL         0x05
#define ICM20948_I2C_SLV0_DO           0x06

// Registers AK09916 
#define AK09916_WIA_1                  0x00 // Who I am, Company ID
#define AK09916_WIA_2                  0x01 // Who I am, Device ID
#define AK09916_STATUS_1               0x10 
#define AK09916_HXL                    0x11
#define AK09916_HXH                    0x12
#define AK09916_HYL                    0x13
#define AK09916_HYH                    0x14
#define AK09916_HZL                    0x15
#define AK09916_HZH                    0x16
#define AK09916_STATUS_2               0x18
#define AK09916_CNTL_2                 0x31
#define AK09916_CNTL_3                 0x32

// Register Bits
#define ICM20948_RESET                 0x80
#define ICM20948_I2C_MST_EN            0x20
#define ICM20948_SLEEP                 0x40
#define ICM20948_LP_EN                 0x20
#define ICM20948_BYPASS_EN             0x02
#define ICM20948_GYR_EN                0x07
#define ICM20948_ACC_EN                0x38
#define ICM20948_FIFO_EN               0x40
#define ICM20948_INT1_ACTL             0x80
#define ICM20948_INT_1_LATCH_EN        0x20
#define ICM20948_ACTL_FSYNC            0x08
#define ICM20948_INT_ANYRD_2CLEAR      0x10
#define ICM20948_FSYNC_INT_MODE_EN     0x06
#define AK09916_16_BIT                 0x10
#define AK09916_OVF                    0x08
#define AK09916_READ                   0x80


///////////
//    enums
///////////
typedef enum ICM20948_CYCLE 
{
    ICM20948_NO_CYCLE              = 0x00,
    ICM20948_GYR_CYCLE             = 0x10, 
    ICM20948_ACC_CYCLE             = 0x20,
    ICM20948_ACC_GYR_CYCLE         = 0x30,
    ICM20948_ACC_GYR_I2C_MST_CYCLE = 0x70
} ICM20948_cycle;

typedef enum ICM20948_INT_PIN_POL 
{
    ICM20948_ACT_HIGH, ICM20948_ACT_LOW
} ICM20948_intPinPol;

typedef enum ICM20948_INT_TYPE 
{
    ICM20948_FSYNC_INT      = 0x01,
    ICM20948_WOM_INT        = 0x02,
    ICM20948_DMP_INT        = 0x04,
    ICM20948_DATA_READY_INT = 0x08,
    ICM20948_FIFO_OVF_INT   = 0x10,
    ICM20948_FIFO_WM_INT    = 0x20
} ICM20948_intType;

typedef enum ICM20948_FIFO_TYPE {
    ICM20948_FIFO_ACC        = 0x10,
    ICM20948_FIFO_GYR        = 0x0E,
    ICM20948_FIFO_ACC_GYR    = 0x1E
} ICM20948_fifoType;

typedef enum ICM20948_FIFO_MODE_CHOICE 
{
    ICM20948_CONTINUOUS, ICM20948_STOP_WHEN_FULL
} ICM20948_fifoMode;

typedef enum ICM20948_GYRO_RANGE 
{
    ICM20948_GYRO_RANGE_250, 
	ICM20948_GYRO_RANGE_500, 
	ICM20948_GYRO_RANGE_1000, 
	ICM20948_GYRO_RANGE_2000
} ICM20948_gyroRange;

typedef enum ICM20948_DLPF 
{
    ICM20948_DLPF_0, 
	ICM20948_DLPF_1, 
	ICM20948_DLPF_2, 
	ICM20948_DLPF_3, 
	ICM20948_DLPF_4, 
	ICM20948_DLPF_5, 
    ICM20948_DLPF_6, 
	ICM20948_DLPF_7, 
	ICM20948_DLPF_OFF
} ICM20948_dlpf;

typedef enum ICM20948_GYRO_AVG_LOW_PWR 
{
    ICM20948_GYR_AVG_1, 
	ICM20948_GYR_AVG_2, 
	ICM20948_GYR_AVG_4, 
	ICM20948_GYR_AVG_8, 
	ICM20948_GYR_AVG_16, 
    ICM20948_GYR_AVG_32, 
	ICM20948_GYR_AVG_64, 
	ICM20948_GYR_AVG_128
} ICM20948_gyroAvgLowPower;

typedef enum ICM20948_ACC_RANGE 
{
    ICM20948_ACC_RANGE_2G, 
	ICM20948_ACC_RANGE_4G, 
	ICM20948_ACC_RANGE_8G, 
	ICM20948_ACC_RANGE_16G
} ICM20948_accRange;

typedef enum ICM20948_ACC_AVG_LOW_PWR 
{
    ICM20948_ACC_AVG_4, 
	ICM20948_ACC_AVG_8, 
	ICM20948_ACC_AVG_16, 
	ICM20948_ACC_AVG_32
} ICM20948_accAvgLowPower;

typedef enum ICM20948_WOM_COMP 
{
    ICM20948_WOM_COMP_DISABLE, 
	ICM20948_WOM_COMP_ENABLE
} ICM20948_womCompEn;

typedef enum AK09916_OP_MODE 
{
    AK09916_PWR_DOWN           = 0x00,
    AK09916_TRIGGER_MODE       = 0x01,
    AK09916_CONT_MODE_10HZ     = 0x02,
    AK09916_CONT_MODE_20HZ     = 0x04,
    AK09916_CONT_MODE_50HZ     = 0x06,
    AK09916_CONT_MODE_100HZ    = 0x08
} AK09916_opMode;

typedef enum ICM20948_ORIENTATION 
{
  ICM20948_FLAT, 
  ICM20948_FLAT_1, 
  ICM20948_XY, 
  ICM20948_XY_1, 
  ICM20948_YX, 
  ICM20948_YX_1
} ICM20948_orientation;


/////////////
//    structs
/////////////
struct xyzFloat 
{
    float x;
    float y;
    float z;
}; typedef struct xyzFloat xyzFloat;

struct xyzU
{
	uint8_t x_L;
	uint8_t x_H;
	uint16_t x;
	uint16_t y;
	uint8_t y_L;
	uint8_t y_H;
	uint16_t z;
	uint8_t z_L;
	uint8_t z_H;
}; typedef struct xyzU xyzU;

struct class_vars
{
	char str_orientation[BUF_SIZ];
	char helper[BUF_SIZ];
	uint8_t i2cAddress;
	uint8_t currentBank;
	uint8_t buffer[UINT8_BUF];
	
	float celsius;
	float g_result;
	float roll;
	float pitch;
	float yaw;
	
	xyzFloat acl;
	xyzFloat gyr;
	xyzFloat mag;
	
	xyzU raw_acl;
	xyzU raw_gyr;
	xyzU raw_mag;
	
	xyzFloat accOffsetVal;
	xyzFloat accCorrFactor;
	xyzFloat gyrOffsetVal;
	
	uint8_t accRangeFactor;
	uint8_t gyrRangeFactor;
	ICM20948_fifoType fifoType;
	
	uint8_t regVal;   // intermediate storage of register values
	
}; typedef struct class_vars class_vars;

class_vars vars; // the instance to operate on


//////////////////////////////////////
//    functions from the gutted class
//////////////////////////////////////

// class_1.c
void c_init();
void c_reset_ICM20948();
void c_sleep(bool sleep);
void c_writeRegister8(uint8_t bank, uint8_t reg, uint8_t val);
void c_switchBank(uint8_t newBank);
uint8_t c_whoAmI();
uint8_t c_readRegister8(uint8_t bank, uint8_t reg);

// class_2.c
void c_autoOffsets();
void c_readSensor();
void c_readAllData(uint8_t* data);
void c_setGyrDLPF(ICM20948_dlpf dlpf);
void c_setGyrRange(ICM20948_gyroRange gyroRange);
void c_setAccRange(ICM20948_accRange accRange);
void c_setAccDLPF(ICM20948_dlpf dlpf);
xyzFloat c_getAccRawValues();
xyzFloat c_getGyrRawValues();

// class_3.c
void c_setAccOffsets(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);
void c_setGyrOffsets(float xOffset, float yOffset, float zOffset);
void c_enableAcc(bool enAcc);
void c_setAccSampleRateDivider(uint16_t accSplRateDiv);
void c_writeRegister16(uint8_t bank, uint8_t reg, int16_t val);
void c_enableGyr(bool enGyr);
void c_setGyrSampleRateDivider(uint8_t gyrSplRateDiv);
void c_setTempDLPF(ICM20948_dlpf dlpf);
void c_setI2CMstSampleRate(uint8_t rateExp);
xyzFloat c_getCorrectedAccRawValues();
xyzFloat c_correctAccRawValues(xyzFloat accRawVal);
xyzFloat c_getGValues();
xyzFloat c_getAccRawValuesFromFifo();
xyzFloat c_readICM20948xyzValFromFifo(); // simplified
xyzFloat c_getCorrectedAccRawValuesFromFifo();
xyzFloat c_getGValuesFromFifo();

// class_4.c
float c_getResultantG(xyzFloat gVal);
float c_getTemperature();
xyzFloat c_getCorrectedGyrRawValues();
xyzFloat c_correctGyrRawValues(xyzFloat gyrRawVal);
xyzFloat c_getGyrValues();
xyzFloat c_getGyrValuesFromFifo();
xyzFloat c_getMagValues();
void c_enableCycle(ICM20948_cycle cycle);
void c_enableLowPower(bool enLP);
void c_setGyrAverageInCycleMode(ICM20948_gyroAvgLowPower avg);
void c_setAccAverageInCycleMode(ICM20948_accAvgLowPower avg);
xyzFloat c_getAngles();
ICM20948_orientation c_getOrientation();
void c_getOrientationAsString(); // modified to update the struct instead of std::string

// class_5.c
float c_getPitch();
float c_getRoll();
void c_setIntPinPolarity(ICM20948_intPinPol pol);
void c_enableIntLatch(bool latch);
void c_enableClearIntByAnyRead(bool clearByAnyRead);
void c_setFSyncIntPolarity(ICM20948_intPinPol pol);
void c_enableInterrupt(ICM20948_intType intType);
void c_disableInterrupt(ICM20948_intType intType);
uint8_t c_readAndClearInterrupts();

// class_6.c
bool c_checkInterrupt(uint8_t source, ICM20948_intType type);
void c_setWakeOnMotionThreshold(uint8_t womThresh, ICM20948_womCompEn womCompEn);
void c_enableFifo(bool fifo);
void c_setFifoMode(ICM20948_fifoMode mode);
void c_startFifo(ICM20948_fifoType fifo);
void c_stopFifo();
void c_resetFifo();
int16_t c_getFifoCount();
int16_t c_readRegister16(uint8_t bank, uint8_t reg); // reads high first, increments address +1 for low, then combines
int16_t c_getNumberOfFifoDataSets();
void c_findFifoBegin();

// class_7.c
void c_initMagnetometer();
int16_t c_whoAmIMag();
void c_setMagOpMode(AK09916_opMode opMode);
void c_resetMag();
void c_setClockToAutoSelect();
void c_writeAK09916Register8(uint8_t reg, uint8_t val);
uint8_t c_readAK09916Register8(uint8_t reg);
int16_t c_readAK09916Register16(uint8_t reg);
void c_enableI2CMaster();
void c_enableMagDataRead(uint8_t reg, uint8_t bytes);


#endif
