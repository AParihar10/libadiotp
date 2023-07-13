SRCS := src/libadiotp.c src/adiotp-cli.c
OBJS := $(SRCS:.c=.o)
CFLAGS := $(CFLAGS) -Iinclude $(OPTEE_CLIENT_EXPORT)/include
LDFLAGS := $(LDFLAGS) -L$(OPTEE_CLIENT_EXPORT)/lib -lteec
DEBUG := -ggdb

all: libadiotp.so adiotp-cli

$(OBJS): %.o : %.c
	$(CC) $(DEBUG) -fPIC $(CFLAGS) -c $< -o $@

adiotp-cli: src/adiotp-cli.o libadiotp.so
	$(CC) $(DEBUG) -o $@ $+ $(LDFLAGS) -L. -ladiotp

libadiotp.so: src/libadiotp.o
	$(CC) $(DEBUG) -fPIC -shared -Wl,-soname,libadiotp.so.2 -o $@ $+ $(LDFLAGS)

clean:
	rm -rf $(OBJS) libadiotp.so adiotp-cli
