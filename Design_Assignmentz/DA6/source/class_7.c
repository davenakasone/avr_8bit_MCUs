#include "ICM20948.h"
    

void c_initMagnetometer()
{
	c_enableI2CMaster();
	c_resetMag();
	c_reset_ICM20948();
	c_sleep(false);
	c_writeRegister8(2, ICM20948_ODR_ALIGN_EN, 1); // aligns ODR
	_delay_ms(PAUSE_RESET_MS);
	c_enableI2CMaster();
	_delay_ms(PAUSE_RESET_MS);
	
	int16_t whoAmI = c_whoAmIMag();
	#ifdef DEBUG_UART
	snprintf(vars.helper, BUF_SIZ-1, "returned:  0x%X  ,  should be:  0x%X or 0x%X\r\n",
	whoAmI, AK09916_WHO_AM_I_1, AK09916_WHO_AM_I_2);
	USART_tx_string(vars.helper);
	#endif
	
	c_setMagOpMode(AK09916_CONT_MODE_100HZ);
}


////~~~~


int16_t c_whoAmIMag()
{
	return c_readAK09916Register16(AK09916_WIA_1);
}


////~~~~


void c_setMagOpMode(AK09916_opMode opMode)
{
	c_writeAK09916Register8(AK09916_CNTL_2, opMode);
	_delay_ms(PAUSE_RESET_MS);
	if(opMode!=AK09916_PWR_DOWN){
		c_enableMagDataRead(AK09916_HXL, 0x08);
	}
}


////~~~~


void c_resetMag()
{
	c_writeAK09916Register8(AK09916_CNTL_3, 0x01);
	_delay_ms(PAUSE_RESET_MS);
}


////~~~~


void c_setClockToAutoSelect()
{
	vars.regVal = c_readRegister8(0, ICM20948_PWR_MGMT_1);
	vars.regVal |= 0x01;
	c_writeRegister8(0, ICM20948_PWR_MGMT_1, vars.regVal);
	_delay_ms(PAUSE_RESET_MS);
}


////~~~~


void c_writeAK09916Register8(uint8_t reg, uint8_t val)
{
	c_writeRegister8(3, ICM20948_I2C_SLV0_ADDR, AK09916_ADDRESS); // write AK09916
	c_writeRegister8(3, ICM20948_I2C_SLV0_REG, reg); // define AK09916 register to be written to
	c_writeRegister8(3, ICM20948_I2C_SLV0_DO, val);
}


////~~~~


uint8_t c_readAK09916Register8(uint8_t reg)
{
	c_enableMagDataRead(reg, 0x01);
	c_enableMagDataRead(AK09916_HXL, 0x08);
	vars.regVal = c_readRegister8(0, ICM20948_EXT_SLV_SENS_DATA_00);
	return vars.regVal;
}


////~~~~


int16_t c_readAK09916Register16(uint8_t reg)
{
	int16_t regValue = 0;
	c_enableMagDataRead(reg, 0x02);
	regValue = c_readRegister16(0, ICM20948_EXT_SLV_SENS_DATA_00);
	c_enableMagDataRead(AK09916_HXL, 0x08);
	return regValue;
}


////~~~~


void c_enableI2CMaster()
{
	c_writeRegister8(0, ICM20948_USER_CTRL, ICM20948_I2C_MST_EN); //enable I2C master
	c_writeRegister8(3, ICM20948_I2C_MST_CTRL, 0x07); // set I2C clock to 345.60 kHz
	_delay_ms(PAUSE_RESET_MS);
}


////~~~~


void c_enableMagDataRead(uint8_t reg, uint8_t bytes)
{
	c_writeRegister8(3, ICM20948_I2C_SLV0_ADDR, AK09916_ADDRESS | AK09916_READ); // read AK09916
	c_writeRegister8(3, ICM20948_I2C_SLV0_REG, reg); // define AK09916 register to be read
	c_writeRegister8(3, ICM20948_I2C_SLV0_CTRL, 0x80 | bytes); //enable read | number of byte
	_delay_ms(PAUSE_RESET_MS);
}


////////~~~~~~~~END>  free_candy_7.c
