--- @class player: entity
local player = entity.new()

player.texture = texture.new("assets/textures/player.png")
player.font = font.new("assets/fonts/normal_font.fnt")

function player:update()
    local x, y = self:get_position()

    if input.is_key_pressed(KEY_SPACE) then
        audio.play_sound("assets/sounds/hit_hurt.wav")
    end

    if input.is_key_down(KEY_W) then y = y + 10 end
    if input.is_key_down(KEY_S) then y = y - 10 end
    if input.is_key_down(KEY_A) then x = x - 10 end
    if input.is_key_down(KEY_D) then x = x + 10 end

    x = x % 800
    y = y % 600

    self:set_position(x, y)
end

function player:render()
    local x, y = self:get_position()
    local _, height = self:get_size()
    local mouse_x, mouse_y = input.get_mouse_position()

    graphics.draw_texture(self.texture, x, y)

    graphics.set_color(1.0, 1.0, 0.0, 1.0)
    graphics.draw_circle(mouse_x, 600 - mouse_y, 16, false)

    graphics.set_color(1.0, 1.0, 1.0, 1.0)
    graphics.draw_string(self.font, mouse_x..", "..mouse_y, mouse_x, 620 - mouse_y);
end
