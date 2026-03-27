-- Class: roboto_debug
local roboto_debug = {}
roboto_debug.__index = roboto_debug

-- print the values of the light sensors.
function roboto_debug.print_light_sensors(rob)
    local debug_values = {}
    for i = 1, #rob.light do
        debug_values[i] = tostring(rob.light[i].value)
    end
    log("[debug] Light sensors: " .. table.concat(debug_values, ", "))
end

-- print the values of the motor ground sensors.
function roboto_debug.print_motor_ground(rob)
    local debug_values = {}
    for i = 1, #rob.motor_ground do
        debug_values[i] = tostring(rob.motor_ground[i].value)
    end
    log("[debug] Motor ground: " .. table.concat(debug_values, ", "))
end

return roboto_debug
