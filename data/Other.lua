local Other = ...

function Other:load()
	self.x = 0
end

function Other:update()	
	print(self.x)

	self.x = self.x + 10
end

return "Other"