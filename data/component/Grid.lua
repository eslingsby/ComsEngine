local Grid = {}

function Grid:load()
	if (not self.entity:has(Transform)) then
		self.entity:add(Transform)
	end
	
	if (not self.entity:has(Mesh)) then
		self.entity:add(Mesh, "mesh/grid.obj")
	end			
	
	if (not self.entity:has(Transform)) then
		self.entity:add(Identifier, "grid")
	end

	self.entity:get(Transform):scale(Vec3(10, 10, 10))
end

return Grid