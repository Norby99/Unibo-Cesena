local utils = require("libraries.utils")
local vector = require("libraries.vector")
local Behavior = require("libraries.behaviors.behavior")

local Follow = setmetatable({}, {__index = Behavior})
Follow.__index = Follow

-- This behavior makes the robot follow the light.
function Follow.new(max_velocity, sensors, max_perceived, factor_of_rotation, min_relative_diff)
    local self = setmetatable(Behavior.new("follow", max_velocity, sensors), Follow)

    self.factor_of_rotation = factor_of_rotation or 40.0
    self.min_relative_diff = min_relative_diff or 0.05
    self.max_perceived = max_perceived
    return self
end

--[[ Performs the action of following the light
    The robot will turn towards the side with more light, and move forward.
    If the relative difference between the two sides is less than the threshold, it will not activate the behavior.

    returns: left and right velocities, or nil if the behavior is not activated.
    ]]
function Follow:action()
    local K = self.factor_of_rotation

    local norm_sensors = utils.normalize_sensors(self.sensors, self.max_perceived)
    local max_sensor = utils.max_sensor_value(norm_sensors)

    b_vec = {length = max_sensor.value, angle = max_sensor.angle}

    log("[follow] intensity: " .. b_vec.length .. " | angle: " .. b_vec.angle)

    return b_vec
end

return Follow
