local out = require("assets.scripts.test")

local x = 0
local y = 0

local x2 = 0
local y2 = 0

local icon = texture.new("assets/textures/icon32.png")
local logo = texture.new("assets/textures/logo.png")

local fnt = font.new("assets/fonts/consolas.fnt")

local function update()
    if input.is_key_down(KEY_UP) then y = y + 10 end
    if input.is_key_down(KEY_DOWN) then y = y - 10 end
    if input.is_key_down(KEY_LEFT) then x = x - 10 end
    if input.is_key_down(KEY_RIGHT) then x = x + 10 end

    if input.is_key_pressed(KEY_SPACE) then
        x = 0
        y = 0
        audio.play_sound("assets/sounds/hit_hurt.wav")
    end

    if input.is_mouse_button_down(MOUSE_BUTTON_LEFT) then
        local mouse_x, mouse_y = input.get_mouse_position()
        x = mouse_x
        y = 600 - mouse_y
    end

    if input.is_mouse_button_down(MOUSE_BUTTON_RIGHT) then
        local mouse_x, mouse_y = input.get_mouse_position()
        x2 = mouse_x
        y2 = 600 - mouse_y
    end

    x = x % 800
    y = y % 600
end


local function render()
    local mouse_x, mouse_y = input.get_mouse_position()

    graphics.clear(0.25, 0.25, 0.25, 1.0)

    graphics.set_color(0.5, 0.5, 0.8, 1.0)
    graphics.draw_rect(x, y, 32, 32, true)

    graphics.set_color(1.0, 0.0, 0.0, 1.0)
    graphics.draw_circle(mouse_x, 600 - mouse_y, 16, true)

    graphics.set_color(0.0, 1.0, 0.0, 1.0)
    graphics.draw_triangle(mouse_x, 600 - mouse_y, x, y, x2, y2, false)

    graphics.set_color(1.0, 1.0, 0.0, 1.0)
    graphics.draw_rect(x2, y2, 32, 32, true)

    graphics.set_color(1.0, 1.0, 1.0, 1.0)
    graphics.draw_texture(logo, mouse_x, 600 - mouse_y)
    graphics.draw_texture(icon, x2, y2)

    graphics.draw_string(fnt, "Mouse: " .. mouse_x .. ", " .. mouse_y, 10, 10);

    out.SaySomething(fnt)

    graphics.submit()
end

function Loop()
    update()
    render()
end
