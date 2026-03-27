local utils = require("libraries.utils")
local Behavior = require("libraries.behaviors.behavior")

local Fear = setmetatable({}, {__index = Behavior})
Fear.__index = Fear

-- This behavior makes the robot avoid obstacles by turning away from them.
function Fear.new(max_velocity, sensors, max_perceived)
    local self = setmetatable(Behavior.new("fear", max_velocity, sensors), Fear)

    self.max_perceived = max_perceived

    return self
end

--[[ Performs the action of avoiding obstacles by turning away from them.
    The robot will turn away from the side with more obstacles, and move forward.
    If the relative difference between the two sides is less than the threshold, it will not activate the behavior.

    returns: left and right velocities, or nil if the behavior is not activated.
    ]]
function Fear:action()
    local norm_sensors = utils.normalize_sensors(self.sensors, self.max_perceived)
    local max_sensor = utils.max_sensor_value(norm_sensors)

    b_vec = {length = max_sensor.value, angle = utils.opposite_angle(max_sensor.angle)}

    log("[fear] intensity: " .. b_vec.length .. " | angle: " .. b_vec.angle)

    return b_vec
end

return Fear
