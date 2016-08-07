local Cube = {}

function Cube:load()
	if (not self.entity:has(Transform)) then
		self.entity:add(Transform)
	end
	
	if (not self.entity:has(Mesh)) then
		self.entity:add(Mesh, "mesh/cube.obj")
	end
	
	if (not self.entity:has(Identifier)) then
		self.entity:add(Identifier, "", "cube_layer")
	end
		
	self.camera = Identifier.getByName("camera"):get(Transform)
	
	self.seconds = 0
	
	-- Destroy self after random time between 0 - 60 seconds
	self.killTime = math.random(0, 600) / 100

	self.transform = self.entity:get(Transform)

	local distance = 512
	
	self.transform:position(Vec3(math.random(-distance, distance), math.random(-distance, distance), -512))
	
	self:reload()
end

function Cube:reload()
	--self.killTime = math.random(0, 60)
	self.speed = math.random(0, 1024)
	
	local spin = 512
	
	self.movement = Vec3(math.random(-spin, spin), math.random(-spin, spin), math.random(-spin, spin))
	
	--self.entity:destroy()

end

function Cube:update()
	self.seconds = self.seconds + 1 * Engine.dt()
	
	--self.transform:lookAt(-self.camera:position(), Vec3(0, 0, 1))
	
	self.transform:localRotate(Quat(self.movement * Engine.dt()))
	self.transform:translate(Vec3(0, 0, self.speed * Engine.dt()))	
	
	-- Destroy self
	if (self.seconds > self.killTime) then
		local cube = Entity.create()
		
		cube:add(Script)
        cube:get(Script):create("Cube")
		
		self.entity:destroy()
	end
end

return Cube