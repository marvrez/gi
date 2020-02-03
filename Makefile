DEBUG  ?= 0
EMBREE ?= 0

OBJ= main.o utils.o image.o sphere.o hit.o camera.o bbox.o kdtree.o scene.o texture.o plane.o renderer.o material.o sampler.o onb.o microfacet_distribution.o loading_bar.o triangle.o mesh.o import.o mat4.o cube.o export.o
EXECOBJA= 

VPATH=./src/
EXEC=gi
OBJDIR=./obj/

CPP=g++
OPTS=-Ofast
LDFLAGS= -lm -pthread
COMMON= -Iinclude/ -Isrc/
CFLAGS=-Wall -Wno-unknown-pragmas -Wfatal-errors -fPIC -std=c++14

ifeq ($(DEBUG), 1)
OPTS=-O0 -g
endif

ifeq ($(EMBREE), 1)
COMMON+= -DEMBREE
CFLAGS+= -DEMBREE
LDFLAGS+=-lembree3
endif

CFLAGS+=$(OPTS)

EXECOBJS = $(addprefix $(OBJDIR), $(EXECOBJA))
OBJS   = $(addprefix $(OBJDIR), $(OBJ))
DEPS   = $(wildcard include/*.h) Makefile

all: obj $(EXEC)

$(EXEC): $(OBJS) $(EXECOBJS)
	$(CPP) $(COMMON) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJDIR)%.o: %.cpp $(DEPS)
	$(CPP) $(COMMON) $(CFLAGS) -c $< -o $@

obj:
	mkdir -p obj

.PHONY: clean
clean:
	rm -rf $(OBJS) $(ALIB) $(EXEC) $(EXECOBJS) $(OBJDIR)/* $(OBJDIR)
