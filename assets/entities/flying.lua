--- @class flying: entity
local flying = entity.new()

function flying:render()
    local x, y = self:get_position()
    local width = self:get_size()

    graphics.set_color(0.0, 0.0, 1.0, 1.0)
    graphics.draw_circle(x, y, width, true)
end
