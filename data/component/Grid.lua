local Grid = {}

function Grid:load()
	-- Transform required for mesh component
	self.entity:add(Transform)
	
	-- Mesh is wavefront .obj file in the data/mesh folder
	self.entity:add(Mesh, "mesh/grid.obj")
	
	-- Add identifier to name "grid"
	self.entity:add(Identifier, "grid")
	
	-- Make grid big
	self.entity:get(Transform):scale(Vec3(10, 10, 10))
	
	-- Creating a large sphere
	self.sphere = Entity.create()
	self.sphere:add(Transform)
	self.sphere:add(Mesh, "mesh/sphere.obj")
	self.sphere:get(Transform):scale(Vec3(1, 1, 1) * 512)
end

function Grid:destroy()
	-- Destroy large sphere entity
	self.sphere:destroy()
end
return Grid