local utils = require "utils"

local Fear = {}
Fear.__index = Fear

function Fear.new(max_velocity, weight, sensors, max_perceived)
    local self = setmetatable({}, Fear)

    self.name = "fear"
    self.weight = weight or 1.0
    self.max_velocity = max_velocity
    self.sensors = sensors
    self.max_perceived = max_perceived

    return self
end

function Fear:action()
    local K = 40.0  -- factor of rotational speed

    -- remove the sensors from the second quarter to the third quarter
    local n = #self.sensors
    local q1 = math.floor(n * 0.25)
    local q3 = math.floor(n * 0.75) + 1

    local norm_sensors = utils.normalize_sensors(self.sensors, self.max_perceived)

    local obs_norm_sensors = {}
    for i = 1, q1 do
        table.insert(obs_norm_sensors, norm_sensors[i])
    end
    for i = q3, n do
        table.insert(obs_norm_sensors, norm_sensors[i])
    end

    local left_sum = 0
    local right_sum = 0

    for i = 1, #obs_norm_sensors / 2 do
        left_sum = left_sum + obs_norm_sensors[i].value
    end

    for i = #obs_norm_sensors / 2 + 1, #obs_norm_sensors do
        right_sum = right_sum + obs_norm_sensors[i].value
    end

    local left_avg = left_sum / (#obs_norm_sensors / 2)
    local right_avg = right_sum / (#obs_norm_sensors / 2)

    local diff = left_avg - right_avg
    local avg = (left_avg + right_avg) * 0.5
    local relative_diff = (avg > 0) and (math.abs(diff) / avg) or 0  -- relative difference between the two groups
    log("[behaviors:fear] Left avg: " .. left_avg .. " | Right avg: " .. right_avg .. " | Relative diff: " .. (relative_diff * 100) .. "%")

    local left_vel = self.max_velocity
    local right_vel = self.max_velocity

    if relative_diff > 0.05 then  -- more than 5% relative difference
        left_vel  = math.max(0, math.min(self.max_velocity, self.max_velocity + K * diff * self.max_velocity))
        right_vel = math.max(0, math.min(self.max_velocity, self.max_velocity - K * diff * self.max_velocity))
    end

    return left_vel, right_vel
end

return Fear
