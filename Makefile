NAME=liliOpenGL
CXX=g++
CPPFLAGS=-std=c++11 -g
LDLIBS=-lglfw -lGLEW -lGL -lSOIL
ODIR=objs
TOOLS=Shader Sprite Object Camera_2D
OBJS=$(TOOLS:%=$(ODIR)/%.o)
SHADERS=spriteVert.glsl spriteFrag.glsl

default: $(NAME)

$(NAME): main.cpp $(OBJS) $(SHADERS)
	$(CXX) $(CPPFLAGS) -o $(NAME) $(LDLIBS) $(OBJS) main.cpp


$(ODIR)/%.o: %.cpp %.hpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

clean: 
	rm $(ODIR)/*
	rm $(NAME)
