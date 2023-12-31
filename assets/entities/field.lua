require("assets.entities.shared")

--- @class field: entity
local field = entity.new()

field.timer = 0

function field:update()
    self.timer = self.timer + 0.1
    self.timer = self.timer % 1000
end

function field:render()
    local x, y = self:get_position()
    local width, _ = self:get_size()
    local half = width / 2

    if self:get_tag() == TAG.ATTRACT then
        graphics.set_color(0.0, 1.0, 0.0, 1.0)
    else
        graphics.set_color(1.0, 0.0, 0.0, 1.0)
    end

    graphics.draw_circle(x + half, y + half, half + math.sin(self.timer), false)
    graphics.draw_circle(x + half, y + half, 8, true)
end
