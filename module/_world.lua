--- @meta world

world = {}

--- does nothing
function world._() end

--- @class entity
entity = {}

--- creates a new entity
--- @return entity
function entity.new() end

--- update this entity
--- @param self entity
function entity.update(self) end

--- render this entity
--- @param self entity
function entity.render(self) end

--- set this entity position
--- @param self entity
--- @param x number
--- @param y number
function entity.set_position(self, x, y) end

--- set this entity size
--- @param self entity
--- @param width number
--- @param height number
function entity.set_size(self, width, height) end

--- get this entity tag
--- @param self entity
--- @return number
function entity.get_tag(self) end

--- get this entity name
--- @param self entity
--- @return string
function entity.get_name(self) end

--- get this entity position
--- @param self entity
--- @return number
--- @return number
function entity.get_position(self) end

--- get this entity size
--- @param self entity
--- @return number
--- @return number
function entity.get_size(self) end
