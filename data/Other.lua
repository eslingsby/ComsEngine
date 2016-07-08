local Other = {}

function Other:load()
	self.seconds = 0
	self.counter = 0
	self.frames = 0
	
	--self.identifier = Identifier(self.entity:id())
	--print("Entity " .. self.entity:id() .. " is called " .. self.identifier.name)
end

function Other:update()
	self.seconds = self.seconds + 1 * Engine.dt()
	self.counter = self.counter + 1 * Engine.dt()
	self.frames = self.frames + 1
	
	if (self.counter > 1) then
		print("Frames " .. self.frames, "", "FPS " .. 1 / Engine.dt())
		print("Scripts Left " .. Identification.hasLayer("test"), "References Left " .. Engine.references() .. "\n")
	
		self.counter = 0
		self.frames = 0
	end
	
	if (self.seconds > 70) then
		self.entity:destroy()
		Engine.shutdown()
	end
end

return Other