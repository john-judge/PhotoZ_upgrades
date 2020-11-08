
CC=g++

DEBFLAGS = -O2

ifdef $(WIN)
CFLAGS += $(DEBFLAGS) -Ic:\edt\pdv 
LDFLAGS += -Lc:\edt\pdv
else
CFLAGS += $(DEBFLAGS) -I/opt/EDTpdv -I.
LDFLAGS += -L/opt/EDTpdv
endif

LDLIBS += -lpdv -lpthread -lm -ldl 

PCIX_OBJS = pcixcfg.o pcix_diagtool.o

all: pcix_diagtool objclean

pcix_diagtool: $(PCIX_OBJS) 
	$(CC) $(CFLAGS) -o $@ $(PCIX_OBJS) $(LDFLAGS) $(LDLIBS) 

%.o : %.cpp
	@echo $(CC) $(CFLAGS) 
	$(CC) $(CFLAGS) -c $< 

objclean: 
	rm -f *.o 
