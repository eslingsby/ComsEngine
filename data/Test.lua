local Test = {}

function Test:load()
	local camera = Identification.getByName("camera")
	
	self.seconds = 0
	self.killTime = math.random(0, 60)
end

function Test:update()
	self.seconds = self.seconds + 1 * Engine.dt()
	
	if (self.seconds > self.killTime) then
		--print("Goodbye cruel world! ID - " .. self.entity:id())
		self.entity:destroy()
	end
end

return Test