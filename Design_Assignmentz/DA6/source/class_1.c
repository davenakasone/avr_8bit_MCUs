#include "ICM20948.h"


void c_init()
{
	#ifdef DEBUG_CLI
	snprintf(vars.helper, BUF_SIZ-1, "<%d> %s/%s()\r\n", __LINE__, __FILE__, __func__);
	USART_tx_string(vars.helper);
	#endif
	vars.i2cAddress = ICM20948_ADDRESS;    // what the constructor did
	vars.currentBank = 0;
	
	c_reset_ICM20948(); // a long delay is caused
	uint8_t temp;
	
	vars.accOffsetVal.x = 0.0;
	vars.accOffsetVal.y = 0.0;
	vars.accOffsetVal.z = 0.0;
	vars.accCorrFactor.x = 1.0;
	vars.accCorrFactor.y = 1.0;
	vars.accCorrFactor.z = 1.0;
	vars.accRangeFactor = 1.0;
	vars.gyrOffsetVal.x = 0.0;
	vars.gyrOffsetVal.y = 0.0;
	vars.gyrOffsetVal.z = 0.0;
	vars.gyrRangeFactor = 1.0;
	vars.fifoType = ICM20948_FIFO_ACC;
	
	temp = c_whoAmI();
	#ifdef DEBUG_UART
	snprintf(vars.helper, BUF_SIZ-1, "returned:  0x%X  ,  should be:  0x%X\r\n",
	temp, ICM20948_WHO_AM_I_CONTENT);
	USART_tx_string(vars.helper);
	#endif
	
	c_sleep(false);
	c_writeRegister8(2, ICM20948_ODR_ALIGN_EN, 1); // aligns ODR
}


////~~~~


void c_reset_ICM20948()
{
	#ifdef DEBUG_CLI
	snprintf(vars.helper, BUF_SIZ-1, "<%d> %s/%s()\r\n", __LINE__, __FILE__, __func__);
	USART_tx_string(vars.helper);
	#endif
	c_writeRegister8(0, ICM20948_PWR_MGMT_1, ICM20948_RESET);
	_delay_ms(PAUSE_PROG_MS);  // wait for registers to reset
}


////~~~~


void c_writeRegister8(uint8_t bank, uint8_t reg, uint8_t val)
{
	#ifdef DEBUG_CLI
	snprintf(vars.helper, BUF_SIZ-1, "<%d> %s/%s()\r\n", __LINE__, __FILE__, __func__);
	USART_tx_string(vars.helper);
	#endif
	c_switchBank(bank);
	i2c_start((vars.i2cAddress << 1) + I2C_WRITE);    //_wire->beginTransmission(vars.i2cAddress);
	i2c_write(reg);                                   //_wire->write(reg);
	i2c_write(val);                                   //_wire->write(val);
	i2c_stop();                                       //_wire->endTransmission();
}


////~~~~


void c_switchBank(uint8_t newBank)
{
	#ifdef DEBUG_CLI
	snprintf(vars.helper, BUF_SIZ-1, "<%d> %s/%s()\r\n", __LINE__, __FILE__, __func__);
	USART_tx_string(vars.helper);
	#endif
	if (vars.currentBank == newBank) {return;}
	vars.currentBank = newBank;
	i2c_start((vars.i2cAddress << 1) + I2C_WRITE);    //_wire->beginTransmission(vars.i2cAddress);
	i2c_write(ICM20948_REG_BANK_SEL);                 //_wire->write(ICM20948_REG_BANK_SEL);
	i2c_write(vars.currentBank << 4);           	  //_wire->write(vars.currentBank << 4);
	i2c_stop();                                       //_wire->endTransmission();
}


////~~~~


uint8_t c_whoAmI()
{
	#ifdef DEBUG_CLI
	snprintf(vars.helper, BUF_SIZ-1, "<%d> %s/%s()\r\n", __LINE__, __FILE__, __func__);
	USART_tx_string(vars.helper);
	#endif
	return c_readRegister8(0, ICM20948_WHO_AM_I);
}


////~~~~


uint8_t c_readRegister8(uint8_t bank, uint8_t reg)
{
	#ifdef DEBUG_CLI
	snprintf(vars.helper, BUF_SIZ-1, "<%d> %s/%s()\r\n", __LINE__, __FILE__, __func__);
	USART_tx_string(vars.helper);
	#endif
	c_switchBank(bank);
	uint8_t regValue = 0;
	i2c_start((vars.i2cAddress << 1) + I2C_WRITE);    //_wire->beginTransmission(vars.i2cAddress);
	i2c_write(reg);                                   //_wire->write(reg);
	i2c_start((vars.i2cAddress << 1) + I2C_READ);     // read mode
	regValue = i2c_readNak();                         // stop condition is sent
	return regValue;
}


////~~~~


void c_sleep(bool sleep)
{
	#ifdef DEBUG_CLI
	snprintf(vars.helper, BUF_SIZ-1, "<%d> %s/%s()\r\n", __LINE__, __FILE__, __func__);
	USART_tx_string(vars.helper);
	#endif
	vars.regVal = c_readRegister8(0, ICM20948_PWR_MGMT_1);
	if(sleep)
	{
		vars.regVal |= ICM20948_SLEEP;
	}
	else
	{
		vars.regVal &= ~ICM20948_SLEEP;
	}
	c_writeRegister8(0, ICM20948_PWR_MGMT_1, vars.regVal);
}


////////~~~~~~~~END>  free_candy_1.c
