PREFIX ?= /usr/local
DESTDIR ?=
CC ?= gcc
CFLAGS ?= -O2 -Wall -Wextra -Wpedantic
LDFLAGS ?=

TARGET = bwfall
SRCS = src/main.c src/cli.c src/ringbuf.c src/reader.c src/audio.c src/visual.c
OBJS = $(SRCS:.c=.o)

LIBS = -lasound -lpthread -lm -lSDL2

.PHONY: all clean install uninstall test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS)

install: $(TARGET)
	install -Dm755 $(TARGET) $(DESTDIR)$(PREFIX)/bin/$(TARGET)
	install -Dm644 man/bwfall.1 $(DESTDIR)$(PREFIX)/share/man/man1/bwfall.1
	install -Dm644 LICENSE $(DESTDIR)$(PREFIX)/share/licenses/$(TARGET)/LICENSE
	install -Dm644 README.md $(DESTDIR)$(PREFIX)/share/doc/$(TARGET)/README.md

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(TARGET)
	rm -f $(DESTDIR)$(PREFIX)/share/man/man1/bwfall.1
	rm -rf $(DESTDIR)$(PREFIX)/share/licenses/$(TARGET)
	rm -rf $(DESTDIR)$(PREFIX)/share/doc/$(TARGET)

test:
	@echo "Running tests..."
	@cd tests && ./test.sh
