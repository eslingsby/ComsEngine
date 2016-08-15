-- This file is called after load, but only once at the start.

io.write("Use this console to input single Lua commands.\n\n>>> ")

-- Camera setup
local camera = Entity.create()
camera:add(Script)
camera:get(Script):create("Camera")
--camera:get(Script):create("Clock")

-- Grid setup
local grid = Entity.create()
grid:add(Script)
grid:get(Script):create("Grid")

-- Example mesh setup
local cube = Entity.create()
cube:add(Transform)
cube:add(Mesh, "mesh/cube.obj")

-- Stress test cubes setup 
--[[
local i = 0
while i < 1024 * 2 do
	local cube = Entity.create()
	
	cube:add(Script)
	cube:get(Script):create("Cube")
	
	i = i + 1
end
]]--