local utils = require("libraries.utils")
local vector = require("libraries.vector")
local Behavior = require("libraries.behaviors.behavior")

local Follow = setmetatable({}, {__index = Behavior})
Follow.__index = Follow

--[[ This behavior makes the robot follow the light.
    max_velocity: the maximum velocity of the robot
    sensors: the light sensors of the robot
    max_perceived: the maximum value that the sensors can perceive, used for normalization
    halt_behaviors: a list of behaviors that can halt this behavior if they are activated
]]
function Follow.new(max_velocity, sensors, max_perceived, halt_behaviors)
    halt_behaviors = halt_behaviors or {}
    local self = setmetatable(Behavior.new("follow", max_velocity, sensors, halt_behaviors), Follow)

    self.max_perceived = max_perceived
    return self
end

--[[ Performs the action of following the light by moving towards the direction of the strongest light sensor.
    Returns:
        - A vector with length proportional to the intensity of the light and angle corresponding to the direction of the strongest light sensor if the behavior is activated
        - nil if the behavior is not activated (i.e., if any of the halt behaviors is activated)
]]
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
