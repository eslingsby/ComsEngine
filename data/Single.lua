local Single = {}

function Single:load()
	-- Counters
	self.seconds = 0
	self.counter = 0
	self.frames = 0

	-- Add identifier component with name
	self.entity:add(Identifier, "waywo")
	self.identifier = self.entity:get(Identifier)
	
	print("Entity " .. self.entity:id() .. " is called " .. self.identifier.name)
end

function Single:reload()
	print("Reload triggered...")
end

function Single:update()
	-- Update counters
	self.seconds = self.seconds + 1 * Engine.dt()
	self.counter = self.counter + 1 * Engine.dt()
	self.frames = self.frames + 1
	
	-- Every second print information
	if (self.counter > 1) then
		os.execute("cls")
	
		print("Total Frames", "- " .. self.frames)
		print("FPS", "", "- " .. 1 / Engine.dt())
		print("Scripts Left", "- " .. Identification.hasLayer("many_layer"))
		print("References Left", "- " .. Engine.references())
		
		self.counter = 0
		self.frames = 0
	end
	
	-- Shut down after 70 seconds
	if (self.seconds > 70) then
		self.entity:destroy()
		Engine.shutdown()
	end
end

return Single