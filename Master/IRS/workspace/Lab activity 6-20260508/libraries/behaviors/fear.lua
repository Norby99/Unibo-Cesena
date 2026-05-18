local utils = require("libraries.utils")
local Behavior = require("libraries.behaviors.behavior")

local Fear = setmetatable({}, {__index = Behavior})
Fear.__index = Fear

--[[ This behavior makes the robot avoid obstacles by turning away from them.
    max_velocity: the maximum velocity of the robot
    sensors: the proximity sensors of the robot
    max_perceived: the maximum value that the sensors can perceive, used for normalization
    halt_behaviors: a list of behaviors that can halt this behavior if they are activated
]]
function Fear.new(max_velocity, sensors, max_perceived, halt_behaviors)
    halt_behaviors = halt_behaviors or {}
    local self = setmetatable(Behavior.new("fear", max_velocity, sensors, halt_behaviors), Fear)

    self.max_perceived = max_perceived

    return self
end

--[[ Performs the action of avoiding obstacles by turning away from them.
    Returns:
        - A vector with length proportional to the intensity of the obstacle and angle opposite to the direction of the obstacle if the behavior is activated (i.e., there is an obstacle to avoid)
        - nil if the behavior is not activated (i.e., there are no obstacles to avoid or a halt behavior is activated)
    ]]
function Fear:action()
    if self:should_halt() then
        return nil
    end

    local norm_sensors = utils.normalize_sensors(self.sensors, self.max_perceived)
    local max_sensor = utils.max_sensor_value(norm_sensors)

    b_vec = {length = max_sensor.value, angle = utils.opposite_angle(max_sensor.angle)}

    -- log("[fear] intensity: " .. b_vec.length .. " | angle: " .. b_vec.angle)

    return b_vec
end

return Fear
