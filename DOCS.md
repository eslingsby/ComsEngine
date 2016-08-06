## Entity (core)
	entity Entity.create()
	entity:add(component, ... args)
	component entity:get(component)
	int entity:id()
	entity:destroy()
	bool entity:destroyed()
	bool entity:valid()
	
## Engine (core)
	number Engine.dt()
	Engine.shutdown()
	Engine.register(string type, string file)
	string Engine.config(string key)
	Engine.config(string key, string value)
	Engine.camera(entity)
	entity Engine.camera()
	
## Input (system)
	Input.addInput(string name, int scancode)
	bool Input.isDown(string name)
	bool Input.wasDown(string name)
	Input.lockMouse(bool locked)
	bool Input.mouseLWasDown()
	bool Input.mouseLIsDown()
	bool Input.mouseRWasDown()
	bool Input.mouseRIsDown()
	vec3 Input.mousePos()
	vec3 Input.mouseRelativePos()
	
## Identification (system)
	entity Identification.getByName(string name)
	entity Identification.getByLayer(string layer, int number)
	bool Identification.hasName(string name)
	int Identification.hasLayer(string layer)

## Transform (component)
	Transform(vec3 position, quat rotation, vec3 scale)
	number transform.x
	number transform.y
	number transform.z
	transform.x = number 
	transform.y = number 
	transform.z = number 
	vec3 transform:position()
	transform:position(vec3 position)
	quat transform:rotation()
	transform:rotation(quat rotation)
	transform:translate(vec3 translation)
	transform:localTranslate(vec3 translation)
	transform:rotate(quat rotation)
	transform:localRotate(quat rotation)
	transform:lookAt(vec3 target, vec3 up)
	
## Identifier (component)
	Identifier(string name, string layer)
	string identifier.name
	string identifier.layer
	
## Mesh (component)
	Mesh(string file)
	
## Script (component)
	Script()
	script:create(string type)
	script:remove(string type, int number)
	script script:get(string type, int number)
	
## Quat (primitive)
	Quat(quat)
	Quat(quat)
	quat.x
	quat.y
	quat.z
	quat.w
	quat:eulerAngles()
	-quat
	quat + quat
	quat - quat
	quat * quat
	quat * vec3
	quat == quat
	
## Vec2 (primitive)
	Vec2(number, number)
	Vec2(Vec2)
	vec2.x
	vec2.y
	-vec2
	vec2 + vec2
	vec2 - vec2
	vec2 * vec2
	vec2 * number
	vec2 / vec2
	vec2 / number
	vec2 == vec2
	
## Vec3 (primitive)
	Vec3(number, number, number)
	Vec3(Vec3)
	vec3.x
	vec3.y
	vec3.z
	-vec3
	vec3 + vec3
	vec3 - vec3
	vec3 * vec3
	vec3 * quat
	vec3 * number
	vec3 / vec3
	vec3 / number
	vec3 == vec3
	
## Vec4 (primitive)
	Vec4(number, number, number, number)
	Vec4(Vec4)
	vec4.x
	vec4.y
	vec4.z
	vec4.w
	-vec4
	vec4 + vec4
	vec4 - vec4
	vec4 * vec4
	vec4 * quat
	vec4 * number
	vec4 / vec4
	vec4 / number
	vec4 == vec4