local Clock = {}

function Clock:load()
	self.seconds = 0
	self.Clock = 0
	self.frames = 0
end

function Clock:reset()
	print("Reload triggered...\n");
end

function Clock:update()
	-- Update Clocks
	self.seconds = self.seconds + 1 * Engine.dt()
	self.Clock = self.Clock + 1 * Engine.dt()
	self.frames = self.frames + 1
	
	-- Every second print information
	if (self.Clock > 1) then
		os.execute("cls")
		
		print("Total Frames", "- " .. self.frames)
		print("FPS", "", "- " .. 1 / Engine.dt())
		print("Scripts Left", "- " .. Identifier.hasLayer("cube_layer"))
		print("References Left", "- " .. Engine.references() .. "\n")
		
		self.Clock = 0
		self.frames = 0
	end
	
	-- Shut down after 70 seconds
	--if (self.seconds > 70) then
		--print("End of program...\n")
		--self.entity:destroy()
		--Engine.shutdown()
	--end
end

return Clock