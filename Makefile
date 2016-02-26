all:
	g++ -o main FrameBuffer.cpp Point.cpp Line.cpp Polygon.cpp Window.cpp View.cpp main.cpp

run:
	./main

clean:
	rm main
