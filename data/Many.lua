local Many = {}

function Many:load()
	self.entity:add(Transform)
	self.entity:add(Mesh, "cube.obj")
	self.entity:add(Identifier, "", "many_layer")

	self.seconds = 0
	
	-- Destroy self after random time between 0 - 60 seconds
	self.killTime = math.random(0, 60)

	self.transform = self.entity:get(Transform)

	local distance = 512
	
	self.transform:position(Vec3(math.random(-distance, distance), math.random(-distance, distance), math.random(-distance, distance)))
	
	--print(self.cameraId.name)
	
	local spin = 512
	
	self.movement = Vec3(math.random(-spin, spin), math.random(-spin, spin), math.random(-spin, spin))
end

function Many:reset()
	self.killTime = math.random(0, 60)
end

function Many:update()
	self.seconds = self.seconds + 1 * Engine.dt()

	
	self.transform:localRotate(Quat(self.movement * Engine.dt()))
	
	--print(self.camera:id())
	--print(self.cameraId.name)
	
	-- Destroy self
	if (self.seconds > self.killTime) then
		--print("Goodbye! ID - " .. self.entity:id())
		local cube = Entity.create()
		
		cube:add(Script)
        cube:get(Script):create("Many")
		
		self.entity:destroy()
	end
end

return Many