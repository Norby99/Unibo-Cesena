local utils = require("libraries.utils")

local StopAtDarkSpot = {}
StopAtDarkSpot.__index = StopAtDarkSpot

function StopAtDarkSpot.new(max_velocity, sensors, min_threshold)
    local self = setmetatable({}, StopAtDarkSpot)

    self.name = "stop_at_dark_spot"
    self.weight = 1.0
    self.max_velocity = max_velocity
    self.sensors = sensors
    self.min_threshold = min_threshold

    return self    
end

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
