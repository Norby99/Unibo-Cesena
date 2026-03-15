local utils = require("libraries.utils")

local StopAtDarkSpot = {}
StopAtDarkSpot.__index = StopAtDarkSpot

-- This behavior stops the robot when it perceives a dark spot
function StopAtDarkSpot.new(max_velocity, sensors, min_threshold)
    local self = setmetatable({}, StopAtDarkSpot)

    self.name = "stop_at_dark_spot"
    self.weight = 1.0
    self.max_velocity = max_velocity
    self.sensors = sensors
    self.min_threshold = min_threshold

    return self    
end

--[[ Performs the action of stopping if the average value of the motor ground sensors is below the threshold
    Returns:
        (left_speed, right_speed) if the behavior is activated, nil otherwise
]]
function StopAtDarkSpot:action()
    local avg_value = utils.avg_sensor_value(self.sensors)

    log("[behaviors:stop_at_dark_spot] Average motor ground value: " .. avg_value)

    if avg_value < self.min_threshold then
        return 0, 0
    else
        return nil
    end
end

return StopAtDarkSpot
