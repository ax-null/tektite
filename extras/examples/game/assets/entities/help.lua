require("assets.entities.shared")

--- @class help: entity
local help = entity.new()

help.font = font.new("assets/fonts/normal_font.fnt");

function help:render()
    local x, y = self:get_position()
    graphics.set_color(1.0, 1.0, 1.0, 1.0)
    graphics.draw_string(self.font, "Press Z on the circles to move.\nRed = REPEL.\nGreen = ATTRACT.", x, y);
end
