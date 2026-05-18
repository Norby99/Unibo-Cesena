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

-- print the values of the wheels' velocities.
function roboto_debug.print_speed(rob)
    log("[debug] [wheel_speed] left: " .. rob.wheels.left_velocity .. " | right: " .. rob.wheels.right_velocity)
end

-- print the value of a 2D vector.
function roboto_debug.print_vec2(name, vec)
    log("[debug] " .. name .. ": x: " .. vec.x .. " | y: " .. vec.y)
end

-- print the value of a polar vector.
function roboto_debug.print_polar_vec(name, vec)
     log("[debug] " .. name .. ": length: " .. vec.length .. " | angle: " .. vec.angle)
end

return roboto_debug
