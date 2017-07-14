OBJS =  obj/sam.o obj/reciter.o obj/render.o obj/debug.o

#RECITE_OBJS = obj/reciter.o obj/recite.o
#PARSE1_OBJS = obj/parse1.o obj/sam.o obj/debug.o obj/render.o
#PARSER_OBJS = obj/reciter.o obj/parser.o obj/render.o obj/debug.o obj/sam.o

CC = gcc

CFLAGS =  -Wall -O2
LFLAGS =

# libsdl present
CFLAGS_SAM =  $(CFLAGS) -Wall -O2 -DUSESDL `sdl-config --cflags`
LFLAGS_SAM =  $(LFLAGS) `sdl-config --libs`

# no libsdl present
#CFLAGS_SAM =  $(CFLAGS)
#LFLAGS_SAM =  $(LFLAGS)

.PHONY: clean all dirs

# sam is special, as it uses main.c :/
BINS=bin/recite bin/parse1 bin/parser
all: bin/sam $(BINS)

bin/sam: $(OBJS) obj/main.o
	@mkdir -p $(dir $@)
	$(CC) -o $@ obj/main.o $(OBJS) $(CFLAGS_SAM) $(LFLAGS_SAM)

$(BINS): bin/%: obj/$(notdir %).o $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) -o $@ $(OBJS) $< $(LFLAGS)

obj/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

package:
	tar -cvzf sam.tar.gz README.md Makefile sing src/

clean:
	rm -rf bin obj
