CC = gcc
LDFLAGS =-std=c99 -Wall -Wextra -Werror -Wshadow -Wstrict-prototypes -Wold-style-definition -pedantic -O2 -Wno-unused-variable -Wno-sign-compare -Wno-unused-variable -Wno-unused-result -Wno-unused-parameter
CFLAGS =-c -std=c99 -Wall -Wextra -Werror -Wshadow -Wstrict-prototypes -Wold-style-definition -pedantic -O2 -Wno-unused-variable -Wno-sign-compare -Wno-unused-variable -Wno-unused-result -Wno-unused-parameter
SOURCES = main.o stack.o
LNAME = dovzhenko
LIB = lib$(LNAME)
LIBSDIR = libs
EXECUTABLE = start
override USE_LIB = test

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES) $(addprefix $(LIBSDIR)/, $(LIB).a)
	$(CC) $(LDLAGS) $(SOURCES) -L./$(LIBSDIR) -l$(USE_LIB) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@ 

$(addprefix $(LIBSDIR)/, $(LIB).a): $(SOURCES)
	ar crs -D $(addprefix $(LIBSDIR)/, $(LIB).a) $(SOURCES) $@

clean:
	-rm -f *.o $(EXECUTABLE) $(addprefix $(LIBSDIR)/, $(LIB).a)