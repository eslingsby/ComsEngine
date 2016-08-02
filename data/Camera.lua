local Camera = {}

function Camera:load()	
	self.entity:add(Transform)	
	self.entity:add(Identifier, "camera")
	
	self.transform = self.entity:get(Transform)
	
	self.transform:position(Vec3(0, 0, 128))
	self.transform:rotation(Quat(Vec3(90, 0, 0)))
	
	self.speed = 512
	
	self.sensitivity = 0.25
	
	Input.addInput("forward", 26);
	Input.addInput("back", 22);
	Input.addInput("left", 4);
	Input.addInput("right", 7);
	Input.addInput("escape", 41);
	
	self.lock = true
	
	Input.lockMouse(self.lock)
end

function Camera:reset()
	self.speed = 512
end

function Camera:update()
	local focused = false

	if (Input.wasDown("escape")) then
		if (self.lock) then
			self.lock = false
		else
			self.lock = true
			focused = true
		end
	
		Input.lockMouse(self.lock)		
	end
	
	if (not self.lock) then
		return
	end
	
	local mouse = Input.mouseRelativePos() * self.sensitivity

	if (focused) then
		mouse = Vec3(0, 0, 0)
	end
	
	self.transform:localRotate(Quat(Vec3(0, 0, -mouse.x)))
	self.transform:rotate(Quat(Vec3(mouse.y, 0, 0)))
	
	if (Input.isDown("forward")) then
		self.transform:localTranslate(Vec3(0, 0, self.speed * Engine.dt()))
	end
	
	if (Input.isDown("back")) then
		self.transform:localTranslate(Vec3(0, 0, -self.speed * Engine.dt()))
	end
	
	if (Input.isDown("left")) then
		self.transform:localTranslate(Vec3(self.speed * Engine.dt(), 0, 0))
	end
	
	if (Input.isDown("right")) then
		self.transform:localTranslate(Vec3(-self.speed * Engine.dt(), 0, 0))
	end
end

return Camera