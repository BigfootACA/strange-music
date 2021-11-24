all: strange-music
strange-music.o: strange-music.c
strange-music: strange-music.o
clean:
	rm -f strange-music.o strange-music
install: strange-music strange-music.service strange-music.timer
	install -vsDm755 strange-music -t $(DESTDIR)/usr/bin
	install -vDm644 strange-music.timer -t $(DESTDIR)/usr/lib/systemd/system
	install -vDm644 strange-music.service -t $(DESTDIR)/usr/lib/systemd/system
