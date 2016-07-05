local Test = ...

function Test:load()
	--self.addComponent(Transform)
	--self.addComponent(Velocity)
	
	--transform.position = { 100, 100, 100 }
	
	--local test = Script.create(1, "Test", 0)
	
	--local entity = entitymanger.findcomponent("name");
	
	--local entity = Engine.Naming:findEntity("camera")
	
	--local entity = Entity(1)
	
	--entity:addComponent(Test)
	
	--entity:destroy()
	
	self.test = Vec3(1, 1, 1)


	--self.vector = Vec3.create(0, 100, 1000)
	
end

function Test:update()
	--self.position = self.transform.position
	
	--self.velocity.impulse({ 0, 0, 0 }, { 0, -10, 0 })
	
	--local temp = engine.createEntity()
	--temp.addComponent(Transform)
	
	--temp.transform.position = self.position
	--temp.transform.position.x -= 100 * Engine.dt
	
	--temp.destroy()
	
	--local entity = engine.createEntity()
	--entity.addComponent(Test)
	
	--entity.transform.position = self.position
	
	--self.destroy()
	
	--print(test.id)
	
	-- local test = Vec3(1, 1, 1)

	
	--test.x = 1
	

	print(self.test)
end

return "Test"