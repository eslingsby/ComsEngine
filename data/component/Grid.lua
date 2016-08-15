local Grid = {}

function Grid:load()
	-- Transform required for mesh component
	self.entity:add(Transform)
	
	-- Mesh is wavefront .obj file in the data/mesh folder
	self.entity:add(Mesh, "mesh/grid.obj")
	
	-- Add identifier to name "grid"
	-- Allows this to be retrieved from anywhere by using 'entity = Identifier.getByName("grid")'
	self.entity:add(Identifier, "grid") 
	
	-- Make grid big
	self.entity:get(Transform):scale(Vec3(10, 10, 10))
end

return Grid