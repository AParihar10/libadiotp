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

#endif
