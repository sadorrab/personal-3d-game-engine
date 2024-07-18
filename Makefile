testGJKCollisions:
	g++ bin/test/TestGJKCollisions.o bin/main/GJKCollisions.o -o tests/TestGJKCollisions
collisionGUI:
	g++ bin/test/PolygonCollisions.o bin/main/Window.o bin/main/GJKCollisions.o bin/main/ShaderLoader.o  -o tests/CollisionsGUI  -lglfw -lGLEW -lGLU -lGL
testCollisions:
	g++ bin/test/TestCollisions.o bin/main/Collisions.o -o tests/TestCollisions
