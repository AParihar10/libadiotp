
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "libadiotp.h"

static void usage(void) {
	fprintf(stderr, "\n");
	fprintf(stderr, " Usage:\n");
	fprintf(stderr, "    adiotp-cli [id]       Read [id]\n");
	fprintf(stderr, "    adiotp-cli -s [id]    Write [id]\n");
	fprintf(stderr, "    adiotp-cli -l         Lock SoC\n");
	fprintf(stderr, "    adiotp-cli -i [id]    Invalidate key [id]\n");
	fprintf(stderr, "    adiotp-cli -v [id]    Check if key [id] is valid\n");
	fprintf(stderr, "    adiotp-cli -w [id]    Check if key [id] is written\n");
	fprintf(stderr, "    adiotp-cli -z         Check if SoC is locked\n");
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

static int cli_lock(struct adi_otp *otp) {
	int ret = adi_otp_lock(otp);

	if (ret)
		fprintf(stderr, " Lock error = %d\n", ret);

	return ret;
}

static int cli_is_locked(struct adi_otp *otp) {
	int ret;
	int locked = 0;

	ret = adi_otp_is_locked(otp, &locked);
	if (ret) {
		fprintf(stderr, " is_locked error = %d\n", ret);
		return ret;
	}

	printf("%s\n", (locked ? "SoC Locked" : "SoC Unlocked"));
	return 0;
}

static int cli_invalidate(struct adi_otp *otp, uint32_t id) {
	int ret;

	ret = adi_otp_invalidate(otp, id);
	if (ret)
		fprintf(stderr, " invalidate failed for %d = %d\n", id, ret);

	return ret;
}

static int cli_is_valid(struct adi_otp *otp, uint32_t id) {
	int valid = 0;
	int ret;

	ret = adi_otp_is_valid(otp, id, &valid);
	if (ret) {
		fprintf(stderr, " is_valid for %d error = %d\n", id, ret);
		return ret;
	}

	printf("%d %s valid\n", id, (valid ? "is" : "is not"));
	return 0;
}

static int cli_is_written(struct adi_otp *otp, uint32_t id) {
	int written = 0;
	int ret;

	ret = adi_otp_is_written(otp, id, &written);
	if (ret) {
		fprintf(stderr, " is_written for %d error = %d\n", id, ret);
		return ret;
	}

	printf("%d %s written\n", id, (written ? "is" : "is not"));
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
		if (argv[1][0] == '-') {
			switch (argv[1][1]) {
			case 'l':
				cli_lock(otp);
				break;
			case 'z':
				cli_is_locked(otp);
				break;
			default:
				usage();
				goto cleanup;
				break;
			}
		}
		else {
			id = atoi(argv[1]);
			cli_read(otp, id);
		}
		break;
	case 3:
		if (argv[1][0] != '-') {
			usage();
			goto cleanup;
		}

		id = atoi(argv[2]);

		switch(argv[1][1]) {
		case 's':
			cli_write(otp, id);
			break;
		case 'i':
			cli_invalidate(otp, id);
			break;
		case 'v':
			cli_is_valid(otp, id);
			break;
		case 'w':
			cli_is_written(otp, id);
			break;
		default:
			usage();
			goto cleanup;
			break;
		}
		break;
	default:
		usage();
		break;
	}

cleanup:
	adi_otp_close(otp);

	return 0;
}
