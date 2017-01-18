This Repository contains the project files of Vivado 2014.3.1.
Aim of the project is the reading of curents of a ZC702 over an external Zedboard. An interrupt controlled measurement during reconfiguartion should be considered as an intermidiate step.
Right now reading of I2C values from a ADXL345 is working.

Connection: 

SCL	MIO10	JE2	G7	(blue)		PMBUS_CLK
SDA	MIO11	JE3	B4	(purple) 	PMBUS_DATA

PMBus Pins are documented in the Schematics of the zc702. (zc702_Schematic_xtp185_rev1_0.pdf Page 47). Physicaly the Pins are on the backside (J59) 
