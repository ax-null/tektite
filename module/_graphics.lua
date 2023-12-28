--- @meta graphics

graphics = {}

--- clears the screen
---@param red number
---@param green number
---@param blue number
---@param alpha number
function graphics.clear(red, green, blue, alpha) end

--- submits the draw commands
function graphics.submit() end

--- sets the drawing color
---@param red number
---@param green number
---@param blue number
---@param alpha number
function graphics.set_color(red, green, blue, alpha) end

--- draws a point
--- @param x number
--- @param y number
function graphics.draw_point(x, y, width, height, fill) end

--- draws a line
--- @param x1 number
--- @param y1 number
--- @param x2 number
--- @param y2 number
function graphics.draw_line(x1, y1, x2, y2) end

--- draws a line
--- @param x1 number
--- @param y1 number
--- @param x2 number
--- @param y2 number
--- @param x3 number
--- @param y3 number
--- @param fill boolean
function graphics.draw_triangle(x1, y1, x2, y2, x3, y3, fill) end

--- draws a rectangle
--- @param x number
--- @param y number
--- @param width number
--- @param height number
--- @param fill boolean
function graphics.draw_rect(x, y, width, height, fill) end

--- draws a circle
--- @param x number
--- @param y number
--- @param radius number
--- @param fill boolean
function graphics.draw_circle(x, y, radius, fill) end

--- draws a texture
--- @param x number
--- @param y number
function graphics.draw_texture(texture, x, y) end

--- draws a string
--- @param font userdata
--- @param text string
--- @param x number
--- @param y number
function graphics.draw_string(font, text, x, y) end

texture = {}

--- creates a new texture
--- @param path string
--- @return userdata
function texture.new(path) end

font = {}

--- creates a new font
--- @param path string
--- @return userdata
function font.new(path) end
