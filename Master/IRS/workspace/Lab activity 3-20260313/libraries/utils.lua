local utils = {}

function utils.normalize_sensors(sensors, max_perceived)
    local norm_sensors = {}
    for _, sensor in ipairs(sensors) do
        local norm_value = math.min(sensor.value / max_perceived, 1.0)
        table.insert(norm_sensors, {id = sensor.id, value = norm_value})
    end
    return norm_sensors
end

function utils.avg_sensor_value(sensors)
    local sum = 0
    for _, sensor in ipairs(sensors) do
        sum = sum + sensor.value
    end
    return sum / #sensors
end

return utils
