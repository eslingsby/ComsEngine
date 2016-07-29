local Many = {}

function Many:load()	
	self.seconds = 0
	
	-- Destroy self after random time between 0 - 60 seconds
	self.killTime = math.random(0, 60)
	
	self.camera = Identification.getByName("camera")
	
	self.cameraId = self.camera:get(Identifier)
	
	--print(self.cameraId.name)
	
end

function Many:reset()
	--self.killTime = math.random(0, 60)
end

function Many:update()
	self.seconds = self.seconds + 1 * Engine.dt()
	

	--print(self.camera:id())
	--print(self.cameraId.name)
	
	-- Destroy self
	if (self.seconds > self.killTime) then
		--print("Goodbye! ID - " .. self.entity:id())
		self.entity:destroy()
	end
end

return Many