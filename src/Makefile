PROTOREAD_SRCS=reader.c
PROTOREAD_OBJS=$(PROTOREAD_SRCS:.c=.o)


LEDSIGNSOURCES=main.c sign_proto.c
LEDSIGNOBJS=$(LEDSIGNSOURCES:.c=.o)

all: LEDSign ProtoRead

LEDSign: $(LEDSIGNOBJS)
	$(CC) $(LDFLAGS) $^ -o $@

ProtoRead: $(PROTOREAD_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

.c.o:
	$(CC) $(CFLAGS) $^ -c -o $@

clean:
	rm -f $(LEDSIGNOBJS) $(PROTOREAD_OBJS)

dist-clean: clean
	rm LEDSign ProtoRead
