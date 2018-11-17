#define filehandler int

filehandler myfopen(char *filename);

int myfread(void *out, int howmanybytes, filehandler fh);

int myfwrite(void *in, int howmanybytes, filehandler fh);

int myfseek(int pos, filehandler fh);

void myfclose(filehandler fh);