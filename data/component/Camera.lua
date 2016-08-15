local Camera = {}

function Camera:load()
	io.write("Camera script created.\n")
	io.write("- Press the ESC key to toggle movement controls.\n")
	io.write("- Use WASD and mouse-look to move.\n\n>>> ")

	-- If entity already has transform
	if (not self.entity:has(Transform)) then
		self.entity:add(Transform)
	end
	
	-- If entity already has identifier, then rename it
	if (not self.entity:has(Identifier)) then
		self.entity:add(Identifier, "camera")
	else
		self.entity:get(Identifier):rename("camera", "")
	end
	
	-- Set position to just above the origin, looking forward
	self.transform = self.entity:get(Transform)
	self.transform:position(Vec3(0, -64, 16))
	self.transform:rotation(Quat(Vec3(110, 0, 0)))
	
	-- Set speed of camera movement
	self.speed = 512

	-- Set sensitivity of mouse movements
	self.sensitivity = 0.25

	-- Set if cursor is locked to screen by default
	self.lock = false
	
	-- Adding keyboard inputs
	Input.addInput("forward", 26) 	-- w
	Input.addInput("back", 22) 		-- s
	Input.addInput("left", 4) 		-- a
	Input.addInput("right", 7) 		-- d
	Input.addInput("escape", 41) 	-- esc
	
	-- Lock the mouse
	Input.lockMouse(self.lock)
	
	-- Set self as the camera
	Engine.camera(self.entity)
	
	-- Bool used to check for first update call
	self.start = true
end


function Camera:update()
	--print(self.transform:position())
	--print(self.transform:rotation())

	-- Bool used to check if escape was pressed and lock enabled
	local focused = false

	if (Input.wasDown("escape")) then
		if (self.lock) then
			self.lock = false
		else
			self.lock = true
			focused = true
		end
	
		-- Lock / Unlock mouse
		Input.lockMouse(self.lock)		
	end
	
	-- If locked, ignore rest of code
	if (not self.lock) then
		return
	end
	
	-- Get mouse position and apply sensitivity
	local mouse = Input.mouseRelativePos() * self.sensitivity

	-- If mouse cursor was locked, set mouse to zero to avoid jerking motions
	if (focused or self.start) then
		mouse = Vec3(0, 0, 0)
		
		if (self.start) then
			self.start = false
		end
	end
	
	-- Transform the camera's rotation
	self.transform:localRotate(Quat(Vec3(0, 0, -mouse.x)))
	self.transform:rotate(Quat(Vec3(mouse.y, 0, 0)))
	
	-- Transform the camera's position based on keyboard input
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