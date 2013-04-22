LEDSIGNSOURCES=main.c sign_proto.c
LEDSIGNOBJS=$(LEDSIGNSOURCES:.c=.o)

all: LEDSign

LEDSign: $(LEDSIGNOBJS)
	$(CC) $(LDFLAGS) $(LEDSIGNOBJS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	rm -f $(LEDSIGNOBJS)

dist-clean: clean
	rm LEDSign
