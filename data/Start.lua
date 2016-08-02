local camera = Entity.create()
camera:add(Script)
camera:get(Script):create("Camera")

Engine.cameraEntity(camera)

local grid = Entity.create()

grid:add(Script)
grid:get(Script):create("Grid")

local i = 0
while i < 512 do
	local cube = Entity.create()
	
	cube:add(Script)
	cube:get(Script):create("Cube")
	
	i = i + 1
end