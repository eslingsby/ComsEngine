local Single = {}

function Single:load()
	self.entity:add(Transform)
	self.entity:add(Mesh, "grid.obj")
	self.entity:add(Identifier, "grid")

	self.entity:get(Transform):scale(Vec3(10, 10, 10))
	
	-- Counters
	self.seconds = 0
	self.counter = 0
	self.frames = 0
end

function Single:reset()
	--os.execute("cls")

	print("Reload triggered...\n")
end

function Single:update()
	-- Update counters
	self.seconds = self.seconds + 1 * Engine.dt()
	self.counter = self.counter + 1 * Engine.dt()
	self.frames = self.frames + 1

	
	-- Every second print information
	if (self.counter > 1) then
		--os.execute("cls")
		
		print("Total Frames", "- " .. self.frames)
		print("FPS", "", "- " .. 1 / Engine.dt())
		print("Scripts Left", "- " .. Identification.hasLayer("many_layer"))
		print("References Left", "- " .. Engine.references())
		print("")
		
		self.counter = 0
		self.frames = 0
	end
	
	-- Shut down after 70 seconds
	if (self.seconds > 70) then
		--print("End of program...\n")
		--self.entity:destroy()
		--Engine.shutdown()
	end
end

return Single