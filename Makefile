EXECUTABLE = app
CC = gcc
CFLAGS = -Wextra -Werror -std=c11 -ISDL2 -D_REENTRANT -DSDL_MAIN_HANDLED
LIBS  = -lm -lSDL2

SOURCE = main.c triangle.c drawline.c
HEADER = drawline.h triangle.h teapot_data.h

include Makefile.in

.PHONY: all
all: $(EXECUTABLE)
$(EXECUTABLE): $(SOURCE) $(HEADER)
	$(info === Compiling...)
	$(shell $(PRE_BUILD))
	$(CC) $(CFLAGS) -o $@ $(SOURCE) $(LIBS)

.PHONY: clean
clean:
	@rm -f *.o app.exe app *~ SDL2.dll libSDL2-2.0.so.0
	$(info === Cleaned)
