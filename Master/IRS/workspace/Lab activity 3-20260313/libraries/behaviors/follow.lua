local utils = require("libraries.utils")

local Follow = {}
Follow.__index = Follow

-- This behavior makes the robot follow the light.
function Follow.new(max_velocity, sensors, max_perceived, activation_threshold, factor_of_rotation, min_relative_diff)
    local self = setmetatable({}, Follow)

    self.name = "follow"
    self.factor_of_rotation = factor_of_rotation or 40.0
    self.min_relative_diff = min_relative_diff or 0.05
    self.max_velocity = max_velocity
    self.sensors = sensors
    self.max_perceived = max_perceived
    self.activation_threshold = activation_threshold
    return self
end

--[[ Performs the action of following the light
    The robot will turn towards the side with more light, and move forward.
    If the relative difference between the two sides is less than the threshold, it will not activate the behavior.

    returns: left and right velocities, or nil if the behavior is not activated.
    ]]
function Follow:action()
    local K = self.factor_of_rotation

    local left_sum = 0
    local right_sum = 0

    local norm_sensors = utils.normalize_sensors(self.sensors, self.max_perceived)

    for i = 1, #norm_sensors / 2 do
        left_sum = left_sum + norm_sensors[i].value
    end

    for i = #norm_sensors / 2 + 1, #norm_sensors do
        right_sum = right_sum + norm_sensors[i].value
    end

    local left_avg = left_sum / (#norm_sensors / 2)
    local right_avg = right_sum / (#norm_sensors / 2)

    local diff = left_avg - right_avg
    local avg = (left_avg + right_avg) * 0.5
    local relative_diff = (avg > 0) and (math.abs(diff) / avg) or 0  -- relative difference between the two groups
    log("[behaviors:follow] Left avg: " .. left_avg .. " | Right avg: " .. right_avg .. " | Relative diff: " .. (relative_diff * 100) .. "%")

    local left_vel = self.max_velocity
    local right_vel = self.max_velocity

    if relative_diff > self.min_relative_diff then  -- more than relative difference threshold
        left_vel  = math.max(0, math.min(self.max_velocity, self.max_velocity - K * diff * self.max_velocity))
        right_vel = math.max(0, math.min(self.max_velocity, self.max_velocity + K * diff * self.max_velocity))
    end
    
    local perceived = (left_avg + right_avg) * 0.5
    if perceived < self.activation_threshold then
        return nil
    end

    return left_vel, right_vel
end

return Follow
