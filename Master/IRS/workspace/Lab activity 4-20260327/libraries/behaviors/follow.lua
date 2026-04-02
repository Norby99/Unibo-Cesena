local utils = require("libraries.utils")
local vector = require("libraries.vector")
local Behavior = require("libraries.behaviors.behavior")

local Follow = setmetatable({}, {__index = Behavior})
Follow.__index = Follow

-- This behavior makes the robot follow the light.
function Follow.new(max_velocity, sensors, max_perceived, halt_behaviors)
    halt_behaviors = halt_behaviors or {}
    local self = setmetatable(Behavior.new("follow", max_velocity, sensors, halt_behaviors), Follow)

    self.max_perceived = max_perceived
    return self
end

function Follow:action()
    if self:should_halt() then
        return nil
    end

    local norm_sensors = utils.normalize_sensors(self.sensors, self.max_perceived)
    local max_sensor = utils.max_sensor_value(norm_sensors)

    b_vec = {length = 1, angle = max_sensor.angle}

    log("[follow] intensity: " .. b_vec.length .. " | angle: " .. b_vec.angle)

    return b_vec
end

return Follow
