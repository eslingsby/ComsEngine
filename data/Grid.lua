local Grid = {}

function Grid:load()
	self.entity:add(Transform)
	self.entity:add(Mesh, "mesh/grid.obj")
	self.entity:add(Identifier, "grid")

	self.entity:get(Transform):scale(Vec3(10, 10, 10))
	
	-- Counters
	self.seconds = 0
	self.counter = 0
	self.frames = 0
end

function Grid:reset()
	--local i = 0
	--while i < 256 do
	--	local cube = Entity.create()
	--	
	--	cube:add(Script)
	--	cube:get(Script):create("Many")
	--	
	--	i = i + 1
	--end
	--
	--print("Reload triggered...\n")
end

function Grid:update()
	-- Update counters
	self.seconds = self.seconds + 1 * Engine.dt()
	self.counter = self.counter + 1 * Engine.dt()
	self.frames = self.frames + 1
	
	-- Every second print information
	if (self.counter > 1) then
		--os.execute("cls")
		
		print("Total Frames", "- " .. self.frames)
		print("FPS", "", "- " .. 1 / Engine.dt())
		print("Scripts Left", "- " .. Identification.hasLayer("cube_layer"))
		print("References Left", "- " .. Engine.references())
		print("")
		
		self.counter = 0
		self.frames = 0
	end
	
	-- Shut down after 70 seconds
	--if (self.seconds > 70) then
		--print("End of program...\n")
		--self.entity:destroy()
		--Engine.shutdown()
	--end
end

return Grid