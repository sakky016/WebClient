obj_files=main.o WebClient.o HTTPResponse.o
src_files=main.cpp WebClient.cpp HTTPResponse.cpp

webclient: $(obj_files)
	g++ $(obj_files) -lcurl -o webclient

%.o: %.cpp
	echo Compiling $<
	g++ -c $< -o $@

clean: 
	rm -rf webclient
	rm -rf *.o 
