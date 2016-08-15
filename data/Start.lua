-- Camera setup
local camera = Entity.create()
camera:add(Script)
camera:get(Script):create("Camera")
--camera:get(Script):create("Clock")

-- Grid setup
local grid = Entity.create()
grid:add(Script)
grid:get(Script):create("Grid")

-- Example cube
local cube = Entity.create()
cube:add(Transform)
cube:add(Mesh, "mesh/cube.obj")

-- Cubes setup 
--[[
local i = 0
while i < 1024 * 2 do
	local cube = Entity.create()
	
	cube:add(Script)
	cube:get(Script):create("Cube")
	
	i = i + 1
end
]]--