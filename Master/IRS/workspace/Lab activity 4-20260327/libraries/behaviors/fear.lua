local utils = require("libraries.utils")
local Behavior = require("libraries.behaviors.behavior")

local Fear = setmetatable({}, {__index = Behavior})
Fear.__index = Fear

-- This behavior makes the robot avoid obstacles by turning away from them.
function Fear.new(max_velocity, sensors, max_perceived, activation_threshold, factor_of_rotation, min_relative_diff, blind_spot_percentage)
    local self = setmetatable(Behavior.new("fear", max_velocity, sensors), Fear)

    self.factor_of_rotation = factor_of_rotation or 40.0
    self.min_relative_diff = min_relative_diff or 0.05
    self.blind_spot_percentage = blind_spot_percentage or 0.23
    self.activation_threshold = activation_threshold
    self.max_perceived = max_perceived

    return self
end

--[[ Performs the action of avoiding obstacles by turning away from them.
    The robot will turn away from the side with more obstacles, and move forward.
    If the relative difference between the two sides is less than the threshold, it will not activate the behavior.

    returns: left and right velocities, or nil if the behavior is not activated.
    ]]
function Fear:action()
    local K = self.factor_of_rotation

    -- remove the sensors from the second quarter to the third quarter
    local n = #self.sensors
    local q1 = math.floor(n * self.blind_spot_percentage)
    local q3 = math.floor(n * (1.0 - self.blind_spot_percentage))

    local norm_sensors = utils.normalize_sensors(self.sensors, self.max_perceived)

    local obs_norm_sensors = self:_get_observable_sensors(norm_sensors, q1, q3)

    local left_sum = 0
    local right_sum = 0

    for i = 1, #obs_norm_sensors / 2 do
        left_sum = left_sum + obs_norm_sensors[i].value
    end

    for i = #obs_norm_sensors / 2 + 1, #obs_norm_sensors do
        right_sum = right_sum + obs_norm_sensors[i].value
    end

    local left_avg = (left_sum / (#obs_norm_sensors / 2))
    local right_avg = (right_sum / (#obs_norm_sensors / 2))

    local diff = left_avg - right_avg
    local avg = (left_avg + right_avg) * 0.5
    local relative_diff = (avg > 0) and (math.abs(diff) / avg) or 0  -- relative difference between the two groups
    log("[behaviors:fear] Left avg: " .. left_avg .. " | Right avg: " .. right_avg .. " | Relative diff: " .. (relative_diff * 100) .. "%")

    local left_vel = self.max_velocity
    local right_vel = self.max_velocity

    if relative_diff > self.min_relative_diff then  -- more than relative difference threshold
        left_vel  = math.max(0, math.min(self.max_velocity, self.max_velocity + K * diff * self.max_velocity))
        right_vel = math.max(0, math.min(self.max_velocity, self.max_velocity - K * diff * self.max_velocity))
    end
    
    local percived = (left_avg + right_avg) * 0.5
    if percived < self.activation_threshold then
        return nil
    end

    return left_vel, right_vel
end

function Fear:_get_observable_sensors(sensors, first_quarter, third_quarter)
    local obs_sensors = {}
    for i = 1, first_quarter do
        table.insert(obs_sensors, sensors[i])
    end
    for i = third_quarter, #sensors do
        table.insert(obs_sensors, sensors[i])
    end
    return obs_sensors
end

return Fear
