local utils = require("libraries.utils")
local Behavior = require("libraries.behaviors.behavior")

local RandomWander = setmetatable({}, {__index = Behavior})
RandomWander.__index = RandomWander

function RandomWander.new(max_velocity, sensors, num_ticks)
    local self = setmetatable(Behavior.new("RandomWander", max_velocity, sensors), RandomWander)

    self.num_ticks = num_ticks
    self.current_tick = num_ticks

    self.angle = 0

    return self
end

function RandomWander:action()
    local max_value = 1

    if self.current_tick >= self.num_ticks then
        self.angle = math.random() * 2 * math.pi
        self.current_tick = 0
    end
    self.current_tick = self.current_tick + 1

    b_vec = {length = max_value, angle = self.angle}

    log("[RandomWander] intensity: " .. b_vec.length .. " | angle: " .. b_vec.angle)

    return b_vec
end

return RandomWander
