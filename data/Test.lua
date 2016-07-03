local Test = ...

function Test:load()
	--self.addComponent(Transform)
	--self.addComponent(Velocity)
	
	--transform.position = { 100, 100, 100 }
	
	self.test = Script.create(1, "Test", 0)
	
	self.vector = Vec3.create(0, 100, 1000)
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
	
	local test = Vec3.create({ z=100, x=5 })
	
	test + test
	
	test.x + 10
	
	print(test.x)
	
	print(test)
	
end

return "Test"