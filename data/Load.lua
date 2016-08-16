-- This file is called on start, and every time a reload is triggered with CTRL + R

-- Setting the start script
Engine.config("script.start", "Start.lua")

-- Registering script files to names
Script.register("Camera", "component/Camera.lua")
Script.register("Grid", "component/Grid.lua")
Script.register("Cube", "component/Cube.lua")
Script.register("Clock", "component/Clock.lua")
Script.register("Empty", "component/Empty.lua")

-- Setting renderer settings
Engine.config("window.title", "Press ESC to control the camera")
Engine.config("window.x", "1000")
Engine.config("window.y", "512")

-- Setting vertex fragment shader locations
Engine.config("shader.vertex", "shader/Vertex.gls")
Engine.config("shader.fragment", "shader/Fragment.gls")

-- If this is the first call, then don't print reload message
if (start == nil) then
	start = true
end

if (start) then
	start = false
else
	io.write("\n\nReload triggered...\n\n>>> ")
end