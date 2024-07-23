testGJKCollisions:
	g++ bin/test/TestGJKCollisions.o bin/main/GJKCollisions.o -o tests/TestGJKCollisions
collisionGUI:
	g++ bin/test/PolygonCollisions.o bin/main/Window.o bin/main/GJKCollisions.o bin/main/ShaderLoader.o  -o tests/CollisionsGUI  -lglfw -lGLEW -lGLU -lGL
testCollisions:
	g++ bin/test/TestCollisions.o bin/main/Collisions.o -o tests/TestCollisions
collisions3d:
	g++ bin/test/Collisions3D.o bin/main/Window.o bin/main/Collisions.o bin/main/ShaderLoader.o bin/main/WavefrontLoader.o -o tests/Collisions3D -lglfw -lGLEW -lGLU -lGL
