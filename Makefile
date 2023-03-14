OBJ=source/main.cxx source/shader_manager.cxx source/dvd_simulation.cxx vendor/glad.c

linux:
	g++ $(OBJ) -lglfw3

apple:
	clang++ -std=c++20 -stdlib=libc++ -lglfw -framework CoreVideo -framework OpenGL -framework IOKit -framework Cocoa -framework Carbon $(OBJ)