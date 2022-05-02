#include "ICM20948.h"
/*
void old_init()
{
	i2c_start(0xD0);					// select ICM20948 (0x68<<1)+0
	i2c_write(0x06);					// select pwr_mgmt_1
	i2c_write(0x01);					// set bit 1
	i2c_stop();
	// added
	ICM_write(0x06, 0x01);    // exit sleep mode, set clk to auto
	ICM_write(0x7F, 0x20);    // select User Bank 2
	ICM_write(0x01, 0x29);    // set gyro rate for 250 with LPF of 17Hz
	ICM_write(0x00, 0x0A);    // set gyroscope sample rate for 100Hz
	ICM_write(0x14, 0x15);    // set accelerometer low pass filter to 136Hz and the rate to 8G
	ICM_write(0x11, 0x0A);    // set accelerometer rate to 100hz
	ICM_write(0x7F, 0x00);    // select User Bank 0
}
*/
////~~~~
/*
void ICM_write(uint8_t reg, uint8_t data)
{
	i2c_start(0xD0);    // start I2C for writing 0xD0
	i2c_write(reg);                   // write register address to read
	i2c_write(data);                  // write data to be saved to register
	i2c_stop();                       // stop I2C
}
*/
////~~~~
/*
void ICM20948_verify_whoami(void)
{
	uint8_t who_am_i = 0;
	i2c_start(0xD0);		// 68 << 1 = D0?
	i2c_write(0);				// select who_am_i
	i2c_stop();							// halt i2c
	i2c_start(0xD1);		// D0 + 1(TWI READ)
	who_am_i = i2c_readNak();			// save to variable
	snprintf(vars.helper,BUF_SIZ-1,"DEVICE ID: %02X\r\n",who_am_i);
	USART_tx_string(vars.helper);
	i2c_stop();
	c_initMagnetometer();
}
*/
////~~~~
/*
void get_raw_data()
{
	vars.raw_acl.x = (int16_t)( (vars.buffer[0] << 8)  | vars.buffer[1]) * 1.0;
	vars.raw_acl.y = (int16_t)( (vars.buffer[2] << 8)  | vars.buffer[3]) * 1.0;
	vars.raw_acl.z = (int16_t)( (vars.buffer[4] << 8)  | vars.buffer[5]) * 1.0;
	vars.raw_gyr.x = (int16_t)( (vars.buffer[6] << 8)  | vars.buffer[7]) * 1.0;
	vars.raw_gyr.y = (int16_t)( (vars.buffer[8] << 8)  | vars.buffer[9]) * 1.0;
	vars.raw_gyr.z = (int16_t)( (vars.buffer[10] << 8) | vars.buffer[11]) * 1.0;
	vars.raw_mag.x = (int16_t)( (vars.buffer[15] << 8) | vars.buffer[14]);
	vars.raw_mag.y = (int16_t)( (vars.buffer[17] << 8) | vars.buffer[16]);
	vars.raw_mag.z = (int16_t)( (vars.buffer[19] << 8) | vars.buffer[18]);
	vars.mag.x = vars.raw_mag.x * AK09916_MAG_LSB;
	vars.mag.y = vars.raw_mag.y * AK09916_MAG_LSB;
	vars.mag.z = vars.raw_mag.z * AK09916_MAG_LSB;
}

*/
////~~~~


void f_writeRegister8(uint8_t bank, uint8_t reg, uint8_t val)
{
	c_switchBank(bank);
	i2c_start((vars.i2cAddress << 1) + I2C_WRITE);    //_wire->beginTransmission(vars.i2cAddress);
	i2c_write(reg);                                   //_wire->write(reg);
	i2c_write(val);                                   //_wire->write(val);
	i2c_stop();                                       //_wire->endTransmission();
}


////~~~~


void f_switchBank(uint8_t newBank)
{
	if (vars.currentBank == newBank) {return;}
	vars.currentBank = newBank;
	i2c_start((vars.i2cAddress << 1) + I2C_WRITE);    //_wire->beginTransmission(vars.i2cAddress);
	i2c_write(ICM20948_REG_BANK_SEL);                 //_wire->write(ICM20948_REG_BANK_SEL);
	i2c_write(vars.currentBank << 4);           	  //_wire->write(vars.currentBank << 4);
	i2c_stop();                                       //_wire->endTransmission();
}


////////~~~~~~~~~END>  workers.c
