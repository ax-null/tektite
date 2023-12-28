local player = entity.create()

function player:init()
	self.texture = resources.loadTexture("textures/player.png")
	self.walking = graphics.createAnimation(texture, 16, 13);
	self.x = 0
	self.y = 0
end

function player:update()
	if input.isPressed(KEY_W) then
		self.y = self.y + 1
	end
	if input.isPressed(KEY_S) then
		self.y = self.y - 1
	end
	if input.isPressed(KEY_D) then
		self.x = self.x + 1
	end
	if input.isPressed(KEY_A) then
		self.x = self.x - 1
	end
end

function player:render()
	renderer.draw(self.texture, self.x, self.y, COLOR_WHITE)
end