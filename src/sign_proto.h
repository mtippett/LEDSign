#include <stdio.h>
#include <string.h>
#include "proto.h"

int sign_proto_attention(int fd);
int sign_proto_write_buffer(int fd, int slot, char *data_buffer, int length, int speed , int action);
int sign_proto_activate(int fd, int num_lines);
int sign_proto_checksum(unsigned char *buffer, int length);
int sign_proto_set_time(int fd);
