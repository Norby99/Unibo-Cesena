local utils = require("libraries.utils")
local vector = require("libraries.vector")
local Behavior = require("libraries.behaviors.behavior")

local Follow = setmetatable({}, {__index = Behavior})
Follow.__index = Follow

-- This behavior makes the robot follow the light.
function Follow.new(max_velocity, sensors, max_perceived)
    local self = setmetatable(Behavior.new("follow", max_velocity, sensors), Follow)

    self.max_perceived = max_perceived
    return self
end

--[[ 
    TODO: attrattiva
    ]]
function Follow:action()
    local norm_sensors = utils.normalize_sensors(self.sensors, self.max_perceived)
    local max_sensor = utils.max_sensor_value(norm_sensors)

    -- TODO: sarebbe figo fare che la lenght è 1 - il value e un valore minimo, se no scende a 0
    b_vec = {length = max_sensor.value, angle = max_sensor.angle}

    log("[follow] intensity: " .. 1-b_vec.length .. " | angle: " .. b_vec.angle)

    return b_vec
end

return Follow
