-- Class: states
local states = {}
states.__index = states

function states.new(robot, max_velocity)
    local self = setmetatable({}, states)
    self.robot = robot
    self.max_velocity = max_velocity
    return self
end

function states:follow(sensors)
    local left_sum = 0
    local right_sum = 0

    for i = 1, #sensors / 2 do
        left_sum = left_sum + sensors[i].value
    end

    for i = #sensors / 2 + 1, #sensors do
        right_sum = right_sum + sensors[i].value
    end

    local left_avg = left_sum / (#sensors / 2)
    local right_avg = right_sum / (#sensors / 2)

    local diff = left_avg - right_avg
    local avg = (left_avg + right_avg) * 0.5
    local relative_diff = (avg > 0) and (math.abs(diff) / avg) or 0  -- relative difference between the two groups
    log("[states:follow] Left avg: " .. left_avg .. " | Right avg: " .. right_avg .. " | Relative diff: " .. (relative_diff * 100) .. "%")

    if relative_diff < 0.05 then  -- less than 5% relative difference
        self.robot.wheels.set_velocity(self.max_velocity, self.max_velocity)
    else
        local K = 40.0  -- factor of rotational speed
        local left_vel  = math.max(0, math.min(self.max_velocity, self.max_velocity - K * diff * self.max_velocity))
        local right_vel = math.max(0, math.min(self.max_velocity, self.max_velocity + K * diff * self.max_velocity))
        self.robot.wheels.set_velocity(left_vel, right_vel)
    end
end

function states:fear(sensors)
    -- remove the sensors from the second quarter to the third quarter
    local n = #sensors
    local q1 = math.floor(n * 0.25)
    local q3 = math.floor(n * 0.75) + 1

    local obs_sensors = {}
    for i = 1, q1 do
        table.insert(obs_sensors, sensors[i])
    end
    for i = q3, n do
        table.insert(obs_sensors, sensors[i])
    end

    local left_sum = 0
    local right_sum = 0

    for i = 1, #obs_sensors / 2 do
        left_sum = left_sum + obs_sensors[i].value
    end

    for i = #obs_sensors / 2 + 1, #obs_sensors do
        right_sum = right_sum + obs_sensors[i].value
    end

    local left_avg = left_sum / (#obs_sensors / 2)
    local right_avg = right_sum / (#obs_sensors / 2)

    local diff = left_avg - right_avg
    local avg = (left_avg + right_avg) * 0.5
    local relative_diff = (avg > 0) and (math.abs(diff) / avg) or 0  -- relative difference between the two groups
    log("[states:fear] Left avg: " .. left_avg .. " | Right avg: " .. right_avg .. " | Relative diff: " .. (relative_diff * 100) .. "%")

    if relative_diff < 0.05 then  -- less than 5% relative difference
        self.robot.wheels.set_velocity(self.max_velocity, self.max_velocity)
    else
        local K = 40.0  -- factor of rotational speed
        local left_vel  = math.max(0, math.min(self.max_velocity, self.max_velocity + K * diff * self.max_velocity))
        local right_vel = math.max(0, math.min(self.max_velocity, self.max_velocity - K * diff * self.max_velocity))
        self.robot.wheels.set_velocity(left_vel, right_vel)
    end
end

return states
