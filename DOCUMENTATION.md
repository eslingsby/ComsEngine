## Entity (core)
	entity Entity.create()
	entity:add(component, ...)
	component entity:get(component)
	int entity:id()
	entity:destroy()
	bool entity:destroyed()
	bool entity:valid()
	
## Engine (core)
	number Engine.dt()
	Engine.shutdown()
	Engine.register(string, string)
	string Engine.config(string)
	Engine.config(string, string)
	Engine.camera(entity)
	entity Engine.camera()
	
## Input (system)
	Input.addInput(string, int)
	bool Input.isDown(string)
	bool Input.wasDown(string)
	Input.lockMouse(bool)
	bool Input.mouseLWasDown()
	bool Input.mouseLIsDown()
	bool Input.mouseRWasDown()
	bool Input.mouseRIsDown()
	vec3 Input.mousePos()
	vec3 Input.mouseRelativePos()
	
## Identification (system)
	entity Identification.getByName(string)
	entity Identification.getByLayer(string, int)
	bool Identification.hasName(string)
	int Identification.hasLayer(string)

## Transform (component)
	Transform(vec3, vec3, vec3)
	number transform.x
	number transform.y
	number transform.z
	vec3 transform:position()
	transform:position(vec3)
	quat transform:rotation()
	transform:rotation(quat)
	transform:translate(vec3)
	transform:localTranslate(vec3)
	transform:rotate(quat)
	transform:localRotate(quat)
	transform:lookAt(vec3, vec3)
	
## Identifier (component)
	Identifier(string, string)
	string identifier.name
	string identifier.layer
	
## Mesh (component)
	Mesh(string)
	
## Script (component)
	Script()
	script:create(string)
	script:remove(string, int)
	script script:get(string, int)
	
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