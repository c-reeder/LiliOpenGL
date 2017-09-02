NAME=liliOpenGL
CXX=g++
CPPFLAGS=-std=c++11 -g
LDLIBS=-lglfw -lGLEW -lGL -lSOIL
ODIR=objs
TOOLS=Shader
OBJS=$(TOOLS:%=$(ODIR)/%.o)
SHADERS=spriteVert.glsl spriteFrag.glsl

default: $(NAME)

$(NAME): main.cpp $(OBJS) $(SHADERS)
	$(CXX) $(CPPFLAGS) -o $(NAME) $(LDLIBS) $(OBJS) main.cpp
	./liliOpenGL

$(ODIR)/Shader.o: Shader.cpp Shader.hpp
	$(CXX) $(CPPFLAGS) -c Shader.cpp -o $@

clean: 
	rm $(ODIR)/*
	rm $(NAME)
