gcc main.c -I ./Frameworks/SDL2_ttf.framework/Headers -I./Frameworks/SDL2.framework/Headers -rpath @loader_path/Frameworks -F./Frameworks/ -framework SDL2 -framework SDL2_ttf
