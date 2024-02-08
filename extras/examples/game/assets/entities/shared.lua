--- @enum tag
TAG = {
    PLAYER = 1 << 0,
    SOLID = 1 << 1,
    ATTRACT = 1 << 2,
    REPEL = 1 << 3,
    COLLECTIBLE = 1 << 4
}

--- gets the signum of the number
--- @param value number
---@return integer
function math.signum(value)
    if value > 0 then
        return 1
    elseif value < 0 then
        return -1
    else
        return 0
    end
end

--- clamps value
--- @param value number
--- @param lower number
--- @param upper number
---@return number
function math.clamp(value, lower, upper)
    return math.max(lower, math.min(upper, value))
end

Collectibles = 0
