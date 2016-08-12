local Cube = {}

function Cube:load()
	-- Transform required for mesh component
	self.entity:add(Transform)

	-- Mesh is wavefront .obj file in the data/mesh folder
	self.entity:add(Mesh, "mesh/cube.obj")

	-- Clock script uses cube_layer to count how many cubes are left
	self.entity:add(Identifier, "", "cube_layer")

	-- Get transform instance
	self.transform = self.entity:get(Transform)

	-- Set transform position to random x y position, on fixed z plane
	local distance = 512
	self.transform:position(Vec3(math.random(-distance, distance), math.random(-distance, distance), -distance))
	
	-- Generate random time to destroy self (0 - 60 seconds)
	self.killTime = math.random(0, 600) / 100
	
	-- Second counter
	self.seconds = 0

	-- Call reload functions to set self.speed and self.movement
	self:reload()
end

function Cube:reload()
	-- Generate random speed
	self.speed = math.random(1, 1024)
	
	-- Generate random rotational movement
	local spin = 256
	self.movement = Vec3(math.random(-spin, spin), math.random(-spin, spin), math.random(-spin, spin))
end

function Cube:update()
	-- Update second counter
	self.seconds = self.seconds + 1 * Engine.dt()
	
	-- Transform entity position
	self.transform:localRotate(Quat(self.movement * Engine.dt()))
	self.transform:translate(Vec3(0, 0, self.speed * Engine.dt()))	
	
	-- If timer is up
	if (self.seconds > self.killTime) then
		-- Create new cube instance on a new entity
		local cube = Entity.create()
		
		cube:add(Script)
		cube:get(Script):create("Cube")
		
		-- Destroy self
		self.entity:destroy()
	end
end

return Cube