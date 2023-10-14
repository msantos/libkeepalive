.PHONY: all clean test

all: libkeepalive libkeepalive_listen libkeepalive_socket

libkeepalive:
	$(CC) -Wall -Wextra -pedantic -D_GNU_SOURCE -nostartfiles -shared -fpic -fPIC \
		-fvisibility=hidden \
		-Wconversion -Wshadow \
		-Wpointer-arith -Wcast-qual \
		-Wstrict-prototypes -Wmissing-prototypes \
		-o $@.so keepalive.c $@.c -ldl \
		-Wl,-z,relro,-z,now -Wl,-z,noexecstack

libkeepalive_listen:
	$(CC) -Wall -Wextra -pedantic -D_GNU_SOURCE -nostartfiles -shared -fpic -fPIC \
		-fvisibility=hidden \
		-Wconversion -Wshadow \
		-Wpointer-arith -Wcast-qual \
		-Wstrict-prototypes -Wmissing-prototypes \
		-o $@.so keepalive.c $@.c -ldl \
		-Wl,-z,relro,-z,now -Wl,-z,noexecstack

libkeepalive_socket:
	$(CC) -Wall -Wextra -pedantic -D_GNU_SOURCE -nostartfiles -shared -fpic -fPIC \
		-fvisibility=hidden \
		-Wconversion -Wshadow \
		-Wpointer-arith -Wcast-qual \
		-Wstrict-prototypes -Wmissing-prototypes \
		-o $@.so keepalive.c $@.c -ldl \
		-Wl,-z,relro,-z,now -Wl,-z,noexecstack

clean:
	-@rm libkeepalive.so libkeepalive_listen.so libkeepalive_socket.so

test:
	@bats test
