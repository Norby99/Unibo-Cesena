local utils = require("libraries.utils")

local Follow = {}
Follow.__index = Follow

function Follow.new(max_velocity, sensors, max_perceived)
    local self = setmetatable({}, Follow)

    self.name = "follow"
    self.weight = 1.0
    self.max_velocity = max_velocity
    self.sensors = sensors
    self.max_perceived = max_perceived

    return self
end

function Follow:action()
    local K = 40.0  -- factor of rotational speed

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

    if relative_diff > 0.05 then  -- more than 5% relative difference
        left_vel  = math.max(0, math.min(self.max_velocity, self.max_velocity - K * diff * self.max_velocity))
        right_vel = math.max(0, math.min(self.max_velocity, self.max_velocity + K * diff * self.max_velocity))
    end
    
    if relative_diff <  0.3 then
        return nil
    end

    return left_vel, right_vel
end

return Follow
