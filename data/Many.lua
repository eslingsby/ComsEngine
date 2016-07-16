local Many = {}

function Many:load()	
	self.seconds = 0
	
	-- Destroy self after random time between 0 - 60 seconds
	self.killTime = math.random(0, 60)
end

function Many:update()
	self.seconds = self.seconds + 1 * Engine.dt()
	
	-- Destroy self
	if (self.seconds > self.killTime) then
		--print("Goodbye! ID - " .. self.entity:id())
		self.entity:destroy()
	end
end

return Many