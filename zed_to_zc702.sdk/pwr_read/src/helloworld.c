/******************************************************************************
 *
 * Copyright (C) 2010 - 2014 Xilinx, Inc.  All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * Use of the Software is limited solely to applications:
 * (a) running on a Xilinx device, or
 * (b) that interact with a Xilinx device through a bus or interconnect.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * XILINX CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Except as contained in this notice, the name of the Xilinx shall not be used
 * in advertising or otherwise to promote the sale, use or other dealings in
 * this Software without prior written authorization from Xilinx.
 *
 ******************************************************************************/
/*****************************************************************************/
/**
 * @file xiicps_polled_master_example.c
 *
 * A design example of using the device in polled mode as master.
 *
 * The example uses buffer size 132. Please set the send buffer of the
 * Aardvark device to be continuous 64 bytes from 0x00 to 0x3F.
 *
 * <pre> MODIFICATION HISTORY:
 *
 * Ver   Who Date     Changes
 * ----- --- -------- -----------------------------------------------
 * 1.00a jz  01/30/10 First release
 *
 * </pre>
 *
 ****************************************************************************/

/***************************** Include Files **********************************/
#include "xparameters.h"
#include "xiicps.h"
#include "xil_printf.h"

/************************** Constant Definitions ******************************/

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define IIC_DEVICE_ID		XPAR_XIICPS_0_DEVICE_ID

/*
 * The slave address to send to and receive from.
 */
#define IIC_SLAVE_ADDR		0b1010011   // PMBus1: 0b1010010 ADXL345: 0x53
#define IIC_SCLK_RATE		100000

// Data
u8 msg_s = 0x8B; // CMD_READ_VOUT 0x8B DEVICE_ID 0x00
//u8 add_s = 0xA6; // zum schreiben
//u8 add_r = 0x8B; // zum lesen
u8 msg_r = 0x00; // wird überschrieben

#define ADD_READ 0xA7
#define ADD_WRITE 0xA6

/**************************** Type Definitions ********************************/


/************************** Function Prototypes *******************************/

int IicPsMasterPolledExample(u16 DeviceId);
/************************** Variable Definitions ******************************/

XIicPs Iic;		/**< Instance of the IIC Device */


/******************************************************************************/
/**
 *
 * Main function to call the polled master example.
 *
 * @param	None.
 *
 * @return	XST_SUCCESS if successful, XST_FAILURE if unsuccessful.
 *
 * @note		None.
 *
 *******************************************************************************/
int main(void)
{
	int Status;

	xil_printf("IIC Master Polled Example Test \r\n");

	/*
	 * Run the Iic polled example in master mode, specify the Device
	 * ID that is specified in xparameters.h.
	 */
	//while(1){
		Status = IicPsMasterPolledExample(IIC_DEVICE_ID);
		//sleep(1);
	//}

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
 *
 * This function sends data and expects to receive data from slave as modular
 * of 64.
 *
 * This function uses interrupt-driven mode of the device.
 *
 * @param	DeviceId is the Device ID of the IicPs Device and is the
 *		XPAR_<IICPS_instance>_DEVICE_ID value from xparameters.h
 *
 * @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
 *
 * @note		None.
 *
 *******************************************************************************/
int IicPsMasterPolledExample(u16 DeviceId)
{
	int Status;
	XIicPs_Config *Config;
	int Index;

	/*
	 * Initialize the IIC driver so that it's ready to use
	 * Look up the configuration in the config table,
	 * then initialize it.
	 */
	Config = XIicPs_LookupConfig(DeviceId);
	if (NULL == Config) {
		return XST_FAILURE;
	}

	Status = XIicPs_CfgInitialize(&Iic, Config, Config->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Perform a self-test to ensure that the hardware was built correctly.
	 */
	Status = XIicPs_SelfTest(&Iic);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Set the IIC serial clock rate.
	 */
	XIicPs_SetSClk(&Iic, IIC_SCLK_RATE);


	// SEND ADRESS of the Register to read from
	Status = XIicPs_MasterSendPolled(&Iic, &msg_s,
			1, IIC_SLAVE_ADDR);

	if (Status != XST_SUCCESS) {
		xil_printf("XST_FAILURE in Master Send Polled. Exit code:");
		xil_printf("%d",Status);

		return XST_FAILURE;
	}
	xil_printf("After Master Sendpolled");


	// RECV DATA from previously set address
	Status = XIicPs_MasterRecvPolled(&Iic, &msg_r,
			1, IIC_SLAVE_ADDR);

	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}


	xil_printf("Hex Inhalt REG %x: %x", msg_s, msg_r);

	return XST_SUCCESS;
}
