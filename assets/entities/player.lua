require("assets.entities.shared")

--- @class player: entity
local player = entity.new()

player.velocity_x = 0
player.velocity_y = 0

player.is_attracting = 0

player.texture = texture.new("assets/textures/player.png")

function player:check_field_collision()
    local x, y = self:get_position()

    local attract = self:collide_first(TAG.ATTRACT, 0, 0)
    if attract then
        local ex, ey = attract:get_position()
        local ew, eh = attract:get_size()
        self.velocity_x = self.velocity_x + (((ex + (ew / 2)) - x) * 0.05)
        self.velocity_y = self.velocity_y + (((ey + (eh / 2)) - y) * 0.05)
    end

    local repel = self:collide_first(TAG.REPEL, 0, 0)
    if repel then
        local ex, ey = repel:get_position()
        local ew, eh = repel:get_size()
        self.velocity_x = self.velocity_x - (((ex + (ew / 2)) - x) * 0.05)
        self.velocity_y = self.velocity_y - (((ey + (eh / 2)) - y) * 0.05)
    end
end

function player:apply_movement()
    local x, y = self:get_position()

    if self:collide_check(TAG.SOLID, self.velocity_x, 0) then
        while not self:collide_check(TAG.SOLID, Signum(self.velocity_x), 0) do
            x = x + Signum(self.velocity_x)
            self:set_position(x, y)
        end
        self.velocity_x = -self.velocity_x
    end
    x = x + math.floor(self.velocity_x + 0.5)

    if self:collide_check(TAG.SOLID, 0, self.velocity_y) then
        while not self:collide_check(TAG.SOLID, 0, Signum(self.velocity_y)) do
            y = y + Signum(self.velocity_y)
            self:set_position(x, y)
        end
        self.velocity_y = -self.velocity_y
    end
    y = y + math.floor(self.velocity_y + 0.5)

    self:set_position(x, y)
end

function player:update()
    self.is_attracting = input.is_key_down(KEY_Z)

    self.velocity_x = Clamp(self.velocity_x, -5, 5)
    self.velocity_y = Clamp(self.velocity_y, -5, 5)

    if self.is_attracting then
        self:check_field_collision()
    end

    self:apply_movement()

    local x, y = self:get_position()
    local width, height = graphics.get_virtual_size()
    graphics.set_camera_lerp_to(x - (width / 2.0), y - (height / 2.0), 0.1)
end

function player:render()
    local x, y = self:get_position()

    graphics.set_color(1.0, 1.0, 1.0, 1.0)
    graphics.draw_texture(self.texture, x, y)
end
