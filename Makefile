.PHONY: all clean test

all: libkeepalive libkeepalive_listen

libkeepalive:
	$(CC) -Wall -Wextra -pedantic -D_GNU_SOURCE -nostartfiles -shared -fpic -fPIC \
		-Wconversion -Wshadow \
		-Wpointer-arith -Wcast-qual \
		-Wstrict-prototypes -Wmissing-prototypes \
	 	-o $@.so keepalive.c $@.c -ldl \
	 	-Wl,-z,relro,-z,now -Wl,-z,noexecstack

libkeepalive_listen:
	$(CC) -Wall -Wextra -pedantic -D_GNU_SOURCE -nostartfiles -shared -fpic -fPIC \
		-Wconversion -Wshadow \
		-Wpointer-arith -Wcast-qual \
		-Wstrict-prototypes -Wmissing-prototypes \
	 	-o $@.so keepalive.c $@.c -ldl \
	 	-Wl,-z,relro,-z,now -Wl,-z,noexecstack

clean:
	-@rm libkeepalive.so libkeepalive_listen.so

test:
	@bats test
