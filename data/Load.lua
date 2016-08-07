Engine.config("window.title", "OpenGL")
Engine.config("window.x", "1000")
Engine.config("window.y", "512")

Engine.config("shader.vertex", "shader/Vertex.gls")
Engine.config("shader.fragment", "shader/Fragment.gls")

Script.register("Camera", "component/Camera.lua")
Script.register("Grid", "component/Grid.lua")
Script.register("Cube", "component/Cube.lua")
Script.register("Clock", "component/Clock.lua")