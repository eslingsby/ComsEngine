Engine.config("window.title", "OpenGL")
Engine.config("window.x", "1000")
Engine.config("window.y", "512")

Engine.config("shader.vertex", "shader/Vertex.gls")
Engine.config("shader.fragment", "shader/Fragment.gls")

Engine.register("Camera", "Camera.lua")
Engine.register("Grid", "Grid.lua")
Engine.register("Cube", "Cube.lua")