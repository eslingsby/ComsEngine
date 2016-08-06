Engine.config("window.title", "OpenGL")
Engine.config("window.x", "1000")
Engine.config("window.y", "512")

Engine.config("shader.vertex", "shader/Vertex.gls")
Engine.config("shader.fragment", "shader/Fragment.gls")

Engine.register("Camera", "component/Camera.lua")
Engine.register("Grid", "component/Grid.lua")
Engine.register("Cube", "component/Cube.lua")
Engine.register("Clock", "component/Clock.lua")