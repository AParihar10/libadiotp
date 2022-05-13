
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "libadiotp.h"

static void usage(void) {
	fprintf(stderr, "\n");
	fprintf(stderr, " Usage:\n");
	fprintf(stderr, "    adiotp-cli [id]\n");
	fprintf(stderr, "    adiotp-cli -s [id]\n");
	fprintf(stderr, "\n");
	fprintf(stderr, " When reading, the OTP value will be sent to stdout.\n");
	fprintf(stderr, " To view it, you may wish to use hexdump or xxd, for example:\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "    adiotp-cli 25 | xxd\n");
	fprintf(stderr, "\n");
	fprintf(stderr, " When writing, the data to be written will be read from stdin as\n");
	fprintf(stderr, " a byte stream. Use ctrl+D to terminate. The size to be written\n");
	fprintf(stderr, " must match the size expected exactly (check the HRM)\n");
	fprintf(stderr, "\n");
}

static int cli_read(struct adi_otp *otp, uint32_t id) {
	uint32_t len = MAX_OTP_LENGTH;
	uint8_t buf[MAX_OTP_LENGTH];
	uint32_t i;
	int ret;

	ret = adi_otp_read(otp, id, buf, &len);
	if (ret) {
		fprintf(stderr, " Read error = %d\n", ret);
		return ret;
	}

	write(STDOUT_FILENO, buf, len);
	return 0;
}

static int cli_write(struct adi_otp *otp, uint32_t id) {
	int res;
	int ch;
	uint8_t buf[MAX_OTP_LENGTH];
	uint32_t pos = 0;

	while ((ch = getc(stdin)) != EOF) {
		if (MAX_OTP_LENGTH == pos) {
			fprintf(stderr, " Read more than %d characters from stdin, exceeding the\n", MAX_OTP_LENGTH);
			fprintf(stderr, " max possible OTP size. Aborted write\n");
			exit(1);
		}
		buf[pos++] = (uint8_t) (ch & 0xff);
	}

	res = adi_otp_write(otp, id, buf, pos);
	if (res) {
		fprintf(stderr, " Write error = %d\n", res);
		return res;
	}

	return 0;
}

int main(int argc, char **argv) {
	struct adi_otp *otp;
	uint32_t id;

	otp = adi_otp_open();
	if (!otp) {
		fprintf(stderr, "Unable to open OTP\n");
		exit(1);
	}

	switch (argc) {
	case 2:
		id = atoi(argv[1]);
		cli_read(otp, id);
		break;
	case 3:
		id = atoi(argv[2]);
		cli_write(otp, id);
		break;
	default:
		usage();
		break;
	}

	adi_otp_close(otp);

	return 0;
}
