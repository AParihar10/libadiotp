/* SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright 2023, Analog Devices, Inc. All rights reserved.
 */

#ifndef LIBADIOTP_H
#define LIBADIOTP_H

#include "adi_otp_pta.h"

struct adi_otp;

struct adi_otp *adi_otp_open(void);
void adi_otp_close(struct adi_otp *);

int adi_otp_get_version(struct adi_otp *otp, int *major, int *minor);

int adi_otp_read(struct adi_otp *otp, uint32_t id, void *buf, uint32_t *len);
int adi_otp_write(struct adi_otp *otp, uint32_t id, void *buf, uint32_t len);
int adi_otp_invalidate(struct adi_otp *otp, uint32_t id);
int adi_otp_is_valid(struct adi_otp *otp, uint32_t id, uint32_t *valid);
int adi_otp_is_written(struct adi_otp *otp, uint32_t id, uint32_t *written);
int adi_otp_is_locked(struct adi_otp *otp, uint32_t *locked);
int adi_otp_lock(struct adi_otp *otp);

#endif
