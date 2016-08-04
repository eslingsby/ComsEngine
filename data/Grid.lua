local Grid = {}

function Grid:load()
	self.entity:add(Transform)
	self.entity:add(Mesh, "mesh/grid.obj")
	self.entity:add(Identifier, "grid")

	self.entity:get(Transform):scale(Vec3(10, 10, 10))
end

return Grid