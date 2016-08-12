local Clock = {}

function Clock:load()
	-- Setting up counters
	self.seconds = 0
	self.clock = 0
	self.frames = 0
end

function Clock:reload()
	-- Called when CTRL + R is pressed, and Lua scripts are reloaded
	print("Reload triggered...\n");
end

function Clock:update()
	-- Updating counters
	self.seconds = self.seconds + 1 * Engine.dt()
	self.clock = self.clock +  Engine.dt()
	self.frames = self.frames + 1
	
	-- Every second print information
	if (self.clock > 1) then
		os.execute("cls")
		
		print("Update Ticks", "- " .. self.frames)
		print("Calc. Ticks", "- " .. 1 / Engine.dt())
		print("Scripts Left", "- " .. Identifier.hasLayer("cube_layer"))
		print("References Left", "- " .. Engine.references() .. "\n")
		
		-- Reset second and frame counter
		self.clock = 0
		self.frames = 0
	end
	
	-- Shut down after 70 seconds
	--if (self.seconds > 70) then
		--print("End of program...\n")
		--Engine.shutdown()
	--end
end

return Clock