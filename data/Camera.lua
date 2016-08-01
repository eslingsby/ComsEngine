local Camera = {}

function Camera:load()	
	self.entity:add(Transform)	
	self.entity:add(Identifier, "camera")
	
	self.transform = self.entity:get(Transform)
	
	self.speed = 1
	
	Input.addInput("forward", 26);
	Input.addInput("back", 22);
	Input.addInput("left", 4);
	Input.addInput("right", 7);
	Input.addInput("quit", 41);
	
	Input.lockMouse(true);
end

function Camera:update()
	if (Input.wasDown("quit")) then
		Engine.shutdown()
		return
	end
	
	local mouse = Input.mouseRelativePos() * 0.25

	self.transform:rotate(Quat(Vec3(0, mouse.x, 0)))
	self.transform:localRotate(Quat(Vec3(mouse.y, 0, 0)))
	
	if (Input.isDown("forward")) then
		self.transform:localTranslate(Vec3(0, 0, self.speed * Engine.dt()))
	end
	
	if (Input.isDown("back")) then
		self.transform:localTranslate(Vec3(0, 0, -self.speed * Engine.dt()))
	end
	
	if (Input.isDown("left")) then
		self.transform:localTranslate(Vec3(-self.speed * Engine.dt(), 0, 0))
	end
	
	if (Input.isDown("right")) then
		self.transform:localTranslate(Vec3(self.speed * Engine.dt(), 0, 0))
	end
end

return Camera