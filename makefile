build : #lib/glfw3.lib
	gcc -obuild -g3 -Iinc -lGL -lm -lglfw -lGLEW -D "LINUX" src/main.c src/display.c src/util/memory.c src/util/filehandling.c src/util/errorhandling.c 

glew :
	mkdir glew
	git clone https://github.com/nigels-com/glew glew
	make -Cglew 
	



