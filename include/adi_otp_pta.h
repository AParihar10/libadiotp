// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright 2022, Analog Devices, Inc.
 */
#ifndef ADI_OTP_PTA_H
#define ADI_OTP_PTA_H

#include <stdint.h>

#define PTA_ADI_OTP_UUID { 0x215205d5, 0x5c02, 0x445a, \
	{ 0xac, 0x3e, 0x09, 0x6a, 0x19, 0xc1, 0xa1, 0x85 } }

/*
 * ADI_OTP_CMD_READ - Read data from OTP by ID
 *
 * param[0] (in value) - ID of OTP data to read
 * param[1] (out memref) - Buffer to write to
 * param[2] unused
 * param[3] unused
 */
#define ADI_OTP_CMD_READ			0x00

/*
 * ADI_OTP_CMD_WRITE - Write data to OTP by ID
 *
 * param[0] (in value) - ID of OTP data to write
 * param[1] (in memref) - Buffer to write from
 * param[2] unused
 * param[3] unused
 */
#define ADI_OTP_CMD_WRITE			0x01

/*
 * ADI_OTP_CMD_INVALIDATE - Invalidate a boot key in OTP
 *
 * param[0] (in value) - ID of key to invalidate
 * param[1] unused
 * param[2] unused
 * param[3] unused
 */
#define ADI_OTP_CMD_INVALIDATE		0x02

/*
 * ADI_OTP_CMD_VERSION - Get library version information
 *
 * @param[0] (out value) - .a = major, .b = minor version information
 * param[1] unused
 * param[2] unused
 * param[3] unused
 */
#define ADI_OTP_CMD_VERSION			0x03

/*
 * IDs for specific items that can be written or read. Not all items are
 * available for both reading and writing, and access may further depend
 * on the session's access level
 */
#define ADI_OTP_ID_huk					1
#define ADI_OTP_ID_rkek					2
#define ADI_OTP_ID_dek					3
#define ADI_OTP_ID_oem_public_key		4
#define ADI_OTP_ID_pvt_128key0			5
#define ADI_OTP_ID_pvt_128key1			6
#define ADI_OTP_ID_pvt_128key2			7
#define ADI_OTP_ID_pvt_128key3			8
#define ADI_OTP_ID_ek					9
#define ADI_OTP_ID_secure_emu_key0		10
#define ADI_OTP_ID_secure_emu_key1		11
#define ADI_OTP_ID_public_key0			12
#define ADI_OTP_ID_public_key1			13
#define ADI_OTP_ID_gp1					14
/* @todo define more fields to read */
#define __ADI_OTP_ID_COUNT				15

/* Maximum possible size of a buffer */
#define MAX_OTP_LENGTH 512

/* Version information to detect mismatch between binary and library */
#define ADI_OTP_MAJOR		1
#define ADI_OTP_MINOR		0

#endif
