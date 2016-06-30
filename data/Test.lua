local Test = ...

function Test:load()
	--self.addComponent(Transform)
	--self.addComponent(Velocity)
	
	--transform.position = { 100, 100, 100 }
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
end

return "Test"