# Makefile for src/mod/megahal.mod/

doofus:
	@echo ""
	@echo "Let's try this from the right directory..."
	@echo ""
	@cd ../../../; make

static: ../megahal.o

modules: ../../../megahal.so

../megahal.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -DMAKING_MODS -c megahal.c
	rm -f ../megahal.o
	mv megahal.o ../

../../../megahal.so: ../megahal.o
	$(LD) -o ../../../megahal.so ../megahal.o
	$(STRIP) ../../../megahal.so

depend:
	$(CC) $(CFLAGS) $(CPPFLAGS) -MM *.c > .depend

clean:
	@rm -f .depend *.o *.so *~

#safety hash


../megahal.o: megahal.c megahal.h ../module.h ../../../config.h \
 ../../main.h ../../lang.h ../../eggdrop.h ../../flags.h ../../proto.h \
 ../../../lush.h ../../cmdt.h ../../tclegg.h ../../tclhash.h \
 ../../chan.h ../../users.h ../modvals.h ../../tandem.h
